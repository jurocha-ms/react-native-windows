//TODO: copyright

#include <CppUnitTest.h>

#include <Microsoft.ReactNative/ReactHost/React.h>

#include <Microsoft.ReactNative/IReactDispatcher.h>

#include <future>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace msrn = winrt::Microsoft::ReactNative;

struct TestUIDispatcher : public winrt::implements<TestUIDispatcher, msrn::IReactDispatcher>
{
  TestUIDispatcher() = default;

  TestUIDispatcher(::Mso::DispatchQueue &&queue) : m_queue{std::move(queue)}
  {
    m_queue.Post([self = get_strong()]() noexcept
    {
      self->m_threadId = GetCurrentThreadId();
    });
  }

  bool HasThreadAccess()
  {
    return m_threadId == GetCurrentThreadId();
  }

  void Post(msrn::ReactDispatcherCallback const& callback)
  {
    m_queue.Post([callback]() noexcept
    {
      callback();
    });
  }

private:
  Mso::DispatchQueue m_queue;
  DWORD m_threadId{0};
};

msrn::ReactPropertyId<winrt::hstring> PlatformNameOverrideProperty() noexcept {
  static msrn::ReactPropertyId<winrt::hstring> prop{L"ReactNative.Injection", L"PlatformNameOverride"};
  return prop;
}

TEST_CLASS (Prototype) {

  TEST_METHOD(Proto1)
  {
    bool succeeded = true;

    {
      msrn::IReactDispatcher dispatcher =
          winrt::make<TestUIDispatcher>(Mso::DispatchQueue::MakeSerialQueue());

      msrn::ReactNativeHost host{};

      auto settings = host.InstanceSettings();
      settings.Properties().Set(msrn::ReactDispatcherHelper::UIDispatcherProperty(), dispatcher);
      settings.Properties().Set(PlatformNameOverrideProperty().Handle(), winrt::box_value(L"windows"));
      settings.UseFastRefresh(true);
      settings.JavaScriptBundleFile(L"IntegrationTests/IntegrationTestsApp");


      auto action = host.ReloadInstance();

      action.get();
    }

    Assert::IsTrue(succeeded);
  }

};
