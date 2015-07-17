//
//  strandsOperator.h
//  test
//
//  Created by  Zhongrui Li on 2015-07-08.
//
//

#ifndef __test__strandsOperator__
#define __test__strandsOperator__

#include <stdio.h>
#include "cv.h"
#include "highgui.h"
#include "matlabhelper.h"
using namespace std;
using namespace cv;
class strandsOperator
{
public:
    
    strandsOperator();
    
    matlabHelper mh;
    
    void StrandsAnalysis(const Mat & _imgErosion);
    
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
     *          _img - the input binary image with line segments.
     *          endPts -  the end points of all the lines in the image.
     * Returns:
     *          lines - the line that have been tracked in the binary image, save in the vector.
     */
    void RemoveShortLines(vector< vector<Point2f> > & _lines, int _lineLengthThreshold);
    

    
};
#endif /* defined(__test__strandsOperator__) */
