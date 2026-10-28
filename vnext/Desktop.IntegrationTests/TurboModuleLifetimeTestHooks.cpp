// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "pch.h"
#include <IReactContext.h>
#include <JsiApi.h>
#include <ReactHost/MsoReactContext.h>
#include <TurboModulesProvider.h>
#include <jsi/jsi.h>
#include <winrt/Windows.Foundation.h>

namespace {

namespace React = winrt::Microsoft::ReactNative;

class LifetimeTestContext final : public Mso::UnknownObject<Mso::React::IReactContext> {
 public:
  explicit LifetimeTestContext(facebook::jsi::Runtime &runtime)
      : m_context(Mso::Make<Mso::React::ReactContext>(
            nullptr, m_properties, React::ReactNotificationServiceHelper::CreateNotificationService())),
        // The test owns the runtime; its ABI wrapper must not extend that lifetime.
        m_runtime(React::implementation::JsiRuntime::GetOrCreate(
            nullptr, std::shared_ptr<facebook::jsi::Runtime>{std::shared_ptr<facebook::jsi::Runtime>{}, &runtime})) {}

  React::IReactNotificationService Notifications() const noexcept override {
    return m_context->Notifications();
  }

  React::IReactPropertyBag Properties() const noexcept override {
    return m_context->Properties();
  }

  void CallJSFunction(std::string &&module, std::string &&method, folly::dynamic &&params) const noexcept override {
    m_context->CallJSFunction(std::move(module), std::move(method), std::move(params));
  }

  void DispatchEvent(int64_t viewTag, std::string &&eventName, folly::dynamic &&eventData) const noexcept override {
    m_context->DispatchEvent(viewTag, std::move(eventName), std::move(eventData));
  }

  React::JsiRuntime JsiRuntime() const noexcept override {
    return m_runtime;
  }

  Mso::React::ReactInstanceState State() const noexcept override {
    return m_context->State();
  }

  bool IsLoaded() const noexcept override {
    return m_context->IsLoaded();
  }

  std::shared_ptr<facebook::react::Instance> GetInnerInstance() const noexcept override {
    return m_context->GetInnerInstance();
  }

  Mso::React::IReactSettingsSnapshot const &SettingsSnapshot() const noexcept override {
    return m_context->SettingsSnapshot();
  }

 private:
  React::IReactPropertyBag m_properties{React::ReactPropertyBagHelper::CreatePropertyBag()};
  Mso::CntPtr<Mso::React::IReactContext> m_context;
  React::JsiRuntime m_runtime;
};

} // namespace

// Compiled into react-native-win32.dll only for EnableV8LifetimeTests builds.
extern "C" __declspec(dllexport) HRESULT __cdecl RNWCreateTurboModuleForLifetimeTest(
    facebook::jsi::Runtime *runtime,
    facebook::jsi::Object *result) {
  namespace React = winrt::Microsoft::ReactNative;
  React::TurboModulesProvider provider;
  auto innerContext = Mso::Make<LifetimeTestContext>(*runtime);
  provider.SetReactContext(winrt::make<React::implementation::ReactContext>(std::move(innerContext)));
  provider.AddModuleProvider(
      L"LifetimeTest",
      [](React::IReactModuleBuilder const &builder) -> winrt::Windows::Foundation::IInspectable {
        builder.AddSyncMethod(
            L"value", [](React::IJSValueReader const &, React::IJSValueWriter const &writer) { writer.WriteInt64(42); });
        return winrt::box_value(true);
      },
      false);
  auto module = provider.getModule("LifetimeTest", nullptr);
  if (!module) {
    return E_FAIL;
  }
  *result = facebook::jsi::Object::createFromHostObject(*runtime, std::move(module));
  return S_OK;
}
