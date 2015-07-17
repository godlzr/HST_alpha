/* CVImage class
 *
 * Author: Zhongrui Li
 * Supervisor: Wonsook Lee
 * zli109@uottawa.ca, wslee@uottawa.ca
 * EECS, Faculty of Engineering, University of Ottawa, Canada
 *
 * Date: March 20th, 2015
 *
 */
/*
 *Rectangle crop the image
 */
#ifndef CROPRECT_H
#define CROPRECT_H
#include <QPoint>
#include <QPainter>

class CropRect
{
public:
  CropRect(){}
  ~CropRect(){}
  void setStart(QPoint s)
  {
    start = s;
  }

  void setEnd(QPoint e)
  {
    end = e;
  }

  QPoint startPoint() const
  {
    return start;
  }

  QPoint endPoint() const
  {
    return end;
  }
  void reset()
  {
    QPoint P(0,0);
    start = P;
    end = P;
  }

  QSize size() const
  {
    return QSize(width(), height());
  }

  int height() const
  {
    return qAbs(startPoint().y() - endPoint().y());
  }

  int width() const
  {
    return qAbs(startPoint().x() - endPoint().x());
  }

private:
  QPoint start;
  QPoint end;
};

#endif // CROPRECT_H
