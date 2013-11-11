//  TangibleTiles Color Extraction
//
//  Created by Cesar Torres on 9/23/13.
//  Copyright (c) 2013 Cesar Torres. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "string.h"
#include "utility.h"


//FUNCTION DECLARATIONS
int setup();
int initializeUI();
int loop();
int analyze(IplImage *img);

// HISTOGRAM VARIABLES
IplImage* src, hsv, hue;
cv::string colorFile = "color.png";
int bins = 25;



int setup(){
    //HISTOGRAM FOR BACK PROJECTION
    src = cvLoadImage(colorFile, CV_LOAD_IMAGE_ANYDEPTH);
//    cvtColor( src, hsv, CV_BGR2HSV );
//    hue.create( hsv.size(), hsv.depth() );
//    int ch[] = { 0, 0 };
//    mixChannels( &hsv, 1, &hue, 1, ch, 1 );
    return initializeUI();
}
int initializeUI(){
//    string window_image = "Source image\0";
//    namedWindow( window_image, CV_WINDOW_AUTOSIZE );
//    createTrackbar("* Hue  bins: ", window_image,&bins, 180, Hist_and_Backproj );
//    Hist_and_Backproj(0, 0);
//    imshow( window_image, src );
    return 1;
}
int loop(){
    CvCapture* capture = cvCaptureFromCAM( CV_CAP_ANY );
    if ( !capture ) {
        fprintf( stderr, "ERROR: capture is NULL \n" );
        getchar();
        return -1;
    }
    cvNamedWindow( "mywindow", CV_WINDOW_AUTOSIZE );
    while ( 1 ) {
        IplImage* frame = cvQueryFrame( capture );
        if ( !frame ) {
            fprintf( stderr, "ERROR: frame is null...\n" );
            getchar();
            break;
        }
        cvShowImage( "Feed", frame );
        int keyPress = cvWaitKey(10);
        if ((keyPress & 255) == 27 ) break;
        else if(keyPress == 'c') analyze(frame);
        
    }
    cvReleaseCapture( &capture );
    cvDestroyWindow( "Feed" );
    return 0;
}


int analyze(IplImage *img){
    fprintf(stderr, "Image sent for analysis ... \n");
    save(img);
    return 1;
}

/* LOGIC CONTROLLER */
int main(int argc, char *argv[])
{
    setup();
    if(setup == 0){
        fprintf( stderr, "ERROR: Setup failed. \n" );
        getchar();
        return -1;
    }
    return loop();
}