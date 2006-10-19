/*** Look at using CvCamera and CvStereoCamera in cvaux.h */

#include "cv.h"       // Include core library interface.
#include "highgui.h"  // Include GUI library interface.
#include "cvaux.h"    // Include expirimental library.
#include "cvcam.h"    // Include cvcam libray.
#include "Definitions.h"
#include <iostream>
#include <conio.h>
#include <string>

using namespace std;

#define DEFAULT_WIDTH 640;   // Defines the default video width.
#define DEFAULT_HEIGHT 480;  // Defines the default video height.

class Camera {

public:

	Camera(int cameraIndex);
	~Camera();
	void setWindow(WINDOW_TYPE window);
	void setCallback(void* callbackFunction);

private:

	int width;   // Video render width.
	int height;  // Video render height.
	int cameraIndex;  // Index of the camera.
	WINDOW_TYPE window;  // Pointer to the video window.
	void* callback;  // Pointer to the callback function.
};

Camera::Camera(int cameraIndex):
	width(640), height(480), cameraIndex(cameraIndex) {

	int success = false;

	if(DEBUG)
		cout << "Camera: " << cameraIndex << endl;

	// Enables the camera.
	success = cvcamSetProperty(cameraIndex, CVCAM_PROP_ENABLE, CVCAMTRUE);	

	if(success) debug("(!) Unable to enable camera");
	else debug("Camera enabled...");

	// Set the width and height of the video.
	cvcamSetProperty(cameraIndex, CVCAM_RNDWIDTH, &width);	
	cvcamSetProperty(cameraIndex, CVCAM_RNDHEIGHT, &height);

	// Renders the stream from this source.
	success = cvcamSetProperty(cameraIndex, CVCAM_PROP_RENDER, CVCAMTRUE);
	
	if(success) debug("(!) Unable to start stream");
	else debug("Stream started...");
}

void Camera::setWindow(WINDOW_TYPE window) {

	int success = false;

	window = window;

	// Select the window for video rendering.
	success = cvcamSetProperty(cameraIndex, CVCAM_PROP_WINDOW, &window);
	
	if(success) debug("(!) Unable to anchor window");
	else debug("Window anchored...");
}

void Camera::setCallback(void* callback) {

	int success = false;

	callback = callback;

	// Select the window for video rendering.
	success = cvcamSetProperty(cameraIndex, CVCAM_PROP_CALLBACK, &callback);
	
	if(success) debug("(!) Unable to link callback");
	else debug("Callback linked...");
}

Camera::~Camera() {
	
	// Find the name of the video window accociated with the camera.
	const char* windowName = cvGetWindowName(window);

	// Destroy video window.
	cvDestroyWindow(windowName);
}