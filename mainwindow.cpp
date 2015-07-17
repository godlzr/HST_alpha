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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <opencv/cv.h>
#include <QTextCodec>
#include <croprect.h>
#include <QDebug>
#include <imageviewer.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    is_start_cropping = false;
    is_HSV = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

/********************************Open file operation***********************/
void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Open Image"),"",tr("Image File(*.bmp *.jpg *.jpeg *.png)"));
    if(filename==NULL)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("No image selected!"));
        msgBox.exec();
    }
    else{
        QTextCodec *code = QTextCodec::codecForName("utf8");
        std::string name = code->fromUnicode(filename).data();
        
        //read image using cv::imread()
        cv_img_controller.oringinal_img_RGB = cv::imread(name);
        if(!cv_img_controller.oringinal_img_RGB.data)
        {
            QMessageBox msgBox;
            msgBox.setText(tr("Cannot open the image!"));
            msgBox.exec();
        }
        else
        {
            cv::cvtColor(cv_img_controller.oringinal_img_RGB,cv_img_controller.oringinal_img,CV_BGR2RGB);
            
            q_displayed_img = QImage((const unsigned char*)(cv_img_controller.oringinal_img.data),cv_img_controller.oringinal_img.cols,cv_img_controller.oringinal_img.rows, cv_img_controller.oringinal_img.cols*cv_img_controller.oringinal_img.channels(),  QImage::Format_RGB888);
            
            ui->scrollArea->setWidget(&img_view_controller);
            q_img = q_displayed_img.scaled(img_view_controller.width(),img_view_controller.height(),Qt::KeepAspectRatio,Qt::FastTransformation);
            
            img_view_controller.setPixmap(QPixmap::fromImage(q_img));
            img_view_controller.resize(QSize(q_img.width(), q_img.height()));
            
            double window_width;
            window_width= ui->groupBox->width() + q_img.width();
            double window_height;
            window_height= ui->groupBox->height();
            ui->centralWidget->resize(window_width, window_height);
            this->resize(window_width, window_height);
            
            img_view_controller.scale_height = (double)q_img.height() / (double)q_displayed_img.height();
            img_view_controller.scale_width = (double)q_img.width() / (double)q_displayed_img.width();


        }
    }
}
// (Open Image)button
void MainWindow::on_openImageButton_clicked()
{
    on_actionOpen_triggered();
}
// (directly open Image)button  for testing.
void MainWindow::on_pushButton_3_clicked()
{
    QString filename = "/Users/GODLZR/Pictures/hair.jpg";
    if(filename==NULL)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("No image selected!"));
        msgBox.exec();
    }
    else{
        QTextCodec *code = QTextCodec::codecForName("utf8");
        std::string name = code->fromUnicode(filename).data();
        cv_img_controller.oringinal_img = cv::imread(name);
        if(!cv_img_controller.oringinal_img.data)
        {
            QMessageBox msgBox;
            msgBox.setText(tr("Cannot open the image!"));
            msgBox.exec();
        }
        else
        {
            cv::cvtColor(cv_img_controller.oringinal_img,cv_img_controller.oringinal_img,CV_BGR2RGB);
            //printf("%d,%d\n",ui->scrollArea->height(),ui->scrollArea->width());
            q_displayed_img = QImage((const unsigned char*)(cv_img_controller.oringinal_img.data),cv_img_controller.oringinal_img.cols,cv_img_controller.oringinal_img.rows, cv_img_controller.oringinal_img.cols*cv_img_controller.oringinal_img.channels(),  QImage::Format_RGB888);
            ui->scrollArea->setWidget(&img_view_controller);
            q_img = q_displayed_img.scaled(img_view_controller.width(),img_view_controller.height(),Qt::KeepAspectRatio,Qt::FastTransformation);
            img_view_controller.setPixmap(QPixmap::fromImage(q_img));
            img_view_controller.resize(QSize(q_img.width(), q_img.height()));
            
            double window_width;
            window_width= ui->groupBox->width() + q_img.width();
            double window_height;
            window_height= ui->groupBox->height();
            ui->centralWidget->resize(window_width, window_height);
            this->resize(window_width, window_height);
            img_view_controller.scale_height = (double)q_img.height() / (double)q_displayed_img.height();
            img_view_controller.scale_width = (double)q_img.width() / (double)q_displayed_img.width();
            
        }
    }
}

