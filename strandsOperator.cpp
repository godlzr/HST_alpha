//
//  strandsOperator.cpp
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

#include "strandsOperator.h"

static bool point_comparator(const cv::Point2f &a, const cv::Point2f &b);
static bool point_equal(const cv::Point2f &a, const cv::Point2f &b);
static Scalar randomColor( RNG& rng );
strandsOperator::strandsOperator()
{
    
}
static Scalar randomColor( RNG& rng )
{
    int icolor = (unsigned) rng;
    return Scalar( icolor&255, (icolor>>8)&255, (icolor>>16)&255 );
}
Mat strandsOperator::StrandsAnalysis(const Mat & _imgErosion)
{
    input_img = _imgErosion.clone();
    vector<Point2f> bifPts;
    bifPts = DetectBifuPoints(_imgErosion);
    
    Mat imgWithoutBifPts;
    RemoveBifuPoints(_imgErosion, bifPts, imgWithoutBifPts);
    
    Mat imgWithoutSinglePts;
    DetectRemoveSinglePoint(imgWithoutBifPts, imgWithoutSinglePts);
    
    vector<Point2f> endPts;
    endPts = DetectEndPoints(imgWithoutSinglePts);
    
    vector<vector<Point2f> > lines;
    lines = TrackLineSegments(imgWithoutSinglePts, endPts);
    
    int lineLengthThreshold = 50;
    RemoveShortLines(lines, lineLengthThreshold);
    
    //DuplicateCheck(lines);
    
    // Get the control point of each spline curve
    int ctrlPtsNum = 7;
    
    ctrlPts = spCtr.GetCtrlPtsFromEachLines(lines, ctrlPtsNum );
    
    // Create Color strands
    Mat curvesImg = DrawCurvesImg(_imgErosion, ctrlPts);
    return curvesImg;
}
//
vector<Point2f> strandsOperator::DetectBifuPoints(const Mat & _imgErosion)
{
    Mat L = mh.nlfilter(_imgErosion);
    Mat LBifu = L.clone();

    cv::compare(LBifu, 3*255, LBifu, CMP_EQ);
    
    Mat LBifuDia = LBifu.clone();
    
    vector<Point2f> bifuPoints;
    Point2f tmpPt;
    for(int i =0; i<LBifuDia.rows; i++)
    {
        for(int j = 0; j< LBifuDia.cols; j++)
        {
            if(LBifuDia.at<uchar>(i,j) == 255)
            {
                int sumX = j,sumY = i;
                int num = 1;
                for(int m =0; m<3; m++)
                {
                    for(int n = 0; n<3; n++)
                    {
                        int r = i-1+m, c = j-1+n;
                        if(LBifuDia.at<uchar>(r,c) == 255)
                        {
                            sumX += c;
                            sumY += r;
                            num++;
                            LBifuDia.at<uchar>(r,c) = 0;
                        }
                    }
                }
                tmpPt.x = sumX/num;
                tmpPt.y = sumY/num;
                bifuPoints.push_back(tmpPt);
                LBifuDia.at<uchar>(i,j) = 255;
            }
        }
    }
    return bifuPoints;
}

// Function removes bifurcation points in the binary image
void strandsOperator::RemoveBifuPoints(const Mat & _imgErosion, vector<Point2f> bifPts, Mat & _imgWithoutBifPts)
{
    _imgWithoutBifPts = _imgErosion.clone();
    _imgWithoutBifPts.convertTo(_imgWithoutBifPts, CV_64FC1);

    int bifPtsNum = (int)bifPts.size();
    Point2f tmpPt;
    for(int i = 0; i < bifPtsNum; i++)
    {
        tmpPt = bifPts.at(i);
        //_imgWithoutBifPts.at<double>(tmpPt.y, tmpPt.x) = 0;
        for(int m =0; m<3; m++)
        {
            for(int n = 0; n<3; n++)
            {
                int r = tmpPt.y-1+m, c = tmpPt.x-1+n;
                _imgWithoutBifPts.at<double>(r, c) = 0;
            }
        }
    }
}

