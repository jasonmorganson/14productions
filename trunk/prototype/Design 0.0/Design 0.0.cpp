/*
Design 0.0
	Type: Application run on a windows system
	Description: Compiles with OpenCV headers
	Status: Completed
	Refer to the Design 0.0 document for more information.
*/

//
// The simplest OpenCV program.
// It creates window and displays a sequence of images in it 
//

#include "cv.h"  // include core library interface
#include "highgui.h" // include GUI library interface
//#include "cvaux.h"  // include expirimental library
#include <stdio.h>
#include <math.h>
#include <string.h>

void main()
{
    cvvNamedWindow( "image", 0 ); // create HighGUI window with name "image"
	cvvWaitKey(0); // wait for key.
}
