#include <winrt/base.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Web.Http.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef WITH_RNW
#include <future>
#include "../Shared/Networking/IHttpResource.h"
#endif

using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Web::Http;

#ifdef WITH_RNW
				using namespace Microsoft::React::Networking;
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

  rc->SendRequest("GET", "https://raw.githubusercontent.com/microsoft/react-native-windows/main/.yarnrc.yml", 0, {}, {}, "text", false, 0, false, [](int64_t) {});

  resPromise.get_future().wait();

  if (errorMessage.size()) {
    printf("\nRNW [FAIL]\n");
    printf("%s\n", errorMessage.c_str());
  } else {
    printf("\nRNW [SUCCESS]\n");
    printf("%d: %s\n", status, content.c_str());
  }
}

#endif // WITH_RNW

winrt::Windows::Foundation::IAsyncAction GetRequest()
{
  try {
    auto client = HttpClient{};
    auto response = co_await client.GetAsync(
        Uri{L"https://raw.githubusercontent.com/microsoft/react-native-windows/main/.yarnrc.yml"});
    auto body = co_await response.Content().ReadAsStringAsync();

    wprintf(L"\nHTTP STATUS [%d]:\n\n%s\n", response.StatusCode(), body.c_str());
  } catch (const winrt::hresult_error &e) {
    auto m = e.message();
    wprintf(L"\n[0x%x] %s\n", static_cast<unsigned int>(e.code()), e.message().c_str());
  }
}

int main(int argc, char ** argv)
{
  winrt::init_apartment();

#ifdef WITH_RNW
  RequestWithRnw();
#else
  GetRequest().get();
#endif // WITH_RNW

  system("pause");
}
