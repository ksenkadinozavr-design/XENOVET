#pragma once

#include <cstdint>

namespace xenovent::telemetry {

enum class EventType : uint8_t {
  AppStartup,
  StorageLoad,
  StorageSave,
  StorageMigration,
  StorageRecovery,
  SensorHealth,
  InputInterpreted,
  UiTransition,
  ActionApplied,
  TickProcessed,
  MutationOccurred,
  DeathReset,
  WarningRaised,
  CooldownStarted,
  CooldownEnded,
  RenderFrame,
};

struct Event {
  EventType type = EventType::AppStartup;
  uint32_t timestampMs = 0;
  int32_t valueA = 0;
  int32_t valueB = 0;
  const char* message = "";
};

}  // namespace xenovent::telemetry
