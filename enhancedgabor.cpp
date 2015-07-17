/* Enhanced Gabor Filter
 *
 * Author: Zhongrui Li
 * Supervisor: Wonsook Lee
 * zli109@uottawa.ca, wslee@uottawa.ca
 * EECS, Faculty of Engineering, University of Ottawa, Canada
 *
 * Date: March 20th, 2015
 *
 * Description: This enhanced gabor class performs the simplified function of fingerprint algorithm to 
 * detect the hair strands.
 * Function enhance the gabor filter response.
 ************************************************************************************
 *              The function hierarchy diagram                                      *
 ************************************************************************************
 *                                                               +-->MeanMat()      *
 *                                                               |                  *
 *                                               +--->*Normalise()--->StdMat()       *
 *                                               |                                  *
 *                          +--->*RidgeSegment()--|--->*BlkProc()----->MeanMat()      *
 *                          |                                    |                  *
 *                          |                                    +-->StdMat()       *
 *   EnhancedGaborFilter()--|                                                       *
 *                          |--->*RidgeOrient()                                      *
 *                          |                                                       *
 *                          |                                                       *
 *                          +--->*RidgeFreq()--->*Freqest()                           *
 *                          |                                                       *
 *                          +--->RidgeFilter()                                      *
 *                                                                                  *
 ************************************************************************************
 */
#include "enhancedgabor.h"
#include <math.h>
#include <cmath>
#include <qdebug.h>
#include "matlabhelper.h"

const double eps = 2.2204e-16;
const double PI = 3.1415926535898;

enhancedGabor::enhancedGabor()
{
    blk_sze_mask = 10;
    blk_sze = 10;
    thres_h = 0.07;
}

Mat enhancedGabor::EnhancedGaborFilter(const Mat & _img)
{
    Mat img;
    _img.Mat::convertTo(img, CV_64FC1);
    /**Image enhancement (Frequency estimation for each block)***/
    
    /*1******************************************/
    Mat normim, maskImg;
    RidgeSegment(img,blk_sze_mask,thres_h, normim, maskImg);
    /********************************************/

    /*2******************************************/
    Mat orientim, reliability;
    RidgeOrient(normim, 0.5, 20, 5, orientim, reliability);
    /********************************************/

    
    /**Frequency estimation for each block***/
    /*3******************************************/
    Mat freq;
    double medianFreq = 0;
    medianFreq = RidgeFreq(normim, maskImg, orientim, blk_sze, 3, 5, 15, freq);
    /********************************************/
    multiply(medianFreq, maskImg, freq);
    divide(freq, 255, freq);

    /**Orientation calculation of blocks***/
    /*4*******************************************/
    Mat newIm;
    RidgeFilter(normim, orientim, freq, 1, 1, 1, newIm);
    /********************************************/
    
    Mat binIm = newIm.clone();
    double *bi;
    for(int i = 0; i<binIm.rows; i++)
    {
        bi = binIm.ptr<double>(i);
        for(int j = 0; j < binIm.cols; j++)
        {
            if(bi[j]/255 > 0.2)
                bi[j] = 255;
            else
                bi[j] = 0;
        }
    }

    /**Thresholding - Gray to Binary***/
    double *r;
    for(int i = 0; i<reliability.rows; i++)
    {
        r = reliability.ptr<double>(i);
        for(int j = 0; j < reliability.cols; j++)
        {
            if(r[j]/255 > 0.25)
                r[j] = 1;
            else
                r[j] = 0;
        }
    }
    Mat imgEnhanced;
    multiply(binIm, maskImg, imgEnhanced);
    multiply(imgEnhanced, reliability, imgEnhanced);
    divide(imgEnhanced, 255, imgEnhanced);
        
    /*for(int i = 0; i<imgEnhanced.rows; i++)
    {
        for(int j = 0; j<imgEnhanced.rows; j++)
        {
            if(imgEnhanced.at<unsigned int>(i,j) != 0)
                imgEnhanced.at<unsigned int>(i,j) =255;
        }
    }*/
    //std::cout<<format(imgEnhanced,"numpy")<<std::endl;
    
    return imgEnhanced;
}

