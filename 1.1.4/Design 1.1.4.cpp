/*** Design 1 ***/

#include "cv.h"       // Include core library interface.
#include "highgui.h"  // Include GUI library interface.
#include "cvaux.h"    // Include expirimental library.
#include "cvcam.h"    // Include cvcam libray.
#include "Definitions.h"
#include "Camera.h"
#include "Tracker.h"
#include "NoiseFilter.h"
#include "BackgroundFilter.h"
//#include "Contours.h"
#include <iostream>
#include <conio.h>
#include <string>

using namespace std;

#define MAX_FRAMES 10  // Number of past frames to process.

void removeNoise( IplImage* src );
void callback( IplImage* src );
CvSeq* findContours( IplImage* src, CvSeq* previous_contour );

int canny_threshold_1 = 0;  // Canny threshold 1
int canny_threshold_2 = 0;  // Canny threshold 2
int apertureSize = 3;       // Must be 1, 3, 5 or 7

int R = 0;
int G = 0;
int B = 0;

int frames = 0;
int frame_index = 0;
CvSeq* previous_contours[MAX_FRAMES];
CvSeq* prev = 0;
CvSeq* previous_contour = 0;

NoiseFilter noiseFilter = NoiseFilter();
BackgroundFilter backgroundFilter = BackgroundFilter();

void main() {

	Camera camera = Camera(0);
	camera.setWindow("Video");
	camera.setCallback(callback);

	cvNamedWindow( "Frame", CV_WINDOW_AUTOSIZE );
	cvNamedWindow( "Mask", CV_WINDOW_AUTOSIZE );
	cvNamedWindow( "Sq", CV_WINDOW_AUTOSIZE );

//	cvNamedWindow( "Settings", 0 );
//	cvCreateTrackbar( "Canny Thresh 1", "Settings", &canny_threshold_1, 100, NULL );
//	cvCreateTrackbar( "Canny Thresh 2", "Settings", &canny_threshold_2, 100, NULL );

	cvcamInit();   // Initalize the camera.
	cvcamStart();  // Start camera.
	
	cvvWaitKey(0); // Wait for a key.

	cvcamStop();   // Stop camera.
	cvcamExit();   // Exit.

	
	cvDestroyWindow( "Frame" );
	cvDestroyWindow( "Mask" );
	cvDestroyWindow( "Sq" );

}


void removeNoise( IplImage* src ) {

	cvSmooth( src, src, CV_GAUSSIAN, 3, 0 );

	CvSize size = cvSize( src->width & -2, src->height & -2 );
	
	IplImage* pyrImage = cvCreateImage( cvSize(size.width/2, size.height/2), src->depth, src->nChannels );
	
	cvPyrDown( src, pyrImage, 7 );
	cvPyrUp( pyrImage, src, 7 );
	cvReleaseImage( &pyrImage );
}


void callback( IplImage* src ) {

	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* current_contours = 0;

	CvSize size = cvGetSize(src);

	IplImage* frame = cvCreateImage( size, IPL_DEPTH_8U, 3 );
	IplImage* backgroundImage = cvCreateImage( size, IPL_DEPTH_8U, 1 );
	IplImage* cannyImage = cvCreateImage( size, IPL_DEPTH_8U, 1 );
	IplImage* hsvImage = cvCreateImage( size, IPL_DEPTH_8U, 3 );
	IplImage* h_plane = cvCreateImage( size, IPL_DEPTH_8U, 1 );

	cvConvertImage( src, frame, 1 );

	//removeNoise( frame );
	//noiseFilter.update( frame );

	//backgroundFilter.update( frame, frames );

	//backgroundImage = backgroundFilter.getBackgroundMask( );

	//cvCanny( srcImage, cannyImage, canny_threshold_1, canny_threshold_2, apertureSize );

/*


	CvContourScanner contour_scanner = cvStartFindContours( cannyImage, storage, sizeof(CvContour), CV_RETR_TREE, CV_CHAIN_APPROX_NONE );

	// Find the current contours.
	while( current_contours = cvFindNextContour( contour_scanner ) ) {

		// Loop through all of the stored past frames.
		for( int past_frame_index = 0; past_frame_index <= MAX_FRAMES; past_frame_index++ ) {

			CvSeq* temp = previous_contours[past_frame_index];

			if( previous_contours[past_frame_index] != 0 ) {

cout << "matching contours...\n";
cout << "current_contours: " << current_contours << endl;
cout << "previous_contours[" << past_frame_index << "]: " << previous_contours[past_frame_index] << endl;

CvSeq* test = previous_contours[past_frame_index]->h_next;

double match = cvMatchContours( current_contours, test, CV_CONTOURS_MATCH_I3, 0 );
cout << "match: " << match << endl;

//				while( prev = temp->h_next ) {
//					double match = cvMatchContours( current_contours, prev, CV_CONTOURS_MATCH_I1, 0 );
//					cout << "match: " << match << endl;
//				}

		
			}

			previous_contours[past_frame_index] = temp;  // return pointer to original index
		}


	}

	current_contours = cvEndFindContours( &contour_scanner );


	previous_contours[frame_index] = current_contours;



	// Draw the current contours.
	while( current_contours = current_contours->h_next )
			cvDrawContours( src, current_contours, CV_RGB( 0, 255, 0 ), CV_RGB( 0, 255, 0 ), 0, 1, 8 );
*/
	cout << "FRAME " << frames << endl;

	frames++;
	frame_index++;
	if( frame_index >= MAX_FRAMES ) /* --> */ frame_index = 0;



/*
double contourArea = cvContourArea( contour, CV_WHOLE_SEQ );
int contourConvexity = cvCheckContourConvexity( contour );
int arcLength = cvArcLength( contour, CV_WHOLE_SEQ, CV_SEQ_FLAG_CLOSED );

cout << "contourArea: " << contourArea << endl;
cout << "contourConvexity: " << contourConvexity << endl;
cout << "arcLength: " << arcLength << endl;
cout << "***********************" << endl;
*/

	cvReleaseImage( &frame );
	cvReleaseImage( &cannyImage );

	cvReleaseMemStorage( &storage );
}