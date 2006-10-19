#include "cv.h"       // Include core library interface.
#include "highgui.h"  // Include GUI library interface.
#include "cvaux.h"    // Include expirimental library.
#include "cvcam.h"    // Include cvcam libray.
#include "Definitions.h"
#include <iostream>
#include <conio.h>
#include <string>

using namespace std;

class Contours {

public:
	void add();
	void remove();
	void find() { cvFindContours( cannyImage, storage, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_NONE ) };


private:

	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contour = 0;
};


class Contour {

public:

	Contour() {} : contour = 0
	CvSeq* next() { return contour->h_next };

private:

	CvSeq* contour;
	double area;
	int color;
};