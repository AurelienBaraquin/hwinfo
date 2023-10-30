// Copyright Leon Freist
// Author Leon Freist <freist@informatik.uni-freiburg.de>

#include <iostream>

#include "hwinfo/platform.h"

#ifdef HWINFO_WINDOWS

#include <hwinfo/battery.h>
#include <hwinfo/utils/stringutils.h>
#include <hwinfo/utils/wmi_wrapper.h>

#include <iostream>

namespace hwinfo {

// =====================================================================================================================
// _____________________________________________________________________________________________________________________
std::string Battery::getVendor() const { return "<unknwon>"; }

// _____________________________________________________________________________________________________________________
std::string Battery::getModel() const { return _model; }

// _____________________________________________________________________________________________________________________
std::string Battery::getSerialNumber() const { return "<unknwon>"; }

// _____________________________________________________________________________________________________________________
std::string Battery::getTechnology() const { return "<unknwon>"; }

// _____________________________________________________________________________________________________________________
uint32_t Battery::getEnergyFull() const { return 0; }

// _____________________________________________________________________________________________________________________
uint32_t Battery::energyNow() const { return 0; }

// _____________________________________________________________________________________________________________________
bool Battery::charging() const { return false; }

// _____________________________________________________________________________________________________________________
bool Battery::discharging() const { return false; }

// =====================================================================================================================
// _____________________________________________________________________________________________________________________
std::vector<Battery> getAllBatteries() {
  utils::WMI::_WMI wmi;
  const std::wstring query_string(L"SELECT DeviceID, FullChargeCapacity, Name FROM Win32_Battery");
  bool success = wmi.execute_query(query_string);
  if (!success) {
    return {};
  }
  int8_t counter = 0;
  for (const auto& v : res) {
    std::wstring tmp(v);
    batteries.emplace_back(counter++);
    batteries.back()._model = {tmp.begin(), tmp.end()};
  }
  return batteries;
}

}  // namespace hwinfo

#endif