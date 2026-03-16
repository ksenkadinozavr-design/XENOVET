#include "storage/state_storage.h"

#include <Preferences.h>

namespace xenovent::storage {

namespace {
constexpr const char* kNs = "xenovent";
constexpr const char* kKeyEssence = "essence";
constexpr const char* kKeyHunger = "hunger";
constexpr const char* kKeyInstability = "instab";
constexpr const char* kKeyBond = "bond";
constexpr const char* kKeyCorruption = "corrupt";
constexpr const char* kKeyForm = "form";
constexpr const char* kKeyAge = "age";
constexpr const char* kKeyDeaths = "deaths";

Preferences prefs;
}

void StateStorage::begin() { initialized_ = true; }

domain::CreatureState StateStorage::defaultState() const { return domain::CreatureState{}; }

void StateStorage::saveState(const domain::CreatureState& state) {
  if (!initialized_) return;

  prefs.begin(kNs, false);
  prefs.putInt(kKeyEssence, state.essence);
  prefs.putInt(kKeyHunger, state.hunger);
  prefs.putInt(kKeyInstability, state.instability);
  prefs.putInt(kKeyBond, state.bond);
  prefs.putInt(kKeyCorruption, state.corruption);
  prefs.putUInt(kKeyForm, static_cast<uint32_t>(state.form));
  prefs.putUInt(kKeyAge, state.ageSeconds);
  prefs.putUInt(kKeyDeaths, state.deaths);
  prefs.end();
}

domain::CreatureState StateStorage::loadState() {
  if (!initialized_) return defaultState();

  prefs.begin(kNs, true);
  domain::CreatureState state;
  state.essence = prefs.getInt(kKeyEssence, state.essence);
  state.hunger = prefs.getInt(kKeyHunger, state.hunger);
  state.instability = prefs.getInt(kKeyInstability, state.instability);
  state.bond = prefs.getInt(kKeyBond, state.bond);
  state.corruption = prefs.getInt(kKeyCorruption, state.corruption);
  state.form = static_cast<domain::CreatureForm>(prefs.getUInt(kKeyForm, static_cast<uint32_t>(state.form)));
  state.ageSeconds = prefs.getUInt(kKeyAge, state.ageSeconds);
  state.deaths = prefs.getUInt(kKeyDeaths, state.deaths);
  prefs.end();
  return state;
}

void StateStorage::factoryReset() {
  if (!initialized_) return;
  prefs.begin(kNs, false);
  prefs.clear();
  prefs.end();
}

}  // namespace xenovent::storage
