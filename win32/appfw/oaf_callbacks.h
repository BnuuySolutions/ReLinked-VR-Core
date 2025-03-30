#pragma once

#include <nlohmann/json.hpp>
#include <rlvr_oaf.h>
#include <string>
#include <vector>

namespace rlvr {
namespace appfw {

class OafCallbacks : public rlvr::IOafCallbacks {
private:
  nlohmann::json _rlvrsettings_json;
  bool _enable_software_autostart = true;
  bool _allow_other_software = false;
  std::string _custom_startup_program_path;
  bool _auto_high_priority = true;
  bool _use_custom_fps = true;
  int _custom_fps = 90;
  bool _is_rlvrsettings_loaded = false;

  std::vector<std::string> _unhandled_gks;
  std::vector<std::string> _enabled_gks;

public:
  OafCallbacks();

  void OafServer_Run() override;
  void OafServer_TearDown() override;
  uint8_t FetchGatekeeper(const char* gk_name) override;
  void NewProcess(uint32_t process_id) override;
  void GetFPSSettingForHeadset(oafFPSSettingForHeadset* out_value, const char* serial_number) override;
  void Shutdown() override;
  void InputEvent(const char* serial_number, oafInputEventType event_type, oafInputControllerEventType controller_event_type, bool value) override;
  void HMDEvent(const char* serial_number, oafHMDEventType event_type) override;
  void SetDefaultHeadset(const char* serial_number) override;
  void NotifyAirLinkPairingStart(const char* pairing_code, const char* serial_number) override;
  uint8_t ConnectIpcRemoteHeadsetClient() override;
};

}
}
