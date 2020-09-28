#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QPixmap>
#include <QLabel>
#include <QString>
#include <QDebug>
#include <QImage>
#include <QRgb>
#include <QColor>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::displayImage(QImage &img, QString title){
    QLabel *label = new QLabel();
    label->setPixmap(QPixmap::fromImage(img));
    label->setWindowTitle(title);
    label->show();
}

void MainWindow::on_btn_browse_clicked(){
    // open file and assign file name to fileName variable
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", "../../images",
                                                    "*.* All Files;;.bmp;;.jpeg;;.ppm;;.png;;.jpg");
    // if file name has no character
    if(fileName.isEmpty()){
        qDebug() << "No file choosen!" << endl;
        return; // exit
    }

    // assign file name to line edit bar ln_fileName (QLineEdit)
    ui->ln_fileName->setText(fileName);

    //QPixmap pixmap(fileName); // read image
    //QLabel *label = new QLabel();
    //label->setPixmap(pixmap); // display image
    //label->setWindowTitle(QFileInfo(fileName).fileName());
    //label->show();
}

/* Histogram slide method => increasing or decreasing BRIGHTNESS of a gray image */
void MainWindow::on_btn_hisSlide_gray_clicked(){
    QString fileName = ui->ln_fileName->text(); // get file name from line edit bar ln_fileName (QLineEdit)
    qDebug() << "File name: " << fileName; // test

    int c = ui->ln_c->text().toInt(); // get the amount from line edit bar ln_c (QLineEdit) and convert it from string to number
    qDebug() << "Constant: " << c; // test

    QImage imageIn(fileName); // read image from file name
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);

    // travesing all pixels of imageIn
    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height(); y++){
            QRgb colorIn = imageIn.pixel(x, y); // get pixel value at (x,y)
            int grayIn = qGray(colorIn); // convert color value at (x,y) to gray value
            int grayOut = grayIn + c; // change gray value aming to increasing or decreasing the brightness of the imageIn

            // check threshold
            if(grayOut > 255)
                grayOut = 255;
            else
                if(grayOut < 0)
                   grayOut = 0;

            QColor colorOut = qRgb(grayOut, grayOut, grayOut);
            imageOut.setPixel(x, y, colorOut.rgb()); // set pixel (x,y) with color (colorOut) for imageOut
        }
    }
    // display imageIn and imageOut
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(imageOut, QFileInfo(fileName).fileName()+"_his_slide");
}

/* Histogram slide method => increasing or decreasing BRIGHTNESS of a color image */
void MainWindow::on_btn_hisSlide_Color_clicked(){
    QString fileName = ui->ln_fileName->text(); // get file name from line edit bar ln_fileName (QLineEdit)
    qDebug() << "File name: " << fileName; // test

    int c = ui->ln_c->text().toInt(); // get the amount from line edit bar ln_c (QLineEdit) and convert it from string to number
    qDebug() << "Constant: " << c; // test

    QImage imageIn(fileName); // read image from file name
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);

    // travesing all pixels of imageIn
    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height(); y++){
            QRgb rgb = imageIn.pixel(x, y); // get pixel value at (x,y)
            QColor colorIn(rgb);
            int h, s, v;

            colorIn.getHsv(&h, &s, &v); // get h, s, v from colorIn
            int vOut = v + c;

            // check threshold
            if(vOut > 255)
                vOut = 255;
            else
                if(vOut < 0)
                   vOut = 0;

            QColor colorOut = QColor::fromHsv(h, s, vOut);
            imageOut.setPixel(x, y, colorOut.rgb()); // set pixel (x,y) with color (colorOut) for imageOut
        }
    }
    // display imageIn and imageOut
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(imageOut, QFileInfo(fileName).fileName()+"_his_slide");
}

/* Histogram stretch method => increasing or decreasing CONTRAST of a gray image */
void MainWindow::on_btn_hisStretch_gray_clicked(){
    QString fileName = ui->ln_fileName->text(); // get file name from ln_fileName (QLineEdit)
    qDebug() << "File name: " << fileName; // test

    float c = ui->ln_c->text().toFloat(); // get the amount from line edit bar ln_c (QLineEdit) and convert it from string to float number
    qDebug() << "Constant: " << c; // test

    QImage imageIn(fileName); // read image from file name
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);

    // travesing all pixels of imageIn
    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height(); y++){
            QRgb colorIn = imageIn.pixel(x, y); // get pixel value at (x,y)
            int grayIn = qGray(colorIn); // convert color value at (x,y) to gray value
//            qDebug() << grayIn;
            int grayOut = (int)(grayIn * c); // change gray value to increasing or decreasing the contrast of imageIn

            // check threshold
            if(grayOut > 255)
                grayOut = 255;
            else
                if(grayOut < 0)
                    grayOut = 0;

            QColor colorOut = qRgb(grayOut, grayOut, grayOut);
            imageOut.setPixel(x, y, colorOut.rgb()); // set pixel (x,y) with color (colorOut) for imageOut
        }
    }
    // display imageIn and imageOut
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(imageOut, QFileInfo(fileName).fileName()+"_his_stretch");
}

/* Histogram stretch method => increasing or decreasing CONTRAST of a color image */
void MainWindow::on_btn_hisStretch_Color_clicked(){
    QString fileName = ui->ln_fileName->text();// get file name from line edit bar ln_fileName (QLineEdit)
    qDebug() << "File name: " << fileName; // test

    float c = ui->ln_c->text().toFloat(); // get the amount from line edit bar ln_c (QLineEdit) and convert it from string to float number
    qDebug() << "Constant: " << c; // test

    QImage imageIn(fileName); // read image from file name
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);

    // travesing all pixels of imageIn
    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height(); y++){
            QRgb rgb = imageIn.pixel(x, y); // get pixel value at (x,y)
            QColor colorIn(rgb);

            int h, s, v;
            colorIn.getHsv(&h, &s, &v); // get h, s, v from colorIn

            int vOut = (int)(v * c); // change value to increasing or decreasing the contrast of imageIn

            // check threshold
            if(vOut > 255)
                vOut = 255;
            else
                if(vOut < 0)
                    vOut = 0;

            QColor colorOut = QColor::fromHsv(h, s, vOut);
            imageOut.setPixel(x, y, colorOut.rgb()); // set pixel (x,y) with color (colorOut) for imageOut
        }
    }
    // display imageIn and imageOut
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(imageOut, QFileInfo(fileName).fileName()+"_his_stretch");
}

/* Segmentation thresholding */
void MainWindow::on_btn_segmentation_gray_clicked(){
    QString fileName = ui->ln_fileName->text(); // get file name from line edit bar ln_fileName (QLineEdit)
    qDebug() << "File name: " << fileName; // test

    int thresholding = ui->ln_c->text().toInt(); // get the amount from line edit bar ln_c (QLineEdit) and convert it from string to number
    qDebug() << "Constant: " << thresholding; // test

    QImage imageIn(fileName); // read image from file name
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);

    // travesing all pixels of imageIn
    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height(); y++){
            QRgb colorIn = imageIn.pixel(x, y); // get pixel value at (x,y)
            int gray = qGray(colorIn); // convert color value at (x,y) to gray value

            // check threshold
            if(gray >= thresholding)
                gray = 255;
            else
                gray = 0;

            QColor colorOut = qRgb(gray, gray, gray);
            imageOut.setPixel(x, y, colorOut.rgb()); // set pixel (x,y) with color (colorOut) for imageOut
        }
    }
    // display imageIn and imageOut
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(imageOut, QFileInfo(fileName).fileName()+"_segmentation");
}
