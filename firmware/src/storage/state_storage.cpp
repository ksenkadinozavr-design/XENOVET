#include "storage/state_storage.h"

#include <Preferences.h>

#include "config/app_config.h"
#include "domain/types.h"
#include "utils/logger.h"

namespace xenovent::storage {

namespace {
constexpr const char* kNs = config::app::kStorageNamespace;
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
constexpr const char* kKeyCdFeed = "cdf";
constexpr const char* kKeyCdSuppress = "cds";
constexpr const char* kKeyCdRitual = "cdr";
constexpr const char* kKeyCdMeditate = "cdm";
constexpr const char* kKeyCdSleep = "cdsl";

Preferences prefs;

void readCommonState(domain::CreatureState& st) {
  st.essence = prefs.getInt(kKeyEssence, st.essence);
  st.hunger = prefs.getInt(kKeyHunger, st.hunger);
  st.instability = prefs.getInt(kKeyInstability, st.instability);
  st.bond = prefs.getInt(kKeyBond, st.bond);
  st.corruption = prefs.getInt(kKeyCorruption, st.corruption);
  st.form = static_cast<domain::CreatureForm>(prefs.getUInt(kKeyForm, static_cast<uint32_t>(st.form)));
  st.ageSeconds = prefs.getUInt(kKeyAge, st.ageSeconds);
  st.deaths = prefs.getUInt(kKeyDeaths, st.deaths);
}

}  // namespace

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
  prefs.putUInt(kKeyCdFeed, s.cooldowns.feed);
  prefs.putUInt(kKeyCdSuppress, s.cooldowns.suppress);
  prefs.putUInt(kKeyCdRitual, s.cooldowns.ritual);
  prefs.putUInt(kKeyCdMeditate, s.cooldowns.meditate);
  prefs.putUInt(kKeyCdSleep, s.cooldowns.sleep);
  prefs.end();
  return true;
}

StorageLoadResult StateStorage::loadState() {
  StorageLoadResult out;
  out.state = defaultState();

  if (!initialized_) {
    out.recoveryStatus = RecoveryStatus::FallbackToDefaults;
    return out;
  }

  prefs.begin(kNs, true);
  const bool saved = prefs.getBool(kKeySaved, false);
  const uint32_t version = prefs.getUInt(kKeyVersion, 0);

  if (!saved) {
    prefs.end();
    out.recoveryStatus = RecoveryStatus::FallbackToDefaults;
    utils::warn("Storage", "no saved state, using defaults");
    return out;
  }

  PersistedStateEnvelope envelope;
  envelope.schemaVersion = version;
  envelope.state = defaultState();

  if (version >= 1 && version <= config::app::kStateVersion) {
    readCommonState(envelope.state);
    if (version >= 2) {
      envelope.state.sleepTicksRemaining = prefs.getUInt(kKeySleepTicks, 0);
      envelope.state.suppressTicksRemaining = prefs.getUInt(kKeySuppressTicks, 0);
    }
    if (version >= 3) {
      envelope.state.cooldowns.feed = prefs.getUInt(kKeyCdFeed, 0);
      envelope.state.cooldowns.suppress = prefs.getUInt(kKeyCdSuppress, 0);
      envelope.state.cooldowns.ritual = prefs.getUInt(kKeyCdRitual, 0);
      envelope.state.cooldowns.meditate = prefs.getUInt(kKeyCdMeditate, 0);
      envelope.state.cooldowns.sleep = prefs.getUInt(kKeyCdSleep, 0);
    }
  }
  prefs.end();

  out.migration = migrateToLatest(envelope, config::app::kStateVersion);
  if (!out.migration.success) {
    out.versionMismatch = true;
    out.recoveryStatus = (out.migration.status == RecoveryStatus::UnsupportedVersion)
                             ? RecoveryStatus::UnsupportedVersion
                             : RecoveryStatus::FallbackToDefaults;
    out.state = defaultState();
    utils::warn("Storage", "migration failed status=%d msg=%s",
                static_cast<int>(out.migration.status), out.migration.message);
    return out;
  }

  out.state = domain::normalizeState(envelope.state);
  out.loadedFromNvs = true;
  out.recoveryStatus = out.migration.status;

  if (!domain::isValidState(out.state)) {
    utils::warn("Storage", "loaded state invalid after normalize, using defaults");
    out.state = defaultState();
    out.loadedFromNvs = false;
    out.recoveryStatus = RecoveryStatus::CorruptedRecovered;
  }

  if (out.migration.status == RecoveryStatus::MigrationApplied) {
    utils::info("Storage", "migration applied v%lu->v%lu",
                static_cast<unsigned long>(out.migration.fromVersion),
                static_cast<unsigned long>(out.migration.toVersion));
  }

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
