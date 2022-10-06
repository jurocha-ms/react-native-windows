#include <stdio.h>
#include <Windows.h>
#include <sddl.h>
#include <string>
#include <stdlib.h>

#include <fwpmtypes.h>

using std::string;
using std::wstring;

static constexpr const WCHAR *swzAdfLowILSandboxSid = L"S-1-16-4096";
static constexpr const WCHAR *privNetSidStr =
  L"S-1-15-3-3";
  //L"S-1-15-3-4096";

PROCESS_INFORMATION processInfoSandbox;
PSID pSIDSandbox;
PSID privNetPsid;
HRESULT CreateLowILProcess() noexcept {
  //wstring commandLine = L"C:\\Windows\\System32\\notepad.exe";
  wstring commandLine = L"SandboxTest.exe";
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
        fConvertSid = ConvertStringSidToSid(privNetSidStr, &privNetPsid);
      }

      if (fConvertSid) {
        TOKEN_MANDATORY_LABEL privNetTml = {0};
        privNetTml.Label.Attributes = FWPM_APPC_NETWORK_CAPABILITY_INTERNET_PRIVATE_NETWORK;
        privNetTml.Label.Sid = privNetPsid;

        // https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/ntifs/ne-ntifs-_token_information_class
        //BOOL privNetSetToken = SetTokenInformation(
        //    hMICToken, TokenCapabilities, &privNetTml, sizeof(privNetTml) + GetLengthSid(privNetPsid));
        //auto err = HRESULT_FROM_WIN32(GetLastError());

        // Set Process IL to Low
        TOKEN_MANDATORY_LABEL TML = {0};
        TML.Label.Attributes = SE_GROUP_INTEGRITY | SE_GROUP_INTEGRITY_ENABLED
          //| SECURITY_CAPABILITY_PRIVATE_NETWORK_CLIENT_SERVER
          //| FWPM_APPC_NETWORK_CAPABILITY_INTERNET_PRIVATE_NETWORK
          ;
        TML.Label.Sid = pSIDSandbox;

        BOOL fPILToken =
            SetTokenInformation(hMICToken, TokenIntegrityLevel, &TML, sizeof(TML) + GetLengthSid(pSIDSandbox));

        if (fPILToken /*&& privNetSetToken*/) {
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
  auto ans = CreateLowILProcess();

  //system("PAUSE");
}
