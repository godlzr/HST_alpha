#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "cv.h"
#include "highgui.h"
#include <QMainWindow>
#include <QLabel>
#include <QPoint>
#include <QMouseEvent>
#include <croprect.h>
#include <imageviewer.h>
#include <cvimage.h>
namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    QImage q_displayed_img;// Original Image
    
    CVImage cv_img_controller;// class used to process cv::mat(opencv image)
    
    QImage q_img;// QImage used to process Qt::QImage (Qt image)
    
    cv::Mat cv_HSV_img;
    
    cv::Mat cv_gabor_img;
    
    cv::Mat cv_enhanced_gabor_img;
    
    cv::Mat cv_eroded_img;

    bool is_start_cropping;//flag for start cropping image

    bool is_HSV;//flag for finish RGB to HSV convertion
    
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

    void paintEvent(QPaintEvent *);


private slots:

    void on_actionOpen_triggered();

    void on_actionExit_triggered();

    void on_actionAbout_triggered();

    void keyPressEvent(QKeyEvent *event);

    void on_actionSave_triggered();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_CropButton_clicked();

    void on_enhancedGaborFilterButton_clicked();

    void on_openImageButton_clicked();

    void on_pushButton_3_clicked();

    void on_imageErosionButton_clicked();

    void on_strandsAnalysisButton_clicked();

private:
    
    Ui::MainWindow *ui;
    
    ImageViewer img_view_controller;


};

#endif // MAINWINDOW_H
