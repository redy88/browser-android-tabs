package org.chromium.chrome.browser.notifications;

import android.app.AlarmManager;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.content.SharedPreferences;
import android.content.pm.ResolveInfo;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.SystemClock;
import android.provider.Settings;
import android.support.v4.app.NotificationCompat;
import android.util.Log;
import org.chromium.chrome.R;
import org.chromium.base.ContextUtils;
import org.chromium.chrome.browser.ChromeActivity;
import org.chromium.chrome.browser.ChromeActivitySessionTracker;

public class BraveAdsNotificationService extends BroadcastReceiver {
    public Context mContext;
    private Intent mIntent;

    // (Albert Wang): One higher than BraveSetDefaultBrowserNotificationService.
    // I didn't check to see if this was used by another service
    public static final int NOTIFICATION_ID = 11;

    // Deep links
    public static final String DEEP_LINK = "deep_link";
    public static final String SHOW_TEST_AD = "SHOW_TEST_AD";

    // Intent types
    public static final String INTENT_TYPE = "intent_type";

    private void showNotification() {
        NotificationCompat.Builder b = new NotificationCompat.Builder(mContext, "com.brave.browser.ads");
        b.setDefaults(Notification.DEFAULT_ALL)
         .setSmallIcon(R.drawable.ic_chrome)
         .setContentTitle("Brave")
         .setContentText("Download now")
         .setCategory(Notification.CATEGORY_MESSAGE)
         .setPriority(Notification.PRIORITY_MAX);

        b.setContentIntent(getAdsDeepLinkIntent(mContext));

        if (Build.VERSION.SDK_INT >= 21) b.setVibrate(new long[0]);

        NotificationManager notificationManager = (NotificationManager) mContext.getSystemService(Context.NOTIFICATION_SERVICE);
        notificationManager.notify(NOTIFICATION_ID, b.build());
    }

    private PendingIntent getAdsDeepLinkIntent(Context context) {
        Intent intent = new Intent(context, BraveAdsNotificationService.class);
        intent.setAction(DEEP_LINK);
        intent.putExtra(DEEP_LINK, SHOW_TEST_AD);
        return PendingIntent.getBroadcast(context, 0, intent, PendingIntent.FLAG_UPDATE_CURRENT);
    }

    private void handleAdsDeepLink(Intent intent) {
         Bundle bundle = intent.getExtras();
         if (bundle.getString(BraveAdsNotificationService.DEEP_LINK).equals(BraveAdsNotificationService.SHOW_TEST_AD)) {
            String appPackageName = "com.brave.browser";
            Intent testIntent = new Intent(Intent.ACTION_VIEW, Uri.parse("https://play.google.com/store/apps/details?id=" + appPackageName));
            testIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            mContext.startActivity(testIntent);
         }
     }

    @Override
    public void onReceive(Context context, Intent intent) {
        mContext = context;
        mIntent = intent;
        if (intent != null && intent.hasExtra(BraveAdsNotificationService.DEEP_LINK)) {
            handleAdsDeepLink(intent);
        } else {
            showNotification();
        }
    }
}
