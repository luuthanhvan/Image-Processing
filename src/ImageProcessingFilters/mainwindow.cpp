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
    int margin = size/2; // margin: avoid removed border of the images
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

void MainWindow::on_btn_gradientFilter_Gx_Gray_clicked(){
    QString fileName = ui->ln_fileName->text();
    QImage imageIn(fileName);
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut.fill(Qt::white);

    int gray1 = 0, gray2 = 0, grayOut;

    for(int x = 0; x < imageIn.width()-1; x++){
        for(int y = 0; y < imageIn.height(); y++){
            QRgb rgb1 = imageIn.pixel(x, y);
            gray1 = qGray(rgb1);

            QRgb rgb2 = imageIn.pixel(x+1, y);
            gray2 = qGray(rgb2);

            // gx = I(x+1, y) - I(x, y)
            grayOut = abs(gray2 - gray1);

            imageOut.setPixel(x, y, qRgb(grayOut, grayOut, grayOut));
        }
    }
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(imageOut, QFileInfo(fileName).fileName()+"_gradient_filter_gx");
}

void MainWindow::on_btn_gradientFilter_Gy_Gray_clicked(){
    QString fileName = ui->ln_fileName->text();
    QImage imageIn(fileName);
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut.fill(Qt::white);
    int gray1 = 0, gray2 = 0, grayOut;
    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height()-1; y++){
            QRgb rgb1 = imageIn.pixel(x, y);
            gray1 = qGray(rgb1);

            QRgb rgb2 = imageIn.pixel(x, y+1);
            gray2 = qGray(rgb2);

            // gy = I(x, y+1) - I(x, y)
            grayOut = abs(gray2 - gray1);

            imageOut.setPixel(x, y, qRgb(grayOut, grayOut, grayOut));
        }
    }
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(imageOut, QFileInfo(fileName).fileName()+"_gradient_filter_gy");
}

void MainWindow::on_btn_gradientFilter_Gx_Color_clicked(){
    QString fileName = ui->ln_fileName->text();
    QImage imageIn(fileName);
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut.fill(Qt::white);

    for(int x = 0; x < imageIn.width()-1; x++){
        for(int y = 0; y < imageIn.height(); y++){
            QColor color1 = imageIn.pixel(x, y);
            int h1, s1, v1;
            color1.getHsv(&h1, &s1, &v1);

            QColor color2 = imageIn.pixel(x+1, y);
            int h2, s2, v2;
            color2.getHsv(&h2, &s2, &v2);

            int vOut = abs(v2 - v1);
            QColor colorOut = QColor::fromHsv(h1, s1, vOut);
            imageOut.setPixel(x, y, colorOut.rgb());
        }
    }
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(imageOut, QFileInfo(fileName).fileName()+"_gradient_filter_gx");
}

void MainWindow::on_btn_gradientFilter_Gy_Color_clicked(){
    QString fileName = ui->ln_fileName->text();
    QImage imageIn(fileName);
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut.fill(Qt::white);

    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height()-1; y++){
            QColor color1 = imageIn.pixel(x, y);
            int h1, s1, v1;
            color1.getHsv(&h1, &s1, &v1);

            QColor color2 = imageIn.pixel(x, y+1);
            int h2, s2, v2;
            color2.getHsv(&h2, &s2, &v2);

            int vOut = abs(v2 - v1);
            QColor colorOut = QColor::fromHsv(h1, s1, vOut);
            imageOut.setPixel(x, y, colorOut.rgb());
        }
    }
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(imageOut, QFileInfo(fileName).fileName()+"_gradient_filter_gy");
}

void MainWindow::on_btn_roberFilter_Gx_Gray_clicked(){
    QString fileName = ui->ln_fileName->text();
    QImage imageIn(fileName);
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut.fill(Qt::white);
    int gray1 = 0, gray2 = 0, grayOut;
    for(int x = 0; x < imageIn.width()-1; x++){
        for(int y = 0; y < imageIn.height()-1; y++){
            QRgb rgb1 = imageIn.pixel(x, y+1);
            gray1 = qGray(rgb1);

            QRgb rgb2 = imageIn.pixel(x+1, y);
            gray2 = qGray(rgb2);

            // gx = I(x+1, y) - I(x, y+1)
            grayOut = abs(gray2 - gray1);

            imageOut.setPixel(x, y, qRgb(grayOut, grayOut, grayOut));
        }
    }
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(imageOut, QFileInfo(fileName).fileName()+"_rober_filter_gx");
}

