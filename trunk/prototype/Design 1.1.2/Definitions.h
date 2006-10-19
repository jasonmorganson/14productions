/* A list of definitions */

#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#include <iostream>
#include <string>

using namespace std;

#define DEBUG true  // Toggles extra information that could be useful for debugging.
#define debug(string) ( cout << string << endl )  // Macro for displaying debugging information.

typedef HWND WINDOW_TYPE;  // Defines the window type to be used. HWND for Windows or Window for Linux.

#endif