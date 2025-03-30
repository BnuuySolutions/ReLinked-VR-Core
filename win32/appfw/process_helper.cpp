#include "process_helper.h"
#include <nlohmann/json.hpp>
#include <filesystem>
#include <windows.h>
#include <psapi.h>

using rlvr_util = rlvr::Util;
using appfw_util = rlvr::appfw::Util;

namespace fs = std::filesystem;

HANDLE rlvr::appfw::ProcessHelper::_process_handle = nullptr;
HANDLE rlvr::appfw::ProcessHelper::_thread_handle = nullptr;

bool rlvr::appfw::ProcessHelper::IsProcessVRServer(uint32_t pid) {
  HANDLE process_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, pid);
  if (process_handle) {
    wchar_t buf[MAX_PATH];
    if (GetModuleFileNameExW(process_handle, 0, buf, MAX_PATH)) {
      if (wcsstr(buf, L"vrserver.exe")) {
        return true;
      }
    }
  }
  return false;
}

void rlvr::appfw::ProcessHelper::RunSteamVRStartup() {
  if (appfw_util::IsProcessOpen(_process_handle)) {
    return;
  }

  std::wstring appdata_dir = rlvr::Util::GetLocalAppDataDir();
  std::wstring openvrpaths_path = rlvr::Util::GetConcatPath_Utf16(appdata_dir, L"openvr\\openvrpaths.vrpath");
  if (fs::exists(openvrpaths_path)) {
    json openvrpaths_json = rlvr::appfw::Util::GetJSONFile(openvrpaths_path);
    json openvrpaths_runtimes = openvrpaths_json["runtime"];
    if (!openvrpaths_runtimes.is_null() && openvrpaths_runtimes.is_array() && openvrpaths_runtimes.size() > 0) {
      std::string openvrpaths_runtime = openvrpaths_runtimes[0];
      std::string vrstartup_path = rlvr::Util::GetConcatPath_Utf8(openvrpaths_runtime, "bin\\win64\\vrstartup.exe");
      if (fs::exists(vrstartup_path)) {
        STARTUPINFOA startup_info{};
        PROCESS_INFORMATION process_info{};
        if (CreateProcessA(vrstartup_path.c_str(),
                           nullptr,
                           nullptr,
                           nullptr,
                           false,
                           0,
                           nullptr,
                           nullptr,
                           &startup_info,
                           &process_info))
        {
          _process_handle = process_info.hProcess;
          _thread_handle = process_info.hThread;
        }
      }
    }
  }
}

void rlvr::appfw::ProcessHelper::RunProcess(std::string path) {
  if (appfw_util::IsProcessOpen(_process_handle)) {
    return;
  }

  STARTUPINFOA startup_info{};
  PROCESS_INFORMATION process_info{};
  if (CreateProcessA(path.c_str(),
                     nullptr,
                     nullptr,
                     nullptr,
                     false,
                     0,
                     nullptr,
                     nullptr,
                     &startup_info,
                     &process_info)) {
    _process_handle = process_info.hProcess;
    _thread_handle = process_info.hThread;
  }
}
