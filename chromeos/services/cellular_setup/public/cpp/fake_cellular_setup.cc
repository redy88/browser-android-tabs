// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chromeos/services/cellular_setup/public/cpp/fake_cellular_setup.h"

#include "chromeos/services/cellular_setup/public/cpp/fake_carrier_portal_handler.h"

#include <utility>

namespace chromeos {

namespace cellular_setup {

FakeCellularSetup::StartActivationInvocation::StartActivationInvocation(
    const std::string& cellular_network_guid,
    mojom::ActivationDelegatePtr activation_delegate,
    StartActivationCallback callback)
    : cellular_network_guid_(cellular_network_guid),
      activation_delegate_(std::move(activation_delegate)),
      callback_(std::move(callback)) {}

FakeCellularSetup::StartActivationInvocation::~StartActivationInvocation() =
    default;

FakeCarrierPortalHandler*
FakeCellularSetup::StartActivationInvocation::ExecuteCallback() {
  DCHECK(callback_);
  DCHECK(!fake_carrier_portal_observer_);

  fake_carrier_portal_observer_ = std::make_unique<FakeCarrierPortalHandler>();
  std::move(callback_).Run(
      fake_carrier_portal_observer_->GenerateInterfacePtr());

  return fake_carrier_portal_observer_.get();
}

FakeCellularSetup::FakeCellularSetup() = default;

FakeCellularSetup::~FakeCellularSetup() = default;

void FakeCellularSetup::StartActivation(
    const std::string& cellular_network_guid,
    mojom::ActivationDelegatePtr activation_delegate,
    StartActivationCallback callback) {
  DCHECK(activation_delegate);
  DCHECK(callback);

  start_activation_invocations_.emplace_back(
      std::make_unique<StartActivationInvocation>(
          cellular_network_guid, std::move(activation_delegate),
          std::move(callback)));
}

}  // namespace cellular_setup

}  // namespace chromeos
