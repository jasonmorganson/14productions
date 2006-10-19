/*** Design 1 ***/

#include "cv.h"       // Include core library interface.
#include "highgui.h"  // Include GUI library interface.
#include "cvaux.h"    // Include expirimental library.
#include "cvcam.h"    // Include cvcam libray.
#include "Definitions.h"
#include "Camera.h"
#include "BackgroundFilter.h"
#include "Tracker.h"
#include <iostream>
#include <conio.h>
#include <string>

using namespace std;

void removeNoise( IplImage* src );
void callback( IplImage* src );

int frames = 0;
int step = 0;
int apertureSize = 3;   // Must be 1, 3, 5 or 7
int dx = 1;             // Sobel dx
int dy = 1;             // Sobel dy
int threshold1 = 0;     // Canny threshold1
int threshold2 = 300;   // Canny threshold2
int blockSize = 3;      // Neighborhood size
int qualityLevel = 3;   // Multiplier for the maxmin eigenvalue; specifies minimal accepted quality of image corners. 
int minDistance = 3;    // Limit, specifying minimum possible distance between returned corners; Euclidian distance is used. 
float alpha, beta, gamma = 0;

BackgroundFilter backgroundFilter = BackgroundFilter();

void main() {

	Camera camera = Camera(0);
	camera.setWindow("Source");
	camera.setCallback(callback);

	cvNamedWindow( "Source", CV_WINDOW_AUTOSIZE );
	cvNamedWindow( "Background Image", CV_WINDOW_AUTOSIZE );
//	cvNamedWindow( "Sobel Image", CV_WINDOW_AUTOSIZE );
//	cvNamedWindow( "Laplace Image", CV_WINDOW_AUTOSIZE );
//	cvNamedWindow( "Canny Image", CV_WINDOW_AUTOSIZE );
//	cvNamedWindow( "Min Eigen Image", CV_WINDOW_AUTOSIZE );
//	cvNamedWindow( "Corners Image", CV_WINDOW_AUTOSIZE );
//	cvNamedWindow( "Eigen Image", CV_WINDOW_AUTOSIZE );
//	cvNamedWindow( "Temp Image", CV_WINDOW_AUTOSIZE );
//	cvNamedWindow( "Contours Image", CV_WINDOW_AUTOSIZE );
//	cvNamedWindow( "Snake Image", CV_WINDOW_AUTOSIZE );

//	cvCreateTrackbar( "dx", "Sobel Image", &dx, 2, NULL );
//	cvCreateTrackbar( "dy", "Sobel Image", &dy, 2, NULL );

//	cvCreateTrackbar( "Threshold 1", "Canny Image", &threshold1, 1000, NULL );
//	cvCreateTrackbar( "Threshold 2", "Canny Image", &threshold2, 1000, NULL );

//	cvCreateTrackbar( "Quality",  "Eigen Image", &qualityLevel+1, 10, NULL );
//	cvCreateTrackbar( "Min Dist", "Eigen Image", &minDistance, 10, NULL );

//	cvCreateTrackbar( "Alpha", "Snake Image", (int*)&alpha, 1000, NULL );
//	cvCreateTrackbar( "Beta",  "Snake Image", (int*)&beta,  1000, NULL );
//	cvCreateTrackbar( "Gamma", "Snake Image", (int*)&gamma, 1000, NULL );

	cvcamInit();   // Initalize the camera.
	cvcamStart();  // Start camera.
	
	cvvWaitKey(0); // Wait for a key.

	cvcamStop();   // Stop camera.
	cvcamExit();   // Exit.

	cvDestroyWindow( "Source" );
	cvDestroyWindow( "Background Image" );
//	cvDestroyWindow( "Sobel Image" );
//	cvDestroyWindow( "Laplace Image" );
//	cvDestroyWindow( "Canny Image" );
//	cvDestroyWindow( "Min Eigen Image" );
//	cvDestroyWindow( "Corners Image" );
//	cvDestroyWindow( "Eigen Image" );
//	cvDestroyWindow( "Temp Image" );
//	cvDestroyWindow( "Contours Image" );
//	cvDestroyWindow( "Snake Image" );
}


