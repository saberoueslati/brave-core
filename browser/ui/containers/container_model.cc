// Copyright (c) 2025 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "brave/browser/ui/containers/container_model.h"

#include <optional>
#include <string_view>
#include <utility>
#include <vector>

#include "base/functional/bind.h"
#include "brave/browser/containers/containers_service_factory.h"
#include "brave/browser/ui/containers/containers_icon_generator.h"
#include "brave/components/containers/core/browser/containers_service.h"
#include "brave/components/containers/core/browser/prefs.h"
#include "chrome/browser/profiles/profile.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/geometry/size.h"

namespace containers {

namespace {

ui::ImageModel GetImageModelForContainer(const mojom::ContainerPtr& container,
                                         float scale_factor) {
  CHECK(container) << "Container must be valid";
  constexpr int kDipSize = 16;
  constexpr int kDipIconSize = 12;
  return ui::ImageModel::FromImageGenerator(
      base::BindRepeating(&GenerateContainerIcon, container->icon,
                          container->background_color, kDipSize, kDipIconSize,
                          scale_factor),
      gfx::Size(kDipSize, kDipSize));
}

}  // namespace

// static
ContainerModel ContainerModel::CreateForUnknown(const std::string& id,
                                                float scale_factor) {
  return ContainerModel(
      mojom::Container::New(id, id, containers::mojom::Icon::kDefault,
                            SkColorSetRGB(0xb7, 0x4d, 0x49)),

      scale_factor);
}

ContainerModel::ContainerModel(mojom::ContainerPtr container,
                               float scale_factor)
    : container_(std::move(container)),
      icon_(GetImageModelForContainer(container_, scale_factor)) {
  CHECK(container_) << "Container must be valid";
}

ContainerModel::ContainerModel(ContainerModel&& other) noexcept = default;

ContainerModel& ContainerModel::operator=(ContainerModel&& other) noexcept =
    default;

ContainerModel::~ContainerModel() = default;

std::vector<ContainerModel> GetContainerModelsFromPrefs(
    const PrefService& prefs,
    float scale_factor) {
  std::vector<ContainerModel> containers;
  for (auto& container : GetContainersFromPrefs(prefs)) {
    containers.emplace_back(std::move(container), scale_factor);
  }
  return containers;
}

std::optional<ContainerModel> GetRuntimeContainerModel(Profile* profile,
                                                       std::string_view id,
                                                       float scale_factor) {
  if (!profile || id.empty()) {
    return std::nullopt;
  }

  if (auto* service = ContainersServiceFactory::GetForProfile(profile)) {
    if (auto container = service->GetContainerById(std::string(id))) {
      return ContainerModel(std::move(container), scale_factor);
    }
  }

  return ContainerModel::CreateForUnknown(std::string(id), scale_factor);
}

}  // namespace containers
