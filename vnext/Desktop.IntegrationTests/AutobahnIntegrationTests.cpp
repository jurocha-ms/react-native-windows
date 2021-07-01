// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

// clang-format off
#include <CppUnitTest.h>
#include <IWebSocketResource.h>
#include <future>

using namespace Microsoft::React;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using std::size_t;
using std::string;
using std::promise;

using CloseCode = IWebSocketResource::CloseCode;
using Error = IWebSocketResource::Error;

TEST_CLASS(AutobahnIntegrationTest)
{
  TEST_METHOD(RunCase)
  {
    // var ws = new WebSocket('ws://localhost:9001/runCase?case=1&agent=unknown');ws.binaryType = "arraybuffer";ws.onopen = (e) => { console.log("Executing test case"); };ws.conclose = (e) => { ws = null; console.log("All executed"); };ws.onmessage = (e) => { console.log(`Got data: ${e.data}`); ws.send(e.data); }; ws.onerror = (e) => { console.log('[FAIL]'); }

    promise<bool> finished;
    std::exception_ptr eptr;
    auto ws = IWebSocketResource::Make("ws://localhost:9001/runCase?agent=RNW&case=1");
    ws->SetOnConnect([]()
      {
        Logger::WriteMessage("[CONNECTED]");
      });
    ws->SetOnError([&finished, &eptr](Error&& e)
      {
        finished.set_exception(eptr);
        Logger::WriteMessage(string("[FAIL]" + e.Message).c_str());
      });
    ws->SetOnMessage([&ws](size_t length, string message, bool isBinary)
      {
        Logger::WriteMessage(string("[MESSAGE] {" + message + "}").c_str());

        if (isBinary)
          ws->SendBinary(std::move(message));
        else
          ws->Send(std::move(message));
      });
    ws->SetOnClose([&finished](CloseCode code, const string& reason)
      {
        finished.set_value_at_thread_exit(true);
        Logger::WriteMessage(string("[CLOSED] With reason {" + reason + "}").c_str());
      });

    ws->Connect();
    ws->Close(CloseCode::Normal, "Finished");

    auto future = finished.get_future();
    future.wait();
    auto success = future.get();
    if (eptr)
      success = false;

    Assert::IsTrue(success);
  }
};

// clang-format on
