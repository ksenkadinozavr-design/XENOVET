#include "storage/state_storage.h"

#include <Preferences.h>

#include "config/app_config.h"
#include "domain/types.h"
#include "utils/logger.h"

namespace xenovent::storage {

namespace {
constexpr const char* kNs = "xenovent";
constexpr const char* kKeySaved = "saved";
constexpr const char* kKeyVersion = "ver";

constexpr const char* kKeyEssence = "essence";
constexpr const char* kKeyHunger = "hunger";
constexpr const char* kKeyInstability = "instab";
constexpr const char* kKeyBond = "bond";
constexpr const char* kKeyCorruption = "corrupt";
constexpr const char* kKeyForm = "form";
constexpr const char* kKeyAge = "age";
constexpr const char* kKeyDeaths = "deaths";
constexpr const char* kKeySleepTicks = "sleep";
constexpr const char* kKeySuppressTicks = "suppr";

Preferences prefs;
}

void StateStorage::begin() {
  initialized_ = true;
  utils::info("Storage", "initialized namespace=%s", kNs);
}

domain::CreatureState StateStorage::defaultState() const { return domain::buildDefaultState(); }

bool StateStorage::hasSavedState() {
  if (!initialized_) return false;
  prefs.begin(kNs, true);
  const bool saved = prefs.getBool(kKeySaved, false);
  prefs.end();
  return saved;
}

bool StateStorage::saveState(const domain::CreatureState& state) {
  if (!initialized_) return false;

  const domain::CreatureState s = domain::normalizeState(state);
  prefs.begin(kNs, false);
  prefs.putBool(kKeySaved, true);
  prefs.putUInt(kKeyVersion, config::app::kStateVersion);

  prefs.putInt(kKeyEssence, s.essence);
  prefs.putInt(kKeyHunger, s.hunger);
  prefs.putInt(kKeyInstability, s.instability);
  prefs.putInt(kKeyBond, s.bond);
  prefs.putInt(kKeyCorruption, s.corruption);
  prefs.putUInt(kKeyForm, static_cast<uint32_t>(s.form));
  prefs.putUInt(kKeyAge, s.ageSeconds);
  prefs.putUInt(kKeyDeaths, s.deaths);
  prefs.putUInt(kKeySleepTicks, s.sleepTicksRemaining);
  prefs.putUInt(kKeySuppressTicks, s.suppressTicksRemaining);
  prefs.end();
  return true;
}

StorageLoadResult StateStorage::loadState() {
  StorageLoadResult out;
  out.state = defaultState();

  if (!initialized_) {
    return out;
  }

  prefs.begin(kNs, true);
  const bool saved = prefs.getBool(kKeySaved, false);
  const uint32_t version = prefs.getUInt(kKeyVersion, 0);

  if (!saved) {
    prefs.end();
    utils::warn("Storage", "no saved state, using defaults");
    return out;
  }

  if (version != config::app::kStateVersion) {
    prefs.end();
    out.versionMismatch = true;
    utils::warn("Storage", "version mismatch stored=%lu expected=%lu",
                static_cast<unsigned long>(version),
                static_cast<unsigned long>(config::app::kStateVersion));
    return out;
  }

  out.state.essence = prefs.getInt(kKeyEssence, out.state.essence);
  out.state.hunger = prefs.getInt(kKeyHunger, out.state.hunger);
  out.state.instability = prefs.getInt(kKeyInstability, out.state.instability);
  out.state.bond = prefs.getInt(kKeyBond, out.state.bond);
  out.state.corruption = prefs.getInt(kKeyCorruption, out.state.corruption);
  out.state.form = static_cast<domain::CreatureForm>(
      prefs.getUInt(kKeyForm, static_cast<uint32_t>(out.state.form)));
  out.state.ageSeconds = prefs.getUInt(kKeyAge, out.state.ageSeconds);
  out.state.deaths = prefs.getUInt(kKeyDeaths, out.state.deaths);
  out.state.sleepTicksRemaining = prefs.getUInt(kKeySleepTicks, out.state.sleepTicksRemaining);
  out.state.suppressTicksRemaining =
      prefs.getUInt(kKeySuppressTicks, out.state.suppressTicksRemaining);
  prefs.end();

  out.state = domain::normalizeState(out.state);
  out.loadedFromNvs = true;
  return out;
}

bool StateStorage::resetState() {
  if (!initialized_) return false;
  prefs.begin(kNs, false);
  prefs.clear();
  prefs.end();
  return true;
}

}  // namespace xenovent::storage
