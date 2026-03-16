#pragma once

#include <cstdint>

#include "domain/types.h"
#include "drivers/adxl345_manager.h"
#include "drivers/bh1750_manager.h"
#include "input/button_manager.h"
#include "output/output_manager.h"
#include "storage/state_storage.h"
#include "ui/renderer.h"
#include "ui/ui_fsm.h"

namespace xenovent::game {

class GameEngine {
 public:
  GameEngine(storage::StateStorage& storage, ui::Renderer& renderer,
             input::ButtonManager& buttons, drivers::Adxl345Manager& adxl,
             drivers::Bh1750Manager& bh1750, output::OutputManager& output,
             ui::UiFsm& fsm);

  void begin();
  void update();

 private:
  void handleInput(const input::InputEvent& event, uint32_t nowMs);
  void runTick(uint32_t nowMs);
  domain::SensorSnapshot readSensors();
  void maybeAutosave(uint32_t nowMs);

  storage::StateStorage& storage_;
  ui::Renderer& renderer_;
  input::ButtonManager& buttons_;
  drivers::Adxl345Manager& adxl_;
  drivers::Bh1750Manager& bh1750_;
  output::OutputManager& output_;
  ui::UiFsm& fsm_;

  domain::CreatureState state_{};
  domain::UiFlags flags_{};

  uint32_t lastFastLoopMs_ = 0;
  uint32_t lastTickMs_ = 0;
  uint32_t lastAutosaveMs_ = 0;
  bool dirty_ = false;
  bool ritualResidual_ = false;
};

}  // namespace xenovent::game
