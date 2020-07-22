// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "UniversalTestInstance.h"

namespace Microsoft::React::Test {

UniversalTestInstance::UniversalTestInstance() noexcept
    : m_queueController{winrt::Windows::System::DispatcherQueueController::CreateOnDedicatedThread()} {}

#pragma region ITestInstance

void UniversalTestInstance::AttachMeasuredRootView(std::string &&appName) noexcept /*override*/ {}

void UniversalTestInstance::DetachRootView() noexcept /*override*/
{}

std::shared_ptr<facebook::react::Instance> UniversalTestInstance::GetInnerInstance() const noexcept /*override*/ {
  return nullptr;
}

#pragma endregion ITestInstance

} // namespace Microsoft::React::Test
