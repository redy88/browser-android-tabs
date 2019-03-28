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
    public static final String DEEP_LINK_TYPE_PAGE = "deep_link_type_page"; // For opening up pages in Brave or redirecting to Google Play

    // Intent types
    public static final String INTENT_TYPE = "intent_type";

    // Payload keys for displaying the information
    public static final String NOTIFICATION_TITLE = "notification_title";
    public static final String NOTIFICATION_BODY = "notification_body";

    // E.g. values: 
    // ["https://play.google.com/store/apps/details?id=com.brave.browser",
    //  "market://details?id=com.brave.browser"]
    public static final String NOTIFICATION_URL = "notification_url";

    private void showNotification(Intent intent) {
        NotificationCompat.Builder b = new NotificationCompat.Builder(mContext, "com.brave.browser.ads");
        b.setDefaults(Notification.DEFAULT_ALL)
         .setSmallIcon(R.drawable.ic_chrome)
         .setContentTitle((String) intent.getStringExtra(NOTIFICATION_TITLE))
         .setContentText((String) intent.getStringExtra(NOTIFICATION_BODY))
         .setCategory(Notification.CATEGORY_MESSAGE)
         .setPriority(Notification.PRIORITY_MAX);

        b.setContentIntent(getAdsDeepLinkIntent(mContext, intent));

        if (Build.VERSION.SDK_INT >= 21) b.setVibrate(new long[0]);

        NotificationManager notificationManager = (NotificationManager) mContext.getSystemService(Context.NOTIFICATION_SERVICE);
        notificationManager.notify(NOTIFICATION_ID, b.build());
    }

    private PendingIntent getAdsDeepLinkIntent(Context context, Intent intent) {
        Intent deepLinkIntent = new Intent(context, BraveAdsNotificationService.class);
        deepLinkIntent.putExtra(DEEP_LINK, DEEP_LINK_TYPE_PAGE);
        deepLinkIntent.putExtra(NOTIFICATION_URL, intent.getStringExtra(NOTIFICATION_URL));
        return PendingIntent.getBroadcast(context, 0, deepLinkIntent, PendingIntent.FLAG_UPDATE_CURRENT);
    }

    private void handleAdsDeepLink(Intent intent) {
        if (intent.getStringExtra(BraveAdsNotificationService.DEEP_LINK).equals(BraveAdsNotificationService.DEEP_LINK_TYPE_PAGE)) {
           Intent deepLinkIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(intent.getStringExtra(NOTIFICATION_URL)));
           deepLinkIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
           mContext.startActivity(deepLinkIntent);
        }
     }

    @Override
    public void onReceive(Context context, Intent intent) {
        mContext = context;
        mIntent = intent;
        if (intent != null && intent.hasExtra(DEEP_LINK)) {
            handleAdsDeepLink(intent);
        } else {
            showNotification(intent);
        }
    }
}
