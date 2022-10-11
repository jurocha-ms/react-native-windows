#include "../Shared/Networking/IHttpResource.h"
#include <winrt/base.h>
#include <winrt/Windows.Web.Http.h>
#include <stdio.h>
#include <future>
#include <stdlib.h>

using namespace Microsoft::React::Networking;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Web::Http;

using std::promise;
using std::string;

void RequestWithRnw()
{
  string errorMessage;
  int64_t status;
  string content;
  promise<void> resPromise;
  auto rc = IHttpResource::Make();

  rc->SetOnResponse([&status](int64_t, IHttpResource::Response res) { status = res.StatusCode; });

  rc->SetOnData([&resPromise, &content](int64_t, string &&data) {
    content = std::move(data);
    resPromise.set_value();
  });

  rc->SetOnError([&errorMessage, &resPromise](int64_t, string &&message, bool) {
    errorMessage = std::move(message);
    resPromise.set_value();
  });

  rc->SendRequest("GET", "http://help.websiteos.com", 0, {}, {}, "text", false, 0, false, [](int64_t) {});

  resPromise.get_future().wait();

  if (errorMessage.size()) {
    printf("\n[FAIL]\n");
    printf("%s\n", errorMessage.c_str());
  } else {
    printf("\n[SUCCESS]\n");
    printf("%d: %s\n", status, content.c_str());
  }
}

winrt::Windows::Foundation::IAsyncAction GetRequest()
{
  auto client = HttpClient{};

  HttpResponseMessage response;
  try {
    response = co_await client.GetAsync(Uri{L"https://raw.githubusercontent.com/microsoft/react-native-windows/main/.yarnrc.yml"});
    auto body = co_await response.Content().ReadAsStringAsync();

    wprintf(L"HTTP STATUS [%d]:\n\n%s\n", response.StatusCode(), body.c_str());
  } catch (const winrt::hresult_error &e) {
    wprintf(L"[%x] %s\n", e.code(), e.message());
  }
}

int main(int argc, char ** argv)
{
  winrt::init_apartment();

  GetRequest().get();
}
