//
//  splineController.cpp
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

#include "splineController.h"
#include <fstream>
#include "cv.h"
#include "highgui.h"
#include "alglib/ap.h"
#include "alglib/interpolation.h"
#include "CRSpline/overhauser.hpp"
using namespace std;
using namespace cv;
using namespace alglib;


static double distancePoint2f(Point2f A, Point2f B);
static bool point_comparator(const cv::Point2f &a, const cv::Point2f &b);
splineController::splineController()
{
}

// Function obtain control points from each line in image
vector<vector<Point2f> > splineController::GetCtrlPtsFromEachLines(vector< vector<Point2f> > & _lines, int numOfCtrlPts)
{
    vector<CRSpline> splines;
    CtrlPts2Splines(_lines, splines);
    
    vector<vector<Point2f> > ctrlPts;
    Spline2CtrlPts(splines, numOfCtrlPts, ctrlPts);
    
    return ctrlPts;
}

void splineController::CtrlPts2Splines(vector< vector<Point2f> > & _lines,vector<CRSpline> & _splines)
{
    vector<Point2f> tmpLine;
    real_1d_array x, y;
    
    
    vec3 tmpVec;
    
    for(int i = 0; i < (int)_lines.size(); i++)
    {
        tmpLine = _lines.at(i);
        int lineLength = (int)tmpLine.size();
        CRSpline * spline = new CRSpline();
        for(int j = 0; j < lineLength; j++)
        {
            tmpVec.x = tmpLine.at(j).x;
            tmpVec.y = tmpLine.at(j).y;
            tmpVec.z = 0;
            spline->AddSplinePoint(tmpVec);
        }
        _splines.push_back(*spline);
        delete spline;
    }
}

// Function calculates control points from cubic splines
void splineController::Spline2CtrlPts( vector<CRSpline> & _splines, int numOfCtrlPts, vector< vector<Point2f> > & ctrlPts)
{
    ctrlPts.clear();
    vector<Point2f> ctrlPtsOfSingleSpline;
    CRSpline * tmpSpline;
    vec3 vec;
    Point2f ctrlPt;
    for(int i = 0; i < (int)_splines.size(); i++)
    {
        tmpSpline = &_splines.at(i);
        for(float j = 0; j <=1; j = j + (float)1/(numOfCtrlPts-1))
        {
            vec = tmpSpline->GetInterpolatedSplinePoint(j);
            if(!ctrlPtsOfSingleSpline.empty())
            {
                if(vec.y <= ctrlPtsOfSingleSpline.back().y)
                {
                    vec = tmpSpline->GetInterpolatedSplinePoint(j+1/(numOfCtrlPts-1)/5);
                }
            }
            ctrlPt.x = vec.x;
            ctrlPt.y = vec.y;
            ctrlPtsOfSingleSpline.push_back(ctrlPt);
        }
        ctrlPts.push_back(ctrlPtsOfSingleSpline);
        ctrlPtsOfSingleSpline.clear();
    }
    
}

