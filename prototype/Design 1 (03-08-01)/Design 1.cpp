/*** Design 1 ***/

#include "cv.h"  // include core library interface
#include "highgui.h" // include GUI library interface
#include "cvaux.h"  // include expirimental library
#include "cvcam.h"
#include <iostream>
#include <conio.h>
#include <string>

using namespace std;

#define DEBUG true  // Toggles extra information that could be useful for debugging.

typedef HWND WINDOW_TYPE;  // Defines the window type to be used. HWND for Windows or Window for Linux.

/* Creates a window with the specified name and returns a pointer to it. */
WINDOW_TYPE createWindow(char* windowName);

/* Launches the specified camera into the specified window. Enables the camera, renders the stream, selects the window for redering. */
void setupCamera(int cameraIndex, WINDOW_TYPE window);

void debug(string s);

void main() {

	if(DEBUG) debug("=== Begin debugging information ===");
	
	// Find the number of available cameras in the system.
	int numberOfCameras = cvcamGetCamerasCount( );

	if(DEBUG)
		cout << "Number of cameras found: " << numberOfCameras << endl;

	// Make sure that there is at least one camera connected before proceeding.
	if(numberOfCameras == 0) {
		cout << "*** No cameras found on the system ***" << endl;
		cout << "Please connect as least one camera and try again." << endl << endl;
		exit(EXIT_FAILURE);
	}

	int success = false;
	
	// Create windows.
	WINDOW_TYPE window1 = createWindow("Camera 1");
	WINDOW_TYPE window2 = createWindow("Camera 2");

	// Manually set the camera(s) to use.
	setupCamera(0, window1);
	setupCamera(1, window2);

	success = cvcamInit();  // Initialize the camera(s).

	if(DEBUG && success) debug("Camera(s) initialized...");
	else debug("(!) Unable to initialize camera(s)");

	success = cvcamStart();  // Start camera(s).
	
	if(DEBUG && success) debug("Camera started(s)...");
	else debug("(!) Unable to start camera(s)");

	cvvWaitKey(0); // Wait for a key.

	success = cvcamStop();  // Stop camera(s).

	if(DEBUG && success) debug("(!) Unable to stop camera(s)");
	else debug("Camera stopped(s)...");

	success = cvcamExit();  // Exit.

	if(DEBUG && success) debug("(!) Unable to exit");
	else debug("Exited...");

	if(DEBUG)
		debug("=== End debugging information ===");
}

WINDOW_TYPE createWindow(char* windowName) {

	// Creates a named window.
    cvNamedWindow(windowName, 0);

	// Returns the window handle.
    return (WINDOW_TYPE) cvGetWindowHandle(windowName);
}

void setupCamera(int cameraIndex, WINDOW_TYPE window) {

	int width = 640;
	int height = 480;

	int success;

	if(DEBUG)
		cout << "Camera: " << cameraIndex << endl;

	// Enables the camera.
	success = cvcamSetProperty(cameraIndex, CVCAM_PROP_ENABLE, CVCAMTRUE);	

	if(DEBUG && success) debug("(!) Unable to enable camera");
	else debug("Camera enabled...");

	// Set the width and height of the video.
	cvcamSetProperty(cameraIndex, CVCAM_RNDWIDTH, &width);	
	cvcamSetProperty(cameraIndex, CVCAM_RNDHEIGHT, &height);

	// Renders the stream from this source.
	success = cvcamSetProperty(cameraIndex, CVCAM_PROP_RENDER, CVCAMTRUE);
	
	if(DEBUG && success) debug("(!) Unable to start stream");
	else debug("Stream started...");

	// Select the window for video rendering.
	success = cvcamSetProperty(cameraIndex, CVCAM_PROP_WINDOW, &window);
	
	if(DEBUG && success) debug("(!) Unable to anchor window");
	else debug("Window anchored...");
}

void debug(string s) {

	cout << s << endl;
}