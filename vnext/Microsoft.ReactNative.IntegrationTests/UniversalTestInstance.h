// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include <TestInstance.h>
#include <winrt/Microsoft.ReactNative.h>
#include <winrt/Windows.System.h>

namespace Microsoft::React::Test {

class UniversalTestInstance : public ITestInstance
{
  winrt::Microsoft::ReactNative::ReactNativeHost m_host;
  winrt::Windows::System::DispatcherQueueController m_queueController;

  public:
  UniversalTestInstance() noexcept;

#pragma region ITestInstance

   void AttachMeasuredRootView(std::string &&appName) noexcept override;
   void DetachRootView() noexcept override;
   std::shared_ptr<facebook::react::Instance> GetInnerInstance() const noexcept override;

#pragma endregion ITestInstance
};

}
