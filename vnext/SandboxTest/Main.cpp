#include "../Shared/Networking/IHttpResource.h"
#include <winrt/base.h>
#include <stdio.h>
#include <future>
#include <stdlib.h>

using namespace Microsoft::React::Networking;

using std::promise;
using std::string;

int main(int argc, char ** argv)
{
  winrt::init_apartment();

  string errorMessage;
  int64_t status;
  string content;
  promise<void> resPromise;
  auto rc = IHttpResource::Make();

  rc->SetOnResponse([&status](int64_t, IHttpResource::Response res) {
    status = res.StatusCode;
  });

  rc->SetOnData([&resPromise, &content](int64_t, string &&data) {
    content = std::move(data);
    resPromise.set_value();
  });

  rc->SetOnError([&errorMessage, &resPromise](int64_t, string &&message, bool)
  {
    errorMessage = std::move(message);
    resPromise.set_value();
  });

  rc->SendRequest(
    "GET",
    "http://help.websiteos.com",
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

  if (errorMessage.size()) {
    printf("[FAIL]\n");
    printf("%s\n", errorMessage.c_str());
  } else {
    printf("[SUCCESS]\n");
    printf("%d: %s\n", status, content.c_str());
  }
}
