//
//  strandsOperator.h
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

#ifndef __test__strandsOperator__
#define __test__strandsOperator__

#include <stdio.h>
#include "cv.h"
#include "highgui.h"
#include "matlabhelper.h"
#include "splineController.h"

using namespace std;
using namespace cv;
class strandsOperator
{
public:
    
    strandsOperator();
    
    matlabHelper mh;
    splineController spCtr;
    vector<vector<Point2f> > ctrlPts;
    
    vector<vector<Point2f> > curCurves;
    
    Mat input_img;
    
    Mat StrandsAnalysis(const Mat & _imgErosion);
    
    // Function to detect bifurcate points on a binary image (background is white and lines are black).
    // Bifurcate points are points that have 3 neighbor points (black) in its 8 neighbors.
    /* Arguments:
     *          _imgErosion - the input binary image
     * Returns:
     *          bifPts - the coordinate of the bifurcate points.     
     */
    vector<Point2f> DetectBifuPoints(const Mat & _imgErosion);
    
    // Function removes bifurcation points in the binary image
    /* Arguments:
     *          _imgErosion - the input binary image wiht bifurcate points
     *          bifPts - the coordinate of the bifurcate points.
     * Returns:
     *          _imgWithoutBifPts - the image without the bifurcate points.
     */
    void RemoveBifuPoints(const Mat & _imgErosion, vector<Point2f> bifPts, Mat & _imgWithoutBifPts);
    
    // Function removes single points in the binary image
    /* Arguments:
     *          _imgWithoutBifPts - the input binary image wiht single points
     *          bifPts - the coordinate of the bifurcate points.
     * Returns:
     *          _imgWithoutSinglePts - the image without the single points.
     */
    void DetectRemoveSinglePoint(const Mat & _imgWithoutBifPts, Mat & _imgWithoutSinglePts);
    
    // Function detects end points in a binary image with line segments (background is white and lines are black).
    // End points are points that have only 1 neigbor points are black in its neighbors.
    /* Arguments:
     *          _img - the input binary image
     * Returns:
     *          endPoints - Coordinates of the end points
     */
    vector<Point2f> DetectEndPoints(const Mat & _img);
    
    // Function tracks line segments in a binary hair image. It uses the endPts as the current seed to trace the line one
    // pixel by one pixel. Ince each line have two end points, this function automatically remove duplicate lines.
    /* Arguments:
     *          _img - the input binary image with line segments.
     *          endPts -  the end points of all the lines in the image.
     * Returns:
     *          lines - all the lines that have been tracked in the binary image, save in the vectors.
     */
    vector< vector<Point2f> > TrackLineSegments(const Mat & _img, vector<Point2f> endPts);
    // Line Tracing of a bw image(binary)
    // Assuming foreground to be in black and background to be in white, this function traces the entire line and requres
    // row and column value of any pixel which lies on the line and returns the (row, col) values of tall the point which lie on the line
    /* Arguments:
     *          _img - the input binary image with line segments.
     *          endPts -  the end points of all the lines in the image.
     * Returns:
     *          lines - the line that have been tracked in the binary image, save in the vector.
     */
    void LinesTrace(const Mat & _img, Point2f endPt, vector<Point2f> & _line);
    
    // Function removes short lines in binayry image under the constrain of the lengthThreshold
    /* Arguments:
     *          _lines - the line segments.
     *          _lineLengthThreshold -  if the length of the lnesare sharoter than the threshold, the 
     *                                  lines will the removed. Otherwise, the lines will be keeped.
     */
    void RemoveShortLines(vector< vector<Point2f> > & _lines, int _lineLengthThreshold);
    
    // Function removes the duplicate pixel of a line
    /* Arguments:
     *          _lines - the input line segments.
     */
    void DuplicateCheck(vector< vector<Point2f> > & _lines);
    
    // Function draw an color image of the spline curves based on the control points
    /* Arguments:
     *          _ctrlPts - the control points of the spline curves.
     */
    Mat DrawCurvesImg(const Mat & _img, vector<vector<Point2f> > & _ctrlPts);
    
    // Function removes the duplicate pixel of a line
    /* Arguments:
    %   ctrlPoints                    - Control points of all the line segments.
    %   extensionParameter            - The parameter that defines the extention
    %                                   level.
    %   connectionThreshold           - The threshold that used to estimated the Euclidean
    %                                   distance of the two connected points. If
    %                                   the distance between two potential connecting points
    %                                   is smaller than the threshold, those two points
    %                                   should be connected. Otherwise, those
    %                                   two points shold not be connected.
    %   newCtrlPtsNum                 - The number of the control points on each
    %                                   connected splines.
    %   timesOfExtendAndConnection    - Time of the extension and connnection.
    %   connectionThresholdIncrease   - The increase value of the connection
    %                                   threshold.
    %
    % Returns:
    %   ctrlPoints                    - Control points of slines
    %   splines                       - Piecewise polynomial of splines
    */
    vector<vector<Point2f> > ExtendAndConnectSplines(double ExtensionParameter, int connectionThreshold, int newCtrlPtsNum, int maximumIterlation, int connectionThresholdIncrease);
    
    /*Median Filter*/
    Mat MedianFilter(int windowLength);
    
    bool ExportCtrlPts(char * filepath);
};
#endif /* defined(__test__strandsOperator__) */
