#include "cv.h"
#include "cvcam.h"
#include "highgui.h"
#include <iostream>

void callback(IplImage* image);
void main()
{

	int cvcamone;

	int ncams = cvcamGetCamerasCount( );//returns the number of available cameras in the 
						   //system

printf("Got camera count\n");

	cvcamSetProperty(0, CVCAM_PROP_ENABLE, &cvcamone); //Selects the 1-st found 
									        //camera

printf("Camera enabled\n");

	cvcamSetProperty(0, CVCAM_PROP_RENDER, &cvcamone);  //We’ll render stream
									        //from this source
	//here I assume your create a window and store it’s id in MyWin variable.
	//MyWin is of type HWND on Windows and Window on linux

printf("Stream rendered\n");

	// Creates a window for rendering.
    cvNamedWindow("cvcam window", 0);

	// Get the window handle.
    HWND MyWin= (HWND)cvGetWindowHandle("cvcam window");


	cvcamSetProperty(0, CVCAM_PROP_WINDOW, &MyWin);   // Selects a window for  
								  //video rendering

printf("Window anchored\n");

	cvcamSetProperty(0, CVCAM_PROP_CALLBACK, callback);//this callback will
								  //process every frame
printf("Callback set\n");

cvcamInit( );
printf("Camera initialized\n");
	cvcamStart( );
printf("Camera started\n");

	cvvWaitKey(0); // Wait for a key.

	//Your app is working
	cvcamStop( );
printf("Camera stopped\n");
	cvcamExit( );
printf("Exit\n");
}
void callback(IplImage* image)//Draws blue horizontal lines across the image
{

IplImage* image1 = image;
int i,j;

assert (image);

for(i=0; i<image1->height; i+=10)
{
   for(j=(image1->widthStep)*i; j<(image1->widthStep)*(i+1); 
         j+=image1->nChannels)
   {
      image1->imageData[j]   = (char)255;
      image1->imageData[j+1] = 0;
      image1->imageData[j+2] = 0;
      

   } 

}

}
