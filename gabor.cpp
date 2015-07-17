/* Gabor FIlter
 *
 * Author: Zhongrui Li
 * Supervisor: Wonsook Lee
 * zli109@uottawa.ca, wslee@uottawa.ca
 * EECS, Faculty of Engineering, University of Ottawa, Canada
 *
 * Date: March 20th, 2015
 *
 * Description: This gabor class performs the simplified function of Gabor Filter
 ****************************************************************
 *              The function hierarchy diagram                  *
 ****************************************************************
 *                                       +--->GaborFn()         *
 *                                       |                      *
 *                  +--->GaborRespSeq()--|--->RealPart()        *
 *                  |                    |                      *
 *                  |                    +--->SqrtResponse()    *
 *   GaborFilter()--|                                           *
 *                  |                                           *
 *                  |                                           *
 *                  +--->GaborResp()--->GaborRespSeqMax()       *
 *                                                              *
 ****************************************************************
 */
#include "gabor.h"
#include <math.h>
#include <cmath>
#include <complex>
#include <QDebug>
#include <string>
Gabor::Gabor()
{
}

Mat Gabor::GaborFilter(Mat _img, int _gaborStep)
{
    std::vector<Mat> gaborRespSequenceReal;
    
    //get the gabor Response Sequence first.
    gaborRespSequenceReal = GaborRespSeq(_img, _gaborStep);

    // calculate the gabor response
    
    gabor_filter_response = GaborResp(_img, gaborRespSequenceReal);
    
    return gabor_filter_response;
}
/**********************Calculate the gabor sequence by different degree*********************************/
std::vector<Mat> Gabor::GaborRespSeq(Mat & _img, int _gaborStep)
{

    Mat tmpGabor;
    /*
    int filterBank  = ceil(180 / _gaborStep) + 1;
    int icol = _img.cols;
    int irow = _img.rows;
    */
    Mat responseReal(_img.rows, _img.cols, CV_64FC1);

    Mat responseR(_img.rows, _img.cols, CV_64FC1);

    std::vector<Mat> gaborRespSeqRealVector;
    
    int cnt = 0;
    matlabHelper mh;
    for(int theta = 10; theta <= 180; theta += _gaborStep)
    {

        theta_sequence.push_back(theta);

        tmpGabor = GaborFn(theta);
    
        filter2D(_img, responseReal, -1, RealPart(tmpGabor));
        //mh.conv2(_img, RealPart(tmpGabor), CONVOLUTION_SAME, responseReal);
        
        responseR = SqrtResponse(responseReal);
        //cv::imwrite("/Users/GODLZR/Pictures/gabor.jpg", responseR);
        gaborRespSeqRealVector.push_back(responseR.clone());

        cnt++;
    }
    
    return gaborRespSeqRealVector;
}


/*************************Calculate the gabor of input theta********************************************/
Mat Gabor::GaborFn(int _theta)
{
    int gaborSize = 7;//size of the Gabor filter is 7*7;
    double sigma_x = 1.8;
    double sigma_y = 2.4;
    int lambda = 4; //should be more than 2
    
    double x_theta = 0;
    double y_theta = 0;
    Mat gabor(gaborSize * 2 + 1, gaborSize * 2 + 1, CV_64FC1);
    for (int x = - gaborSize; x <= gaborSize; x++)
    {
        for(int y = -gaborSize; y <= gaborSize; y++)
        {
            x_theta = x * cos(_theta*M_PI/180.0) + y * sin(_theta*M_PI/180.0);
            y_theta = y * cos(_theta*M_PI/180.0) - x * sin(_theta*M_PI/180.0);
            gabor.at<double>(gaborSize+x, gaborSize+y) = exp(-0.5*((x_theta*x_theta/(sigma_x*sigma_x))+y_theta*y_theta/(sigma_y*sigma_y)))*cos(2*M_PI*x_theta/lambda);
            
        }
    }
    return gabor;
}
/*************************Calculate the Real part and Imaginary part of input Mat********************************************/
Mat Gabor::RealPart(Mat & _inputGabor)
{
    Mat realGabor = _inputGabor.clone();
    double *rg;
    for (int i = 0; i < realGabor.rows; i++)
    {
        rg = realGabor.ptr<double>(i);
        for (int j = 0; j < realGabor.cols; j++)
        {
            Complex <double>com = rg[j];
            rg[j]= (double)(com.re);
        }
    }
    return realGabor;
}

Mat Gabor::ImagPart(Mat & _inputGabor)
{
    Mat imgGabor = _inputGabor.clone() ;
    double *ig;
    for (int i = 0; i < imgGabor.rows; i++)
    {
        ig = imgGabor.ptr<double>(i);
        for (int j = 0; j < imgGabor.cols; j++)
        {
            Complex <double>com = ig[j];
            ig[j] = (double)(com.im);
        }
    }
    return imgGabor;
}
/*********************************************************************/


Mat Gabor::SqrtResponse(Mat & _resp)
{
    Mat procResp = _resp.clone();
    double *pr;
    
    for(int i = 0; i < procResp.rows; i++)
    {
        pr = procResp.ptr<double>(i);
        
        for(int j = 0; j < procResp.cols; j++)
        {
            pr[j] = sqrt(pr[j] * pr[j]);
        }
    }
    return procResp;
}

/*********************************************************************/
Mat Gabor::GaborResp(Mat & _img, std::vector<Mat> _gaborRespSequenceReal)
{
    
    
    Mat orientationmMap(_img.rows, _img.cols, CV_8UC1);
    
    Mat gaborFilterResponse(_img.rows, _img.cols, CV_64FC1);
    
    double grMax = 0;
    double grMin = 0;
    
    Mat gaborResponseMax = GaborRespSeqMax(_gaborRespSequenceReal, orientationmMap);
    
    double *grm, *gfr;
    for(int row = 0; row < gaborResponseMax.rows; row++)
    {
        grm = gaborResponseMax.ptr<double>(row);
        for(int col  = 0; col < gaborResponseMax.cols; col++)
        {
            if(grm[col] > grMax)
                grMax = grm[col];
            
            if(grm[col] < grMin)
                grMin = grm[col];
        }
    }
    
    for(int row = 0; row < gaborResponseMax.rows; row++)
    {
        gfr = gaborFilterResponse.ptr<double>(row);
        grm = gaborResponseMax.ptr<double>(row);
        
        for(int col  = 0; col < gaborResponseMax.cols; col++)
        {
            gfr[col] = ((grm[col]-grMin)/(grMax-grMin));
        }
    }
    
    return gaborFilterResponse;
}

Mat Gabor::GaborRespSeqMax(std::vector<Mat> _gaborRespSeq, Mat& _orientationMap)
{
    int col = _gaborRespSeq.at(0).cols;
    int row = _gaborRespSeq.at(0).rows;
    int framesCount = (int)_gaborRespSeq.size();

    double *tf, *grm, *om;
    //Mat tmpFrame;
    Mat gaborRespMax = _gaborRespSeq.at(0).clone();
    for(int f = 0; f < framesCount; f++)
    {
        //tmpFrame = ;
        
        for(int i = 0; i < row; i++)
        {
            tf = _gaborRespSeq.at(f).ptr<double>(i);
            grm = gaborRespMax.ptr<double>(i);
            om = _orientationMap.ptr<double>(i);
            for (int j = 0; j < col; j++)
            {
                if(fabs(tf[j]) > grm[j])
                {
                    grm[j] = fabs(tf[j]);
                    //om[j] = theta_sequence.at(f);
                }
            }
        }
    }
    return gaborRespMax;
}




