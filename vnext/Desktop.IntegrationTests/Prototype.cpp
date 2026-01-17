//TODO: copyright

#include <CppUnitTest.h>

#include <Microsoft.ReactNative/ReactHost/React.h>

#include <future>

winrt::Windows::Foundation::IAsyncOperation<bool> Foo()
{
  bool result = false;
  winrt::Microsoft::ReactNative::ReactNativeHost host{};
  auto settings = host.InstanceSettings();
  //auto action = host.ReloadInstance();

  //action.Completed([&result](auto const &task, winrt::Windows::Foundation::AsyncStatus status)
  //{
  //  result = true;
  //});
  co_await host.ReloadInstance();
  result = true;

  co_return result;
}

TEST_CLASS (Prototype) {

  TEST_METHOD(Proto1)
  {
    bool succeeded = true;

    {
      winrt::Microsoft::ReactNative::ReactNativeHost host{};
      auto settings = host.InstanceSettings();
      auto mainQueue = Mso::DispatchQueue::CurrentQueue();
      auto action = host.ReloadInstance();

      action.get();
    }

    Microsoft::VisualStudio::CppUnitTestFramework::Assert::IsTrue(succeeded);
  }

};
