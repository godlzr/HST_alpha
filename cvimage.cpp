/*
 *
 * Author: Zhongrui Li
 * Supervisor: Wonsook Lee
 * zli109@uottawa.ca, wslee@uottawa.ca
 * EECS, Faculty of Engineering, University of Ottawa, Canada
 *
 * Date: March 20th, 2015
 *
 */
#include "cvimage.h"
#include <core/core.hpp>
CVImage::CVImage()
{
}

Mat CVImage::CroppingImage(Mat _originalImg, CvRect _cpRect)
{
    cropped_img = _originalImg(_cpRect);
    return cropped_img;
}

Mat CVImage::ConvertRgbToHsv(Mat _rgbImg)
{
    //convert rgb to hsv and return s channel.
    Mat hsvImg; //hsv image

    //convert rgb to hsv
    cvtColor(_rgbImg, hsvImg, CV_BGR2HSV);
    
    // image mat used to seperate h,s,v channels.
    std::vector<Mat>sepHsvImg(hsvImg.channels());
    
    //seperate h,s,v channels
    cv::split(hsvImg, sepHsvImg);
    
    sepHsvImg[1].copyTo(s_Chanel_Hsv_Img);
    
    return s_Chanel_Hsv_Img;
}

Mat CVImage::GaborFilter(Mat _sChanelHsvImg)
{
    int gabor_step = 10;

    _sChanelHsvImg.convertTo(_sChanelHsvImg, CV_64FC1);
    
    //Mat testimg = (Mat_<double>(3,3) << 1, 2, 3, 3, 4, 5, 5, 6, 7);
    
    gabor_filter_resp = gabor.GaborFilter(_sChanelHsvImg, gabor_step);
    //std::cout<<format(gabor_filter_resp, "numpy")<<std::endl;
    gabor_filter_img = gabor_filter_resp.clone();
    double * gfi;
    
    for(int i = 0 ; i < gabor_filter_img.rows; i++)
    {
        gfi = gabor_filter_img.ptr<double>(i);
        for(int j = 0 ; j < gabor_filter_img.cols; j++)
        {
            gfi[j] = gfi[j]*255;
        }
    }
    
    gabor_filter_img.convertTo(gabor_filter_img, CV_8UC1);
    
    return gabor_filter_img;
}

Mat CVImage::EnhancedGaborFilter()
{
    enhanced_gabor_img = enGabor.EnhancedGaborFilter(gabor_filter_img);
    enhanced_gabor_img.convertTo(enhanced_gabor_img, CV_8UC1);
    return enhanced_gabor_img;
}

Mat CVImage::ErodeImage()
{
    matHelper.erodeImage(enhanced_gabor_img, eroded_img);
    Mat eroded_img_CV8UC1;
    eroded_img.convertTo(eroded_img_CV8UC1, CV_8UC1);
    return eroded_img_CV8UC1;
}

Mat CVImage::StrandsAnalysis()
{
    curves_img = strOpt.StrandsAnalysis(eroded_img);
    curves_img.convertTo(curves_img, cropped_img.type());
    double alpha = 0.35;
    double beta = ( 1.0 - alpha );

    Mat blend_img;
    
    Mat colorImg = oringinal_img_RGB(crop_rect);
    addWeighted( curves_img, alpha, colorImg, beta, 0.0, blend_img);
    return blend_img;
}

Mat CVImage::StrandsEnxtendAndConnection()
{
    /* Get the hair strand splines
    % Extension of splines into two directions;
    % Connect the two splines which are near;
    % Use median filter to smooth the connected splines;
    % Then repeat the procedure several times
    */
    double extensionParameter = 1;       //The extension parameter of splines 1 = one pixel
    int connectionThreshold = 5;         // The distance threshold for connection
    int connectionThresholdIncrease =1; // The increase parameter of the threshold of connection
    int newCtrlPtsNum = 30;              // The number of control points in a spline
    int maximumIterlation = 5; // The times of extension
    vector<vector<Point2f> > ctrlPts;
    ctrlPts = strOpt.ExtendAndConnectSplines(extensionParameter,connectionThreshold,newCtrlPtsNum,maximumIterlation, connectionThresholdIncrease);
    Mat connectedSplinesImg;
    connectedSplinesImg = strOpt.DrawCurvesImg(curves_img, ctrlPts);
    
    double alpha = 0.35;
    double beta = ( 1.0 - alpha );
    
    Mat blend_img;
    
    Mat colorImg = oringinal_img_RGB(crop_rect);
    connectedSplinesImg.convertTo(connectedSplinesImg, colorImg.type());
    addWeighted( connectedSplinesImg, alpha, colorImg, beta, 0.0, blend_img);
    return blend_img;
}

Mat CVImage::MedianFilter()
{
    int windowSize = 2;
    Mat filteredCurvesImg;
    
    filteredCurvesImg = strOpt.MedianFilter(windowSize);
    
    double alpha = 0.35;
    double beta = ( 1.0 - alpha );
    
    Mat blend_img;
    
    Mat colorImg = oringinal_img_RGB(crop_rect);
    filteredCurvesImg.convertTo(filteredCurvesImg, colorImg.type());
    addWeighted( filteredCurvesImg, alpha, colorImg, beta, 0.0, blend_img);
    return blend_img;
}

bool CVImage::ExportControlPoints(char * filepath)
{
    return strOpt.ExportCtrlPts(filepath);
}
