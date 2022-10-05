//clang-format off

#include "../Shared/Networking/IHttpResource.h"
#include <winrt/base.h>
#include <stdio.h>
#include <future>
#include <Windows.h>
#include <sddl.h>

using namespace Microsoft::React::Networking;

using std::promise;
using std::string;

// https://learn.microsoft.com/en-us/previous-versions/dotnet/articles/bb625960(v=msdn.10)?redirectedfrom=MSDN
BOOL CreateLowProcess()
{
  BOOL fRet;
  HANDLE                hToken        = NULL;
  HANDLE                hNewToken     = NULL;
  PSID                  pIntegritySid = NULL;
  TOKEN_MANDATORY_LABEL TIL           = {0};
  PROCESS_INFORMATION   ProcInfo      = {0};
  STARTUPINFO           StartupInfo   = {0};

  // Notepad is used as an example
  WCHAR wszProcessName[MAX_PATH] = L"C:\\Windows\\System32\\Notepad.exe";

  // Low integrity SID
  WCHAR wszIntegritySid[20] = L"S-1-16-1024";
  //PSID pIntegritySid = NULL;

  fRet = OpenProcessToken(GetCurrentProcess(),
                          TOKEN_DUPLICATE |
                          TOKEN_ADJUST_DEFAULT |
                          TOKEN_QUERY |
                          TOKEN_ASSIGN_PRIMARY,
                          & hToken);

  if (!fRet)
  {
    goto CleanExit;
  }

  fRet = DuplicateTokenEx(hToken,
                          0,
                          NULL,
                          SecurityImpersonation,
                          TokenPrimary,
                          & hNewToken);

  if (!fRet)
  {
    goto CleanExit;
  }

  fRet = ConvertStringSidToSid(wszIntegritySid, &pIntegritySid);

  if (!fRet)
  {
    goto CleanExit;
  }

  TIL.Label.Attributes = SE_GROUP_INTEGRITY;
  TIL.Label.Sid         = pIntegritySid;

  //
  // Set the process integrity level
  //

  fRet = SetTokenInformation(hNewToken,
                              TokenIntegrityLevel,
                              & TIL,
                              sizeof(TOKEN_MANDATORY_LABEL) + GetLengthSid(pIntegritySid));

  if (!fRet)
  {
    goto CleanExit;
  }

  //
  // Create the new process at Low integrity
  //

  fRet  = CreateProcessAsUser(hNewToken,
                              NULL,
                              wszProcessName,
                              NULL,
                              NULL,
                              FALSE,
                              0,
                              NULL,
                              NULL,
                              & StartupInfo,
                              & ProcInfo);

CleanExit:
  if (ProcInfo.hProcess != NULL)
  {
    CloseHandle(ProcInfo.hProcess);
  }

  if (ProcInfo.hThread != NULL)
  {
    CloseHandle(ProcInfo.hThread);
  }

  LocalFree(pIntegritySid);

  if (hNewToken != NULL)
  {
    CloseHandle(hNewToken);
  }

  if (hToken != NULL)
  {
    CloseHandle(hToken);
  }

  return fRet;
}

int main()
{
#if 0
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

#else
  auto ans = CreateLowProcess();
#endif // 0
}
