// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/network_error_logging/mock_persistent_nel_store.h"

#include <sstream>

namespace net {

MockPersistentNELStore::Command::Command(
    Type type,
    NELPoliciesLoadedCallback loaded_callback)
    : type(type), loaded_callback(std::move(loaded_callback)) {}

MockPersistentNELStore::Command::Command(
    Type type,
    const NetworkErrorLoggingService::NELPolicy& policy)
    : type(type), origin(policy.origin) {}

MockPersistentNELStore::Command::Command(Type type) : type(type) {}

MockPersistentNELStore::Command::Command(const Command& other)
    : type(other.type), origin(other.origin) {}

MockPersistentNELStore::Command::Command(Command&& other) = default;

MockPersistentNELStore::Command::~Command() = default;

bool operator==(const MockPersistentNELStore::Command& lhs,
                const MockPersistentNELStore::Command& rhs) {
  if (lhs.type != rhs.type)
    return false;
  switch (lhs.type) {
    // For LOAD_NEL_POLICIES and FLUSH, just check the type.
    case MockPersistentNELStore::Command::Type::LOAD_NEL_POLICIES:
    case MockPersistentNELStore::Command::Type::FLUSH:
      return true;
    // For ADD_NEL_POLICY, UPDATE_NEL_POLICY, and DELETE_NEL_POLICY,
    // additionally check the policy's origin.
    case MockPersistentNELStore::Command::Type::ADD_NEL_POLICY:
    case MockPersistentNELStore::Command::Type::UPDATE_NEL_POLICY:
    case MockPersistentNELStore::Command::Type::DELETE_NEL_POLICY:
      return (lhs.origin == rhs.origin);
  }
}

bool operator!=(const MockPersistentNELStore::Command& lhs,
                const MockPersistentNELStore::Command& rhs) {
  return !(lhs == rhs);
}

MockPersistentNELStore::MockPersistentNELStore()
    : load_started_(false), policy_count_(0), queued_policy_count_delta_(0) {}

MockPersistentNELStore::~MockPersistentNELStore() = default;

void MockPersistentNELStore::LoadNELPolicies(
    NELPoliciesLoadedCallback loaded_callback) {
  DCHECK(!load_started_);
  command_list_.emplace_back(Command::Type::LOAD_NEL_POLICIES,
                             std::move(loaded_callback));
  load_started_ = true;
}

void MockPersistentNELStore::AddNELPolicy(
    const NetworkErrorLoggingService::NELPolicy& policy) {
  DCHECK(load_started_);
  command_list_.emplace_back(Command::Type::ADD_NEL_POLICY, policy);
  ++queued_policy_count_delta_;
}

void MockPersistentNELStore::UpdateNELPolicyAccessTime(
    const NetworkErrorLoggingService::NELPolicy& policy) {
  DCHECK(load_started_);
  command_list_.emplace_back(Command::Type::UPDATE_NEL_POLICY, policy);
}

void MockPersistentNELStore::DeleteNELPolicy(
    const NetworkErrorLoggingService::NELPolicy& policy) {
  DCHECK(load_started_);
  command_list_.emplace_back(Command::Type::DELETE_NEL_POLICY, policy);
  --queued_policy_count_delta_;
}

void MockPersistentNELStore::Flush() {
  // Can be called before |load_started_| is true, if the
  // NetworkErrorLoggingService is destroyed before getting a chance to load.
  command_list_.emplace_back(Command::Type::FLUSH);
  policy_count_ += queued_policy_count_delta_;
  queued_policy_count_delta_ = 0;
}

void MockPersistentNELStore::SetPrestoredPolicies(
    std::vector<NetworkErrorLoggingService::NELPolicy> policies) {
  DCHECK(!load_started_);
  DCHECK_EQ(0, policy_count_);
  policy_count_ += policies.size();
  prestored_policies_.swap(policies);
}

void MockPersistentNELStore::FinishLoading(bool load_success) {
  DCHECK(load_started_);
  for (size_t i = 0; i < command_list_.size(); ++i) {
    Command& command = command_list_[i];
    if (command.type == Command::Type::LOAD_NEL_POLICIES) {
      // If LOAD_NEL_POLICIES has been initiated, it should be the first
      // operation.
      DCHECK_EQ(0u, i);
      DCHECK(!command.loaded_callback.is_null());
      if (load_success) {
        std::move(command.loaded_callback).Run(std::move(prestored_policies_));
      } else {
        std::move(command.loaded_callback)
            .Run(std::vector<NetworkErrorLoggingService::NELPolicy>());
      }
    }
    if (i > 0) {
      // LOAD_NEL_POLICIES should not have been called twice.
      DCHECK(command.type != Command::Type::LOAD_NEL_POLICIES);
    }
  }
}

bool MockPersistentNELStore::VerifyCommands(
    const CommandList& expected_commands) const {
  return command_list_ == expected_commands;
}

MockPersistentNELStore::CommandList MockPersistentNELStore::GetAllCommands()
    const {
  return command_list_;
}

std::string MockPersistentNELStore::GetDebugString() const {
  std::ostringstream s;

  for (const Command& command : command_list_) {
    switch (command.type) {
      case Command::Type::LOAD_NEL_POLICIES:
        s << "LOAD; ";
        break;
      case Command::Type::ADD_NEL_POLICY:
        s << "ADD(" << command.origin.Serialize() << "); ";
        break;
      case Command::Type::UPDATE_NEL_POLICY:
        s << "UPDATE(" << command.origin.Serialize() << "); ";
        break;
      case Command::Type::DELETE_NEL_POLICY:
        s << "DELETE(" << command.origin.Serialize() << "); ";
        break;
      case Command::Type::FLUSH:
        s << "FLUSH; ";
        break;
    }
  }

  return s.str();
}

}  // namespace net
