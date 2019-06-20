/** Copyright (c) 2019 The Brave Authors. All rights reserved.
  * This Source Code Form is subject to the terms of the Mozilla Public
  * License, v. 2.0. If a copy of the MPL was not distributed with this file,
  * You can obtain one at http://mozilla.org/MPL/2.0/.
  */

package org.chromium.chrome.browser.notifications;

import android.annotation.TargetApi;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.provider.Settings;

import org.chromium.base.annotations.CalledByNative;
import org.chromium.base.ContextUtils;
import org.chromium.chrome.browser.notifications.channels.ChannelDefinitions;
import org.chromium.chrome.browser.BraveRewardsHelper;
import org.chromium.chrome.browser.ChromeTabbedActivity;

/**
 * This class provides the Brave Ads related methods for the native library
 * (brave/components/brave_ads/browser/notification_helper_android)
 */
public abstract class BraveAds {
    @CalledByNative
    public static String getBraveAdsChannelId() {
      return ChannelDefinitions.ChannelId.BRAVE_ADS;
    }

    @CalledByNative
    public static void openSystemNotifPrefs() {
      ChromeTabbedActivity activity =
          BraveRewardsHelper.GetChromeTabbedActivity();
      if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O && activity != null) {
        Intent intent = new Intent(Settings.ACTION_APP_NOTIFICATION_SETTINGS);
        intent.putExtra(Settings.EXTRA_APP_PACKAGE,
            ContextUtils.getApplicationContext().getPackageName());
        activity.startActivity(intent);
      } else {
        Intent intent = new Intent(Settings.ACTION_APPLICATION_DETAILS_SETTINGS);
        intent.setData(Uri.parse("package:" +
            ContextUtils.getApplicationContext().getPackageName()));
        activity.startActivity(intent);
      }
    }
}
