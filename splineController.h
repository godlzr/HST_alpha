//
//  splineController.h
//  test
//
//  Created by  Zhongrui Li on 2015-07-14.
//
//

#ifndef __test__splineController__
#define __test__splineController__

#include <stdio.h>
#include "cv.h"
#include "highgui.h"

using namespace std;
using namespace cv;
class splineController
{
    splineController();
    
    
    // Function obtain control points from each line in image
    /* Arguments:
     *          lines        - Lines in image
     *          numOfCtrlPts - Desired number of control points
     * Returns:
     *          ctrlPts      - Control points of lines
     */
    vector<vector<Point2f> > GetCtrlPtsFromEachLines(vector< vector<Point2f> > & _lines, int numOfCtrlPts);
    
};
#endif/* defined(__test__splineController__) */