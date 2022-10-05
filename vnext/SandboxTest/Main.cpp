#include "../Shared/Networking/IHttpResource.h"
#include <winrt/base.h>
#include <stdio.h>
#include <future>
#include <Windows.h>
#include <sddl.h>

using namespace Microsoft::React::Networking;

using std::promise;
using std::string;
using std::wstring;

static constexpr const WCHAR *swzAdfLowILSandboxSid = L"S-1-16-4096";

PSID pSIDSandbox;
PROCESS_INFORMATION processInfoSandbox;
HRESULT CreateLowILProcess() noexcept
{
  wstring commandLine = L"C:\\Windows\\System32\\notepad.exe";
  commandLine = L"R:\\lowil\\vnext\\target\\x64\\Debug\\SandboxTest\\SandboxTest.exe SOMEARG";
  wstring wstrEventSyncName =
      L"60758A28-A98E-47F8-84D8-795A8D5A0C54";

  BOOL fSetToken = FALSE;
  HANDLE hProcTokenLowIl;
  HANDLE hMICToken;
  BOOL fOpenProcToken = OpenProcessToken(GetCurrentProcess(), MAXIMUM_ALLOWED, &hProcTokenLowIl);
  {
    BOOL fConvertSid = TRUE;
    BOOL fDuplicateToken = DuplicateTokenEx(
      hProcTokenLowIl,
      MAXIMUM_ALLOWED,
      NULL,
      SecurityImpersonation,
      TokenImpersonation,
      &hMICToken);

    if (fDuplicateToken) {
      if (pSIDSandbox == nullptr) {
        fConvertSid = ConvertStringSidToSid(swzAdfLowILSandboxSid, &pSIDSandbox);
      }

      if (fConvertSid) {
        // Set Process IL to Low
        TOKEN_MANDATORY_LABEL TML = {0};
        TML.Label.Attributes = SE_GROUP_INTEGRITY | SE_GROUP_INTEGRITY_ENABLED;
        TML.Label.Sid = pSIDSandbox;

        BOOL fPILToken =
            SetTokenInformation(hMICToken, TokenIntegrityLevel, &TML, sizeof(TML) + GetLengthSid(pSIDSandbox));

        if (fPILToken) {
          fSetToken = SetThreadToken(nullptr, hMICToken);
        }
      } // if (fConvertSid)

      if (!fOpenProcToken || !fDuplicateToken || !fConvertSid || !fSetToken) {
        // FAIL
      }
    }
  } // Scope

  HANDLE hEventRemoter = ::CreateEvent(
      NULL /*lpEventAttributes*/, false /*bManualReset*/, false /*bInitialState*/, wstrEventSyncName.c_str());
  //TODO: check

  STARTUPINFOEX si = {};
  si.StartupInfo.cb = sizeof(si);

  const int32_t iCountRetry = 5;
  bool fCreateProcess = false;
  fCreateProcess = CreateProcessAsUser(
        hMICToken,
        nullptr,
        (LPWSTR)commandLine.c_str(),
        nullptr /*lpProcessAttributes*/,
        nullptr /*lpThreadAttributes*/,
        false /*bInheritHandles*/,
        0 /*EXTENDED_STARTUP_INFO_PRESENT*/,
        NULL /*lpEnvironment*/,
        NULL /*lpCurrentDirectory*/, (LPSTARTUPINFO)&si, &processInfoSandbox);

  if (!fCreateProcess || processInfoSandbox.hProcess == NULL || processInfoSandbox.hThread == NULL) {
    auto err = HRESULT_FROM_WIN32(GetLastError());
  }

  auto dwWaitStatus = WaitForSingleObject(hEventRemoter, 0);

  return S_OK;
}

int main(int argc, char ** argv)
{
  if (argc < 2) {
    auto ans = CreateLowILProcess();
  } else {
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
}
