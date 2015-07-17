/* Matlab Helper
 *
 * Author: Zhongrui Li
 * Supervisor: Wonsook Lee
 * zli109@uottawa.ca, wslee@uottawa.ca
 * EECS, Faculty of Engineering, University of Ottawa, Canada
 *
 * Date: March 20th, 2015
 *
 * Description:
 * This class contain a serise of matlab functions which are written using C++
 *
 */
#ifndef MATLABHELPER_H
#define MATLABHELPER_H
#include <cv.h>
#include <highgui.h>

using namespace cv;
enum ConvolutionType {
    /* Return the full convolution, including border */
    CONVOLUTION_FULL,
    
    /* Return only the part that corresponds to the original image */
    CONVOLUTION_SAME,
    
    /* Return only the submatrix containing elements that were not influenced by the border
     */
    CONVOLUTION_VALID
};
class matlabHelper
{
public:
    matlabHelper();

    //matlab::fspecial(gaussian)
    Mat gaussianKernal(double sigma, int size);
    //matlab::meshgrid()
    void meshgrid(const cv::Range &xgv, const cv::Range &ygv, cv::Mat &X, cv::Mat &Y);
    //matlab::atan2()
    Mat atan2Mat(Mat & a, Mat & b);

    std::pair<Mat,Mat> gradient(Mat & img, float spaceX, float spaceY);
    Mat gradientX(Mat & mat, float spacing);
    Mat gradientY(Mat & mat, float spacing);
    
    // matlab::conv2()
    void conv2(const Mat &img, const Mat &kernel, ConvolutionType type, Mat& dest);
    
    // matlab::imrotate()
    Mat imrotate(const Mat source, double angle);
    
    //matlab::ordfilt2(A,windsze,ones(1,windsze));
    void ordfilt2(Mat _proj, int windsze, Mat windmat, Mat & _dilation);
    /*****************************************************/
    /*ErodeImg - Function erodes the lines in the binary image. The eroded lines have the width of one pixel.
     *Arguments:
     *          im  - A grey-level input image
     *Return:
     *          Mat - the eroded image
     */
    void erodeImage(const Mat & _img, Mat & _erodedImg);
    void applylut_1(Mat &src,Mat &dst);
    void applylut_8(Mat &src,Mat &dst,Mat& lut);
    void GetLutSkel(Mat& Lut);
    void endp(Mat &src,Mat &dst);
    
    // function:UO_mode, window[3,3]
    Mat nlfilter(const Mat & _im);
};

#endif // MATLABHELPER_H