/*1. RidgeSegment - Function to ormalise fingerprint image and segments ridge region*/
void enhancedGabor::RidgeSegment(const Mat & _im, int _blockSize, double _thresHold, Mat & normim,Mat & maskImg)
{
    double sum = 0;
    _thresHold *= 255;
    std::vector<int> maskind;
    
    //normalise to have zero mean, unit std dev
    Normalise(_im, 0, 1, normim);
    
    maskImg = BlkProc(normim, _blockSize, _blockSize);

    //maskImg = stdImg.clone();
    for(int i = 0; i<maskImg.rows;i++)
    {
        for(int j = 0; j<maskImg.cols;j++)
        {
            if(maskImg.at<double>(i,j) > _thresHold)
            {
                maskImg.at<double>(i,j) = 255;
                int ind = maskImg.cols*i+j;
                maskind.push_back(ind);
                sum = sum + normim.at<double>(i,j);
            }
            else
                maskImg.at<double>(i,j) = 0;
        }
    }

    double stdsum = 0;
    double mean = sum/maskind.size();
    
    double *ni;
    
    for(int i = 0; i<normim.rows;i++)
    {
        ni = normim.ptr<double>(i);
        for(int j = 0; j<normim.cols;j++)
        {
                ni[j] -= mean;
                stdsum += ((ni[j]-mean) * (ni[j]-mean));
        }
    }
    
    double stdsqrt = sqrt(stdsum/(maskind.size()-1));
    
    for(int i = 0;i<normim.rows;i++)
    {
        for(int j = 0; j<normim.cols;j++)
        {
            normim.at<double>(i,j) = (normim.at<double>(i,j)/stdsqrt)*255;
        }
    }

    return;
    
}
/*2. RidgeOrient - Function to estimates the local orientation of ridges in afingerprint*/
void enhancedGabor::RidgeOrient(const Mat & _im, double _gradientSigma, double _blockSigma, double _orientSmoothSigma, Mat & _orientim, Mat & _reliability)
{
    
    /*******************************/
    // Calculate image gradients.
    double sze;
    if(_gradientSigma >= 0)
        sze = floor(6*_gradientSigma);
    else
        sze = ceil(6*_gradientSigma);
    if((int)sze%2 == 0)
        sze += 1;
    /*******************************/
    
    
    Mat f = mh.gaussianKernal(_gradientSigma,sze).clone();

    std::pair<Mat, Mat>fxfy = mh.gradient(f, 1, 1);

    Mat Gx, Gy;
    //Mat img(3,3,CV_64FC1,Scalar(2));

    filter2D(_im, Gx, -1, fxfy.first, cv::Point(-1,-1), 0, cv::BORDER_CONSTANT);
    filter2D(_im, Gy, -1, fxfy.second, cv::Point(-1,-1), 0, cv::BORDER_CONSTANT);

    Mat Gxx, Gxy, Gyy;
    multiply(Gx, Gx, Gxx);
    divide(Gxx, 255, Gxx);
    multiply(Gx, Gy, Gxy);
    divide(Gxy, 255, Gxy);
    multiply(Gy, Gy, Gyy);
    divide(Gyy, 255, Gyy);

    /*********************************************************************/
    //Now smooth the covariance data to perform a weighted summation of the data
    if(_blockSigma >= 0)
        sze = floor(6*_blockSigma);
    else
        sze = ceil(6*_blockSigma);

    if((int)sze%2 == 0)
        sze += 1;
    
    f = mh.gaussianKernal(_blockSigma,sze);

    filter2D(Gxx, Gxx, -1, f, cv::Point(-1,-1), 0, cv::BORDER_CONSTANT);

    filter2D(Gxy, Gxy, -1, f, cv::Point(-1,-1), 0, cv::BORDER_CONSTANT);
    Gxy = Gxy.mul(2);

    filter2D(Gyy, Gyy, -1, f, cv::Point(-1,-1), 0, cv::BORDER_CONSTANT);

    /*Analytic solution of principal direction*/
    //denom = sqrt(Gxy.^2 + (Gxx - Gxy).^2) + eps;
    Mat denom, tmp1, tmp2, tmp3;
    multiply(Gxy, Gxy, tmp1);
    subtract(Gxx, Gyy, tmp2);
    multiply(tmp2, tmp2, tmp2);
    add(tmp1, tmp2, tmp3);
    tmp3.convertTo(tmp3, CV_64F);
    sqrt(tmp3, tmp3);
    add(tmp3, eps, denom);

    /*******************************************/
    
    Mat sin2theta, cos2theta;
    divide(Gxy, denom, sin2theta);
    multiply(sin2theta, 255, sin2theta);
    
    subtract(Gxx, Gyy, tmp2);
    divide(tmp2, denom, cos2theta);
    multiply(cos2theta, 255, cos2theta);
    
    if(_orientSmoothSigma != 0)
    {
        if(_orientSmoothSigma >= 0)
            sze = floor(6*_orientSmoothSigma);
        else
            sze = ceil(6*_orientSmoothSigma);

        if((int)sze%2 == 0)
            sze += 1;

        f = mh.gaussianKernal(_orientSmoothSigma,sze);
        filter2D(cos2theta, cos2theta, -1, f, cv::Point(-1,-1), 0, cv::BORDER_CONSTANT);
        filter2D(sin2theta, sin2theta, -1, f, cv::Point(-1,-1), 0, cv::BORDER_CONSTANT);
    }

    //orientim = pi/2 +atan2(sin2theta, cos2theta)/2
    tmp1 = mh.atan2Mat(sin2theta,cos2theta);
    divide(tmp1, 2, tmp2);
    add(tmp2, PI/2, _orientim);
    multiply(_orientim, 255, _orientim);

    //Imin = (Gyy+Gxx)/2 - (Gxx-Gyy).*cos2theta/2 - Gxy.*sin2theta/2
    Mat Imin, Imax;
    add(Gyy, Gxx, tmp1);
    divide(tmp1, 2, tmp1);
    
    subtract(Gxx, Gyy, tmp2);
    multiply(tmp2, cos2theta, tmp2);
    divide(tmp2, 2, tmp2);
    divide(tmp2, 255, tmp2);
    
    multiply(Gxy, sin2theta,tmp3);
    divide(tmp3, 2, tmp3);
    divide(tmp3, 255, tmp3);
    
    subtract(tmp1, tmp2, tmp1);
    subtract(tmp1, tmp3, Imin);

    //Imax = Gyy+Gxx - Imin
    add(Gyy, Gxx, tmp1);
    subtract(tmp1, Imin, Imax);

    //reliability = 1-Imin./(Imax+0.001)
    Mat coherence;
    add(Imax,0.001*255,tmp1);
    divide(Imin, tmp1, tmp1);
    subtract(1, tmp1, _reliability);
    multiply(_reliability, 255, _reliability);
    
    //coherence = ((Imax - Imin)./(Imax + Imin)).^2
    subtract(Imax, Imin, tmp1);
    add(Imax, Imin, tmp2);
    divide(tmp1, tmp2, tmp3);
    multiply(tmp3, tmp3, coherence);
    multiply(coherence, 255, coherence);
    
    Mat denomMask = denom.clone();
    double *dm;
    for(int i=0; i<denomMask.rows; i++)
    {
        dm = denomMask.ptr<double>(i);
        for(int j=0; j<denomMask.cols; j++)
        {
            if(dm[j]>(0.001*255))
                dm[j] = 1;
            else
                dm[j] = 0;
        }
    }
    
    multiply(_reliability, denomMask, _reliability);
    
    return;
}
/*3. RidgeFreq - Function to estimates the fingerprint ridge frequency accross a fingerprint image.*/
double enhancedGabor::RidgeFreq(Mat & _im, Mat & _mask, Mat & _orient, int _blkSze, int _windSze, int _minWaveLength, int _maxWaveLength, Mat & _freq)
{
    int rows = _im.rows;
    int cols = _im.cols;
    _freq = _im.clone();
    _freq = Scalar(0);

    
    Mat freqIm, freqRect;
    CvRect rect;
    Mat blkim, blkor;
    
    for(int r = 0; r < (rows - _blkSze ); r += _blkSze)
    {
        for(int c = 0; c < (cols - _blkSze ); c += _blkSze)
        {
            /*if(c >= (cols - _blkSze) && r < (rows- _blkSze))
                rect = cvRect(c, r, (cols - c), _blkSze );
            else if(c < (cols - _blkSze) && r >= (rows- _blkSze))
                rect = cvRect(c, r, _blkSze, (rows - r));
            else if(c >= (cols - _blkSze) && r >= (rows- _blkSze))
                rect = cvRect(c, r, (cols - c), (rows - r));
            else*/
            rect = cvRect(c, r, _blkSze, _blkSze);
            blkim = _im(rect);
            blkor = _orient(rect);

            //freq(rect) = Freqest(blkim, blkor, _windSze, _minWaveLength, _maxWaveLength);
            Freqest(blkim, blkor, _windSze, _minWaveLength, _maxWaveLength, freqIm);
            freqRect = _freq(rect);
            freqIm.copyTo(freqRect);
        }
    }
    
    // Mask out frequencies calculated for non ridge regions
    multiply(_freq, _mask, _freq);
    divide(_freq, 255, _freq);

    // Find median frequcey over al the valid regions of the image
    std::vector<double> medList;
    double *fr;
    for(int i = 0; i<_freq.rows; i++)
    {
        fr = _freq.ptr<double>(i);
        for(int j = 0; j<_freq.cols; j++)
        {
            if(fr[j] > 0)
            {
                medList.push_back(fr[j]);
            }
        }
    }
    
    size_t n = medList.size() / 2;
    nth_element(medList.begin(), medList.begin()+n, medList.end());
    
    return medList[n];
}