void strandsOperator::DetectRemoveSinglePoint(const Mat & _imgWithoutBifPts, Mat & _imgWithoutSinglePts)
{
    _imgWithoutSinglePts = _imgWithoutBifPts.clone();
    for(int i =0; i<_imgWithoutSinglePts.rows; i++)
    {
        for(int j = 0; j< _imgWithoutSinglePts.cols; j++)
        {
            if(_imgWithoutSinglePts.at<double>(i,j) == 255)
            {
                int num = 0;
                for(int m =0; m<3; m++)
                {
                    for(int n = 0; n<3; n++)
                    {
                        int r = i-1+m, c = j-1+n;
                        if(_imgWithoutSinglePts.at<double>(r,c) == 0)
                        {
                            num++;
                        }
                    }
                }
                if(num == 8)
                    _imgWithoutSinglePts.at<double>(i,j) = 0;
            }
        }
    }
}

// Function detects end points in a binary image with line segments (background is white and lines are black).
// End points are points that have only 1 neigbor points are black in its neighbors.

vector<Point2f> strandsOperator::DetectEndPoints(const Mat & _img)
{
    vector<Point2f> endPoints;
    Point2f tmpPt;
    for(int i =0; i<_img.rows; i++)
    {
        for(int j = 0; j< _img.cols; j++)
        {
            if(_img.at<double>(i,j) == 255)
            {
                int num = 0;
                for(int m =0; m<3; m++)
                {
                    for(int n = 0; n<3; n++)
                    {
                        int r = i-1+m, c = j-1+n;
                        if(_img.at<double>(r,c) == 255)
                        {
                            num++;
                        }
                    }
                }
                if(num == 2)
                {
                    tmpPt.x = j;
                    tmpPt.y = i;
                    endPoints.push_back(tmpPt);
                }
            }
        }
    }
    return endPoints;
}

// Function tracks line segments in a binary hair image. It uses the endPts as the current seed to trace the line one
// pixel by one pixel. Ince each line have two end points, this function automatically remove duplicate lines.
vector<vector<Point2f> > strandsOperator::TrackLineSegments(const Mat & _img, vector<Point2f> endPts)
{
    vector<vector<Point2f> > lineSegments;
    vector<Point2f> tmpLine;
    for(int i = 0; i < (int)endPts.size(); i++)
    {
        LinesTrace(_img, endPts.at(i), tmpLine);
        lineSegments.push_back(tmpLine);
    }

    Point2f startPtM, endPtM, startPtN, endPtN;
    for(int m = 0; m < (int)lineSegments.size(); m++)
    {
        for(int n = 0; n < (int)lineSegments.size(); n++)
        {
            if((n != m) && (!lineSegments.at(n).empty()) && (!lineSegments.at(m).empty()) && (lineSegments.at(n).size() == lineSegments.at(m).size()))
            {
                startPtM = lineSegments.at(m).front();
                endPtM = lineSegments.at(m).back();
                startPtN = lineSegments.at(n).front();
                endPtN = lineSegments.at(n).back();
                if(startPtM == endPtN && endPtM == startPtN)
                {
                    if(startPtM.y > endPtM.y)
                        lineSegments.at(m).clear();
                    else
                        lineSegments.at(n).clear();
                }
            }
        }
    }
    for(int i = 0 ; i <(int)lineSegments.size(); i++)
    {
        if(lineSegments.at(1).empty())
        {
            std::vector<vector<Point2f> >::const_iterator it = lineSegments.begin()+i;
            lineSegments.erase(it);
        }
    }
    return lineSegments;
}

