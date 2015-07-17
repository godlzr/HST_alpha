//
//  imageDisplayer.cpp
//  test
//
//  Created by  Zhongrui Li on 2015-06-24.
//
//

#include "imageDisplayer.h"
#include <iostream>


imageDisplayer::imageDisplayer()
{}

void imageDisplayer::displayer(cv::Mat _img)
{
    //cv::Mat img = imread("MyPic.JPG", CV_LOAD_IMAGE_UNCHANGED); //read the image data in the file "MyPic.JPG" and store it in 'img'
    cv::Mat img;
    _img.copyTo(img);

    if (img.empty()) //check whether the image is loaded or not
    {
        std::cout << "Error : Image cannot be loaded..!!" << std::endl;
        //system("pause"); //wait for a key press
    }
    
    cv::namedWindow("ObjImage", CV_WINDOW_AUTOSIZE); //create a window with the name "MyWindow"
    
    cv::imshow("ObjImage", img); //display the image which is stored in the 'img' in the "MyWindow" window
    
    cv::waitKey(0); //wait infinite time for a keypress
    
    cv::destroyWindow("ObjImage"); //destroy the window with the name, "MyWindow"
    
}