// Function calculates control points from cubic splines
void splineController::SplinesExtension(vector< vector<Point2f> > & _ctrlPts, double extensionValue, vector< vector<Point2f> > & _extendedCtrlPts)
{
    vector<Point2f> tmpLine;
    real_2d_array xy;
    Point2f tempHead, tempEnd;
    _extendedCtrlPts.clear();
    _extendedCtrlPts = _ctrlPts;
    for(int lineCnt = 0 ; lineCnt < (int)_ctrlPts.size(); lineCnt++)
    {
        tmpLine = _ctrlPts.at(lineCnt);
        std::vector<double> X((int)tmpLine.size()), Y((int)tmpLine.size());
        for(int ptCnt = 0; ptCnt < (int)tmpLine.size(); ptCnt++)
        {
            X[ptCnt] = (double)tmpLine.at(ptCnt).x;
            Y[ptCnt] = (double)tmpLine.at(ptCnt).y;
        }
        xy.setlength(X.size(), 2);
        
        for ( int i = 0; i < (int)X.size(); i++ )
        {
            xy[i][1] = X.at(i);
            xy[i][0] = Y.at(i);
        }
        
        alglib::pspline2interpolant spline;
        
        try
        {
            pspline2build(xy, tmpLine.size(), 2, 2, spline);
        }
        catch(alglib::ap_error e)
        {
            printf("error msg: %s\n", e.msg.c_str());
        }
        
        
        double lengthY = abs(tmpLine.back().y - tmpLine.front().y);
        double extensionPar = extensionValue/lengthY;
        //double extendHeadY = tmpLine.front().y - extensionValue;
        //double extendEndY = tmpLine.back().y + extensionValue;
        double extendHeadX,extendHeadY,extendEndX, extendEndY;
        try
        {
            pspline2calc(spline, 0 - extensionPar, extendHeadY, extendHeadX);
            pspline2calc(spline, 1 + extensionPar, extendEndY, extendEndX);
        }
        catch(alglib::ap_error e)
        {
            printf("error msg: %s\n", e.msg.c_str());
        }
        
        tempHead.x = extendHeadX;
        tempHead.y = extendHeadY;
        tempEnd.x = extendEndX;
        tempEnd.y = extendEndY;

        vector<Point2f>::iterator it = _extendedCtrlPts.at(lineCnt).begin();
        _extendedCtrlPts.at(lineCnt).insert(it,tempHead);
        it = _extendedCtrlPts.at(lineCnt).end();
        _extendedCtrlPts.at(lineCnt).insert(it, tempEnd);
    }
    return;
}

