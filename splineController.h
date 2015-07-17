//
//  splineController.h
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

#ifndef __test__splineController__
#define __test__splineController__

#include <stdio.h>
#include "cv.h"
#include "highgui.h"
#include "alglib/interpolation.h"
#include "CRSpline/overhauser.hpp"
using namespace std;
using namespace cv;
using namespace alglib;
class splineController
{
public:
    
    splineController();
    
    // Function obtain control points from each line in image
    /* Arguments:
     *          lines        - Lines in image
     *          numOfCtrlPts - Desired number of control points
     * Returns:
     *          ctrlPts      - Control points of lines
     */
    vector<vector<Point2f> > GetCtrlPtsFromEachLines(vector< vector<Point2f> > & _lines, int numOfCtrlPts);
    
    // Function build cubic spline from the control points on the splines
    /* Arguments:
     *          lines        - control points of each line in image
     * Returns:
     *          spline1dinterpolant      - cubic splines
     */
    void CtrlPts2Splines(vector< vector<Point2f> > & _lines, vector<CRSpline> & _splines);

    // Function calculates control points from cubic splines
    /* Arguments:
     *          _splines     - spline list
     *          numOfCtrlPts - desired number of control points
     * Returns:
     *          ctrlPts      - control points of splines
     */
    void Spline2CtrlPts( vector<CRSpline> & _splines, int numOfCtrlPts, vector< vector<Point2f> > & ctrlPts);
    // Function calculates control points from cubic splines
    /*% Arguments:
    %    ctrlPts        - Control points from splines.
    %    extensionValue - Desired value of extension in two directions.
    %
    % Returns:
    %    newCtrlPts     - Control points of splines.
    %    newSplines     - Piecewise polynomial of splines.
    */
    void SplinesExtension(vector< vector<Point2f> > & _ctrlPts, double extensionValue, vector< vector<Point2f> > & _extendedCtrlPts);
    
    // Function connects two splines.
    /*% Arguments:
     %  ctrlPts              - All the control points on each spline.
     %  connectionThreshold  - Predefined threshold to estimate the distance between
     %                                1. dist1 - the end point on the 1st spline and the start point
     %                                on the 2nd spline.
     %                                2. dist2 - the start point on the 1st spline and the end point
     %                                on the 2nd spline.
     %                         If the minimum distance of dist1 and dist2 is smaller than the threshold,
     %                         then connect those two splines.
     %                         Otherwise do not connect those two splines.
     %   numberOfNewCtrlPts   - Define the number of the control points on each connnected spline.
     %
     % Returns:
     %   newCtrlPts           - Control points on each connected splines
     %   newSplines           - Piecewise polynomial of each spline
     */
    void ConnectSpline2D(vector< vector<Point2f> > & _extenedCtrlPts, int connectionThreshold, int numberOfNewCtrlPts, vector<vector<Point2f> > & _newCtrlPts);
    
    /* Description:
    %   Function performs median filter on splines using the predefined window
    %   size that indicated using windowLength
    %
    % Syntax:
    %   [newCtrlPts, newSplines] = UO_medianFilter(ctrlPts, windowLength)
    %
    % Arguments:
    %   ctrlPts        -  Control points on splines.
    %   windowLength   -  Half size of the filter's window
    %
    % Returns:
    %    newCtrlPts     - Control points of the filtered splines
    %    newSplines     - Piecewise polynomial of splines.
    */
    void MedianFilter(vector<vector<Point2f> > & _ctrlPts, int windowLength,vector<vector<Point2f> > & _filteredCurves);
    
    bool ExportCtrlPts(vector<vector<Point2f> > & _ctrlPts,char * filepath);
};
#endif/* defined(__test__splineController__) */