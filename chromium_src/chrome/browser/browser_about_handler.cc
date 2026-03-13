/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "content/public/browser/browser_context.h"
#include "content/public/common/url_constants.h"
#include "url/gurl.h"

#define HandleChromeAboutAndChromeSyncRewrite \
  HandleChromeAboutAndChromeSyncRewrite_ChromiumImpl
#include <chrome/browser/browser_about_handler.cc>
#undef HandleChromeAboutAndChromeSyncRewrite

bool HandleChromeAboutAndChromeSyncRewrite(
    GURL* url,
    content::BrowserContext* browser_context) {
  bool result = false;
  if (url->SchemeIs(content::kBraveUIScheme)) {
    GURL::Replacements replacements;
    replacements.SetSchemeStr(content::kChromeUIScheme);
    *url = url->ReplaceComponents(replacements);
    // We want both the real and virtual url to be chrome so rewrite both
    result = true;
  }

  return result || HandleChromeAboutAndChromeSyncRewrite_ChromiumImpl(
                       url, browser_context);
}