// Function connects two splines.
void splineController::ConnectSpline2D(vector< vector<Point2f> > & _ctrlPts, int connectionThreshold, int numberOfNewCtrlPts, vector<vector<Point2f> > & _newCtrlPts)
{
    _newCtrlPts.clear();
    int totalNumOfSplines = (int)_ctrlPts.size();
    Point2f curSplineStartPt, curSplineNextToStartPt, curSplineEndPt, curSplineNextToEndPt;
    for(int i = totalNumOfSplines-1; i >=0 ; i--)
    {
        if(!_ctrlPts.at(i).empty())
        {
            int numOfCtrlPtsForEachSpline = (int)_ctrlPts.at(i).size();
            curSplineStartPt = _ctrlPts.at(i).front();
            curSplineNextToStartPt = _ctrlPts.at(i).at(1);
            
            curSplineEndPt = _ctrlPts.at(i).back();
            curSplineNextToEndPt = _ctrlPts.at(i).at(numOfCtrlPtsForEachSpline - 2);
            
            double angStartPt = atan((curSplineStartPt.x - curSplineNextToStartPt.x)/(curSplineStartPt.y - curSplineNextToStartPt.y));
            double angEndPt = atan((curSplineEndPt.x-curSplineNextToEndPt.x)/(curSplineEndPt.y-curSplineNextToEndPt.y));

            /*Look for the nearest points among other splines for a given spline*/
            vector<double> distToStartPt(totalNumOfSplines), distToEndPt(totalNumOfSplines);

            for(int j = totalNumOfSplines-1; j >=0; j-- )
            {
                if( j != i && !_ctrlPts.at(j).empty() && !_ctrlPts.at(i).empty())
                {
                    int numOfCtrlPtsForEachSplineTwo = (int)_ctrlPts.at(j).size();
                    
                    Point2f tmpStartPt,tmpNextToStartPt, tmpEndPt, tmpNextToEndPt;
                    
                    tmpStartPt = _ctrlPts.at(j).front();
                    tmpNextToStartPt = _ctrlPts.at(j).at(1);
                    
                    tmpEndPt = _ctrlPts.at(j).back();
                    tmpNextToEndPt = _ctrlPts.at(j).at(numOfCtrlPtsForEachSplineTwo - 2);
                    
                    double angStartPtTmp, angEndPtTmp;
                    
                    angStartPtTmp = atan((tmpStartPt.x - tmpNextToStartPt.x)/(tmpStartPt.y - tmpNextToStartPt.y));
                    angEndPtTmp = atan((tmpEndPt.x - tmpNextToEndPt.x)/(tmpEndPt.y - tmpNextToEndPt.y));
                    /* Calculate the distances between:
                     % 1. the end point of the current spline and start points
                     %    of other splines;
                     % 2. the start point of the current spline and end points
                     %    of other splines;
                     */
                    distToStartPt.at(j) = distancePoint2f(curSplineEndPt, tmpStartPt);
                    distToEndPt.at(j) = distancePoint2f(curSplineStartPt, tmpEndPt);
                }
                else
                {
                    distToStartPt.at(j) = 5000000;
                    distToEndPt.at(j) = 5000000;
                }
            }

            int IndexNearestPtToStartPt = (int)(min_element(distToStartPt.begin(), distToStartPt.end()) - distToStartPt.begin());
            double NearestPtToStartPt = distToStartPt.at(IndexNearestPtToStartPt);
            
            int IndexNearestPtToEndPt = (int)(min_element(distToEndPt.begin(), distToEndPt.end()) - distToEndPt.begin());
            double NearestPtToEndPt = distToEndPt.at(IndexNearestPtToEndPt);
            
            /*Connect tow splines to create new one*/
            if(NearestPtToStartPt < NearestPtToEndPt && NearestPtToStartPt<=connectionThreshold)
            {
                vector<Point2f> tmpSpline = _ctrlPts.at(i);
                for(int m = 0; m < (int)_ctrlPts.at(IndexNearestPtToStartPt).size(); m++)
                {
                    tmpSpline.push_back(_ctrlPts.at(IndexNearestPtToStartPt).at(m));
                }
                _newCtrlPts.push_back(tmpSpline);

                _ctrlPts.at(i).clear();
                _ctrlPts.at(IndexNearestPtToStartPt).clear();
            }
            else if(NearestPtToStartPt>=NearestPtToEndPt && NearestPtToEndPt<=connectionThreshold)
            {
                
                vector<Point2f> tmpSpline = _ctrlPts.at(IndexNearestPtToEndPt);
                for(int m = 0; m < (int)_ctrlPts.at(i).size(); m++)
                {
                    tmpSpline.push_back(_ctrlPts.at(i).at(m));
                }
                _newCtrlPts.push_back(tmpSpline);
                _ctrlPts.at(i).clear();
                _ctrlPts.at(IndexNearestPtToEndPt).clear();
            }
        }
    }
    /****************************************************************/
    /*also save the unconnected splines*/

    vector<CRSpline> splines;
    vector<vector<Point2f> > ctrlPtsOfUnconnectedSplines;
    
    for(int i = (int)_ctrlPts.size()-1; i >= 0; i--)
    {
        if(_ctrlPts.at(i).empty())
        {
            std::vector<vector<Point2f> >::const_iterator it = _ctrlPts.begin()+i;
            _ctrlPts.erase(it);
        }
    }
    CtrlPts2Splines(_ctrlPts, splines);
    Spline2CtrlPts(splines, numberOfNewCtrlPts, ctrlPtsOfUnconnectedSplines);
    
    for(int i = 0; i<(int)ctrlPtsOfUnconnectedSplines.size(); i++)
    {
        _newCtrlPts.push_back(ctrlPtsOfUnconnectedSplines.at(i));
    }
    
    /*****************************************************************/
    /*Collection of control points --> collection of splines*/
    vector<CRSpline> newConnectedSplines;
    CtrlPts2Splines(_newCtrlPts, newConnectedSplines);
    try{
    Spline2CtrlPts(newConnectedSplines, numberOfNewCtrlPts, _newCtrlPts);
    }catch(exception e)
    {
        cout<<e.what()<<endl;
    }
}

