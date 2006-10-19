// StereoCam.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

#include <cv.h>
#include <cvcam.h>

void stereocallback(IplImage* image1, IplImage* image2)
{

	//convert video#1 from RGB to HSV
	cvCvtColor(image1, image1, CV_RGB2HSV);

	//convert video#2 from RGB to Gray
	IplImage * GRAY = cvCreateImage(cvGetSize(image2),IPL_DEPTH_8U,1);
	cvCvtColor(image2, GRAY, CV_RGB2GRAY);
	cvCvtColor(GRAY, image2, CV_GRAY2RGB);
	cvReleaseImage(&GRAY);
}

void ShowCamVideo(HWND hwnd1, HWND hwnd2)
{
	int* out;
	int ncams = cvcamGetCamerasCount( );
	int nselected = cvcamSelectCamera(&out);

	cvcamSetProperty(out[0], CVCAM_PROP_ENABLE, CVCAMTRUE);
	cvcamSetProperty(out[0], CVCAM_PROP_RENDER, CVCAMTRUE);
	cvcamSetProperty(out[0], CVCAM_STEREO_CALLBACK,stereocallback);
	cvcamSetProperty(out[0], CVCAM_PROP_WINDOW, &hwnd1);   

	if ( nselected == 2 ) //if the 2nd camera has been selected
	{
		cvcamSetProperty(out[1], CVCAM_PROP_ENABLE, CVCAMTRUE);
		cvcamSetProperty(out[1], CVCAM_PROP_RENDER, CVCAMTRUE);
		cvcamSetProperty(out[1], CVCAM_PROP_WINDOW, &hwnd2); 
		cvcamSetProperty(out[1], CVCAM_STEREO_CALLBACK,stereocallback);
	}

	cvcamInit( ); 
	cvcamStart( );
}

void ExitCamera()
{
	cvcamStop( );
	cvcamExit( );
}

long FAR PASCAL WindowProc( HWND hwnd, UINT msg, UINT wParam, LONG lParam)
{
    switch (msg)
    {
        case WM_DESTROY:
			ExitCamera();
            PostQuitMessage(0);
            break;
        default:
            return (DefWindowProc(hwnd, msg, wParam, lParam));
    }
    return(NULL);
}


int PASCAL WinMain( HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR  lpCmdLine, 
    int nCmdShow )
{
    MSG         msg;
    WNDCLASS    wc;
	
    ZeroMemory(&wc, sizeof wc);
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInst;
    wc.hIcon         = LoadIcon( NULL, IDI_APPLICATION );
    wc.hCursor       = LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground = (HBRUSH)GetStockObject( DKGRAY_BRUSH );
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = "CAMERAONE";
    RegisterClass( &wc );

    HWND hwnd1 = CreateWindow(
        "CAMERAONE",
        "CVCAM Sample1",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInst,
        NULL);

    wc.lpszClassName = "CAMERATWO";
    RegisterClass( &wc );

    HWND hwnd2 = CreateWindow(
        "CAMERATWO",
        "CVCAM Sample2",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInst,
        NULL);

    ShowWindow( hwnd1, nCmdShow );
    UpdateWindow( hwnd1 );

    ShowWindow( hwnd2, nCmdShow );
    UpdateWindow( hwnd2 );
 
	//Show me the Stereo CvCam in Action!
	ShowCamVideo(hwnd1, hwnd2);

    while( GetMessage( &msg, NULL, 0, 0 ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }

    return msg.wParam;
}