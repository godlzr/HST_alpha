#include "cvimage.h"
#include <core/core.hpp>
CVImage::CVImage()
{
}

Mat CVImage::CroppingImage(Mat _originalImg, CvRect _cpRect)
{
    Mat croppedImage = _originalImg(_cpRect);
    return croppedImage;
}

Mat CVImage::ConvertRgbToHsv(Mat _rgbImg)
{
    //convert rgb to hsv and return s channel.
    Mat hsvImg; //hsv image

    Mat sChanelHsvImg;
    
    //convert rgb to hsv
    cvtColor(_rgbImg, hsvImg, CV_BGR2HSV);
    
    // image mat used to seperate h,s,v channels.
    std::vector<Mat>sepHsvImg(hsvImg.channels());
    
    //seperate h,s,v channels
    cv::split(hsvImg, sepHsvImg);
    
    sepHsvImg[1].copyTo(sChanelHsvImg);
    
    return sChanelHsvImg;
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

void CVImage::StrandsAnalysis()
{
    strOpt.StrandsAnalysis(eroded_img);
}