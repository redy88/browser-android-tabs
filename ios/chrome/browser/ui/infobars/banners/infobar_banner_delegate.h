// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IOS_CHROME_BROWSER_UI_INFOBARS_BANNERS_INFOBAR_BANNER_DELEGATE_H_
#define IOS_CHROME_BROWSER_UI_INFOBARS_BANNERS_INFOBAR_BANNER_DELEGATE_H_

#import <Foundation/Foundation.h>

#include "base/ios/block_types.h"

// Delegate to handle InfobarBanner actions.
@protocol InfobarBannerDelegate

// Called when the InfobarBanner button was pressed.
- (void)bannerInfobarButtonWasPressed:(UIButton*)sender;

// Asks the delegate to dismiss the InfobarBanner. |completion| will always run.
- (void)dismissInfobarBanner:(id)sender
                    animated:(BOOL)animated
                  completion:(ProceduralBlock)completion;

// Asks the delegate to present the InfobarModal for this InfobarBanner.
- (void)presentInfobarModalFromBanner;

@end

#endif  // IOS_CHROME_BROWSER_UI_INFOBARS_BANNERS_INFOBAR_BANNER_DELEGATE_H_
