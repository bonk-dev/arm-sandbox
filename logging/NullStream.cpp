#include "NullStream.h"

namespace {
    Logging::NullStream ns{};
}


Logging::NullStream & Logging::getNullStream() {
    return ns;
}
