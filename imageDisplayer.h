//
//  imageDisplayer.h
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

#ifndef __test__imageDisplayer__
#define __test__imageDisplayer__

#include <stdio.h>
#include <cv.h>
#include <highgui.h>

class imageDisplayer {
public:
    imageDisplayer();
    void displayer(cv::Mat _img);
};
#endif /* defined(__test__imageDisplayer__) */
