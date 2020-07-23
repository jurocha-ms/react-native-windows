// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "UniversalTestInstance.h"

namespace Microsoft::React::Test {

UniversalTestInstance::UniversalTestInstance() noexcept
    : m_queueController{winrt::Windows::System::DispatcherQueueController::CreateOnDedicatedThread()} {
}

#pragma region ITestInstance

void UniversalTestInstance::AttachMeasuredRootView(std::string &&appName) noexcept /*override*/ {
  m_queueController.DispatcherQueue().TryEnqueue([this]() noexcept {
    //TODO? Add package providers

    m_host.InstanceSettings().UseDeveloperSupport(false);
    m_host.InstanceSettings().UseWebDebugger(false);
    m_host.InstanceSettings().UseFastRefresh(false);
    m_host.InstanceSettings().UseLiveReload(true);
    m_host.InstanceSettings().EnableDeveloperMenu(false);
    m_host.InstanceSettings().JavaScriptMainModuleName(L"IntegrationTests/DummyTest");

    m_host.LoadInstance();
  });
}

void UniversalTestInstance::DetachRootView() noexcept /*override*/ {
  m_host.UnloadInstance().get();
  m_queueController.ShutdownQueueAsync().get();
}

std::shared_ptr<facebook::react::Instance> UniversalTestInstance::GetInnerInstance() const noexcept /*override*/ {
  return nullptr;
}

#pragma endregion ITestInstance

} // namespace Microsoft::React::Test
