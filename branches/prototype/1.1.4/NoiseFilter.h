#include "cv.h"       // Include core library interface.
#include "highgui.h"  // Include GUI library interface.
#include "cvaux.h"    // Include expirimental library.
#include "cvcam.h"    // Include cvcam libray.
#include "Definitions.h"

using namespace std;

class NoiseFilter {

public:

	void update( IplImage* src );

private:

	void smooth( IplImage* src );
	void erode( IplImage* src );
};

void NoiseFilter::update( IplImage* src ) {

	smooth( src );
	erode( src );
}

void NoiseFilter::smooth( IplImage* src ) {

	cvSmooth( src, src, CV_GAUSSIAN, 3, 0 );
}

void NoiseFilter::erode( IplImage* src ) {

	CvSize size = cvSize( src->width & -2, src->height & -2 );
	
	IplImage* pyrImage = cvCreateImage( cvSize(size.width/2, size.height/2), src->depth, src->nChannels );
	
	cvPyrDown( src, pyrImage, 7 );
	cvPyrUp( pyrImage, src, 7 );
	cvReleaseImage( &pyrImage );
}