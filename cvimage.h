/* CVImage class
 *
 * Author: Zhongrui Li
 * Supervisor: Wonsook Lee
 * zli109@uottawa.ca, wslee@uottawa.ca
 * EECS, Faculty of Engineering, University of Ottawa, Canada
 *
 * Date: March 20th, 2015
 *
 * Description:
 * This class is used to process the cv::mat under opencv space.
 */
#ifndef CVIMAGE_H
#define CVIMAGE_H
#include "cv.h"
#include "highgui.h"
#include "gabor.h"
#include "enhancedgabor.h"
#include "matlabhelper.h"
#include "strandsOperator.h"
using namespace cv;
class CVImage
{
public:
    Mat oringinal_img;// The original image;

    Mat oringinal_img_RGB;
    
    CvRect crop_rect;

    Mat cropped_img;

    Gabor gabor;

    enhancedGabor enGabor;
    
    matlabHelper matHelper;
    
    strandsOperator strOpt;

    CVImage();
    
    Mat s_Chanel_Hsv_Img;

    Mat gabor_filter_resp;
    
    Mat gabor_filter_img;

    Mat enhanced_gabor_img;
    
    Mat eroded_img;
    
    Mat curves_img;
    
    // crop the original image
    Mat CroppingImage(Mat _originalImg, CvRect _cpRect);

    // convert rgb to hsv and return s channel
    Mat ConvertRgbToHsv(Mat _rgbImg);

    Mat GaborFilter(Mat _sChanelHSVImg);

    Mat EnhancedGaborFilter();
    
    Mat ErodeImage();
    
    Mat StrandsAnalysis();
    
    Mat StrandsEnxtendAndConnection();
    
    Mat MedianFilter();
    
    bool ExportControlPoints(char * filepath);

};

#endif // CVIMAGE_H
