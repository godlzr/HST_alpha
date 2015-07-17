//
//  imageDisplayer.h
//  test
//
//  Created by  Zhongrui Li on 2015-06-24.
//
//

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