void removeNoise( IplImage* src ) {
	
	CvSize sz = cvSize( src->width & -2, src->height & -2 );
	
	IplImage* pyr = cvCreateImage( cvSize(sz.width/2, sz.height/2), src->depth, src->nChannels );
	
	cvPyrDown( src, pyr, 7 );
	cvPyrUp( pyr, src, 7 );
	cvReleaseImage( &pyr );
}

void callback( IplImage* src ) {

	/* for cvFindCountours */
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contour = 0;

	/* for cvGoodFeaturesToTrack */	
//	CvPoint2D32f corners;
//	int cornerCount = 10;

	/* for cvSnakeImage */
//	CvPoint* points;
//	int length = 10;
//	CvSize win = cvSize( 3, 3 );  // must be odd

//	CvTermCriteria criteria;
//	criteria.type = CV_TERMCRIT_ITER;
//	criteria.maxIter = 10;

	frames++;

	CvSize size = cvGetSize(src);

	IplImage* srcImage        = cvCreateImage( size, IPL_DEPTH_8U,  1 );
	IplImage* backgroundImage = cvCreateImage( size, IPL_DEPTH_8U,  1 );
	IplImage* sobelImage      = cvCreateImage( size, IPL_DEPTH_16S, 1 );
 	IplImage* laplaceImage    = cvCreateImage( size, IPL_DEPTH_16S, 1 );
 	IplImage* cannySrcImage   = cvCreateImage( size, IPL_DEPTH_8U,  1 );
	IplImage* cannyImage      = cvCreateImage( size, IPL_DEPTH_8U,  1 );
	IplImage* minEigenImage   = cvCreateImage( size, IPL_DEPTH_8U,  1 );
 	IplImage* cornersImage    = cvCreateImage( size, IPL_DEPTH_32F, 1 );
	IplImage* eigenImage      = cvCreateImage( size, IPL_DEPTH_32F, 1 );
	IplImage* tempImage       = cvCreateImage( size, IPL_DEPTH_32F, 1 );
	IplImage* contoursImage   = cvCreateImage( size, IPL_DEPTH_8S,  3 );	
	IplImage* snakeImage      = cvCreateImage( size, IPL_DEPTH_8S,  3 );

	cvConvertImage( src, srcImage, 1);

//	removeNoise( srcImage );
//	cvSobel( srcImage, sobelImage, dx, dy, apertureSize );
//	cvLaplace( srcImage, laplaceImage, apertureSize );
//	cvCanny( srcImage, cannyImage, threshold1, threshold2, apertureSize ); 
//	cvDilate( cannyImage, cannyImage, 0, 1 );
//	cvCornerMinEigenVal( If srcImage, minEigenImage, blockSize, apertureSize );
//	cvPreCornerDetect( cannyImage, cornersImage, apertureSize );
//	cvGoodFeaturesToTrack( cannyImage, eigenImage, tempImage, &corners, &cornerCount, qualityLevel, minDistance );
/*	cvFindContours( cannyImage, storage, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_NONE );

	// Draw contours
	for( cvZero( contoursImage ); contour != 0; contour = contour->h_next ) {
		int color = CV_RGB( rand(), rand(), rand() );
        cvDrawContours( contoursImage, contour, color, color, -1, 1, 8 );
	}
*/
//	cvSnakeImage( contoursImage, points, length, &alpha, &beta, &gamma, CV_VALUE, win, criteria, 0 );

	cvShowImage( "Source", srcImage );
	cvShowImage( "Background Image", backgroundImage );
//	cvShowImage( "Sobel Image", sobelImage );
//	cvShowImage( "Laplace Image", laplaceImage );
//	cvShowImage( "Canny Image", cannyImage );
//	cvShowImage( "Corners Image", cornersImage );
//	cvShowImage( "Min Eigen Image", minEigenImage );
//	cvShowImage( "Eigen Image", cornersImage );
//	cvShowImage( "Temp Image", cornersImage );
//	cvShowImage( "Contours Image", contoursImage );

	cvReleaseImage( &srcImage );
//	cvReleaseImage( &backgroundImage );
	cvReleaseImage( &sobelImage );
	cvReleaseImage( &laplaceImage );
	cvReleaseImage( &cannyImage );
	cvReleaseImage( &minEigenImage );
	cvReleaseImage( &cornersImage );
	cvReleaseImage( &eigenImage );
	cvReleaseImage( &tempImage );
	cvReleaseImage( &contoursImage );

	cvReleaseMemStorage( &storage );
}
