#include <windows.h>
#include <MinHook.h>
#include <rlvr_util.h>
#include <rlvr_ovrtypes.h>
#include <fstream>
#include <vector>

// Disables DLL validation inside OVRService.
char DllValidateHook(LPCWSTR file_name, HANDLE** handles) {
  if (!handles || !file_name) return 2;
  auto file_handle = CreateFileW(file_name, 0x80000000, 1, 0, 3, 1, 0);
  if (file_handle == INVALID_HANDLE_VALUE) return 3;
  handles[0] = &file_handle;
  return 0; // NoError(?)
}

int WINAPI main() {
  if (MH_Initialize() != MH_OK) {
    MessageBoxW(nullptr, L"MinHook initialization failed.", L"svchook", MB_ICONERROR | MB_OK);
    return -1;
  }
  auto dll_validate_addr = (void*)rlvr::Util::FindPattern(
    "\x4C\x8B\xDC\x55\x00\x49\x8D\xAB\x00\xF9\xFF\xFF\x48\x81\xEC\x00\x07\x00\x00\x48\x8B\x05\x00\x00\x00\x00",
    "xxxx?xxx?xxxxxx?xxxxxx???x"
  );
  if (!dll_validate_addr) {
    MessageBoxW(nullptr, L"Finding dll_validate_addr pattern failed.", L"svchook", MB_ICONERROR | MB_OK);
    return -1;
  }
  MH_CreateHook(dll_validate_addr, (void*)DllValidateHook, nullptr);
  MH_EnableHook(dll_validate_addr);
  return 0;
}

extern "C" FARPROC GetFileVersionInfoA_orig = 0;
extern "C" FARPROC GetFileVersionInfoByHandle_orig = 0;
extern "C" FARPROC GetFileVersionInfoExA_orig = 0;
extern "C" FARPROC GetFileVersionInfoExW_orig = 0;
extern "C" FARPROC GetFileVersionInfoSizeA_orig = 0;
extern "C" FARPROC GetFileVersionInfoSizeExA_orig = 0;
extern "C" FARPROC GetFileVersionInfoSizeExW_orig = 0;
extern "C" FARPROC GetFileVersionInfoSizeW_orig = 0;
extern "C" FARPROC GetFileVersionInfoW_orig = 0;
extern "C" FARPROC VerFindFileA_orig = 0;
extern "C" FARPROC VerFindFileW_orig = 0;
extern "C" FARPROC VerInstallFileA_orig = 0;
extern "C" FARPROC VerInstallFileW_orig = 0;
extern "C" FARPROC VerLanguageNameA_orig = 0;
extern "C" FARPROC VerLanguageNameW_orig = 0;
extern "C" FARPROC VerQueryValueA_orig = 0;
extern "C" FARPROC VerQueryValueW_orig = 0;

extern "C" void GetFileVersionInfoA_svchook();
extern "C" void GetFileVersionInfoByHandle_svchook();
extern "C" void GetFileVersionInfoExA_svchook();
extern "C" void GetFileVersionInfoExW_svchook();
extern "C" void GetFileVersionInfoSizeA_svchook();
extern "C" void GetFileVersionInfoSizeExA_svchook();
extern "C" void GetFileVersionInfoSizeExW_svchook();
extern "C" void GetFileVersionInfoSizeW_svchook();
extern "C" void GetFileVersionInfoW_svchook();
extern "C" void VerFindFileA_svchook();
extern "C" void VerFindFileW_svchook();
extern "C" void VerInstallFileA_svchook();
extern "C" void VerInstallFileW_svchook();
extern "C" void VerLanguageNameA_svchook();
extern "C" void VerLanguageNameW_svchook();
extern "C" void VerQueryValueA_svchook();
extern "C" void VerQueryValueW_svchook();

bool DllInit() {
  wchar_t sys_dir[MAX_PATH];
  if (!GetSystemDirectoryW(sys_dir, MAX_PATH)) return false;
  wchar_t dll_path[MAX_PATH];
  wsprintfW(dll_path, L"%s/%s", sys_dir, L"version.dll");
  auto dll_handle = LoadLibraryW(dll_path);
  if (dll_handle) {
    GetFileVersionInfoA_orig = GetProcAddress(dll_handle, "GetFileVersionInfoA");
    GetFileVersionInfoByHandle_orig = GetProcAddress(dll_handle, "GetFileVersionInfoByHandle");
    GetFileVersionInfoExA_orig = GetProcAddress(dll_handle, "GetFileVersionInfoExA");
    GetFileVersionInfoExW_orig = GetProcAddress(dll_handle, "GetFileVersionInfoExW");
    GetFileVersionInfoSizeA_orig = GetProcAddress(dll_handle, "GetFileVersionInfoSizeA");
    GetFileVersionInfoSizeExA_orig = GetProcAddress(dll_handle, "GetFileVersionInfoSizeExA");
    GetFileVersionInfoSizeExW_orig = GetProcAddress(dll_handle, "GetFileVersionInfoSizeExW");
    GetFileVersionInfoSizeW_orig = GetProcAddress(dll_handle, "GetFileVersionInfoSizeW");
    GetFileVersionInfoW_orig = GetProcAddress(dll_handle, "GetFileVersionInfoW");
    VerFindFileA_orig = GetProcAddress(dll_handle, "VerFindFileA");
    VerFindFileW_orig = GetProcAddress(dll_handle, "VerFindFileW");
    VerInstallFileA_orig = GetProcAddress(dll_handle, "VerInstallFileA");
    VerInstallFileW_orig = GetProcAddress(dll_handle, "VerInstallFileW");
    VerLanguageNameA_orig = GetProcAddress(dll_handle, "VerLanguageNameA");
    VerLanguageNameW_orig = GetProcAddress(dll_handle, "VerLanguageNameW");
    VerQueryValueA_orig = GetProcAddress(dll_handle, "VerQueryValueA");
    VerQueryValueW_orig = GetProcAddress(dll_handle, "VerQueryValueW");
    return true;
  }
  return false;
}

bool APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
  int result = -1;
  if (dwReason == DLL_PROCESS_ATTACH) {
    if (!DllInit()) {
      MessageBoxW(nullptr, L"DLL initialization failed.", L"svchook", MB_ICONERROR | MB_OK);
      return false;
    }
    result = main();
  }
  return result == 0;
}
