/* Copyright (c) 2026 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "url/gurl.h"

namespace {

void MaybeRewriteVirtualURL(GURL* url_to_load, GURL* virtual_url) {
  if (url_to_load->SchemeIs("chrome")) {
    *virtual_url = *url_to_load;
  }
}

}  // namespace

#include <content/browser/renderer_host/navigation_controller_impl.cc>
