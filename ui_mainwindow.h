/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionExit;
    QAction *actionAbout;
    QAction *actionZoom_IN;
    QAction *actionZoom_Out;
    QAction *actionCrop_Image;
    QAction *actionSave;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_5;
    QLabel *label;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QFrame *Opreations;
    QVBoxLayout *verticalLayout_2;
    QPushButton *openImageButton;
    QFrame *frame;
    QVBoxLayout *verticalLayout_7;
    QPushButton *CropButton;
    QLabel *label_2;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_6;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *enhancedGaborFilterButton;
    QPushButton *imageErosionButton;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_8;
    QPushButton *strandsAnalysisButton;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *Export_CtrlPts_Button;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuEdit;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(562, 557);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionZoom_IN = new QAction(MainWindow);
        actionZoom_IN->setObjectName(QStringLiteral("actionZoom_IN"));
        actionZoom_Out = new QAction(MainWindow);
        actionZoom_Out->setObjectName(QStringLiteral("actionZoom_Out"));
        actionCrop_Image = new QAction(MainWindow);
        actionCrop_Image->setObjectName(QStringLiteral("actionCrop_Image"));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        centralWidget->setMaximumSize(QSize(16777215, 16777215));
        verticalLayout_4 = new QVBoxLayout(centralWidget);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        scrollArea = new QScrollArea(centralWidget);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 293, 473));
        sizePolicy.setHeightForWidth(scrollAreaWidgetContents->sizePolicy().hasHeightForWidth());
        scrollAreaWidgetContents->setSizePolicy(sizePolicy);
        verticalLayout_5 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        label = new QLabel(scrollAreaWidgetContents);
        label->setObjectName(QStringLiteral("label"));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        verticalLayout_5->addWidget(label);

        scrollArea->setWidget(scrollAreaWidgetContents);

        horizontalLayout->addWidget(scrollArea);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        Opreations = new QFrame(groupBox);
        Opreations->setObjectName(QStringLiteral("Opreations"));
        verticalLayout_2 = new QVBoxLayout(Opreations);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        openImageButton = new QPushButton(Opreations);
        openImageButton->setObjectName(QStringLiteral("openImageButton"));

        verticalLayout_2->addWidget(openImageButton);

        frame = new QFrame(Opreations);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setMaximumSize(QSize(16777215, 60));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_7 = new QVBoxLayout(frame);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        CropButton = new QPushButton(frame);
        CropButton->setObjectName(QStringLiteral("CropButton"));
        CropButton->setMinimumSize(QSize(0, 32));

        verticalLayout_7->addWidget(CropButton);

        label_2 = new QLabel(frame);
        label_2->setObjectName(QStringLiteral("label_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);
        label_2->setMaximumSize(QSize(200, 30));

        verticalLayout_7->addWidget(label_2);


        verticalLayout_2->addWidget(frame);


        verticalLayout->addWidget(Opreations);

        groupBox_2 = new QGroupBox(groupBox);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout_6 = new QVBoxLayout(groupBox_2);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        pushButton = new QPushButton(groupBox_2);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        verticalLayout_6->addWidget(pushButton);

        pushButton_2 = new QPushButton(groupBox_2);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        verticalLayout_6->addWidget(pushButton_2);

        enhancedGaborFilterButton = new QPushButton(groupBox_2);
        enhancedGaborFilterButton->setObjectName(QStringLiteral("enhancedGaborFilterButton"));

        verticalLayout_6->addWidget(enhancedGaborFilterButton);

        imageErosionButton = new QPushButton(groupBox_2);
        imageErosionButton->setObjectName(QStringLiteral("imageErosionButton"));

        verticalLayout_6->addWidget(imageErosionButton);


        verticalLayout->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(groupBox);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        verticalLayout_8 = new QVBoxLayout(groupBox_3);
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setContentsMargins(11, 11, 11, 11);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        strandsAnalysisButton = new QPushButton(groupBox_3);
        strandsAnalysisButton->setObjectName(QStringLiteral("strandsAnalysisButton"));

        verticalLayout_8->addWidget(strandsAnalysisButton);

        pushButton_4 = new QPushButton(groupBox_3);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        verticalLayout_8->addWidget(pushButton_4);

        pushButton_5 = new QPushButton(groupBox_3);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));

        verticalLayout_8->addWidget(pushButton_5);


        verticalLayout->addWidget(groupBox_3);

        Export_CtrlPts_Button = new QPushButton(groupBox);
        Export_CtrlPts_Button->setObjectName(QStringLiteral("Export_CtrlPts_Button"));

        verticalLayout->addWidget(Export_CtrlPts_Button);


        verticalLayout_3->addWidget(groupBox);


        horizontalLayout->addLayout(verticalLayout_3);


        verticalLayout_4->addLayout(horizontalLayout);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 562, 22));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionExit);
        menuHelp->addAction(actionAbout);
        menuEdit->addAction(actionCrop_Image);
        menuEdit->addAction(actionZoom_IN);
        menuEdit->addAction(actionZoom_Out);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Hair Strand Tracking", 0));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", 0));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0));
        actionAbout->setText(QApplication::translate("MainWindow", "about", 0));
        actionZoom_IN->setText(QApplication::translate("MainWindow", "Zoom In", 0));
        actionZoom_Out->setText(QApplication::translate("MainWindow", "Zoom Out", 0));
        actionCrop_Image->setText(QApplication::translate("MainWindow", "Crop Image", 0));
        actionSave->setText(QApplication::translate("MainWindow", "Save", 0));
        label->setText(QString());
        groupBox->setTitle(QApplication::translate("MainWindow", "Operations", 0));
        openImageButton->setText(QApplication::translate("MainWindow", "Open Image File", 0));
        CropButton->setText(QApplication::translate("MainWindow", "Select Hair Area", 0));
        label_2->setText(QApplication::translate("MainWindow", "Press \"Enter\" to confirm", 0));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Image Processing", 0));
        pushButton->setText(QApplication::translate("MainWindow", "Convert to HSV", 0));
        pushButton_2->setText(QApplication::translate("MainWindow", "Gabor Filter", 0));
        enhancedGaborFilterButton->setText(QApplication::translate("MainWindow", "Enhanced Gabor", 0));
        imageErosionButton->setText(QApplication::translate("MainWindow", "Image Erosion", 0));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Pixel Processing", 0));
        strandsAnalysisButton->setText(QApplication::translate("MainWindow", "Strands Anaylsis", 0));
        pushButton_4->setText(QApplication::translate("MainWindow", "Connect Strands", 0));
        pushButton_5->setText(QApplication::translate("MainWindow", "Median Filter", 0));
        Export_CtrlPts_Button->setText(QApplication::translate("MainWindow", "Export CtrlPts Postion", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
