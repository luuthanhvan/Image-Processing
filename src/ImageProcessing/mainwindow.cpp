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
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", "../images",
                                                    "*.* All Files;;.bmp;;.jpeg;;.ppm;;.png;;.jpg");
    // if file name has no character
    if(fileName.isEmpty()){
        qDebug() << "File name has no character" << endl;
        return; // exit
    }
    // assign file name to ln_fileName (QLineEdit)
    ui->ln_fileName->setText(fileName);

    //QPixmap pixmap(fileName); // read image
    //QLabel *label = new QLabel();
    //label->setPixmap(pixmap); // display image
    //label->setWindowTitle(QFileInfo(fileName).fileName());
    //label->show();
}

/* Histogram slide method => increasing or decreasing BRIGHTNESS of a gray image */
void MainWindow::on_btn_hisSlide_gray_clicked(){
    QString fileName = ui->ln_fileName->text(); // get file name from ln_fileName (QLineEdit)
    qDebug() << "File name: " << fileName; // test

    QImage imageIn(fileName); // read image from file name
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);

    int c = ui->ln_c->text().toInt(); // convert string to number

    // travesing all pixels of imageIn
    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height(); y++){
            QRgb colorIn = imageIn.pixel(x, y); // get pixel value at (x,y)
            int grayIn = qGray(colorIn); // convert color value at (x,y) to gray value
            int grayOut = grayIn + c; // change gray value aming to increasing or decreasing brightness

            // check threshold
            if(grayOut > 255)
                grayOut = 255;
            else
                if(grayOut < 0)
                   grayOut = 0;

            QRgb colorOut = qRgb(grayOut, grayOut, grayOut); // color out for image out
            imageOut.setPixel(x, y, colorOut);
        }
    }
    // display image input and image output
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(imageOut, QFileInfo(fileName).fileName()+"_his_slide");
}

/* Histogram slide method => increasing or decreasing BRIGHTNESS of a color image */
void MainWindow::on_btn_hisSlide_Color_clicked(){
    QString fileName = ui->ln_fileName->text(); // get file name from ln_fileName (QLineEdit)
    qDebug() << "File name: " << fileName; // test

    QImage imageIn(fileName); // read image from file name
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);

    int c = ui->ln_c->text().toInt(); // convert string to number

    // travesing all pixels of imageIn
    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height(); y++){
            QRgb rgb = imageIn.pixel(x, y); // get pixel value at (x,y)
            QColor colorIn(rgb);
            int h, s, v;
            colorIn.getHsv(&h, &s, &v); // get h, s, v component from colorIn
            int vOut = v + c;

            // check threshold
            if(vOut > 255)
                vOut = 255;
            else
                if(vOut < 0)
                   vOut = 0;

            QColor colorOut = QColor::fromHsv(h, s, vOut); // color out for image out
            imageOut.setPixel(x, y, colorOut.rgb());
        }
    }
    // display image input and image output
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(imageOut, QFileInfo(fileName).fileName()+"_his_slide");
}

/* Histogram stretch method => increasing or decreasing CONTRAST of a gray image */
void MainWindow::on_btn_hisStretch_gray_clicked(){
    QString fileName = ui->ln_fileName->text(); // get file name from ln_fileName (QLineEdit)
    qDebug() << "File name: " << fileName; // test

    QImage imageIn(fileName); // read image from file name
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);

    int c = ui->lb_c->text().toFloat(); // convert string to float number

    // travesing all pixels of imageIn
    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height(); y++){
            QRgb colorIn = imageIn.pixel(x, y); // get pixel value at (x,y)
            int grayIn = qGray(colorIn); // convert color value at (x,y) to gray value
            int grayOut = grayIn * c; // change gray value to increasing or decreasing the contrast of the image

            // check threshold
            if(grayOut > 255)
                grayOut = 255;
            else
                if(grayOut < 0)
                    grayOut = 0;
            QRgb colorOut = qRgb(grayOut, grayOut, grayOut); // color out for image out
            imageOut.setPixel(x, y, colorOut);
        }
    }
    // save image out
    /*QString selectedFilter;
    QString name = QFileDialog::getSaveFileName(this, "Save image", QDir::currentPath(),
                                                "BMP (*.bmp);;PNG (*.png);;JPEG (*.jpg);;All files (*.*)",
                                                &selectedFilter);

    if(selectedFilter == "BMP (*.bmp)")
        imageOut.save(name, "BMP");
    else
        if(selectedFilter == "PNG (*.png)")
            imageOut.save(name, "PNG");
        else
            if(selectedFilter == "JPEG (*.jpg)")
                imageOut.save(name, "JPEG");
            else
                imageOut.save(name);
    */
    // display image input and image output
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(imageIn, QFileInfo(fileName).fileName()+"_his_stretch");
}

/* Histogram stretch method => increasing or decreasing CONTRAST of a color image */
void MainWindow::on_btn_hisStretch_Color_clicked(){
    QString fileName = ui->ln_fileName->text(); // get file name from ln_fileName (QLineEdit)
    qDebug() << "File name: " << fileName; // test

    QImage imageIn(fileName);
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);

    int c = ui->ln_c->text().toFloat(); // convert string to float number

    // travesing all pixels of imageIn
    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height(); y++){
            QColor colorIn = imageIn.pixel(x, y);
            int h, s, v;
            colorIn.getHsv(&h, &s, &v);

            int vOut = v * c;

            // check threshold
            if(vOut > 255)
                vOut = 255;
            else
                if(vOut < 0)
                    vOut = 0;

            QColor colorOut = QColor::fromHsv(h, s, vOut); // color out for image out
            imageOut.setPixel(x, y, colorOut.rgb());
        }
    }
    // display image input and image output
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(imageIn, QFileInfo(fileName).fileName()+"_his_stretch");
}

void MainWindow::on_btn_segmentation_gray_clicked(){
    QString fileName = ui->ln_fileName->text(); // get file name from ln_fileName (QLineEdit)
    qDebug() << "File name: " << fileName; // test

    QImage imageIn(fileName);
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);

    int c = ui->ln_c->text().toInt(); // convert string to number

    // travesing all pixels of imageIn
    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height(); y++){
            QRgb colorIn = imageIn.pixel(x, y); // get pixel value at (x,y)
            int gray = qGray(colorIn); // convert color value at (x,y) to gray value

            // check threshold
            if(gray > c)
                gray = 255;
            else
                gray = 0;

            QRgb colorOut = qRgb(gray, gray, gray);
            imageOut.setPixel(x, y, colorOut);
        }
    }
    // display image input and image output
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(imageIn, QFileInfo(fileName).fileName()+"_segmentation");
}
