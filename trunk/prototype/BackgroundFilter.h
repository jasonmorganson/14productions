#include "cv.h"       // Include core library interface.
#include "highgui.h"  // Include GUI library interface.
#include "cvaux.h"    // Include expirimental library.
#include "cvcam.h"    // Include cvcam libray.
#include "Definitions.h"

using namespace std;

#define SIGMA 3   // Sigma constant

class BackgroundFilter {

public:

	BackgroundFilter();
	~BackgroundFilter();
	void update( IplImage* src, int nFrame );  // Add the current frame (N = int frames) to the filter
	IplImage* getBackgroundMask( ) { return mask; }

private:

	// * NEED TO AUTO ADJUST SIZE *

	IplImage* S;
	IplImage* Sq;
	IplImage* mean;
	IplImage* stdDev;
	IplImage* mask;
};



BackgroundFilter::BackgroundFilter() {

	const CvSize size = cvSize(320, 240);

	S      = cvCreateImage( size, IPL_DEPTH_32F, 1 );
	Sq     = cvCreateImage( size, IPL_DEPTH_32F, 1 );
	mean   = cvCreateImage( size, IPL_DEPTH_32F, 1 );
	stdDev = cvCreateImage( size, IPL_DEPTH_32F, 1 );
	mask   = cvCreateImage( size, IPL_DEPTH_8U,  1 );
}

BackgroundFilter::~BackgroundFilter() {

	cvReleaseImage( &S );
	cvReleaseImage( &Sq );
	cvReleaseImage( &mean );
	cvReleaseImage( &stdDev );
	cvReleaseImage( &mask );
}

void BackgroundFilter::update( IplImage* src, int nFrame ) {

	const CvSize size = cvSize(320, 240);

	IplImage* frame  = cvCreateImage( size, IPL_DEPTH_8U,  1 );
	IplImage* N      = cvCreateImage( size, IPL_DEPTH_32F, 1 );
	IplImage* meanSq = cvCreateImage( size, IPL_DEPTH_32F, 1 );
	IplImage* SqMean = cvCreateImage( size, IPL_DEPTH_32F, 1 );
	IplImage* sub    = cvCreateImage( size, IPL_DEPTH_32F, 1 );

	cvConvertImage( src, frame, 0 );

	// Accumulate images
	cvAcc( frame, S );            // S(x,y)
	cvSquareAcc( frame, Sq );     // Sq(x,y)

///////////////////////////
cvShowImage( "Sq", Sq );
///////////////////////////


	// Set image values to the current frame count
	cvSet( N, cvScalar(nFrame) );

	// Find mean
	cvDiv( S, N, mean );          // mean = S(x,y) / N

	// Find standard deviation
	cvDiv( Sq, N, SqMean );       // Sq(x,y) / N
	cvPow( mean, meanSq, 2 );     // mean^2
	cvSub( SqMean, meanSq, sub ); // Sq/N - mean^2
	cvPow( sub, stdDev, 1./2 );   // stdDev = sqrt( Sq/N - mean^2 )        ***** SLOW!!! *****

	cvReleaseImage( &N );
	cvReleaseImage( &meanSq );
	cvReleaseImage( &SqMean );
	cvReleaseImage( &sub );

	/*** Find background mask ***/



	IplImage* mean8U    = cvCreateImage( size, IPL_DEPTH_8U, 1 );
	IplImage* stdDev8U  = cvCreateImage( size, IPL_DEPTH_8U, 1 );
	IplImage* sigma     = cvCreateImage( size, IPL_DEPTH_8U, 1 );	
	IplImage* sigmaRule = cvCreateImage( size, IPL_DEPTH_8U, 1 );	
	IplImage* abs       = cvCreateImage( size, IPL_DEPTH_8U, 1 );

	// Convert images into 8U
	cvConvertImage( mean, mean8U, 0 );
	cvConvertImage( stdDev, stdDev8U, 0 );

cvShowImage( "Frame", frame );

	cvSet( sigma, cvScalar(SIGMA) );
	cvAbsDiff( mean8U, frame, abs ); // abs( mean - p(x,y) )
	cvMul( stdDev8U, sigma, sigmaRule );  // SIGMA * stdDev
	cvCmp( abs, sigmaRule, mask, CV_CMP_GT );  // abs( mean - p(x,y) ) > SIGMA * stdDev

cvShowImage( "Mask", mask );

	cvReleaseImage( &sigma );
	cvReleaseImage( &abs );

	cvReleaseImage( &frame );
}