/* Freqest - Function to estimate the fingerprint ridge frequency within a small block*/
void enhancedGabor::Freqest(Mat & _im, Mat & _orientIm, int _windSze, int _minWaveLength, int _maxWaveLength, Mat & _freqim)
{
    int rows = _im.rows;
    int cols = _im.cols;
    
    /*Find mean orientation within the block. This is done by averaging the sines
     *and cosines of the doubled angles before reconstructing the angle again. This avoids
     *wraparound problems at the origin.
     */
    Mat cosor = _orientIm.clone();
    Mat sinor = _orientIm.clone();
    double *cp, *sp;
    for(int i = 0; i<rows; i++)
    {
        cp = cosor.ptr<double>(i);
        sp = sinor.ptr<double>(i);
        for(int j = 0; j<cols; j++)
        {
            double degree = (_orientIm.at<double>(i,j) / 255) * 2;
            cp[j] = cos(degree);
            sp[j] = sin(degree);
        }
    }
    double cosorient = MeanMat(cosor);
    double sinorient = MeanMat(sinor);
    double orient = atan2(sinorient, cosorient)/2;
    
    /*Rotate the image block so that the ridges are vertical*/
    //Mat t = (Mat_<double>(3,3) <<1,2,3,4,5,6,7,8,9 );
    Mat rotim = mh.imrotate(_im, orient/PI*180+90).clone();

    /* Now crop the image so that the rotated image does not contain any 
     * invalid regions. This prevents the projectio odwn the columns from
     * being muched up.
     */
    double cropsze = 0.0f, offset = 0.0f;
    if(rows/sqrt(2) >= 0)
        cropsze = floor(rows/sqrt(2));
    else
        cropsze = ceil(rows/sqrt(2));

    if((rows - cropsze)/2 >= 0)
        offset = floor((rows - cropsze)/2);
    else
        offset = ceil((rows - cropsze)/2);

    CvRect rect;
    rect = cvRect(offset-1, offset-1, cropsze+1, cropsze+1);
    rotim = rotim(rect);
    
    /*
     * Sum down the columns to get a projection of the grey values down the ridges.
     */
    Mat proj(1, rotim.cols, rotim.type());
    proj = Scalar(0);
    
    double *ri;
    for(int i = 0; i < rotim.rows; i++)
    {
        ri = rotim.ptr<double>(i);
        for(int j = 0; j < rotim.cols; j++)
        {
            proj.at<double>(0, j) += ri[j]/255;
        }
    }

    /*
     * Find peaks in projected grey values by performing a greyscale
     * dilation and then finding where the dialtion equals the original values.
     */
    Mat windS(1, _windSze, proj.type(), Scalar(1));
    Mat dilation = proj.clone();
    
    //erode(proj, dilation, windS,cv::Point(-1,-1),1,BORDER_CONSTANT );
    mh.ordfilt2(proj, _windSze, windS, dilation);
    
    Mat maxpts = dilation.clone();
    for(int j = 0; j<maxpts.cols; j++)
    {
        maxpts.at<double>(0,j) = (proj.at<double>(0, j) == dilation.at<double>(0, j))&(proj.at<double>(0 ,j) > MeanMat(proj));
    }
    vector<int> maxind;
    for(int j = 0; j<maxpts.cols; j++)
    {
        if(maxpts.at<double>(0,j))
            maxind.push_back(j);
    }

    /* Determine the spatial frequency of the ridges by dividing the distance between the
     * 1st and last peaks by the (No of peaks-1). If no peaks are detected, or the wavelenght
     * is outside the allowed bounds, the frequency image is set to 0
     */
    
    _freqim = _im.clone();
    _freqim = Scalar(0);

    if(maxind.size() < 2)
        return;
    else
    {
        int NoOfPeaks = (int)maxind.size();
        
        double waveLength = (maxind.at(maxind.size()-1)-maxind.at(0))/(NoOfPeaks - 1);
        if(waveLength > _minWaveLength && waveLength < _maxWaveLength)
        {
            _freqim = Scalar(1/waveLength*255);
        }
        return;
    }
    
}
/*4. RidgeFilter- Function to enhance fingerprint image via oriented filters*/
void enhancedGabor::RidgeFilter(Mat & _im, Mat & _orient, Mat& _freq, int _kx, int _ky, int _showFilter, Mat & _newim)
{
    int angleInc = 10;// Fixed angle increment between filter orientations in degrees. This should divide evenly into 180
    _newim = _im.clone();
    _newim = Scalar(0);
    
    Mat tmpFreq = _freq.clone();
    vector<int> validr, validc, ind;
    vector<double> freqList;
    int index  = 0;
    for(int i = 0; i < tmpFreq.cols; i++)
    {
        for(int j = 0; j < tmpFreq.rows; j++)
        {
            freqList.push_back(tmpFreq.at<double>(j,i));
            if(tmpFreq.at<double>(j,i) > 0)
            {
                validc.push_back(i);
                validr.push_back(j);
                index = i * _im.rows + j;
                ind.push_back(index);
            }
        }
    }
    /*int len = (int)validr.size();
    for(int i = 0; i < len; i++)
    {
        int index = validc.at(i)*_im.rows+validr.at(i);
        ind.push_back(index);
    }*/
    
    // Round the array of frequencies to the nearest 0.01 to reduce the number of distinct frequencies we have to deal with
    int len = (int)validr.size();
    for(int i =0; i<len; i++)
    {
        tmpFreq.at<double>(ind.at(i)) = round(tmpFreq.at<double>(ind.at(i))/255*100)/100;
    }
    
    // Generate an array of the distinct frequencies present in the array freq
    vector<double> unfreq;
    std::map<double, bool> unfreqMap;
    std::map<double, bool>::iterator mapSearch;
    double cur;
    for(int i =0; i<len; i++)
    {
        cur = freqList.at(ind.at(i));
        mapSearch = unfreqMap.find(cur);
        if(mapSearch == unfreqMap.end())
        {
            unfreq.push_back(cur/255);
            unfreqMap.insert(std::map<double, bool>::value_type(cur, true));
        }
    }
    
    // Generate a table, given the frequency value multiplied by 100 to obtain an integer index, returns the
    // index within hte unfreq array that it corresponds to
    double freqIndex[100];
    for(int i = 0; i < 100; i++)
        freqIndex[i] = 1;
    for(int k = 1; k < (int)unfreq.size(); k++)
    {
        int j = round(unfreq.at(k) * 100);
        
        freqIndex[j] = k;
    }
    
    // Generate filters corresponding to these distinct frequencies and orientations in 'angleInc' increments
    int unfreqSize = (int)unfreq.size();
    std::vector<std::vector<Mat> > filter(unfreqSize);
    
    for(int i = 0; i < unfreqSize; i++)
        filter[i].resize(180/angleInc);

    std::vector<double> sze;
    for(int i = 0; i < (int)unfreq.size(); i++)
        sze.push_back(0);
    
    double sigmaX;
    double sigmaY;
    for(int k = 0; k < (int)unfreq.size(); k++)
    {
        sigmaX = 1 / (unfreq.at(k)) * _kx;
        sigmaY = 1 / (unfreq.at(k)) * _ky;
        
        sze.at(k) = round(3 * (sigmaX >= sigmaY ? sigmaX : sigmaY));
        Mat x(sze.at(k)*2+1, sze.at(k)*2+1, tmpFreq.type(), Scalar(0));
        Mat y(sze.at(k)*2+1, sze.at(k)*2+1, tmpFreq.type(), Scalar(0));
        Range xgv(-sze.at(k), sze.at(k));
        Range ygv(-sze.at(k), sze.at(k));
        mh.meshgrid(xgv, ygv, x, y);
        x.convertTo(x, CV_64FC1);
        y.convertTo(y, CV_64FC1);
        
        // reffilter = exp(-(x.^2/sigmax^2 + y.^2/sigmay^2)/2) .* cos(2*pi*unfreq(k)*x)
        Mat tmp1, tmp2, reffilter;
        
        /*exp(-(x.^2/sigmax^2 + y.^2/sigmay^2)/2)*/
        multiply(x, x, tmp1);
        divide(tmp1, sigmaX*sigmaX, tmp1);
        multiply(y, y, tmp2);
        divide(tmp2, sigmaY*sigmaY, tmp2);
        add(tmp1, tmp2, tmp1);
        divide(tmp1, 2, tmp1);
        subtract(0, tmp1, tmp1);
        
        exp(tmp1, tmp1);
        /*cos(2*pi*unfreq(k)*x)*/
        multiply(x, 2*PI*unfreq.at(k), tmp2);
        double *t2;
        for(int i = 0; i < tmp2.rows; i++)
        {
            t2 = tmp2.ptr<double>(i);
            for(int j = 0; j < tmp2.cols; j++)
            {
                t2[j] = cos(t2[j]);
            }
        }
        /***********************/
        multiply(tmp1, tmp2, reffilter);
        /***********************/
        
        // Generate rotated versions of the filter. Note orientation image provides orientation *along*
        // the ridges, hence +90 degrees, and imrotate requires angles +ve anticlockwise, hence the minus sign.
        Mat rotatedImg;
        for(int o = 0; o < 180/angleInc; o++)
        {
            rotatedImg = mh.imrotate(reffilter, -((o+1)*angleInc + 90));
            filter.at(k).at(o) = rotatedImg.clone();
        }
    }
    
    // Find indices of matrix points greater than maxsze from the image boundary.
    double maxSez = sze[0];
    vector<int> finalInd;
    for(int i =0; i < (int)validc.size(); i++)
    {
        if((validc.at(i) > maxSez) && (validc.at(i)<(_im.cols - maxSez - 1))
           && (validr.at(i)> maxSez) && (validr.at(i) < (_im.rows - maxSez - 1)) )
            finalInd.push_back(i);
    }
    
    // Convert orientation matrix values from radians to an index value
    // that corresponds to round(degree/angleInc)
    double maxOrientIndex = round(180/angleInc);
    Mat orientIndex = _orient.clone();

    double *oi;
    for(int i = 0; i < orientIndex.rows; i++)
    {
        oi = orientIndex.ptr<double>(i);
        for(int j = 0; j < orientIndex.cols; j++)
        {
            oi[j] = round(oi[j]/255/PI*180/angleInc);
            if(oi[j] < 1)
                oi[j] = oi[j] + maxOrientIndex;
            if(oi[j] > maxOrientIndex)
                oi[j] = oi[j] - maxOrientIndex;
        }
    }
    // Finally do the filtering

    for(int k = 0; k < (int)finalInd.size(); k++)
    {
        double r = validr.at(finalInd.at(k));
        double c = validc.at(finalInd.at(k));
        
        // find filter corresponding to freq(r,c)
        int ind = round(tmpFreq.at<double>(r, c)/255*100);
        int filterIndex = freqIndex[ind] - 1;
        int s = sze[filterIndex];
        
        //sum(sum(_ im(r-s:r+s, c-s:c+s) .* filter{filterindex, orientindex(r,c)} _))
        Rect rect;
        int x = c-s-1;
        int y = r-s-1;
        int size = 2*s+1;
        
        rect = Rect(x, y, size, size);
        Mat tmp1, tmp2, tmp3;
        tmp1 = _im(rect);
            
        int orInd = orientIndex.at<double>(r, c) - 1;
            
        tmp2 = filter[filterIndex][orInd];
        
        multiply(tmp1, tmp2, tmp3);
        
        double tmpvalue = sum(tmp3)[0];
        _newim.at<double>(r, c) = tmpvalue;

    }
    return;
}

