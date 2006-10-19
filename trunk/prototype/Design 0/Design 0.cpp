/*** Design 0 ***/

//#include "ipl.h"
#include "cv.h"  // include core library interface
#include "highgui.h" // include GUI library interface
//#include "cvaux.h"  // include expirimental library
#include "cvcam.h"
#include <iostream>
#include <conio.h>
#include <string>

using namespace std;

#define DEBUG true  // Toggles extra information that could be useful for debugging.
#define CAMERA_PROMPT false  // Prompt the user for camera(s) to select.

typedef HWND WINDOW_TYPE;  // Defines the window type to be used. HWND for Windows or Window for Linux.

/* Displays a dialog box that allows the user to select from multiple cameras attached to the system. */
/*** APPEARS BUGGY! */
void selectCamera();

void setupCamera(int cameraIndex);

void debug(string s);

/* Launches the specified camera into the specified window. Enables the camera, renders the stream, selects the window for redering. */
void callback(IplImage* image);

void main()
{
	int cameraIndex;  // Index of the camera to configure.
//	WINDOW_TYPE cameraWindow;  // Window handle for the video redering window.

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

	// If the camera prompt flag is set, then prompt the user to select a camera.
	if(CAMERA_PROMPT)
		selectCamera();

	int success = false;
	
	// Manually set the camera(s) to use.
	setupCamera(0);
	//setupCamera(1);

	success = cvcamInit();
	
	if(DEBUG && success) debug("Camera initialized...");
	else debug("(!) Unable to initialize camera");

	success = cvcamStart();
	
	if(DEBUG && success) debug("(!) Unable to start camera");
	else debug("Camera started...");

	cvvWaitKey(0); // Wait for a key.

	success = cvcamStop();

	if(DEBUG && success) debug("(!) Unable to stop camera");
	else debug("Camera stopped...");

	success = cvcamExit();

	if(DEBUG && success) debug("(!) Unable to exit");
	else debug("Exited...");

	if(DEBUG)
		debug("=== End debugging information ===");
}

void selectCamera() {

	int* out;
	int nselected = cvcamSelectCamera(&out);
	
	if(nselected>0)
		printf("the 1-st selected camera is camera number %d", out[0]);
	if(nselected == 2)
		printf("the 2-nd selected camera is camera number %d", out[1]);

	free(out);
	return;
}

void setupCamera(int cameraIndex) {

	int width = 640;
	int height = 480;

	int success;
	WINDOW_TYPE cameraWindow;  // Window handle for the video redering window.

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

	// Creates a window for rendering.
    cvNamedWindow("cvcam window", 0);

	// Get the window handle.
    cameraWindow = (WINDOW_TYPE)cvGetWindowHandle("cvcam window");

	// Select the window for video rendering.
	success = cvcamSetProperty(cameraIndex, CVCAM_PROP_WINDOW, &cameraWindow);
	
	if(DEBUG && success) debug("(!) Unable to anchor window");
	else debug("Window anchored...");

/***************************************************/
	// Set the callback function.
	cvcamSetProperty(cameraIndex, CVCAM_PROP_CALLBACK, callback);
		//if(DEBUG) debug("Callback set...");
/****************************************************/
}

void callback(IplImage* image)//Draws blue horizontal lines across the image
{
	IplImage* image1 = image;
	int i,j;

	assert (image);

	for(i=0; i<image1->height; i+=10)
	{
		for(j=(image1->widthStep)*i; j<(image1->widthStep)*(i+1); 
				j+=image1->nChannels)
		{
			image1->imageData[j]   = (char)255;
			image1->imageData[j+1] = 0;
			image1->imageData[j+2] = 0;
		      

		} 

	}

}

void debug(string s) {

	cout << s << endl;
}