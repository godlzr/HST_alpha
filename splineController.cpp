//
//  splineController.cpp
//  test
//
//  Created by  Zhongrui Li on 2015-07-14.
//
//

#include "splineController.h"
#include "cv.h"
#include "highgui.h"
#include ""

using namespace std;
using namespace cv;
splineController::splineController()
{
}

// Function obtain control points from each line in image
vector<vector<Point2f> > splineController::GetCtrlPtsFromEachLines(vector< vector<Point2f> > & _lines, int numOfCtrlPts)
{
    vector<alglib::spline1dinterpolant> splines;
    real_1d_array x, y;
    vector<vector<Point2f> > ctrlPts;
    
}
