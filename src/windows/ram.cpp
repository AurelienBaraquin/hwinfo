// Copyright (c) Leon Freist <freist@informatik.uni-freiburg.de>
// This software is part of HWBenchmark

#include <hwinfo/platform.h>

#ifdef HWINFO_WINDOWS
#include <Windows.h>
#include <hwinfo/ram.h>
#include <hwinfo/utils/stringutils.h>
#include <hwinfo/utils/wmi_wrapper.h>

#include <string>
#include <vector>

namespace hwinfo {
namespace ram {

// _____________________________________________________________________________________________________________________
std::string getVendor() {
  std::vector<const wchar_t*> names{};
  wmi::queryWMI("WIN32_PhysicalMemory", "Manufacturer", names);
  if (names.empty()) {
    return "<unknown>";
  }
  auto ret = names[0];
  if (!ret) {
    return "<unknown>";
  }
  return utils::wstring_to_std_string(ret);
}

// _____________________________________________________________________________________________________________________
std::string getName() {
  std::vector<const wchar_t*> names{};
  wmi::queryWMI("WIN32_PhysicalMemory", "Name", names);
  if (names.empty()) {
    return "<unknown>";
  }
  auto ret = names[0];
  if (!ret) {
    return "<unknown>";
  }
  return utils::wstring_to_std_string(ret);
}

// _____________________________________________________________________________________________________________________
std::string getModel() {
  std::vector<const wchar_t*> names{};
  wmi::queryWMI("WIN32_PhysicalMemory", "PartNumber", names);
  if (names.empty()) {
    return "<unknown>";
  }
  auto ret = names[0];
  if (!ret) {
    return "<unknown>";
  }
  return utils::wstring_to_std_string(ret);
}

// _____________________________________________________________________________________________________________________
std::string getSerialNumber() {
  std::vector<const wchar_t*> names{};
  wmi::queryWMI("WIN32_PhysicalMemory", "SerialNumber", names);
  if (names.empty()) {
    return "<unknown>";
  }
  auto ret = names[0];
  if (!ret) {
    return "<unknown>";
  }
  std::wstring tmp(ret);
  return {tmp.begin(), tmp.end()};
}

// _____________________________________________________________________________________________________________________
int64_t getTotalSize_Bytes() {
  MEMORYSTATUSEX status;
  status.dwLength = sizeof(status);
  GlobalMemoryStatusEx(&status);
  return static_cast<int64_t>(status.ullTotalPhys);
}

// _____________________________________________________________________________________________________________________
int64_t getFreeMemory() {
  // it will return L"FreePhysicalMemory" Str
  std::vector<wchar_t*> memories{};
  // Number of kilobytes of physical memory currently unused and available.
  wmi::queryWMI("CIM_OperatingSystem", "FreePhysicalMemory", memories);
  if (!memories.empty()) {
    if (memories.front() == nullptr) {
      return -1;
    }
    // keep it as totalSize_Bytes
    return std::stoll(utils::wstring_to_std_string(memories[0])) * 1024;
  }
  return -1;
}
}  // namespace ram

// _____________________________________________________________________________________________________________________
RAM::RAM() {
  _name = ram::getName();
  _vendor = ram::getVendor();
  _serialNumber = ram::getSerialNumber();
  _model = ram::getModel();
  _total_Bytes = ram::getTotalSize_Bytes();
  _free_Bytes = ram::getFreeMemory();
  _available_Bytes = ram::getFreeMemory();
}

// _____________________________________________________________________________________________________________________
int64_t RAM::available_Bytes() {
  // TODO: Get actual available memory size...
  return free_Bytes();
}

// _____________________________________________________________________________________________________________________
RAM::RAM(int id) : _id(id) {}

// _____________________________________________________________________________________________________________________
std::vector<RAM> getAllRAM() {
  utils::WMI::_WMI wmi;
  const std::wstring query_string(
      L"SELECT Capacity, ConfiguredClockSpeed, Manufacturer, SerialNumber, PartNumber FROM Win32_PhysicalMemory");
  bool success = wmi.execute_query(query_string);
  if (!success) {
    return {};
  }
  ULONG u_return = 0;
  IWbemClassObject* obj = nullptr;
  std::vector<RAM> rams;
  int id = 0;
  while (wmi.enumerator) {
    wmi.enumerator->Next(WBEM_INFINITE, 1, &obj, &u_return);
    if (!u_return) {
      break;
    }
    VARIANT vt_prop;
    HRESULT hr;
    RAM ram(id++);
    hr = obj->Get(L"Manufacturer", 0, &vt_prop, nullptr, nullptr);
    if (SUCCEEDED(hr)) {
      ram._vendor = utils::wstring_to_std_string(vt_prop.bstrVal);
    }
    hr = obj->Get(L"partNumber", 0, &vt_prop, nullptr, nullptr);
    if (SUCCEEDED(hr)) {
      ram._model = utils::wstring_to_std_string(vt_prop.bstrVal);
      // TODO: One expects an actual name of the RAM but wmi does not provide such a property...
      //       The "Name"-property of WMI returns "PhysicalMemory".
      ram._name = ram._model;
    }
    hr = obj->Get(L"Capacity", 0, &vt_prop, nullptr, nullptr);
    if (SUCCEEDED(hr)) {
      ram._total_Bytes = std::stoll(utils::wstring_to_std_string(vt_prop.bstrVal));
    }
    hr = obj->Get(L"ConfiguredClockSpeed", 0, &vt_prop, nullptr, nullptr);
    if (SUCCEEDED(hr)) {
      ram._frequency_Hz = static_cast<int64_t>(vt_prop.ulVal) * 1000 * 1000;
    }
    hr = obj->Get(L"SerialNumber", 0, &vt_prop, nullptr, nullptr);
    if (SUCCEEDED(hr)) {
      ram._serialNumber = utils::wstring_to_std_string(vt_prop.bstrVal);
    }
    VariantClear(&vt_prop);
    obj->Release();
    rams.push_back(std::move(ram));
  }
  return rams;
}

}  // namespace hwinfo

#endif  // HWINFO_WINDOWS