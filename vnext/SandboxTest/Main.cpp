//clang-format off

#include "../Shared/Networking/IHttpResource.h"
#include <winrt/base.h>
#include <stdio.h>
#include <future>

using namespace Microsoft::React::Networking;

using std::promise;
using std::string;

int main()
{
  winrt::init_apartment();

  string errorMessage;
  promise<void> resPromise;
  auto rc = IHttpResource::Make();

  rc->SetOnResponse([](int64_t, IHttpResource::Response res)
  {
    int x = 88;
  });

  rc->SetOnData([&resPromise](int64_t, string &&content)
  {
    auto x = content.size();
    resPromise.set_value();
  });

  rc->SetOnError([&errorMessage, &resPromise](int64_t, string &&message, bool)
  {
    errorMessage = std::move(message);
    resPromise.set_value();
  });

  rc->SendRequest(
    "GET",
    "http://help.websiteos.com/websiteos/",
    0,
    {},
    {},
    "text",
    false,
    0,
    false,
    [](int64_t) {}
  );

  resPromise.get_future().wait();

  printf("[%s]\n", errorMessage.c_str());
}
