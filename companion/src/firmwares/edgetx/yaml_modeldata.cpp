/*
 * Copyright (C) OpenTX
 *
 * Based on code named
 *   th9x - http://code.google.com/p/th9x
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "yaml_modeldata.h"
#include "modeldata.h"

static const YamlLookupTable timerModeLut = {
    {TimerData::TIMERMODE_OFF, "OFF"},
    {TimerData::TIMERMODE_ON, "ON"},
    {TimerData::TIMERMODE_START, "START"},
    {TimerData::TIMERMODE_THR, "THR"},
    {TimerData::TIMERMODE_THR_REL, "THR_REL"},
    {TimerData::TIMERMODE_THR_START, "THR_START"},
};

static const YamlLookupTable trainerModeLut = {
  {  TRAINER_MODE_OFF, "OFF"  },
  {  TRAINER_MODE_MASTER_JACK, "MASTER_TRAINER_JACK"  },
  {  TRAINER_MODE_SLAVE_JACK, "SLAVE"  },
  {  TRAINER_MODE_MASTER_SBUS_EXTERNAL_MODULE, "MASTER_SBUS_EXT"  },
  {  TRAINER_MODE_MASTER_CPPM_EXTERNAL_MODULE, "MASTER_CPPM_EXT"  },
  {  TRAINER_MODE_MASTER_BATTERY_COMPARTMENT, "MASTER_BATT_COMP"  },
  {  TRAINER_MODE_MASTER_BLUETOOTH, "MASTER_BT"  },
  {  TRAINER_MODE_SLAVE_BLUETOOTH, "SLAVE_BT"  },
  {  TRAINER_MODE_MULTI, "MASTER_MULTI"  },
};

namespace YAML
{
Node convert<TimerData>::encode(const TimerData& rhs)
{
  Node node;
  node["swtch"] = rhs.swtch;
  node["mode"] = timerModeLut << rhs.mode;
  node["name"] = rhs.name;
  node["minuteBeep"] = rhs.minuteBeep;
  node["countdownBeep"] = rhs.countdownBeep;
  node["start"] = rhs.val;
  node["persistent"] = rhs.persistent;
  node["countdownStart"] = rhs.countdownStart;
  node["value"] = rhs.pvalue;
  return node;
}

bool convert<TimerData>::decode(const Node& node, TimerData& rhs)
{
  node["swtch"] >> rhs.swtch;
  node["mode"] >> timerModeLut >> rhs.mode;
  node["name"] >> rhs.name;
  node["minuteBeep"] >> rhs.minuteBeep;
  node["countdownBeep"] >> rhs.countdownBeep;
  node["start"] >> rhs.val;
  node["persistent"] >> rhs.persistent;
  node["countdownStart"] >> rhs.countdownStart;
  node["value"] >> rhs.pvalue;
  return true;
}

Node convert<ModelData>::encode(const ModelData& rhs)
{
  Node node;
  node["header"]["name"] = rhs.name;
  node["header"]["bitmap"] = rhs.bitmap;
  // TODO: modelId

  for (int i=0; i<CPN_MAX_TIMERS; i++) {
    Node timer;
    timer = rhs.timers[i];
    node["timers"][std::to_string(i)] = timer;
  }

  node["noGlobalFunctions"] = rhs.noGlobalFunctions;
  node["thrTrim"] = rhs.thrTrim;
  node["trimInc"] = rhs.trimInc;
  node["displayTrims"] = rhs.trimsDisplay;
  node["disableThrottleWarning"] = rhs.disableThrottleWarning;
  // node[] = rhs.beepANACenter;
  node["extendedLimits"] = rhs.extendedLimits;
  node["extendedTrims"] = rhs.extendedTrims;
  node["throttleReversed"] = rhs.throttleReversed;

  // flightModeData[]
  // mixData[]
  // limitData[]
  // inputNames[]
  // expoData[]
  // curves[]
  // logicalSw[]
  // customFn[]

  // swashRingData
  // thrTraceSrc
  // switchWarningStates
  // thrTrimSwitch
  // potsWarningMode
  // potsWarnEnabled[]
  // potsWarnPosition[]

  node["displayChecklist"] = rhs.displayChecklist;

  // gvarData[]
  // mavlink ???
  // telemetryProtocol ???
  // frsky
  // rssiSource
  // rssiAlarms

  node["trainerMode"] = trainerModeLut << rhs.trainerMode;

  // moduleData[]
  // scriptData[]
  // sensorData[]

  node["toplcdTimer"] = rhs.toplcdTimer;

  // customScreens
  // topBarData
  node["view"] = rhs.view;
  node["modelRegistrationID"] = rhs.registrationId;

  return node;
}

bool convert<ModelData>::decode(const Node& node, ModelData& rhs)
{
  if (!node.IsMap()) return false;

  if (node["header"]) {
    const auto& header = node["header"];
    if (!header.IsMap()) return false;

    header["name"] >> rhs.name;
    header["bitmap"] >> rhs.bitmap;
    // TODO: modelId
  }

  if (node["timers"]) {
    const auto& timers = node["timers"];
    if (!timers.IsMap()) return false;
    timers >> rhs.timers;
  }

  node["noGlobalFunctions"] >> rhs.noGlobalFunctions;
  node["thrTrim"] >> rhs.thrTrim;
  node["trimInc"] >> rhs.trimInc;
  node["displayTrims"] >> rhs.trimsDisplay;
  node["disableThrottleWarning"] >> rhs.disableThrottleWarning;
  // node[] >> rhs.beepANACenter;
  node["extendedLimits"] >> rhs.extendedLimits;
  node["extendedTrims"] >> rhs.extendedTrims;
  node["throttleReversed"] >> rhs.throttleReversed;

  // flightModeData[]
  // mixData[]
  // limitData[]
  // inputNames[]
  // expoData[]
  // curves[]
  // logicalSw[]
  // customFn[]

  // swashRingData
  // thrTraceSrc
  // switchWarningStates
  // thrTrimSwitch
  // potsWarningMode
  // potsWarnEnabled[]
  // potsWarnPosition[]

  node["displayChecklist"] >> rhs.displayChecklist;

  // gvarData[]
  // mavlink ???
  // telemetryProtocol ???
  // frsky
  // rssiSource
  // rssiAlarms

  node["trainerMode"] >> trainerModeLut >> rhs.trainerMode;

  // moduleData[]
  // scriptData[]
  // sensorData[]

  node["toplcdTimer"] >> rhs.toplcdTimer;

  // customScreens
  // topBarData
  node["view"] >> rhs.view;
  node["modelRegistrationID"] >> rhs.registrationId;

  return true;
}

}  // namespace YAML