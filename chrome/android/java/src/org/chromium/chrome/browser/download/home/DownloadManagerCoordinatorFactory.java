// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.chromium.chrome.browser.download.home;

import android.app.Activity;
import android.content.ComponentName;

import org.chromium.chrome.browser.ChromeFeatureList;
import org.chromium.chrome.browser.download.ui.DownloadManagerUi;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.chrome.browser.snackbar.SnackbarManager;
import org.chromium.ui.modaldialog.ModalDialogManager;

/** A helper class to build and return an {@link DownloadManagerCoordinator}. */
public class DownloadManagerCoordinatorFactory {
    /**
     * Returns an instance of a {@link DownloadManagerCoordinator} to be used in the UI.
     * @param activity           The parent {@link Activity}.
     * @param config             A {@link DownloadManagerUiConfig} to provide configuration params.
     * @param parentComponent    The parent component.
     * @param snackbarManager    The {@link SnackbarManager} that should be used to show snackbars.
     * @param modalDialogManager The {@link ModalDialogManager} that should be used to show dialog.
     * @return                   A new {@link DownloadManagerCoordinator} instance.
     */
    public static DownloadManagerCoordinator create(Activity activity,
            DownloadManagerUiConfig config, SnackbarManager snackbarManager,
            ComponentName parentComponent, ModalDialogManager modalDialogManager) {
        if (ChromeFeatureList.isEnabled(ChromeFeatureList.DOWNLOAD_HOME_V2)) {
            return new DownloadManagerCoordinatorImpl(Profile.getLastUsedProfile(), activity,
                    config, snackbarManager, modalDialogManager);
        } else {
            return new DownloadManagerUi(activity, config.isOffTheRecord, parentComponent,
                    config.isSeparateActivity, snackbarManager);
        }
    }
}
