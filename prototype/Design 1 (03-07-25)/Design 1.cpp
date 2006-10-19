/*** Design 1 ***/

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

WINDOW_TYPE createWindow(char* windowName);
void setupCamera(int cameraIndex, WINDOW_TYPE window);

void debug(string s);

/* Launches the specified camera into the specified window. Enables the camera, renders the stream, selects the window for redering. */
void callback(IplImage* image);
void stereo_calib_callback();

#define LEFT_ORIENTATED_ARM  0  // Flag for a left orientated arm.
#define RIGHT_ORIENTATED_ARM 1  // Flag for a right orientated arm.

void main() {

	// Set up output parameters.
	CvPoint3D32f* center = new CvPoint3D32f();
	CvMemStorage* storage = new CvMemStorage();
	CvSeq **numbers = 0;

//	CvPoint3D32f* points = new cvPoint3D32f(1, 2, 3);
	int count = 1;  // Number of input points.
	CvSeq* indexs = cvCreateSeq(CV_SEQ_POINT_SET, sizeof(CvSeq), sizeof(CvPoint), storage);  // Sequence of the input points indices in the initial image.
	float* line;  // Pointer to the input points approximation line.
//	CvSize2D32f size;  // Size of the initial image.



//	cvFindHandRegion(&points, count, &indexs, &line, CvSize2D32f size, LEFT_ORIENTATED_ARM, &center, &storage, &numbers);

}

/*
void main()
{
//	int cameraIndex;  // Index of the camera to configure.
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
//	setupCamera(0, createWindow("Camera 1"));
//	setupCamera(1, createWindow("Camera 2"));

cvNamedWindow("camera1", 1);
cvNamedWindow("camera2", 1);
HWND mywin1 = (HWND)cvGetWindowHandle("camera1");
HWND mywin2 = (HWND)cvGetWindowHandle("camera2");
 
cvcamSetProperty(0, CVCAM_PROP_ENABLE, CVCAMTRUE);
cvcamSetProperty(0, CVCAM_PROP_WINDOW, &mywin1);
cvcamSetProperty(0, CVCAM_STEREO_CALLBACK,stereo_calib_callback);
 
cvcamSetProperty(1, CVCAM_PROP_ENABLE, CVCAMTRUE);
cvcamSetProperty(1, CVCAM_PROP_WINDOW, &mywin2); 
cvcamSetProperty(1, CVCAM_STEREO_CALLBACK,stereo_calib_callback);

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
*/
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

/***************************************************/
	// Set the callback function.
//	cvcamSetProperty(cameraIndex, CVCAM_PROP_CALLBACK, callback);
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

void stereo_calib_callback() {}

void debug(string s) {

	cout << s << endl;
}