/*scuess*1.1 NORMALISE - Normalises image values to 0-1, or to desired mean and variance*/
void enhancedGabor::Normalise(const Mat & _img, double _reqmean, double _reqvar, Mat & normalisedImg)
{
    if (_img.channels() == 3)
    {
        printf("Cannot normalise colour image to desired mean and variance");
    }
    
    _img.copyTo(normalisedImg);
    
    double mean = MeanMat( _img );
    double *t;
    for (int i = 0; i < normalisedImg.rows; i++)
    {
        t = normalisedImg.ptr<double>(i);
        for (int j = 0; j < normalisedImg.cols; j++)
        {
            t[j] -= mean;
        }
    }
    
    double std = StdMat(_img);

    for (int i = 0; i < normalisedImg.rows; i++)
    {
        t = normalisedImg.ptr<double>(i);
        for (int j = 0; j < normalisedImg.cols; j++)
        {
            t[j] /= std;
        }
    }

    for (int i = 0; i < normalisedImg.rows; i++)
    {
        t = normalisedImg.ptr<double>(i);
        for (int j = 0; j < normalisedImg.cols; j++)
        {
            t[j] = (_reqmean + t[j]*sqrt(_reqvar))*255;
        }
    }
    
    return;
}
/*1.2 *********************Process the image by block********************************/
Mat enhancedGabor::BlkProc(const Mat & _img, int _colBlkSize, int _rowBlkSize)
{
    //Process Function is 'std(x(:))*ones(size(x)' matlab format
    Mat stddevim;
    stddevim = Mat::zeros(_img.rows,_img.cols, CV_64FC1);
    
    Mat tmp;
    CvRect rect;
    int colblkNum = ceil(_img.cols/(double)_colBlkSize);
    int rowblkNum = ceil(_img.rows/(double)_rowBlkSize);
    int rectWidth = 0;
    int rectHeight = 0;
    
    for(int x = 1; x<=rowblkNum;x++)
    {
        for(int y = 1; y<=colblkNum;y++)
        {
            if(x==rowblkNum&&y!=colblkNum)
            {
                rectHeight = _img.rows-(rowblkNum-1)*_rowBlkSize;
                rectWidth = _colBlkSize;
            }
            else if(y==colblkNum&&x!=rowblkNum)
            {
                rectHeight = _rowBlkSize;
                rectWidth = _img.cols-(colblkNum-1)*_colBlkSize;
            }
            else if(x==rowblkNum&&y==colblkNum)
            {
                rectHeight = _img.rows-(rowblkNum-1)*_rowBlkSize;
                rectWidth = _img.cols-(colblkNum-1)*_colBlkSize;
            }
            else
            {
                rectHeight = _rowBlkSize;
                rectWidth = _colBlkSize;
            }
            
            rect = cvRect((y-1)*_colBlkSize,(x-1)*_rowBlkSize,rectWidth,rectHeight);
            tmp = _img(rect);
            /*Mat tmpZeros(_rowBlkSize, _colBlkSize, CV_64FC1, Scalar(0));
            for(int i = 0; i < tmp.rows; i++)
            {
                for(int j = 0; j < tmp.cols; j++)
                {
                    tmpZeros.at<double>(i, j) = tmp.at<double>(i, j );
                }
            }*/
           
            double stdV = StdMat(tmp);
            
            for(int i = rect.y;i<(rect.y+rect.height);i++)
            {
                for(int j = rect.x;j<(rect.x+rect.width);j++)
                {
                    stddevim.at<double>(i,j) = stdV;
                    
                }
            }
            
        }
    }
    return stddevim;
}

