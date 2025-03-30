#pragma once

#include <cstdint>
#include <format>

// runtime ver 0 = v0.0.1 (initial release)
// runtime ver 1 = v0.0.2
// runtime ver 2 = v0.0.3
#define RUNTIME_VER 2

#define OAF_SERVICE_LOG(category, level, ...) OafService_Log(category, level, std::format(__VA_ARGS__).c_str());

#if (RUNTIME_VER == 0)
// Legacy runtime
enum oafServiceType {
  oafService_ChangeFocus = 3,
  oafService_ChangeInputFocus = 5,
  oafService_SetModalSystemOverlay = 7,
  oafService_SetDepthBuffersRequestStatus = 9,
  oafService_ActivateHeadset = 24,
  oafService_Log = -9999, // dummy.
  oafService_SetTrackingMode = 46,
  oafService_SetAirLinkPairingResult = 86
};

enum oafCallbackType {
  oafCallback_OafServer_TestConnection = 0,
  oafCallback_OafServer_UploadCrashReports = 1,
  oafCallback_OafServer_Run = 2,
  oafCallback_OafServer_TearDown = 3,
  oafCallback_FetchGatekeeper = 6,
  oafCallback_NewProcess = 7,
  oafCallback_GetFPSSettingForHeadset = 9,
  oafCallback_Shutdown = 15,
  oafCallback_InputEvent = 16,
  oafCallback_HMDEvent = 18,
  oafCallback_SetDefaultHeadset = 59,
  oafCallback_NotifyAirLinkPairingStart = 70
};
#elif (RUNTIME_VER == 1)
// Binary IDs:
// oculus-runtime: 3056908297774642
// oculus-drivers: 3681864058615858
// oculus-diagnostics: 7931351073640758

enum oafServiceType {
  oafService_ChangeFocus = 3,
  oafService_ChangeInputFocus = 5,
  oafService_SetModalSystemOverlay = 7,
  oafService_SetDepthBuffersRequestStatus = 9,
  oafService_ActivateHeadset = 24,
  oafService_Log = 46,
  oafService_SetTrackingMode = 48,
  oafService_SetAirLinkPairingResult = 88
};

enum oafCallbackType {
  oafCallback_OafServer_TestConnection = 0,
  oafCallback_OafServer_UploadCrashReports = 1,
  oafCallback_OafServer_Run = 2,
  oafCallback_OafServer_TearDown = 3,
  oafCallback_FetchGatekeeper = 6,
  oafCallback_NewProcess = 7,
  oafCallback_GetFPSSettingForHeadset = 9,
  oafCallback_Shutdown = 15,
  oafCallback_InputEvent = 16,
  oafCallback_HMDEvent = 18,
  oafCallback_SetDefaultHeadset = 59,
  oafCallback_NotifyAirLinkPairingStart = 72,
  oafCallback_ConnectIpcRemoteHeadsetClient = 75
};
#elif (RUNTIME_VER == 2)
// Binary IDs:
// oculus-runtime: 3056908297774642
// oculus-drivers: 3681864058615858
// oculus-diagnostics: 7931351073640758

enum oafServiceType {
  oafService_ChangeFocus = 3,
  oafService_ChangeInputFocus = 5,
  oafService_SetModalSystemOverlay = 7,
  oafService_SetDepthBuffersRequestStatus = 9,
  oafService_ActivateHeadset = 24,
  oafService_Log = 46,
  oafService_SetTrackingMode = 48,
  oafService_SetAirLinkPairingResult = 88
};

enum oafCallbackType {
  oafCallback_OafServer_TestConnection = 0,
  oafCallback_OafServer_UploadCrashReports = 1,
  oafCallback_OafServer_Run = 2,
  oafCallback_OafServer_TearDown = 3,
  oafCallback_FetchGatekeeper = 6,
  oafCallback_NewProcess = 7,
  oafCallback_GetFPSSettingForHeadset = 9,
  oafCallback_Shutdown = 15,
  oafCallback_InputEvent = 16,
  oafCallback_HMDEvent = 18,
  oafCallback_SetDefaultHeadset = 59,
  oafCallback_NotifyAirLinkPairingStart = 71,
  oafCallback_ConnectIpcRemoteHeadsetClient = 74
};
#endif

