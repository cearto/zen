//
//  main.cpp
//  webcamsave
//
//  Created by Cesar Torres on 9/23/13.
//  Copyright (c) 2013 Cesar Torres. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include "string.h"




int sendPictureToAnalysis(IplImage *img);
int saveImage(IplImage * img){
    cvSaveImage("input.jpg", img);
    return 1;
}
int main(int argc, char *argv[])
{
    CvCapture* capture = cvCaptureFromCAM( CV_CAP_ANY );
    if ( !capture ) {
        fprintf( stderr, "ERROR: capture is NULL \n" );
        getchar();
        return -1;
    }
    // Create a window in which the captured images will be presented
    cvNamedWindow( "mywindow", CV_WINDOW_AUTOSIZE );
    // Show the image captured from the camera in the window and repeat
    while ( 1 ) {
        // Get one frame
        IplImage* frame = cvQueryFrame( capture );
        if ( !frame ) {
            fprintf( stderr, "ERROR: frame is null...\n" );
            getchar();
            break;
        }
        cvShowImage( "mywindow", frame );
        // Do not release the frame!
        //If ESC key pressed, Key=0x10001B under OpenCV 0.9.7(linux version),
        //remove higher bits using AND operator
        int keyPress = cvWaitKey(10);
        if ((keyPress & 255) == 27 ) break;
        else if(keyPress == 'c') sendPictureToAnalysis(frame);
        
    }
    // Release the capture device housekeeping
    cvReleaseCapture( &capture );
    cvDestroyWindow( "mywindow" );
    return 0;
}

int sendPictureToAnalysis(IplImage *img){
    fprintf(stderr, "Image sent for analysis ... \n");
    saveImage(img);
    return 1;
}