/*********************Return the mean value of a Mat********************************/
double enhancedGabor::MeanMat(const Mat & _imgBlock)
{
    cv::Scalar tmpValue = cv::mean(_imgBlock);
    double mean = tmpValue[0];
    return mean;
}
/*********************Return the Standard Deviation value of a Mat********************************/
double enhancedGabor::SDMat(const Mat & _imgBlock)
{
    double sd = 0;

    double sum = 0;
    
    double tmpMean = MeanMat(_imgBlock);
    
    Mat tmp = _imgBlock.clone();

    //_imgBlock.convertTo(tmp, CV_64FC1);

    int eleNum = _imgBlock.cols * _imgBlock.rows - 1;
    double *t;
    for (int i = 0; i < _imgBlock.rows; i++)
    {
        t = tmp.ptr<double>(i);
        for (int j = 0; j < _imgBlock.cols; j++)
        {
            double a = (t[j] - tmpMean);
            sum = sum + a * a;
        }
    }
    sd = sqrt(sum / eleNum);

    return sd;
}
double enhancedGabor::StdMat(const Mat & _imgBlock)
{
    Scalar mean;
    Scalar stddev;
    
    cv::meanStdDev ( _imgBlock, mean, stddev );
    double m = mean.val[0];
    double std = stddev.val[0];
    
    return std;
}
/*********************************************************************************/