/********************************exit and about operation***********************/
void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this,tr("About"),tr("   HairStrandTracking V0.1\n\n Author: Zhongrui Li\n Algorithm Design: Chao Sun\n Supervisor: Dr.Won-Sook Lee \n University of Ottawa\n\n March 2015\n"));
}
/*******************************paint function***********************/
void MainWindow::paintEvent(QPaintEvent *)
{
   if(!q_displayed_img.isNull())
    {
        QImage q_tmp_img;
        q_tmp_img = q_displayed_img.scaled(ui->scrollArea->width(),ui->scrollArea->height(),Qt::KeepAspectRatio,Qt::FastTransformation);
        //ui->scrollArea->resize(q_tmp_img.width(),q_tmp_img.height());
        
        ui->scrollArea->setWidget(&img_view_controller);
        img_view_controller.setPixmap(QPixmap::fromImage(q_tmp_img));
        img_view_controller.resize(QSize(q_tmp_img.width(),q_tmp_img.height()));
        
        img_view_controller.regual_width = ui->scrollArea->width();
        img_view_controller.regual_height = ui->scrollArea->height();
        img_view_controller.scaled_img_width = q_tmp_img.width();
        img_view_controller.scaled_img_height = q_tmp_img.height();
        
        img_view_controller.scale_height = (double)q_tmp_img.height()/(double)q_displayed_img.height();
        img_view_controller.scale_width = (double)q_tmp_img.width()/(double)q_displayed_img.width();

    }
}

/*****************************Image cropping operation***********************/
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(!img_view_controller.is_cropping)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Please select the hair region first!"));
        msgBox.exec();
    }
    else
    {
        if((event->key()==Qt::Key_Enter)||(event->key()==Qt::Key_Return))
        {
            cv_img_controller.crop_rect = cvRect(img_view_controller.Rect.x(),img_view_controller.Rect.y(),img_view_controller.Rect.width(),img_view_controller.Rect.height());
            
            cv_img_controller.cropped_img = cv_img_controller.CroppingImage(cv_img_controller.oringinal_img,cv_img_controller.crop_rect);
            
            QImage cropped = QImage((const unsigned char*)(cv_img_controller.cropped_img.data),
                                                            cv_img_controller.cropped_img.cols,
                                                            cv_img_controller.cropped_img.rows,
                                                            cv_img_controller.cropped_img.step,
                                                            QImage::Format_RGB888);
            q_displayed_img = cropped;
            img_view_controller.cropRect.reset();
            img_view_controller.is_cropping = false;
            this->update();
        }

    }
}

void MainWindow::on_CropButton_clicked()
{
    if(q_displayed_img.isNull())
    {
        QMessageBox::about(this,tr("Warning!"),tr("Please load a image first !"));
    }
    else
        img_view_controller.is_cropping = true;
    
}
/********************************save image operation***********************/
void MainWindow::on_actionSave_triggered()
{
    if(cv_img_controller.oringinal_img.empty())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("No image to save!"));
        msgBox.exec();
    }
    else
    {
        QString savefilename = QFileDialog::getSaveFileName(this,tr("Save Image"),"",tr("Image File(*.bmp *.jpg *.jpeg *.png)"));
        QTextCodec *code = QTextCodec::codecForName("utf8");
        std::string name = code->fromUnicode(savefilename).data();
        cv::Mat saveImage;
        cv::cvtColor(cv_img_controller.cropped_img, saveImage, CV_BGR2RGB);
        cv::imwrite(name,saveImage);

    }

}
/***************************convert RGB channel to HSV channel operation***********************/
void MainWindow::on_pushButton_clicked()
{
    if (cv_img_controller.oringinal_img.empty())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Please load hair image first!"));
        msgBox.exec();
    }
    else
    {
        if (cv_img_controller.cropped_img.empty())
        {
            QMessageBox msgBox;
            msgBox.setText(tr("Please select the hair region first!"));
            msgBox.exec();
        }
        else
        {
            Mat tmp_cropped_img = cv_img_controller.cropped_img.clone();
            cv_HSV_img = cv_img_controller.ConvertRgbToHsv(tmp_cropped_img);
            
            QImage qHsvImg = img_view_controller.ConvertMatToQImage(cv_HSV_img);
            
            q_displayed_img = qHsvImg;
            is_HSV = true;
            this->update();
        }

    }
}
/*****************************gabor filtering operation***********************/
void MainWindow::on_pushButton_2_clicked()
{
    if(is_HSV)
    {
        cv_gabor_img = cv_img_controller.GaborFilter(cv_HSV_img);
        QImage qGaborImg = img_view_controller.ConvertMatToQImage(cv_gabor_img);
        q_displayed_img = qGaborImg;
        this->update();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText(tr("You can't use gabor filter at this time!"));
        msgBox.exec();
    }

}

