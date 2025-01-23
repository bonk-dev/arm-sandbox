#include <iostream>
#include "clear_terminal.h"
#if defined(_WIN32) or defined(WIN32)
#include <windows.h>
#endif

void clear_terminal() {
#if defined(_WIN32) or defined(WIN32)
	//https://gist.github.com/karolisjan/6e4d33211847b0eb77317ab186b3840b

	// Get the Win32 handle representing standard output.
	// This generally only has to be done once, so we make it static.
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD topLeft = { 0, 0 };

	// std::cout uses a buffer to batch writes to the underlying console.
	// We need to flush that to the console because we're circumventing
	// std::cout entirely; after we clear the console, we don't want
	// stale buffered text to randomly be written out.
	std::cout.flush();

	// Figure out the current width and height of the console window
	if (!GetConsoleScreenBufferInfo(hOut, &csbi)) {
		// TODO: Handle failure!
		abort();
	}
	DWORD length = csbi.dwSize.X * csbi.dwSize.Y;

	DWORD written;

	// Flood-fill the console with spaces to clear it
	FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);

	// Reset the attributes of every character to the default.
	// This clears all background colour formatting, if any.
	FillConsoleOutputAttribute(hOut, csbi.wAttributes, length, topLeft, &written);

	// Move the cursor back to the top left for the next sequence of writes
	SetConsoleCursorPosition(hOut, topLeft);
#elif defined(__unix) or defined(__unix__)
	// CSI[2J clears
	// CSI[H moves cursor to top left
	std::cout << "\x1B[2J\x1B[H";
#else
	static_assert(false, "clear_console not implemented");
#endif
}