// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <tuple>
#include <vector>

#include "base/run_loop.h"
#include "base/strings/utf_string_conversions.h"
#include "components/spellcheck/common/spellcheck_result.h"
#include "components/spellcheck/renderer/spellcheck_provider_test.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace {

class SpellCheckProviderMacTest : public SpellCheckProviderTest {};

TEST_F(SpellCheckProviderMacTest, SingleRoundtripSuccess) {
  FakeTextCheckingCompletion completion;

  provider_.RequestTextChecking(base::ASCIIToUTF16("hello "), &completion);
  EXPECT_EQ(completion.completion_count_, 0U);
  EXPECT_EQ(provider_.text_check_requests_.size(), 1U);
  EXPECT_EQ(provider_.pending_text_request_size(), 1U);

  const auto& text = provider_.text_check_requests_.back().first;
  auto& callback = provider_.text_check_requests_.back().second;
  EXPECT_EQ(text, base::ASCIIToUTF16("hello "));
  EXPECT_TRUE(callback);

  std::vector<SpellCheckResult> fake_results;
  std::move(callback).Run(fake_results);
  base::RunLoop().RunUntilIdle();

  EXPECT_EQ(completion.completion_count_, 1U);
  EXPECT_EQ(provider_.pending_text_request_size(), 0U);

  provider_.text_check_requests_.clear();
}

TEST_F(SpellCheckProviderMacTest, TwoRoundtripSuccess) {
  FakeTextCheckingCompletion completion1;
  provider_.RequestTextChecking(base::ASCIIToUTF16("hello "), &completion1);
  FakeTextCheckingCompletion completion2;
  provider_.RequestTextChecking(base::ASCIIToUTF16("bye "), &completion2);

  EXPECT_EQ(completion1.completion_count_, 0U);
  EXPECT_EQ(completion2.completion_count_, 0U);
  EXPECT_EQ(provider_.text_check_requests_.size(), 2U);
  EXPECT_EQ(provider_.pending_text_request_size(), 2U);

  const auto& text1 = provider_.text_check_requests_[0].first;
  auto& callback1 = provider_.text_check_requests_[0].second;
  EXPECT_EQ(text1, base::ASCIIToUTF16("hello "));
  EXPECT_TRUE(callback1);

  const auto& text2 = provider_.text_check_requests_[1].first;
  auto& callback2 = provider_.text_check_requests_[1].second;
  EXPECT_EQ(text2, base::ASCIIToUTF16("bye "));
  EXPECT_TRUE(callback2);

  std::vector<SpellCheckResult> fake_results;

  std::move(callback1).Run(fake_results);
  base::RunLoop().RunUntilIdle();
  EXPECT_EQ(completion1.completion_count_, 1U);
  EXPECT_EQ(completion2.completion_count_, 0U);
  EXPECT_EQ(provider_.pending_text_request_size(), 1U);

  std::move(callback2).Run(fake_results);
  base::RunLoop().RunUntilIdle();
  EXPECT_EQ(completion1.completion_count_, 1U);
  EXPECT_EQ(completion2.completion_count_, 1U);
  EXPECT_EQ(provider_.pending_text_request_size(), 0U);

  provider_.text_check_requests_.clear();
}

}  // namespace