/*****************************enhanced gabor filter operation***********************/
void MainWindow::on_enhancedGaborFilterButton_clicked()
{
    if(cv_gabor_img.empty())
        QMessageBox::about(this,tr("Warning!"),tr("You can't use enhanced gabor filter at this time!"));
    else
    {
        cv_enhanced_gabor_img = cv_img_controller.EnhancedGaborFilter();
        QImage qEnhancedGaborImg = img_view_controller.ConvertMatToQImage(cv_enhanced_gabor_img);
        q_displayed_img = qEnhancedGaborImg;
        this->update();
    }
}

void MainWindow::on_imageErosionButton_clicked()
{
    if(cv_enhanced_gabor_img.empty())
        QMessageBox::about(this,tr("Warning!"),tr("You can't erode the image at this time!"));
    else
    {
        cv_eroded_img = cv_img_controller.ErodeImage();
        QImage qErodedImg = img_view_controller.ConvertMatToQImage(cv_eroded_img);
        q_displayed_img = qErodedImg;
        this->update();
    }
}

void MainWindow::on_strandsAnalysisButton_clicked()
{
    if(cv_eroded_img.empty())
        QMessageBox::about(this,tr("Warning!"),tr("Can't analysis strands at this time!"));
    else
    {
        cv_blend_curves_img = cv_img_controller.StrandsAnalysis();
        QImage qBlendCurvesImg = img_view_controller.ConvertMatToQImage(cv_blend_curves_img);
        q_displayed_img = qBlendCurvesImg;
        this->update();
    }
    

}

// Strand Extend Button
void MainWindow::on_pushButton_4_clicked()
{
    if(cv_blend_curves_img.empty())
        QMessageBox::about(this,tr("Warning!"),tr("Can't connect strands at this time!"));
    else
    {
        cv_connect_curves_img = cv_img_controller.StrandsEnxtendAndConnection();
        QImage qConnectCurvesImg = img_view_controller.ConvertMatToQImage(cv_connect_curves_img);
        q_displayed_img = qConnectCurvesImg;
        this->update();
    }
    
}
// Median Filter Button
void MainWindow::on_pushButton_5_clicked()
{
    if(cv_connect_curves_img.empty())
        QMessageBox::about(this,tr("Warning!"),tr("Can't use median filter at this time!"));
    else
    {
        cv_filter_curves_img = cv_img_controller.MedianFilter();
        QImage qFilteredCurvesImg = img_view_controller.ConvertMatToQImage(cv_filter_curves_img);
        q_displayed_img = qFilteredCurvesImg;
        this->update();
    }
}

void MainWindow::on_Export_CtrlPts_Button_clicked()
{
    if(cv_filter_curves_img.empty())
        QMessageBox::about(this,tr("Warning!"),tr("No data to save right now!"));
    else
    {
        QString filename = QFileDialog::getSaveFileName(this,tr("Save Data"),"../Strand_ControlPoint_Data.txt",tr("Image File(*.txt)"));
        char* filepath = new char[filename.length() + 1];
        strcpy(filepath, filename.toLatin1().constData());
        if(cv_img_controller.ExportControlPoints(filepath))
            QMessageBox::about(this,tr("Great!"),tr("Save Data Successfully!"));
        else
            QMessageBox::about(this,tr("Fail!"),tr("Can not save the data!"));
    }
}
