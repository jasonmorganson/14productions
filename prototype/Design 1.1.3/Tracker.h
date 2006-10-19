#include "cv.h"       // Include core library interface.
#include "highgui.h"  // Include GUI library interface.
#include "cvaux.h"    // Include expirimental library.
#include "cvcam.h"    // Include cvcam libray.
#include "Definitions.h"
#include <iostream>
#include <conio.h>
#include <string>

using namespace std;

class Tracker {

public:

	Tracker();
	~Tracker();
	void addFilter(int filter);
	void calibrateWith(int nFrames);
	void calibrate();
	bool isCalibrated();
	void findStereo();
	void findForegroundDist();
	void setForegroundDist();
	void getForegroundMask();
	void getForegroundImage();
	void start();
	void getTrackingData();

private:

	int numberOfCameras;  // The number of available cameras in the system.
//	Camera cameras[];  // An array of cameras attached to the system.
};