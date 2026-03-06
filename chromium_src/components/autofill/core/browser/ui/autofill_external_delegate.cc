/* Copyright (c) 2026 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "components/autofill/core/browser/ui/autofill_external_delegate.h"

#include <ranges>
#include <vector>

#include "base/containers/map_util.h"
#include "components/autofill/core/browser/foundations/autofill_driver.h"
#include "components/autofill/core/browser/foundations/autofill_manager.h"
#include "components/autofill/core/browser/suggestions/suggestion.h"

#define CanShowAutofillUi() \
  CanShowAutofillUi() ||    \
      (manager_->client().BraveAddSuggestions(suggestions), false)

#define DidAcceptSuggestion(...) DidAcceptSuggestion_ChromiumImpl(__VA_ARGS__)

#include <components/autofill/core/browser/ui/autofill_external_delegate.cc>

#undef CanShowAutofillUi
#undef DidAcceptSuggestion

namespace autofill {

void AutofillExternalDelegate::DidAcceptSuggestion(
    const Suggestion& suggestion,
    const SuggestionMetadata& metadata) {
  if (manager_->client().BraveHandleSuggestion(suggestion,
                                               query_field_.global_id())) {
    return;
  }

  DidAcceptSuggestion_ChromiumImpl(suggestion, metadata);
}

}  // namespace autofill
