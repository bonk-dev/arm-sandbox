#include "EmulSegFault.h"

Emulation::Exceptions::EmulSegFault::EmulSegFault() : std::runtime_error("Emulation segmentation fault") {}