// Line Tracing of a bw image(binary)
void strandsOperator::LinesTrace(const Mat & _img, Point2f endPt, vector<Point2f> & _line)
{
    _line.clear();
    _line.push_back(endPt);
    
    Mat img = _img.clone();

    int loop = 0;
    
    while(true)
    {
        int prev_size = (int) _line.size();
        Point2f tmpPt;
        while(true)
        {
            if((_line.at(loop).x - 1) >= 0 && (_line.at(loop).y - 1) >= 0 )
            {
                if(img.at<double>(_line.at(loop).y - 1, _line.at(loop).x - 1) == 255)
                {
                    tmpPt.y = _line.at(loop).y - 1;
                    tmpPt.x = _line.at(loop).x - 1;
                    _line.push_back(tmpPt);
                    img.at<double>(_line.at(loop).y - 1, _line.at(loop).x - 1) = 0;
                }
            }
            
            if((_line.at(loop).x ) >= 0 && (_line.at(loop).y - 1) >= 0 )
            {
                if(img.at<double>(_line.at(loop).y -1  , _line.at(loop).x) == 255)
                {
                    tmpPt.y = _line.at(loop).y - 1;
                    tmpPt.x = _line.at(loop).x;
                    _line.push_back(tmpPt);
                    img.at<double>(_line.at(loop).y - 1, _line.at(loop).x) = 0;
                }
            }
            
            if((_line.at(loop).x + 1) <= img.cols && (_line.at(loop).y - 1) >= 0 )
            {
                if(img.at<double>(_line.at(loop).y - 1, _line.at(loop).x + 1) == 255)
                {
                    tmpPt.y = _line.at(loop).y - 1;
                    tmpPt.x = _line.at(loop).x + 1;
                    _line.push_back(tmpPt);
                    img.at<double>(_line.at(loop).y - 1, _line.at(loop).x + 1) = 0;
                }
            }
            
            if((_line.at(loop).x - 1) >= 0 && (_line.at(loop).y + 1) <= img.rows )
            {
                if(img.at<double>(_line.at(loop).y + 1, _line.at(loop).x - 1) == 255)
                {
                    tmpPt.y = _line.at(loop).y + 1;
                    tmpPt.x = _line.at(loop).x - 1;
                    _line.push_back(tmpPt);
                    img.at<double>(_line.at(loop).y + 1, _line.at(loop).x - 1) = 0;
                }
            }
            
            if((_line.at(loop).x ) >= 0 && (_line.at(loop).y + 1) <= img.rows )
            {
                if(img.at<double>(_line.at(loop).y + 1, _line.at(loop).x) == 255)
                {
                    tmpPt.y = _line.at(loop).y + 1;
                    tmpPt.x = _line.at(loop).x;
                    _line.push_back(tmpPt);
                    img.at<double>(_line.at(loop).y + 1, _line.at(loop).x) = 0;
                }
            }
            
            if((_line.at(loop).x + 1) <= img.cols && (_line.at(loop).y + 1) <= img.rows )
            {
                if(img.at<double>(_line.at(loop).y + 1, _line.at(loop).x + 1) == 255)
                {
                    tmpPt.y = _line.at(loop).y + 1;
                    tmpPt.x = _line.at(loop).x + 1;
                    _line.push_back(tmpPt);
                    img.at<double>(_line.at(loop).y + 1, _line.at(loop).x + 1) = 0;
                }
            }
            
            if((_line.at(loop).x + 1) <= img.cols && (_line.at(loop).y ) >= 0 )
            {
                if(img.at<double>(_line.at(loop).y, _line.at(loop).x + 1) == 255)
                {
                    tmpPt.y = _line.at(loop).y;
                    tmpPt.x = _line.at(loop).x + 1;
                    _line.push_back(tmpPt);
                    img.at<double>(_line.at(loop).y, _line.at(loop).x + 1) = 0;
                }
            }
            
            if((_line.at(loop).x - 1) >= 0 && (_line.at(loop).y) >= 0 )
            {
                if(img.at<double>(_line.at(loop).y, _line.at(loop).x - 1) == 255)
                {
                    tmpPt.y = _line.at(loop).y;
                    tmpPt.x = _line.at(loop).x - 1;
                    _line.push_back(tmpPt);
                    img.at<double>(_line.at(loop).y, _line.at(loop).x - 1) = 0;
                }
            }
            img.at<double>(_line.at(loop).y, _line.at(loop).x) = 0;
            loop++;
            if(loop >= prev_size)
                break;
        }
        
        if(prev_size == (int)_line.size())
            break;
    }
}

