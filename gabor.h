#ifndef GABOR_H
/*
 The function hierarchy diagram
 
                                     +--->GaborFn()
                                     |
                +--->GaborRespSeq()--|--->RealPart()
                |                    |
                |                    +--->SqrtResponse()
 GaborFilter()--|
                |
                |
                +--->GaborResp()--->GaborRespSeqMax()
 
 */
#define GABOR_H
#include "cv.h"
#include "highgui.h"
#include "matlabhelper.h"
using namespace cv;

class Gabor
{
public:
    Gabor();
    
    Mat gabor_filter_response;
    
    std::vector<int> theta_sequence;

    Mat GaborFilter(Mat _img, int _gaborStep);

    /****Function calculates all the responses of the gabor band
     *filters and save them in a 3D matrix.*/
    std::vector<Mat> GaborRespSeq(Mat & _img, int _gaborStep );

    /*  Function calculates the orientation map and the corresponding
     *gabor response and return the maximun response and the corresponding orientation.*/
    Mat GaborResp(Mat & _img, std::vector<Mat> _gaborRespSequenceReal);

    /*  Generate gabor filter and return gabor function*/
    Mat GaborFn(int _theta);

    /* get the real and imag part of the gabor matrix*/
    Mat RealPart(Mat & _inputGabor);
    Mat ImagPart(Mat & _inputGabor);

    Mat SqrtResponse(Mat & _resp);

    /* get the max response mat from the response sequences*/
    Mat GaborRespSeqMax(std::vector<Mat> _gaborRespSeq, Mat& _orientationMap);

};

#endif // GABOR_H
