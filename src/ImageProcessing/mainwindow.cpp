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
#define HEIGHT 128

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

void MainWindow::saveImage(QImage &imgOut){
    QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(this, "Save image", QDir::currentPath(),
                                                    "BMP (*.bmp);;PNG (*.png);;JPEG (*.jpg);;All files (*.*)",
                                                    &selectedFilter);
    if(selectedFilter == "BMP (*.bmp)")
        imgOut.save(fileName, "BMP");
    else
        if(selectedFilter == "PNG (*.png)")
            imgOut.save(fileName, "PNG");
        else
            if(selectedFilter == "JPEG (*.jpg)")
                imgOut.save(fileName, "JPEG");
            else
                imgOut.save(fileName);
}

int MainWindow::max(int num1, int num2){
    return num1 > num2 ? num1 : num2;
}

int MainWindow::min(int num1, int num2){
    return num1 < num2 ? num1 : num2;
}

QImage MainWindow::drawHisGray(QImage &img){
    QImage histogram(256, HEIGHT, QImage::Format_ARGB32);
    histogram.fill(Qt::white); // init histogram white
    int h[256]; // mang luu tru so diem anh, diem anh tuong ung co gia tri tu 0..255
    // vi du gia tri diem anh 100 xuat hien 10 lan thi h[100] = 10

    for(int i = 0; i < 256; i++)
        h[i] = 0;

    // dem so luong diem anh
    for(int x = 0; x < img.width(); x++){
        for(int y = 0; y < img.height(); y++){
            QRgb rgb = img.pixel(x, y); // lay gia tri diem anh tai vi tri (x, y)
            int gray = qGray(rgb); // chuyen gia tri diem anh tai vi tri (x, y) ve gia tri muc xam
            h[gray]++; // tang so luong diem anh len 1
        }
    }

    // find max value in h[]
    int maxValue = 0;
    for(int i = 0; i < 256; i++){
        maxValue = max(maxValue, h[i]);
    }

    int lineHeight = 0;
    for(int x = 0; x < 256; x++){
        lineHeight = (HEIGHT*h[x])/maxValue; // calculating ratio
//        for(int y = HEIGHT-1; y > HEIGHT-1-lineHeight; y--)
//            histogram.setPixel(x, y, qRgb(0, 0, 255)); // draw histogram
        for(int y = HEIGHT-lineHeight; y < HEIGHT; y++)
            histogram.setPixel(x, y, qRgb(0, 0, 255)); // draw histogram
    }

    return histogram;
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

            imageIn.setPixel(x, y, qRgb(grayIn, grayIn, grayIn));
            imageOut.setPixel(x, y, qRgb(grayOut, grayOut, grayOut)); // set pixel (x,y) with color (colorOut) for imageOut
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

            imageIn.setPixel(x, y, qRgb(grayIn, grayIn, grayIn));
            imageOut.setPixel(x, y, qRgb(grayOut, grayOut, grayOut)); // set pixel (x,y) with color (colorOut) for imageOut
        }
    }

    // save imageOut
    saveImage(imageOut);

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

/* Segmentation method */
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
            imageIn.setPixel(x, y, qRgb(gray, gray, gray));

            // check threshold
            if(gray >= thresholding)
                gray = 255;
            else
                gray = 0;

            imageOut.setPixel(x, y, qRgb(gray, gray, gray)); // set pixel (x,y) with color (colorOut) for imageOut
        }
    }
    // display imageIn and imageOut
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(imageOut, QFileInfo(fileName).fileName()+"_segmentation");
}

void MainWindow::on_btn_showHis_gray_clicked(){
    QString fileName = ui->ln_fileName->text(); // get file name from line edit bar ln_fileName (QLineEdit)
    qDebug() << "File name: " << fileName; // test

    QImage imageIn(fileName); // read image from file name
//    const int HEIGHT = 128; // height of histogram

    QImage histogram(256, HEIGHT, QImage::Format_ARGB32);
    histogram.fill(Qt::white); // init histogram white
    int h[256]; // mang luu tru so diem anh, diem anh tuong ung co gia tri tu 0..255
    // vi du gia tri diem anh 100 xuat hien 10 lan thi h[100] = 10

    for(int i = 0; i < 256; i++)
        h[i] = 0;

    // dem so luong diem anh
    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height(); y++){
            QRgb rgb = imageIn.pixel(x, y); // lay gia tri diem anh tai vi tri (x, y)
            int gray = qGray(rgb); // chuyen gia tri diem anh tai vi tri (x, y) ve gia tri muc xam
            h[gray]++; // tang so luong diem anh len 1
        }
    }

    // find max value in h[]
    int maxValue = 0;
    for(int i = 0; i < 256; i++){
        maxValue = max(maxValue, h[i]);
    }

    int lineHeight = 0;
    for(int x = 0; x < 256; x++){
        lineHeight = (HEIGHT*h[x])/maxValue; // calculating ratio
//        for(int y = HEIGHT-1; y > HEIGHT-1-lineHeight; y--)
//            histogram.setPixel(x, y, qRgb(0, 0, 255)); // draw histogram
        for(int y = HEIGHT-lineHeight; y < HEIGHT; y++)
            histogram.setPixel(x, y, qRgb(0, 0, 255)); // draw histogram
    }

    // display imageIn and histogram
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(histogram, QFileInfo(fileName).fileName()+"_histogram");
}

