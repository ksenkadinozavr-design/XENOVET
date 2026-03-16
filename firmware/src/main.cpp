#include <Arduino.h>

#include "drivers/adxl345_manager.h"
#include "drivers/bh1750_manager.h"
#include "game/game_engine.h"
#include "input/button_manager.h"
#include "output/output_manager.h"
#include "storage/state_storage.h"
#include "telemetry/event_bus.h"
#include "telemetry/recorder.h"
#include "ui/renderer.h"
#include "ui/ui_fsm.h"
#include "utils/logger.h"

using namespace xenovent;

namespace {
storage::StateStorage gStorage;
ui::Renderer gRenderer;
input::ButtonManager gButtons;
drivers::Adxl345Manager gAdxl;
drivers::Bh1750Manager gBh;
output::OutputManager gOutput;
ui::UiFsm gFsm;
telemetry::EventBus gBus;
game::GameEngine gEngine(gStorage, gRenderer, gButtons, gAdxl, gBh, gOutput, gFsm, gBus);
}  // namespace

void setup() {
  utils::initLogger();
  utils::setLogLevel(utils::LogLevel::Info);
  gBus.subscribe(telemetry::globalRecorderSink);
  gEngine.begin();
}

void loop() { gEngine.update(); }
