//
//  utility.cpp
//  webcamsave
//
//  Created by Cesar Torres on 10/16/13.
//  Copyright (c) 2013 Cesar Torres. All rights reserved.
//

#include "utility.h"

int save(IplImage * img){
    cvSaveImage("input.jpg", img);
    fprintf(stdout, "Saved image.");
    return 1;
}



/**
 * @function Hist_and_Backproj
 * @brief Callback to Trackbar
 */
//void Hist_and_Backproj(int, void*)
//{
//    MatND hist;
//    int histSize = MAX( bins, 2 );
//    //float hue_range[] = { 0, 180 };
//    //const float* ranges = { hue_range };
//    
//    /// Get the Histogram and normalize it
//    //calcHist( &hue, 1, 0, Mat(), hist, 1, &histSize, &ranges, true, false );
//    normalize( hist, hist, 0, 255, NORM_MINMAX, -1, Mat() );
//    
//    /// Get Backprojection
//    MatND backproj;
//    //calcBackProject( &hue, 1, 0, hist, backproj, &ranges, 1, true );
//    
//    /// Draw the backproj
//    //imshow( "BackProj", backproj );
//    
//    /// Draw the histogram
//    int w = 400; int h = 400;
//    int bin_w = cvRound( (double) w / histSize );
//    Mat histImg = Mat::zeros( w, h, CV_8UC3 );
//    
//    for( int i = 0; i < bins; i ++ )
//    { rectangle( histImg, Point( i*bin_w, h ), Point( (i+1)*bin_w, h - cvRound( hist.at<float>(i)*h/255.0 ) ), Scalar( 0, 0, 255 ), -1 ); }
//    
    //imshow( "Histogram", histImg );
//}