#ifdef __unix__
#include <elf.h>
#elif defined(_WIN32) || defined(WIN32)
#include "elf-windows.h"
#endif // __unix__ else defined(_WIN32) || defined(WIN32)
