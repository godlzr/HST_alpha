/* ImageViewer class
 *
 * Author: Zhongrui Li
 * Supervisor: Wonsook Lee
 * zli109@uottawa.ca, wslee@uottawa.ca
 * EECS, Faculty of Engineering, University of Ottawa, Canada
 *
 * Date: March 20th, 2015
 *
 * Description:
 * This class is used to control the image representation under Qt framework
 *
 */
#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H
#include "cv.h"
#include "highgui.h"
#include <QLabel>
#include <croprect.h>
#include <QMouseEvent>
class ImageViewer : public QLabel
{
    Q_OBJECT
public:

    double scale_width, scale_height;

    CropRect cropRect;

    bool is_cropping;

    bool is_start_cropping;

    QRect Rect;

    double regual_width, regual_height, scaled_img_width, scaled_img_height;

    explicit ImageViewer(QWidget *parent = 0);

    bool isContainPoint(QPoint p);

    QPoint MapScreenPointToQImage(QPoint screenPoint);
    
    QImage ConvertMatToQImage(cv::Mat &inMat);

signals:
    
public slots:
    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

};

#endif // IMAGEVIEWER_H
