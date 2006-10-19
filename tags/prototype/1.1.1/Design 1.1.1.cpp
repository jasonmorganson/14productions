/*** Design 1 ***/

#include "cv.h"       // Include core library interface.
#include "highgui.h"  // Include GUI library interface.
#include "cvaux.h"    // Include expirimental library.
#include "cvcam.h"    // Include cvcam libray.
#include "Definitions.h"
#include "Camera.h"
#include "Tracker.h"
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

/* Define the variables for the thre threshold filter. */

#define THRESHOLD 20  // Threshold value. 
#define MAXVALUE 255  // Maximum value to use with CV_THRESH_BINARY, CV_THRESH_BINARY_INV, and CV_THRESH_TRUNC thresholding types. 
#define THRESHOLD_TYPE CV_THRESH_BINARY // Thresholding type (CV_THRESH_BINARY, CV_THRESH_BINARY_INV, CV_THRESH_TRUNC, CV_THRESH_TOZERO, and CV_THRESH_TOZERO_INV)

CvCalibFilter CalibFilter;
double EtalonParams[3] = { 6, 8, 3 };  /*
										etalonParams[0] is number of squares per one side of etalon
										etalonParams[1] is number of squares per another side of etalon
										etalonParams[2] is linear size of squares in the board in arbitrary units. */

#define NUM_CALIBRATION_FRAMES 15  // Set the number of frames to use for calibration.

/* Performs pre-processing functions on the left and right images. */
void preProcessImages(IplImage** left, IplImage** right);

/* Performs pre-processing functions an image. */
IplImage* preProcessImage(IplImage* src);

/* Creates a window with the specified name and returns the pointer to it. */
WINDOW_TYPE createWindow(char* windowName);

/* Calculates and displays the stereo correspondence of the two cameras. */
IplImage* findStereoCorrespondence(IplImage* srcLeft, IplImage* srcRight);

/* Creates a binary image mask from the given image. */
IplImage* createMask(IplImage* src);

void removeNoise(IplImage* src);

/* Filters the source images using the given mask image. */
IplImage* filter(IplImage* srcLeft, IplImage* srcRight, IplImage* mask);

enum { NONE, WAIT, CLOSE, QUIT };
void display(char* windowName, IplImage* image, int flag);
void display(char* windowName, IplImage* image) { display(windowName, image, NONE); }

void main() {

	// Find the number of available cameras in the system.
	int numberOfCameras = cvcamGetCamerasCount( );

	// Make sure that there is at least one camera connected before proceeding.
	if(numberOfCameras == 0) {
		cout << "*** No cameras found on the system ***" << endl;
//		exit(EXIT_FAILURE);
	}

/*********************************************************************************/

    CalibFilter.SetEtalon( CV_CALIB_ETALON_CHESSBOARD, EtalonParams );
    CalibFilter.SetCameraCount(numberOfCameras);  // Set the number of cameras.
    CalibFilter.SetFrames(NUM_CALIBRATION_FRAMES);  // Set the number of frames to use for calibration.

//	IplImage* calibImage = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );
//	CalibFilter.DrawPoints(&calibImage);

//	void cvFindFundamentalMatrix (int* points1, int* points2, int numpoints, 0, CvMatrix3* matrix);
//	cvMakeScanlines (CvMatrix3* matrix, CvSize imgSize, int* scanlines1, int* scanlines2, int* lens1, int* lens2, int* numlines);
//	cvPreWarpImage (int numLines, IplImage* img, uchar* dst, int* dstNums, int* scanlines);

/*********************************************************************************/

	// Load the source images.
	IplImage* srcLeft = cvLoadImage( "left.jpg", 1 );
	IplImage* srcRight = cvLoadImage( "right.jpg", 1 );

	// Get the images from left and right.
	IplImage* leftImage = cvCloneImage(srcLeft);
	IplImage* rightImage = cvCloneImage(srcRight);

	// Pre-process the images.
	preProcessImages(&leftImage, &rightImage);

	// Find the stereo correspondence between the images.
	IplImage* depthImage = findStereoCorrespondence(leftImage, rightImage);

	// Create the image mask.
	IplImage* maskImage = createMask(depthImage);

	// Filter the image.
	IplImage* filterImage = filter(srcLeft, srcRight, maskImage);

	int dx = 1;
	int dy = 1;
	int apertureSize = 3;  // must be 1, 3, 5 or 7
	double threshold1 = 1;
	double threshold2 = 2;

	IplImage* sobelImage = cvCreateImage( cvGetSize(filterImage), IPL_DEPTH_16S, 1 );
 	IplImage* laplaceImage = cvCreateImage( cvGetSize(filterImage), IPL_DEPTH_16S, 1 );
 	IplImage* cannyImage = cvCreateImage( cvGetSize(filterImage), IPL_DEPTH_8U, 1 );
   
	cvSobel( maskImage, sobelImage, dx, dy, apertureSize );
	cvLaplace( maskImage, laplaceImage, apertureSize );
	cvCanny( maskImage, cannyImage, threshold1, threshold2, apertureSize );

	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contour = 0;
	IplImage* src = cvCloneImage(maskImage);

	cvFindContours( src, storage, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_NONE );

	IplImage* contoursImage = cvCreateImage( cvGetSize(src), 8, 3 );

    for( cvZero( contoursImage ); contour != 0; contour = contour->h_next ) {
        int externalColor = CV_RGB( rand(), rand(), rand() );
        int holeColor = CV_RGB( 255, 255, 255 );
        cvDrawContours( contoursImage, contour, externalColor, holeColor, -1, 1, 8 );
    }

// Finds the rectangle that the countour image fits into
// Use this to set the ROI and filter out outlying noise
	// Cant find proper matrix parameter
	// Probably wont work anyways
	// Probably better to use a statistical function
//	CvRect rect = cvBoundingRect( contoursImage, 1 );
//	CvPoint pt1 = cvPoint( rect.x, rect.y );
//	CvPoint pt2 = cvPoint( rect.x + rect.width, rect.y + rect.height);
//  cvRectangle( contoursImage, pt1, pt2, CV_RGB( rand(), rand(), rand() ), 1 );


	IplImage* searchImage = maskImage;
	IplImage* templateImage = maskImage;

	// If I is W×H and T is w×h then R must be W-w+1×H-h+1.
	int width = searchImage->width - (templateImage->width+1);
	int height = searchImage->height - (templateImage->height+1);
	CvSize size = cvSize(width, height);
	IplImage* matchImage = cvCreateImage( size, IPL_DEPTH_32F, 1 );

// Fails because the template image is to big, size of match image get set to (-1, -1)
//	cvMatchTemplate( searchImage, templateImage, matchImage, CV_TM_SQDIFF );

	// Save images.	
	cvSaveImage("depth.jpg", depthImage);
	cvSaveImage("mask.jpg", maskImage);
	cvSaveImage("filter.jpg", filterImage);

	// Create windows.
	display( "Left Image", srcLeft, CLOSE );
	display( "Right Image", srcRight, WAIT | CLOSE );
	display( "Depth Image", depthImage );
	display( "Mask Image", maskImage );
	display( "Filter Image", filterImage, WAIT | CLOSE  );
	display( "Sobel Image", sobelImage );
	display( "Laplace Image", laplaceImage );
	display( "Canny Image", cannyImage );
	display( "Contours Image", contoursImage, WAIT | CLOSE );
}