enum oafLogLevelType {
  OAF_LOG_LEVEL_TYPE_DISABLED = 0,
  OAF_LOG_LEVEL_TYPE_TRACE = 1,
  OAF_LOG_LEVEL_TYPE_DEBUG = 2,
  OAF_LOG_LEVEL_TYPE_INFO = 3,
  OAF_LOG_LEVEL_TYPE_WARNING = 4,
  OAF_LOG_LEVEL_TYPE_ERROR = 5
};

const char* oafLogLevelType_ToString(oafLogLevelType log_level_type);

enum oafInputEventType {
  // NOTE(Kaitlyn): Is this correct?
  // This event also has a sub-category of events, seehere: oafInputControllerEventType
  // I'm unaware as to if this is the correct way of doing this, but oh well. We'll find out.
  OAF_INPUT_EVENT_TYPE_CONTROLLER = 1,
  OAF_INPUT_EVENT_TYPE_PROXIMITY_SENSOR = 3,
  OAF_INPUT_EVENT_TYPE_DEVICE_MOTION = 4
};

const char* oafInputEventType_GetName(oafInputEventType event_type);

// Is this also even correct?
enum oafInputControllerEventType {
  OAF_INPUT_CONTROLLER_EVENT_TYPE_OCULUS_BUTTON = 1
};

const char* oafInputControllerEventType_GetName(oafInputControllerEventType event_type);

enum oafHMDEventType {
  OAF_HMD_EVENT_TYPE_DISPLAY_ATTACHED = 1,
  OAF_HMD_EVENT_TYPE_DISPLAY_DETACHED = 2,

  OAF_HMD_EVENT_TYPE_HMD_ATTACHED = 3,
  OAF_HMD_EVENT_TYPE_HMD_AVAILABLE = 4, // TODO(Kaitlyn): Is this correct?
  OAF_HMD_EVENT_TYPE_HMD_DETACHED = 5,

  OAF_HMD_EVENT_TYPE_IAD_CHANGED = 9 // Fired when IPD is changed.
};

const char* oafHMDEventType_GetName(oafHMDEventType event_type);

struct oafFPSSettingForHeadset {
  uint32_t fps;
  bool use_fps_setting;
};

namespace rlvr {
namespace oaf_service {

extern int32_t (*OafService_ChangeFocus)(uint32_t process_id);
extern int32_t (*OafService_ChangeInputFocus)(uint32_t process_id, uint64_t a2);
extern int32_t (*OafService_SetModalSystemOverlay)(bool value);
extern int32_t (*OafService_SetDepthBuffersRequestStatus)(bool value);
extern int32_t (*OafService_ActivateHeadset)(const char serial_number[16], int32_t a2);
extern void (*OafService_Log)(const char* category, oafLogLevelType log_level_type, const char* msg);
extern uint8_t (*OafService_SetTrackingMode)(bool value);
extern uint8_t (*OafService_SetAirLinkPairingResult)(bool value, const char serial_number[16]);

void OafService_Init(void** functions);

}

class IOafCallbacks {
public:
  static IOafCallbacks* oaf_callbacks;

  virtual void OafServer_Run() = 0; // Blocks OVRServer thread when called, calls TearDown when complete.
  virtual void OafServer_TearDown() = 0;
  virtual uint8_t FetchGatekeeper(const char* gk_name) = 0;
  virtual void NewProcess(uint32_t process_id) = 0; // PID is Win32 process ID.
  virtual void GetFPSSettingForHeadset(oafFPSSettingForHeadset* out_value, const char* serial_number) = 0;
  virtual void Shutdown() = 0; // Shuts down OafServer in the process.
  virtual void InputEvent(const char* serial_number, oafInputEventType event_type, oafInputControllerEventType controller_event_type, bool value) = 0;
  virtual void HMDEvent(const char* serial_number, oafHMDEventType event_type) = 0; // There is another argument at the end, dunno what it is yet.
  virtual void SetDefaultHeadset(const char* serial_number) = 0;
  virtual void NotifyAirLinkPairingStart(const char* pairing_code, const char* serial_number) = 0;
  virtual uint8_t ConnectIpcRemoteHeadsetClient() = 0;

  static void Init(void** callbacks);
};

}