void MainWindow::on_btn_roberFilter_Gy_Gray_clicked(){
    QString fileName = ui->ln_fileName->text();
    QImage imageIn(fileName);
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut.fill(Qt::white);
    int gray1 = 0, gray2 = 0, grayOut;
    for(int x = 0; x < imageIn.width()-1; x++){
        for(int y = 0; y < imageIn.height()-1; y++){
            QRgb rgb1 = imageIn.pixel(x+1, y+1);
            gray1 = qGray(rgb1);

            QRgb rgb2 = imageIn.pixel(x, y);
            gray2 = qGray(rgb2);

            // gy = I(x, y) - I(x+1, y+1)
            grayOut = abs(gray2 - gray1);

            imageOut.setPixel(x, y, qRgb(grayOut, grayOut, grayOut));
        }
    }
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(imageOut, QFileInfo(fileName).fileName()+"_rober_filter_gy");
}

void MainWindow::on_btn_roberFilter_Gx_Color_clicked(){
    QString fileName = ui->ln_fileName->text();
    QImage imageIn(fileName);
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut.fill(Qt::white);

    for(int x = 0; x < imageIn.width()-1; x++){
        for(int y = 0; y < imageIn.height()-1; y++){
            QColor color1 = imageIn.pixel(x, y+1);
            int h1, s1, v1;
            color1.getHsv(&h1, &s1, &v1);

            QColor color2 = imageIn.pixel(x+1, y);
            int h2, s2, v2;
            color2.getHsv(&h2, &s2, &v2);

            int vOut = abs(v2 - v1);
            QColor colorOut = QColor::fromHsv(h1, s1, vOut);
            imageOut.setPixel(x, y, colorOut.rgb());
        }
    }
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(imageOut, QFileInfo(fileName).fileName()+"_rober_filter_gx");
}

void MainWindow::on_btn_roberFilter_Gy_Color_clicked(){
    QString fileName = ui->ln_fileName->text();
    QImage imageIn(fileName);
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut.fill(Qt::white);

    for(int x = 0; x < imageIn.width()-1; x++){
        for(int y = 0; y < imageIn.height()-1; y++){
            QColor color1 = imageIn.pixel(x+1, y+1);
            int h1, s1, v1;
            color1.getHsv(&h1, &s1, &v1);

            QColor color2 = imageIn.pixel(x, y);
            int h2, s2, v2;
            color2.getHsv(&h2, &s2, &v2);

            int vOut = abs(v2 - v1);
            QColor colorOut = QColor::fromHsv(h1, s1, vOut);
            imageOut.setPixel(x, y, colorOut.rgb());
        }
    }
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(imageOut, QFileInfo(fileName).fileName()+"_rober_filter_gy");
}

void MainWindow::on_btn_sobelFilter_Gx_Gray_clicked(){
    QString fileName = ui->ln_fileName->text();
    QImage imageIn(fileName);
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut.fill(Qt::white);
    int maskSize = 3;
    int margin = maskSize/2;
    int maskSobel[maskSize][maskSize];
    int sumMask;

    maskSobel[0][0] = -1; maskSobel[0][1] = 0; maskSobel[0][2] = 1;
    maskSobel[1][0] = -2; maskSobel[1][1] = 0; maskSobel[1][2] = 2;
    maskSobel[2][0] = -1; maskSobel[2][1] = 0; maskSobel[2][2] = 1;

    for(int x = margin; x < imageIn.width()-margin; x++){
        for(int y = margin; y < imageIn.height()-margin; y++){
            sumMask = 0;
            for(int i = -margin; i <= margin; i++){
                for(int j = -margin; j <= margin; j++){
                    QRgb rgb = imageIn.pixel(x+j, y+i);
                    int gray = qGray(rgb);
                    sumMask += (gray*maskSobel[i+margin][j+margin]);
                }
            }
            imageOut.setPixel(x, y, qRgb(abs(sumMask), abs(sumMask), abs(sumMask)));
        }
    }
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(imageOut, QFileInfo(fileName).fileName()+"_sobel_filter_gx");
}

