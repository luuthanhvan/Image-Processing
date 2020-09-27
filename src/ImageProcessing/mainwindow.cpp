#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QPixmap>
#include <QLabel>
#include <QString>
#include <QDebug>
#include <QImage>
#include <QRgb>

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

void MainWindow::on_btn_hisSlide_gray_clicked(){
    QString fileName = ui->ln_fileName->text(); // get file name from ln_fileName (QLineEdit)
    qDebug() << "File name: " << fileName; // test

    QImage imageIn(fileName); // read image from file name
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);

    int c = ui->ln_c->text().toInt(); // convert string to number

    // travesing all pixels of imageIn
    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height(); y++){
            QRgb color = imageIn.pixel(x, y); // get pixel value at (x,y)
            int grayIn = qGray(color); // convert color value at (x,y) to gray level
            int grayOut = grayIn + c; // change gray level aming to increasing or decreasing brightness

            // check threshold
            if(grayOut > 255)
                grayOut = 255;
            else
                if(grayOut < 0)
                   grayOut = 0;

            QRgb colorOut = qRgb(grayOut, grayOut, grayOut); // gray image
            imageOut.setPixel(x, y, colorOut);
        }
    }
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(imageOut, QFileInfo(fileName).fileName()+"_his_slide");
}

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

            QColor colorOut = QColor::fromHsv(h, s, vOut); // color image
            imageOut.setPixel(x, y, colorOut.rgb());
        }
    }
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(imageOut, QFileInfo(fileName).fileName()+"_his_slide");
}
