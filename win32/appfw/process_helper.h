#pragma once

#include <rlvr_util.h>
#include "util.h"

namespace rlvr {
namespace appfw {

class ProcessHelper {
private:
  static HANDLE _process_handle;
  static HANDLE _thread_handle;

public:
  static bool IsProcessVRServer(uint32_t pid);
  static void RunSteamVRStartup();
  static void RunProcess(std::string path);
};

}
}
