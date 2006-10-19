/*** Design 1 ***/

#include "cv.h"       // Include core library interface.
#include "highgui.h"  // Include GUI library interface.
#include "cvaux.h"    // Include expirimental library.
#include "cvcam.h"    // Include cvcam libray.
#include "Definitions.h"
#include "Camera.h"
#include <iostream>
#include <conio.h>
#include <string>

using namespace std;

/* Creates a window with the specified name and returns the pointer to it. */
WINDOW_TYPE createWindow(char* windowName);

/* Launches the specified camera into the specified window. Enables the camera, renders the stream, selects the window for redering. */
void setupCamera(int cameraIndex, WINDOW_TYPE window);

void main() {

/*******************************************************************/
// Stereo Correspondence
/*******************************************************************/

cout << "Stereo Correspondence test... ";

IplImage* srcLeft = cvLoadImage("left.jpg", 1);
IplImage* srcRight = cvLoadImage("right.jpg", 1);

CvSize size = cvSize(320, 240);

IplImage* leftImage = cvCreateImage(size, IPL_DEPTH_8U, 1);
IplImage* rightImage = cvCreateImage(size, IPL_DEPTH_8U, 1);
IplImage* depthImage = cvCreateImage(size, IPL_DEPTH_8U, 1);

//cvConvertImage(srcLeft, leftImage, 0);
//cvConvertImage(srcRight, rightImage, 0);

//cvCvtColor(srcLeft, leftImage, CV_BGR2GRAY);
//cvCvtColor(srcRight, rightImage, CV_BGR2GRAY);

//cvFindStereoCorrespondence( leftImage, rightImage, CV_DISPARITY_BIRCHFIELD, depthImage, 50, 15, 3, 6, 8, 15 );

/*
WINDOW_TYPE window = createWindow("Stereo depth");
cvShowImage("Stereo depth", depthImage);
cvWaitKey(0);
cvDestroyWindow("Stereo depth");
cvReleaseImage(&depthImage);
*/
cout << "completed\n\n";

/*******************************************************************/

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

	// Setup the cameras to use.
	Camera camera1 = Camera(0);
	Camera camera2 = Camera(1);

	// Create the video windows.
	WINDOW_TYPE window1 = createWindow("Camera 1");
	WINDOW_TYPE window2 = createWindow("Camera 2");

	// Set the cameras video windows.
	camera1.setWindow(window1);
	camera2.setWindow(window2);

	success = cvcamInit();  // Initalize the camera(s).
	
	if(DEBUG && success) debug("Camera initialized...");
	else debug("(!) Unable to initialize camera");

	success = cvcamStart();  // Start camera(s).
	
	if(DEBUG && success) debug("(!) Unable to start camera");
	else debug("Camera started...");

/*******************************************************************/
// Calibrate camera
/*******************************************************************/

	if(DEBUG) debug("Calibrating camera...");
/*
	success = cv3dTrackerCalibrateCameras(numberOfCameras,
           const Cv3dTrackerCameraIntrinsics camera_intrinsics[],
           CvSize checkerboard_size,
           IplImage *samples[],
           Cv3dTrackerCameraInfo camera_info[]);
*/

	success = false;

	if(DEBUG && success) debug("(!) Unable to calibrate camera");
	else debug("Camera calibrated...");

/*******************************************************************/

	MessageBox(NULL, "Press any key to exit", NULL, MB_OK);
	cvvWaitKey(0); // Wait for a key.

	success = cvcamStop();  // Stop camera(s).

	if(DEBUG && success) debug("(!) Unable to stop camera");
	else debug("Camera stopped...");

	success = cvcamExit();  // Exit.

	if(DEBUG && success) debug("(!) Unable to exit");
	else debug("Exited...");

	if(DEBUG)
		debug("=== End debugging information ===");
}

WINDOW_TYPE createWindow(char* windowName) {

	// Creates a named window.
    cvNamedWindow(windowName, CV_WINDOW_AUTOSIZE);

	// Returns the window handle.
    return (WINDOW_TYPE) cvGetWindowHandle(windowName);
}