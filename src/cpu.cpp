// Copyright (c) Leon Freist <freist@informatik.uni-freiburg.de>
// This software is part of HWBenchmark

#include "hwinfo/cpu.h"

#include <string>
#include <vector>

namespace hwinfo {

// _____________________________________________________________________________________________________________________
int CPU::id() const { return _id; }

// _____________________________________________________________________________________________________________________
const std::string& CPU::modelName() const { return _modelName; }

// _____________________________________________________________________________________________________________________
const std::string& CPU::vendor() const { return _vendor; }

// _____________________________________________________________________________________________________________________
int64_t CPU::cacheSize_Bytes() const { return _cacheSize_Bytes; }

// _____________________________________________________________________________________________________________________
int CPU::numPhysicalCores() const { return _numPhysicalCores; }

// _____________________________________________________________________________________________________________________
int CPU::numLogicalCores() const { return _numLogicalCores; }

// _____________________________________________________________________________________________________________________
int64_t CPU::maxClockSpeed_MHz() const { return _maxClockSpeed_MHz; }

// _____________________________________________________________________________________________________________________
int64_t CPU::regularClockSpeed_MHz() const { return _regularClockSpeed_MHz; }

// _____________________________________________________________________________________________________________________
int64_t CPU::minClockSpeed_MHz() const { return _minClockSpeed_MHz; }

// _____________________________________________________________________________________________________________________
const std::vector<std::string>& CPU::flags() const { return _flags; }

}  // namespace hwinfo