void MainWindow::on_btn_sobelFilter_Gy_Gray_clicked(){
    QString fileName = ui->ln_fileName->text();
    QImage imageIn(fileName);
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut.fill(Qt::white);
    int maskSize = 3;
    int margin = maskSize/2;
    int maskSobel[maskSize][maskSize];
    int sumMask;
    maskSobel[0][0] = -1; maskSobel[0][1] = -2; maskSobel[0][2] = -1;
    maskSobel[1][0] = 0; maskSobel[1][1] = 0; maskSobel[1][2] = 0;
    maskSobel[2][0] = 1; maskSobel[2][1] = 2; maskSobel[2][2] = 1;

    for(int x = margin; x < imageIn.width()-margin; x++){
        for(int y = margin; y < imageIn.height()-margin; y++){
            sumMask = 0;
            for(int i = -margin; i <= margin; i++){
                for(int j = -margin; j <= margin; j++){
                    QRgb rgb = imageIn.pixel(x+j, y+i);
                    int gray = qGray(rgb);
                    sumMask += (gray*maskSobel[i+margin][j+margin]);
                }
            }
            imageOut.setPixel(x, y, qRgb(abs(sumMask), abs(sumMask), abs(sumMask)));
        }
    }
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(imageOut, QFileInfo(fileName).fileName()+"_sobel_filter_gy");
}

void MainWindow::on_btn_prewittFilter_Gx_Gray_clicked(){
    QString fileName = ui->ln_fileName->text();
    QImage imageIn(fileName);
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut.fill(Qt::white);
    int maskSize = 3;
    int margin = maskSize/2;
    int maskPrewitt[maskSize][maskSize];
    int sumMask;

    maskPrewitt[0][0] = -1; maskPrewitt[0][1] = 0; maskPrewitt[0][2] = 1;
    maskPrewitt[1][0] = -1; maskPrewitt[1][1] = 0; maskPrewitt[1][2] = 1;
    maskPrewitt[2][0] = -1; maskPrewitt[2][1] = 0; maskPrewitt[2][2] = 1;

    for(int x = margin; x < imageIn.width()-margin; x++){
        for(int y = margin; y < imageIn.height()-margin; y++){
            sumMask = 0;
            for(int i = -margin; i <= margin; i++){
                for(int j = -margin; j <= margin; j++){
                    QRgb rgb = imageIn.pixel(x+j, y+i);
                    int gray = qGray(rgb);
                    sumMask += (gray*maskPrewitt[i+margin][j+margin]);
                }
            }
            imageOut.setPixel(x, y, qRgb(abs(sumMask), abs(sumMask), abs(sumMask)));
        }
    }
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(imageOut, QFileInfo(fileName).fileName()+"_prewitt_filter_gx");
}

void MainWindow::on_btn_prewittFilter_Gy_Gray_clicked(){
    QString fileName = ui->ln_fileName->text();
    QImage imageIn(fileName);
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut.fill(Qt::white);
    int maskSize = 3;
    int margin = maskSize/2;
    int maskPrewitt[maskSize][maskSize];
    int sumMask;

    maskPrewitt[0][0] = -1; maskPrewitt[0][1] = -1; maskPrewitt[0][2] = -1;
    maskPrewitt[1][0] = 0; maskPrewitt[1][1] = 0; maskPrewitt[1][2] = 0;
    maskPrewitt[2][0] = 1; maskPrewitt[2][1] = 1; maskPrewitt[2][2] = 1;

    for(int x = margin; x < imageIn.width()-margin; x++){
        for(int y = margin; y < imageIn.height()-margin; y++){
            sumMask = 0;
            for(int i = -margin; i <= margin; i++){
                for(int j = -margin; j <= margin; j++){
                    QRgb rgb = imageIn.pixel(x+j, y+i);
                    int gray = qGray(rgb);
                    sumMask += (gray*maskPrewitt[i+margin][j+margin]);
                }
            }
            imageOut.setPixel(x, y, qRgb(abs(sumMask), abs(sumMask), abs(sumMask)));
        }
    }
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(imageOut, QFileInfo(fileName).fileName()+"_prewitt_filter_gy");
}

