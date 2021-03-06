// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/ui/tabs/tab_group_data.h"

#include "base/strings/string_number_conversions.h"
#include "base/strings/utf_string_conversions.h"
#include "third_party/skia/include/utils/SkRandom.h"

TabGroupData::TabGroupData() {
  static int next_placeholder_title_number = 1;
  title_ = base::ASCIIToUTF16(
      "Group " + base::NumberToString(next_placeholder_title_number));
  ++next_placeholder_title_number;

  static SkRandom rand;
  stroke_color_ = rand.nextU() | 0xff000000;
}
