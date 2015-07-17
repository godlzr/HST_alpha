#include "imageviewer.h"
#include <QDebug>

ImageViewer::ImageViewer(QWidget *parent) :
    QLabel(parent)
{
    is_cropping = false;
    is_start_cropping = false;
}

bool ImageViewer::isContainPoint(QPoint _p)
{
    int heightFloat = (regual_height-scaled_img_height)/2;
    
    if( ( _p.x() >= 0 && _p.x() <= scaled_img_width ) && ( _p.y() >= heightFloat && _p.y() <=(regual_height-heightFloat) ) )
        return true;
    else
        return false;
}

QPoint ImageViewer::MapScreenPointToQImage(QPoint _screenPoint)
{
     QPoint mappedPoint;
    if(isContainPoint(_screenPoint))
    {
        mappedPoint.setX(_screenPoint.x());
        mappedPoint.setY(_screenPoint.y()-(regual_height-scaled_img_height)/2);
    }
    else
    {
        mappedPoint.setX(-1);
        mappedPoint.setY(-1);
    }
    return mappedPoint;
}
/*******************Image format convertion(from cv::Mat to qt::QImage)***********************/
QImage ImageViewer::ConvertMatToQImage(cv::Mat &inMat)
{
    QImage img;
    switch ( inMat.type() )
    {
            // 8-bit, 4 channel
        case CV_8UC4:
        {
            QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB32 );
            
            return image;
        }
            
            // 8-bit, 3 channel
        case CV_8UC3:
        {
            QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB888 );
            
            return image.rgbSwapped();
        }
            
            // 8-bit, 1 channel
        case CV_8UC1:
        {
            static QVector<QRgb>  sColorTable;
            
            // only create our color table once
            if ( sColorTable.isEmpty() )
            {
                for ( int i = 0; i < 256; ++i )
                    sColorTable.push_back( qRgb( i, i, i ) );
            }
            
            QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_Indexed8 );
            
            image.setColorTable( sColorTable );
            
            return image;
        }
            
        default:
            qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
            break;
    }
    return img;
}

void ImageViewer::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    QPainter painter(this);
    if (is_cropping)
    {
        painter.setPen(Qt::darkRed);
        QPen pen;
        pen.setBrush(Qt::red);
        pen.setStyle(Qt::DashLine);
        pen.setWidth(1);
        painter.setPen(pen);

        if (cropRect.startPoint().x() < cropRect.endPoint().x())
        {

            if (cropRect.startPoint().y() < cropRect.endPoint().y())
            {
                //start point in the top to the end point.
                painter.drawRect(cropRect.startPoint().x(), cropRect.startPoint().y()+((regual_height-scaled_img_height)/2) , cropRect.width() , cropRect.height() );
            }
            else{
                //start point in the bottom to the end point.
                painter.drawRect(cropRect.startPoint().x() , cropRect.endPoint().y()+((regual_height-scaled_img_height)/2) , cropRect.width() , cropRect.height() );
            }
        }
        else
        {
            if (cropRect.startPoint().y() > cropRect.endPoint().y())
            {
                painter.drawRect( cropRect.endPoint().x() , cropRect.endPoint().y()+((regual_height-scaled_img_height)/2) , cropRect.width() , cropRect.height() );
            }
            else{
                painter.drawRect(cropRect.endPoint().x() ,  cropRect.startPoint().y()+((regual_height-scaled_img_height)/2) , cropRect.width() , cropRect.height() );
            }
        }
    }
}

void ImageViewer::mousePressEvent(QMouseEvent *event)
{
    if ((event->buttons() == Qt::LeftButton) &&isContainPoint(event->pos())&& is_cropping)
    {
            cropRect.setStart(MapScreenPointToQImage(event->pos()));
            cropRect.setEnd(MapScreenPointToQImage(event->pos()));
            is_start_cropping = true;
    }
}

void ImageViewer::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() == Qt::LeftButton) &&isContainPoint(event->pos())&& is_start_cropping)
        {
            cropRect.setEnd(MapScreenPointToQImage(event->pos()));
            update();
        }
    //qDebug()<<cropRect.startPoint()<<cropRect.endPoint();
}

void ImageViewer::mouseReleaseEvent(QMouseEvent *event)
{

    QPoint startPoint, endPoint;
    startPoint.setX(cropRect.startPoint().x()/scale_width);
    startPoint.setY(cropRect.startPoint().y()/scale_height);
    endPoint.setX(cropRect.endPoint().x()/scale_width);
    endPoint.setY(cropRect.endPoint().y()/scale_height);

    Rect = QRect(startPoint,endPoint);
    is_start_cropping = false;
}






