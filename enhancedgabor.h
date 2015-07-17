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
#ifndef ENHANCEDGABOR_H
#define ENHANCEDGABOR_H
#include <cv.h>
#include <highgui.h>
#include "imageDisplayer.h"
#include "matlabhelper.h"
using namespace cv;
class enhancedGabor
{
public:

    int blk_sze_mask;

    int blk_sze;

    double thres_h;
    
    imageDisplayer imd;
    
    matlabHelper mh;
    
    enhancedGabor();
    // The main access function
    Mat EnhancedGaborFilter(const Mat & _img);
    /*******************************************************/
    /*RidgeSegment - Function to ormalise fingerprint image and segments ridge region
     *Arguments:
     *          _img       - Fingerprint image to be segmented.
     *          _blockSize - Block size over which the strandard deviation is determined (try a value of 16)
     *          _thresHold - Threshodl of standard deviation to decide if a block is a ridge region(try a value 0.1-0.2)
     *Return:
     *          normin - Image where the ridge regions are renormalised to have zero mean, unit standard deviation.
     *          mask   - Mask indicating ridge-like regions of the image, 0 fornon ridge regions , 1 for ridge regions.
     */
    
    void RidgeSegment(const Mat & _im, int _blockSize, double _thresHold, Mat &normim, Mat & maskImg);
    
    /*******************************************************/
    /*RidgeOrient - Function to estimates the local orientation of ridges in afingerprint
     *Arguments:
     *          _im                - A normalised input image.
     *          _gradientSigma     - Sigma of the derivative of Gaussian used ot compute image gradients.
     *          _blockSigme        - Sigma of the Gaussian weighting used to sum the gradient momnets.
     *          _orientSmoothSigma - Sigma of the Gaussian used to smooth the final orientation vector field.
     *Return:
     *          orientim      - The orientation image in radians. Orientation values are +ve clockwise and give the 
     *                          direction *along* the ridges.
     *          Reliability   - Measure of the reliability of the orientaiton measure. This is a value between 0  and 1.
     *                          The value above about 0.5 can be considered 'reliable'.
     */

    void RidgeOrient(const Mat & _im, double _gradientSigma, double _blockSigma, double _orientSmoothSigma, Mat & _orientim, Mat & _reliability);

    /*******************************************************/
    /*RidgeFreq - Function to estimates the fingerprint ridge frequency accross a fingerprint image.
     *            This is done by considering blocks of the image and determining a ridgecount within eachbloc by a call to FREQEST
     *Arguments:
     *          _im                 - Image to be processed.
     *          _mask               - Mask defining ridge redions (obtained from RIDGESEGMENT)
     *          _orient             - Ridge orientation image(obtained from RIDGEORIENT)
     *          _blkSze             - Size of image block to use
     *          _windSze            - Window lenght used to identify peaks . This should be an add integer, 3 or 5
     *          _minWaveLength      - Minimum and maximum ridge wavelengths, in pixels considered acceptable.
     *          _maxWaveLength
     *Return:
     *          freq                - An image the same size as im with values set to the estimated ridge spatial frequency within each 
     *                                image block. If a ridge frequency cannot be found within a block, or cannot be found within the 
     *                                limits set by min and max Wavelength freqim is set to zeros within that block.
     *          medianFreq          - Median frequency value evaluated over all the valid regions of the image.
     */
    double RidgeFreq(Mat & _im, Mat & _mask, Mat & _orient, int _blkSze, int _windSze, int _minWaveLength, int _maxWaveLength, Mat & _freq);
    
    /*******************************************************
    * Freqest - Function to estimate the fingerprint ridge frequency within a small block
    *           of a fingerprint image.  This function is used by RIDGEFREQ
    *
    %
    % Arguments:
    %         _im       - Image block to be processed.
    %         _orientIm - Ridge orientation image of image block.
    %         _windSze  - Window length used to identify peaks. This should be
    %                    an odd integer, say 3 or 5.
    %         _minWaveLength,  _maxWaveLength - Minimum and maximum ridge
    %                     wavelengths, in pixels, considered acceptable.
    %
    % Returns:
    %         freqim    - An image block the same size as im with all values
    %                     set to the estimated ridge spatial frequency.  If a
    %                     ridge frequency cannot be found, or cannot be found
    %                     within the limits set by min and max Wavlength
    %                     freqim is set to zeros.
    */
    void Freqest(Mat & _im, Mat & _orientIm, int _windSze, int _minWaveLength, int _maxWaveLength, Mat & _freqim);
    
    /*******************************************************
    % RidgeFilter- Function to enhance fingerprint image via oriented filters
    %
    % Arguments:
    %         _im           - Image to be processed.
    %         _orientim     - Ridge orientation image, obtained from RIDGEORIENT.
    %         _freqim       - Ridge frequency image, obtained from RIDGEFREQ.
    %         _kx, _ky      - Scale factors specifying the filter sigma relative
    %                       to the wavelength of the filter.  This is done so
    %                       that the shapes of the filters are invariant to the
    %                       scale.  kx controls the sigma in the x direction
    %                       which is along the filter, and hence controls the
    %                       bandwidth of the filter.  ky controls the sigma
    %                       across the filter and hence controls the
    %                       orientational selectivity of the filter. A value of
    %                       0.5 for both kx and ky is a good starting point.
    %         _showFilter   - An optional flag 0/1.  When set an image of the
    %                       largest scale filter is displayed for inspection.
    %
    % Returns:
    %         newim    - The enhanced image
    */
    void RidgeFilter(Mat & _im, Mat & _orient, Mat & _freq, int _kx, int _ky, int _showFilter, Mat & _newim);

    /*******************************************************/
    /*NORMALISE - Normalises image values to 0-1, or to desired mean and variance
     *Arguments:
     *          im      - A grey-level input image
     *          reqmean - The required mean value of the image
     *          reqvar  - THe required variance of the image
     *Return:
     *          Mat - the normalised image
     */
    void Normalise(const Mat & _img, double _reqmean, double _reqvar, Mat & normalisedImg);
    
    Mat BlkProc(const Mat & _img, int colBlksize, int rowBlksize);

    // calculte the Mean Value of the Mat image;
    double MeanMat(const Mat & _imgBlock);

    // calculte the Standard Deviation of the Mat image;
    double SDMat(const Mat &_imgBlock);
    double StdMat(const Mat & _imgBlock);
    

};

#endif // ENHANCEDGABOR_H