static double distancePoint2f(Point2f A, Point2f B)
{
    double dist;
    dist = sqrt(pow((A.x - B.x), 2) + pow((A.y-B.y), 2));
    return dist;
}

/* Description:
 %   Function performs median filter on splines using the predefined window
 %   size that indicated using windowLength*/
static bool point_comparator(const cv::Point2f &a, const cv::Point2f &b) {
    return (a.x < b.x);
}
void splineController::MedianFilter(vector<vector<Point2f> > & _ctrlPts, int windowLength, vector<vector<Point2f> > & _filteredCurves)
{
    for(int i = 0; i<(int)_ctrlPts.size(); i++)
    {
        for(int j = 0; j <(int)_ctrlPts.at(i).size(); j++)
        {
            if(j >windowLength && j<(int)_ctrlPts.at(i).size()-windowLength)
            {
                vector<Point2f> window;
                for(int m = 0; m <2*windowLength +1; m++)
                {
                    window.push_back(_ctrlPts.at(i).at(j-windowLength+m));
                }
                /********meidian filter*******************/
                sort(window,point_comparator);
                _ctrlPts.at(i).at(j).x = window.at(windowLength).x;

            }
        }
    }
    
    for(int i = 0; i<(int)_ctrlPts.size();i++)
    {
        for(int j = 1; j < (int)_ctrlPts.at(i).size();j++)
        {
            if(_ctrlPts.at(i).at(j).y<_ctrlPts.at(i).at(j-1).y)
            {
                vector<Point2f>::iterator it = _ctrlPts.at(i).begin()+j;
                _ctrlPts.at(i).erase(it);
                j--;
            }
        }
    }
    
    vector<Point2f> tmpLine;
    real_2d_array xy;
    for(int lineCnt = 0 ; lineCnt < (int)_ctrlPts.size(); lineCnt++)
    {
        tmpLine = _ctrlPts.at(lineCnt);
        std::vector<double> X((int)tmpLine.size()), Y((int)tmpLine.size());
        for(int ptCnt = 0; ptCnt < (int)tmpLine.size(); ptCnt++)
        {
            X[ptCnt] = (double)tmpLine.at(ptCnt).x;
            Y[ptCnt] = (double)tmpLine.at(ptCnt).y;
        }
        xy.setlength(X.size(), 2);
        
        for ( int i = 0; i < (int)X.size(); i++ )
        {
            xy[i][1] = X.at(i);
            xy[i][0] = Y.at(i);
        }
        
        alglib::pspline2interpolant spline;
        
        try
        {
            pspline2build(xy, tmpLine.size(), 2, 2, spline);
        }
        catch(alglib::ap_error e)
        {
            printf("error msg: %s\n", e.msg.c_str());
        }
        tmpLine.clear();
        Point2f tmpPt;
        for(double j = 0; j < 1; j = j + (double)1/20)
        {
            double ptY,ptX;
            try
            {
                alglib::pspline2calc(spline, j, ptY, ptX);
                tmpPt.x = ptX;
                tmpPt.y = ptY;
                tmpLine.push_back(tmpPt);
            }
            catch(alglib::ap_error e)
            {
                printf("error msg: %s\n", e.msg.c_str());
            }
        }
        _filteredCurves.push_back(tmpLine);
    }
}

bool splineController::ExportCtrlPts(vector<vector<Point2f> > & _ctrlPts,char * filepath, CvRect crop_rect)
{
    ofstream outfile;
    outfile.open(filepath);     if(outfile.is_open())
    {
        for(int i =0; i<(int)_ctrlPts.size();i++)
        {
            outfile<<"*"<<i+1<<"********************************"<<endl;
            for(int j = 0; j<(int)_ctrlPts.at(i).size(); j++)
            {
                outfile<<(float)_ctrlPts.at(i).at(j).x + crop_rect.x<<","<<(float)_ctrlPts.at(i).at(j).y + crop_rect.y<<endl;
            }
        }
        
        outfile.close();
        return true;
    }
    else
    {
        return false;
    }

}