void MainWindow::on_btn_autoSegmentation_Gray_clicked(){
    QString fileName = ui->ln_fileName->text();
    QImage imageIn(fileName);
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);

    int c;
    int newc = 127; // init new thresholding

    do{
        int G1 = 0, G2 = 0;
        int count1 = 0; // the probability of the first class occurrence
        int count2 = 0; // the probability of the second class occurrence
        c = newc;
        for(int x = 0; x < imageIn.width(); x++){
            for(int y = 0; y < imageIn.height(); y++){
                QRgb rgb = imageIn.pixel(x, y);
                int gray = qGray(rgb);
                if(gray > c){
                    G1 += gray;
                    count1++;
                }
                else if(gray <= c){
                    G2 += gray;
                    count2++;
                }
            }
        }
        int M1 = G1/count1;
        int M2 = G2/count2;
        newc = (M1 + M2)/2;
    } while(abs(newc - c) != 0);

    /* segmentation */
    int thresholding = newc;
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

            imageOut.setPixel(x, y, qRgb(gray, gray, gray)); // set pixel (x,y) with color (colorOut) for imageOut
        }
    }
    // display imageIn and imageOut
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(imageOut, QFileInfo(fileName).fileName()+"_auto_segmentation");
}

void MainWindow::on_btn_autoSegmentation_Otsu_Gray_clicked(){
    QString fileName = ui->ln_fileName->text();
    QImage imageIn(fileName);
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);

    // initialize variables
    int meanCurrent = 0;
    int count1 = 0; // the probability of the first class occurrence
    int count2 = 0; // the probability of the second class occurrence
    float maxVariance = 0;
    int thresholding = 0;
    int maxIntensity = 256;
    int nbPixels = imageIn.width() * imageIn.height(); // number of pixels

    // initialize histogram h(x)
    int histogram[maxIntensity];
    for(int i = 0; i < maxIntensity; i++)
        histogram[i] = 0;

    // calculate historgram h(x)
    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height(); y++){
            QRgb rgb = imageIn.pixel(x, y);
            int gray = qGray(rgb);
            histogram[gray]++;
        }
    }

    // calculate mean value for the overall histogram (x * h(x))
    int meanTotal = 0;
    for(int i = 0; i < maxIntensity; i++){
        meanTotal += i*histogram[i];
    }

    /* choose thresholding */
    for (int t = 0; t <  maxIntensity; t++){
        // calculate count1 for the current t
        count1 += histogram[t];

        if(count1 == 0)
            continue;

        // calculate count2 for the current t
        count2 = nbPixels - count1;

        meanCurrent += (t*histogram[t]);

        // calculate mean value for the first class
        float mean1 = (float)meanCurrent/(float)count1;
        // calculate mean value for the second class
        float mean2 = ((float)meanTotal - (float)meanCurrent)/(float)count2;

        // calculate the between-class varience
        float variance = (float)count1 * (float)count2 * (mean2 - mean1) * (mean2 - mean1);

        // check if new maximum variance found
        if(variance > maxVariance){
            maxVariance = variance; // update maximum variance
            thresholding = t;
        }
    }

    /* segmentation */
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

            imageOut.setPixel(x, y, qRgb(gray, gray, gray)); // set pixel (x,y) with color (colorOut) for imageOut
        }
    }
    // display imageIn and imageOut
    displayImage(imageIn, QFileInfo(fileName).fileName());
    displayImage(imageOut, QFileInfo(fileName).fileName()+"_auto_segmentation");
}
