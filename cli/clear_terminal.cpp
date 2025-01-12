#include <iostream>
#include "clear_terminal.h"

void clear_terminal() {
#if defined(_WIN32) or defined(WIN32)
	static_assert(false, "clear_console not implemented");
#elif defined(__unix) or defined(__unix__)
	// CSI[2J clears
	// CSI[H moves cursor to top left
	std::cout << "\x1B[2J\x1B[H";
#else
	static_assert(false, "clear_console not implemented");
#endif
}