// Function removes short lines in binayry image under the constrain of the lengthThreshold
void strandsOperator::RemoveShortLines(vector< vector<Point2f> > & _lines, int _lineLengthThreshold)
{
    for(int i = 0; i < (int)_lines.size(); i++)
    {
        if((int)_lines.at(i).size() < _lineLengthThreshold)
        {
            std::vector<vector<Point2f> >::const_iterator it = _lines.begin()+i;
            _lines.erase(it);
            i--;
        }
    }
}
/************************************************************************/
void strandsOperator::DuplicateCheck(vector< vector<Point2f> > & _lines)
{
    vector<Point2f> * tmpLine;
    for(int i = 0; i < (int)_lines.size(); i++)
    {
        tmpLine = &_lines.at(i);
        std::sort(tmpLine->begin(), tmpLine->end(), point_comparator);
        vector<Point2f>::iterator pos;
        pos = unique(tmpLine->begin(), tmpLine->end(), point_equal);
        tmpLine->erase(pos, tmpLine->end());
    }
}

static bool point_comparator(const cv::Point2f &a, const cv::Point2f &b) {
    return (a.x < b.x);
}
static bool point_equal(const cv::Point2f &a, const cv::Point2f &b) {
    return (a.x == b.x && a.y == b.y);
}
/************************************************************************/

// Function draw an color image of the spline curves based on the control points
Mat strandsOperator::DrawCurvesImg(const Mat & _img,vector<vector<Point2f> > & _ctrlPts)
{
    // Create Color strands
    Mat curvesImg(_img.rows, _img.cols, CV_64FC3, Scalar(0,0,0));
    
    RNG rng( 0xFFFFFFFF );
    for(int i =0; i< (int)_ctrlPts.size(); i++)
    {
        Scalar color =randomColor(rng);
        for(int j = 0; j<(int)_ctrlPts.at(i).size()-1; j++)
        {
            line( curvesImg, _ctrlPts.at(i).at(j), _ctrlPts.at(i).at(j+1), color,  2, 8 );
        }
    };
    return curvesImg;
}

// Function removes the duplicate pixel of a line
vector<vector<Point2f> > strandsOperator::ExtendAndConnectSplines(double ExtensionParameter, int connectionThreshold, int newCtrlPtsNum, int maximumIterlation, int connectionThresholdIncrease)
{
    vector<vector<Point2f> > extendedCtrlPts;
    vector<vector<Point2f> > newCtrlPts = ctrlPts;
    int length = 0;
    Mat connectImg;
    while(maximumIterlation != 0 && length != (int)newCtrlPts.size())
    {
        length = (int)newCtrlPts.size();
        spCtr.SplinesExtension(newCtrlPts, ExtensionParameter, extendedCtrlPts);
        connectImg = DrawCurvesImg(input_img, extendedCtrlPts);
        
        spCtr.ConnectSpline2D(extendedCtrlPts, connectionThreshold, newCtrlPtsNum, newCtrlPts);
        
        maximumIterlation--;
        
        connectionThreshold = connectionThreshold + connectionThresholdIncrease;
        
        connectImg = DrawCurvesImg(input_img, newCtrlPts);
    }
    curCurves = newCtrlPts;
    return newCtrlPts;
}

/*Median Filter*/
Mat strandsOperator::MedianFilter(int windowLength)
{
    vector<vector<Point2f> > filteredCurves;
    spCtr.MedianFilter(curCurves, windowLength, filteredCurves);
    curCurves = filteredCurves;
    return DrawCurvesImg(input_img, filteredCurves);
}

bool strandsOperator::ExportCtrlPts(char * filepath, CvRect crop_rect)
{
    return spCtr.ExportCtrlPts(curCurves, filepath, crop_rect);
}