void preProcessImages(IplImage** left, IplImage** right) {

	*left = preProcessImage(*left);
	*right = preProcessImage(*right);
}

IplImage* preProcessImage(IplImage* src) {

	// Create a temporary image object to process,
	// so that the orginal image remains intact.
	IplImage* temp = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );

	// Convert image to gray scale.
	cvCvtColor(src, temp, CV_BGR2GRAY);

	// Rectify images.
	CalibFilter.Rectify(&temp, &temp);

	// Return the filtered image.
	return temp;
}

IplImage* findStereoCorrespondence(IplImage* left, IplImage* right) {

	// Create a temporary image object to copy the stereo image to.
	IplImage* depthImage = cvCreateImage( cvGetSize(right), right->depth, right->nChannels );

	// Find the stereo corresopondence depth image.
	cvFindStereoCorrespondence( left,
								right,
								CV_DISPARITY_BIRCHFIELD,
								depthImage,
								MAX_DISPARITY,
								CONSTANT_OCCLUSION_PENALTY,
								CONSTANT_MATCH_REWARD,
								HIGHLY_RELIABLE_REGION,
								MODERATELY_RELIABLE_REGION,
								SLIGHTLY_RELIABLE_REGION );

	// Return the depth image.
	return depthImage;
}

IplImage* createMask(IplImage* src) {

	// Create a temporary image object to copy the mask to,
	// so that the orginal image remains intact.
	IplImage* temp = cvCreateImage( cvGetSize(src), src->depth, src->nChannels );
    
	// Filter using a threshold
	cvThreshold( src, temp, THRESHOLD, MAXVALUE, THRESHOLD_TYPE );
	
	// Return the image mask.
	return temp;
}

void removeNoise(IplImage* src) {
	
	CvSize sz = cvSize( src->width & -2, src->height & -2 );
	
	IplImage* pyr = cvCreateImage( cvSize(sz.width/2, sz.height/2), src->depth, src->nChannels );
	
	cvPyrDown( src, pyr, 7 );
	cvPyrUp( pyr, src, 7 );
	cvReleaseImage(&pyr);
}

IplImage* filter(IplImage* srcLeft, IplImage* srcRight, IplImage* mask) {

	// Create a temporary image object to copy the filter to.
	IplImage* temp = cvCreateImage( cvGetSize(srcLeft), srcLeft->depth, srcLeft->nChannels );
  
	cvOr(srcLeft, srcRight, temp, mask);

	// Return the filtered image.
	return temp;
}

WINDOW_TYPE createWindow(char* windowName) {

	// Creates a named window.
    cvNamedWindow(windowName, CV_WINDOW_AUTOSIZE);

	// Returns the window handle.
    return (WINDOW_TYPE) cvGetWindowHandle(windowName);
}

void display(char* windowName, IplImage* image, int flag) {

	cvNamedWindow( windowName , CV_WINDOW_AUTOSIZE );
 
	// Display images.
	cvShowImage( windowName, image );

	// If the wait flag is set, then wait for input.
	if((flag & WAIT) == WAIT)
		cvWaitKey(0);  // Wait for input.

	// If the close flag is set, then close the window.
	if((flag & CLOSE) == CLOSE)
		cvDestroyWindow(windowName);
}