void MainWindow::on_btn_showHis_Color_clicked(){
    QString fileName = ui->ln_fileName->text(); // get file name from line edit bar ln_fileName (QLineEdit)
    QImage imageIn(fileName);

//    const int HEIGHT = 125;

    QImage histogram(256, HEIGHT*3, QImage::Format_ARGB32);
    histogram.fill(Qt::black);

    int h_red[256], h_green[256], h_blue[256];
    for(int i = 0; i < 256; i++){
        h_red[i] = 0;
        h_green[i] = 0;
        h_blue[i] = 0;
    }

    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height(); y++){
            QColor color = imageIn.pixel(x, y);
            int red = color.red();
            int green = color.green();
            int blue = color.blue();
            h_red[red]++;
            h_green[green]++;
            h_blue[blue]++;
        }
    }

    int maxValue = 0;
    for(int x = 0; x < 256; x++){
        if(maxValue < h_red[x])
            maxValue = h_red[x];
        if(maxValue < h_green[x])
            maxValue = h_green[x];
        if(maxValue < h_blue[x])
            maxValue = h_blue[x];
    }

    int lineHeight = 0;
    for(int x = 0; x < 256; x++){
        lineHeight = (HEIGHT*h_red[x])/maxValue;
        for(int y = HEIGHT-1; y > HEIGHT-1-lineHeight; y--)
            histogram.setPixel(x, y, qRgb(255, 0, 0));

        lineHeight = (HEIGHT*h_green[x])/maxValue;
        for(int y = 2*HEIGHT-1; y > 2*HEIGHT-1-lineHeight; y--)
            histogram.setPixel(x, y, qRgb(0, 255, 0));

        lineHeight = (HEIGHT*h_blue[x])/maxValue;
        for(int y = 3*HEIGHT-1; y > 3*HEIGHT-1-lineHeight; y--)
            histogram.setPixel(x, y, qRgb(0, 0, 255));
    }

    // display imageIn and histogram
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(histogram, QFileInfo(fileName).fileName()+"_histogram");
}

void MainWindow::on_btn_linearModification_gray_clicked(){
    QString fileName = ui->ln_fileName->text(); // get file name from line edit bar ln_fileName (QLineEdit)
    QImage imageIn(fileName);
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);

    int minValue = 255, maxValue = 0;

    // duyet qua tung diem anh va tim diem anh co gia tri lon nhat va nho nhat
    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height(); y++){
            QRgb rgb = imageIn.pixel(x, y);
            int gray = qGray(rgb);
            maxValue = max(maxValue, gray);
            minValue = min(minValue, gray);
        }
    }
    qDebug() << "max value:" << maxValue;
    qDebug() << "min value:" << minValue;

    // duyet qua tung diem anh 1 lan nua, thay doi gia tri cac diem anh bang cong thuc:
    // O(x,y) = ((I(x,y) - min[I(x,y)]) * 255) / (max[I(x,y)] - min[I(x,y)])
    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height(); y++){
            QRgb rgb = imageIn.pixel(x, y);
            int grayIn = qGray(rgb);
            int grayOut = ((grayIn-minValue)*255)/(maxValue-minValue);

            // check threshold
            if(grayOut > 255)
                grayOut = 255;
            else
                if(grayOut < 0)
                    grayOut = 0;

            imageIn.setPixel(x, y, qRgb(grayIn, grayIn, grayIn));
            imageOut.setPixel(x, y, qRgb(grayOut, grayOut, grayOut));
        }
    }

    // draw histogram of imageIn and display it
    QImage histogramImgIn = drawHisGray(imageIn);
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(histogramImgIn, QFileInfo(fileName).fileName()+"_histogram");

    // draw histogram of imageOut and display it
    QImage histogramImgOut = drawHisGray(imageOut);
    displayImage(imageOut, QFileInfo(fileName).fileName()+"_linear_modification");
    displayImage(histogramImgOut, QFileInfo(fileName).fileName()+"_linear_modification_histogram");
}

void MainWindow::on_btn_histogramEqual_gray_clicked(){
    QString fileName = ui->ln_fileName->text();
    QImage imageIn(fileName);
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);

    int h[256];
    float hn[256], C[256];
    for(int i = 0; i < 256; i++){
        h[i] = 0;
        hn[i] = 0;
        C[i] = 0;
    }

    // tinh to chuc do h[x]
    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height(); y++){
            QRgb rgb = imageIn.pixel(x, y);
            int gray = qGray(rgb);
            h[gray]++;
        }
    }

    // chuan hoa to chuc do h[x] thanh hn[x]
    for(int x = 0; x < 256; x++){
        hn[x] = (float)h[x]/(float)(imageIn.width()*imageIn.height());
    }

    // tinh ham mat do xac xuat C[x]
    C[0] = hn[0];
    for(int x = 1; x < 256; x++){
        C[x] = C[x-1] + hn[x];
    }

    // duyet qua cac diem anh 1 lan nua de tinh gia tri cho anh output: O(x,y)
    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height(); y++){
            QRgb rgb = imageIn.pixel(x, y);
            int grayIn = qGray(rgb);
            imageIn.setPixel(x, y, qRgb(grayIn, grayIn, grayIn));

            int grayOut = C[grayIn]*255;

            imageOut.setPixel(x, y, qRgb(grayOut, grayOut, grayOut));
        }
    }

    // draw histogram of imageIn and display it
    QImage histogramImgIn = drawHisGray(imageIn);
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(histogramImgIn, QFileInfo(fileName).fileName()+"_histogram");

    // draw histogram of imageOut and display it
    QImage histogramImgOut = drawHisGray(imageOut);
    displayImage(imageOut, QFileInfo(fileName).fileName()+"_histogram_equalization");
    displayImage(histogramImgOut, QFileInfo(fileName).fileName()+"_histogram_equalization_histogram");
}
