#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QDebug>
#include <QFileDialog>
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
}

void MainWindow::on_btn_meanFilter_gray_clicked(){
    QString fileName = ui->ln_fileName->text();
    QImage imageIn(fileName);
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut.fill(Qt::white);
    int size = 3;
    int cells = size*size;
    int margin = size/2;
    int sumGray;
    for(int x = margin; x < imageIn.width()-margin; x++){
        for(int y = margin; y < imageIn.height()-margin; y++){
            sumGray = 0;
            for(int i = -margin; i <= margin; i++){
                for(int j = -margin; j <= margin; j++){
                    QRgb rgb = imageIn.pixel(x+i, y+j);
                    int gray = qGray(rgb);
                    sumGray += gray;
                }
            }
            imageOut.setPixel(x, y, qRgb(sumGray/cells, sumGray/cells, sumGray/cells));
        }
    }
    // display imageIn and imageOut
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(imageOut, QFileInfo(fileName).fileName()+"_mean_filter");
}

void MainWindow::on_btn_meanFilter_Color_clicked(){
    QString fileName = ui->ln_fileName->text();
    QImage imageIn(fileName);
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut.fill(Qt::white);
    int size = 3;
    int cells = size*size;
    int margin = size/2;
    int sumR, sumG, sumB;
    QColor color;
    for(int x = margin; x < imageIn.width()-margin; x++){
        for(int y = margin; y < imageIn.height()-margin; y++){
            sumR = 0; sumG = 0; sumB = 0;
            for(int i = -margin; i <= margin; i++){
                for(int j = -margin; j <= margin; j++){
                    color = imageIn.pixel(x+i, y+j);
                    sumR += color.red();
                    sumG += color.green();
                    sumB += color.blue();
                }
            }
            imageOut.setPixel(x, y, qRgb(sumR/cells, sumG/cells, sumB/cells));
        }
    }
    // display imageIn and imageOut
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(imageOut, QFileInfo(fileName).fileName()+"_mean_filter");
}

void MainWindow::on_btn_medianFilter_gray_clicked(){
    QString fileName = ui->ln_fileName->text();
    QImage imageIn(fileName);
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut.fill(Qt::white);
    int size=3;
    int cells = size*size;
    int margin = size/2;
    int h[cells];
    for(int x = margin; x < imageIn.width()-margin; x++){
        for(int y = margin; y < imageIn.height()-margin; y++){
            int k = 0;
            for(int i = -margin; i <= margin; i++){
                for(int j = -margin; j <= margin; j++){
                    QRgb rgb = imageIn.pixel(x+i, y+j);
                    int gray = qGray(rgb);
                    h[k] = gray;
                    k++;
                }
            }
            qSort(h, h+cells);
            imageOut.setPixel(x, y, qRgb(h[cells/2], h[cells/2], h[cells/2]));
        }
    }
    // display imageIn and imageOut
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(imageOut, QFileInfo(fileName).fileName()+"_median_filter");
}

void MainWindow::on_btn_medianFilter_Color_clicked(){
    QString fileName = ui->ln_fileName->text();
    QImage imageIn(fileName);
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut.fill(Qt::white);
    int size = 3;
    int cells = size*size;
    int margin = size/2;
    int hR[cells], hG[cells], hB[cells];
    QColor color;
    for(int x = margin; x < imageIn.width()-margin; x++){
        for(int y = margin; y < imageIn.height()-margin; y++){
            int r = 0, g = 0, b = 0;
            for(int i = -margin; i <= margin; i++){
                for(int j = -margin; j <= margin; j++){
                    color = imageIn.pixel(x+i, y+j);
                    hR[r++] = color.red();
                    hG[g++] = color.green();
                    hB[b++] = color.blue();
                }
            }
            qSort(hR, hR+cells);
            qSort(hG, hG+cells);
            qSort(hB, hB+cells);
            imageOut.setPixel(x, y, qRgb(hR[cells/2], hG[cells/2], hB[cells/2]));
        }
    }
    // display imageIn and imageOut
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(imageOut, QFileInfo(fileName).fileName()+"_mean_filter");
}
