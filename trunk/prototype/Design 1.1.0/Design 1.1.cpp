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

/* Define the variables for the stereo correspondence algorithm. */

// Maximum possible disparity. The closer the objects to the cameras,
// the larger value should be specified here. Too big values slow down
// the process significantly. 
#define MAX_DISPARITY 50

#define CONSTANT_OCCLUSION_PENALTY 15		// Constant occlusion penalty.
#define CONSTANT_MATCH_REWARD 3				// Constant match reward.
#define HIGHLY_RELIABLE_REGION 6			// Defines a highly reliable region (set of contiguous pixels whose reliability is at least this large)
#define MODERATELY_RELIABLE_REGION 8		// Defines a moderately reliable region
#define SLIGHTLY_RELIABLE_REGION 15			// Defines a slightly reliable region.

// If some parameter is omitted default value is used.
// In Birchfield's algorithm param1 = 25, param2 = 5, param3 = 12, param4 = 15, param5 = 25

/* Creates a window with the specified name and returns the pointer to it. */
WINDOW_TYPE createWindow(char* windowName);

/* Calculates and displays the stereo correspondence of the two cameras. */
void stereocallback(IplImage* srcLeft, IplImage* srcRight);

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

	// Setup the cameras to use.
	Camera camera1 = Camera(0);
	Camera camera2 = Camera(1);

	// Create the video windows.
	WINDOW_TYPE camera_1_window = createWindow("Camera 1");
	WINDOW_TYPE camera_2_window = createWindow("Camera 2");
	WINDOW_TYPE depth_window = createWindow("Depth Image");

	// Set the cameras video windows.
	camera1.setWindow(camera_1_window);
	camera2.setWindow(camera_2_window);

	// Set the cameras callback function.
//	cvcamSetProperty(0, CVCAM_STEREO_CALLBACK , stereocallback);
//	cvcamSetProperty(1, CVCAM_STEREO_CALLBACK , stereocallback);

	success = cvcamInit();  // Initalize the camera(s).
	
	if(DEBUG && success) debug("Camera initialized...");
	else debug("(!) Unable to initialize camera");

	success = cvcamStart();  // Start camera(s).
	
	if(DEBUG && success) debug("(!) Unable to start camera");
	else debug("Camera started...");

cvvWaitKey(0); // Wait for a key.

	success = cvcamStop();  // Stop camera(s).

	if(DEBUG && success) debug("(!) Unable to stop camera");
	else debug("Camera stopped...");

	success = cvcamExit();  // Exit.

	if(DEBUG && success) debug("(!) Unable to exit");
	else debug("Exited...");

	if(DEBUG) debug("=== End debugging information ===");
}


void stereocallback(IplImage* srcLeft, IplImage* srcRight) {

	// Allocate memory for images.
	IplImage* leftImage = cvCreateImage(cvGetSize(srcLeft), IPL_DEPTH_8U, 1);
	IplImage* rightImage = cvCreateImage(cvGetSize(srcRight), IPL_DEPTH_8U, 1);
	IplImage* depthImage = cvCreateImage(cvGetSize(srcRight), IPL_DEPTH_8U, 1);

	// Convert images to gray scale.
	cvCvtColor(srcLeft, leftImage, CV_BGR2GRAY);
	cvCvtColor(srcRight, rightImage, CV_BGR2GRAY);

	// Find the stereo corresopondence depth image.
	cvFindStereoCorrespondence( leftImage,
								rightImage,
								CV_DISPARITY_BIRCHFIELD,
								depthImage,
								MAX_DISPARITY,
								CONSTANT_OCCLUSION_PENALTY,
								CONSTANT_MATCH_REWARD,
								HIGHLY_RELIABLE_REGION,
								MODERATELY_RELIABLE_REGION,
								SLIGHTLY_RELIABLE_REGION );

	// Display the depth image.
	cvShowImage( "Depth Image", depthImage );
}


WINDOW_TYPE createWindow(char* windowName) {

	// Creates a named window.
    cvNamedWindow(windowName, CV_WINDOW_AUTOSIZE);

	// Returns the window handle.
    return (WINDOW_TYPE) cvGetWindowHandle(windowName);
}






	/* Stereo Correspondence algorithm. */
	
	/*
	
	FindStereoCorrespondence
	Calculates disparity for stereo-pair
	cvFindStereoCorrespondence(
                   const  CvArr* leftImage, const  CvArr* rightImage,
                   int     mode, CvArr*  depthImage,
                   int     maxDisparity,
                   double  param1, double  param2, double  param3,
                   double  param4, double  param5  );
	

	if(DEBUG) debug("Loading images");

	IplImage* srcLeft = cvLoadImage( "left.jpg", 1 );
	IplImage* srcRight = cvLoadImage( "right.jpg", 1 );

	if(DEBUG) debug("Images loaded");
	if(DEBUG) debug("Creating copies");

	IplImage* leftImage = cvCreateImage(cvGetSize(srcLeft), IPL_DEPTH_8U, 1);
	IplImage* rightImage = cvCreateImage(cvGetSize(srcRight), IPL_DEPTH_8U, 1);
	IplImage* depthImage = cvCreateImage(cvGetSize(srcRight), IPL_DEPTH_8U, 1);

	if(DEBUG) debug("Copies created");
	if(DEBUG) debug("Converting images");

	cvCvtColor(srcLeft, leftImage, CV_BGR2GRAY);
	cvCvtColor(srcRight, rightImage, CV_BGR2GRAY);

	if(DEBUG) debug("Images converted");
	if(DEBUG) debug("Computing correspondence");

	cvFindStereoCorrespondence( leftImage, rightImage, CV_DISPARITY_BIRCHFIELD, depthImage, 50, 15, 3, 6, 8, 15 );

//	cvFindStereoCorrespondence( leftImage, rightImage, CV_DISPARITY_BIRCHFIELD, depthImage, 50, 25, 5, 12, 15, 25 );

	if(DEBUG) debug("Stereo Correspondence results:");

	cvNamedWindow( "Depth Image", CV_WINDOW_AUTOSIZE );
	cvShowImage( "Depth Image", depthImage );
	cvWaitKey(0); // wait for input
*/
