#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imagewidget.h"
#include <QMdiSubWindow>
#include <QFileDialog>
#include <QInputDialog>
#include <QDebug>

#define HEIGHT_HISTOGRAM 128 // define the default height of the histogram

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    mdiArea = new QMdiArea(this);
    setCentralWidget(mdiArea);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::displayImage(QImage &image, QString title){
    if(image.isNull()){
        return;
    }
    QMdiSubWindow *subWindow = new QMdiSubWindow();
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    mdiArea->addSubWindow(subWindow);
    subWindow->setWindowTitle(title);
    ImageWidget *imageWidget = new ImageWidget();
    imageWidget->setImage(image);
    subWindow->setWidget(imageWidget);
    subWindow->resize(image.size());
    subWindow->show();
}

int MainWindow::findMaxPixel(int pixels[]){
    int maxValue = -1;
    for(int i = 0; i < 256; i++){
        if(maxValue < pixels[i])
            maxValue = pixels[i];
    }
    return maxValue;
}

int MainWindow::max(int num1, int num2){
    return num1 > num2 ? num1 : num2;
}

int MainWindow::min(int num1, int num2){
    return num1 < num2 ? num1 : num2;
}

QImage MainWindow::drawHisGray(QImage &imageIn){
    QImage histogram(256, HEIGHT_HISTOGRAM, QImage::Format_ARGB32);
    histogram.fill(Qt::white);

    int h[256];
    for(int i = 0; i < 256; i++)
        h[i] = 0;

    // calculating histogram <- h[]
    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height(); y++){
            int gray = qGray(imageIn.pixel(x, y));
            h[gray]++; // count number of pixels corresponding to gray values
        }
    }

    // find the maximum pixels
    int maxPixel = findMaxPixel(h);
    int lineHeight = 0;

    // set histogram's pixels
    for(int x = 0; x < 256; x++){
        lineHeight = (HEIGHT_HISTOGRAM * h[x])/maxPixel;
        for(int y = HEIGHT_HISTOGRAM-1; y > HEIGHT_HISTOGRAM-1-lineHeight; y--)
            histogram.setPixel(x, y, qRgb(0, 0, 255));
    }

    return histogram;
}

QImage MainWindow::drawHisColor(QImage &imageIn){
    QImage histogram(256, HEIGHT_HISTOGRAM*3, QImage::Format_ARGB32);
    histogram.fill(Qt::white);

    int h_R[256];
    int h_G[256];
    int h_B[256];
    for(int i = 0; i < 256; i++){
        h_R[i] = 0;
        h_G[i] = 0;
        h_B[i] = 0;
    }

    // calculating histogram <- h_R[], h_G[], h_B[]
    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height(); y++){
            QColor color = imageIn.pixel(x, y);
            int red = color.red();
            int green = color.green();
            int blue = color.blue();
            h_R[red]++;
            h_G[green]++;
            h_B[blue]++;
        }
    }

    // find the maximum pixel
    int maxRed = findMaxPixel(h_R);
    int maxGreen = findMaxPixel(h_G);
    int maxBlue = findMaxPixel(h_B);

    int maxPixel = maxRed;
    if(maxPixel < maxGreen)
        maxPixel = maxGreen;
    if(maxPixel < maxBlue)
        maxPixel= maxBlue;

    // set histogram's pixel
    int lineHeight = 0;
    for(int x = 0; x < 256; x++){
        lineHeight = (HEIGHT_HISTOGRAM * h_R[x])/maxPixel;
        for(int y = HEIGHT_HISTOGRAM-1; y > HEIGHT_HISTOGRAM-1-lineHeight; y--)
            histogram.setPixel(x, y, qRgb(255, 0, 0));

        lineHeight = (HEIGHT_HISTOGRAM * h_G[x])/maxPixel;
        for(int y = 2*HEIGHT_HISTOGRAM-1; y > 2*HEIGHT_HISTOGRAM-1-lineHeight; y--)
            histogram.setPixel(x, y, qRgb(0, 255, 0));

        lineHeight = (HEIGHT_HISTOGRAM * h_B[x])/maxPixel;
        for(int y = 3*HEIGHT_HISTOGRAM-1; y > 3*HEIGHT_HISTOGRAM-1-lineHeight; y--)
            histogram.setPixel(x, y, qRgb(0, 0, 255));
    }

    return histogram;
}

/* Histogram slide method => increasing or decreasing BRIGHTNESS of an image */
QImage MainWindow::histogramSlideGray(QImage &imageIn, int c){
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);

    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height(); y++){
            int gray = qGray(imageIn.pixel(x, y));

            int grayOut = gray + c; // change gray out value aming to increasing or decreasing the brightness of the imageIn

            // check thresholding
            if(grayOut > 255)
                grayOut = 255;
            else if(grayOut < 0)
                grayOut = 0;

            imageOut.setPixel(x, y, qRgb(grayOut, grayOut, grayOut));
        }
    }

    return imageOut;
}

QImage MainWindow::histogramSlideColor(QImage &imageIn, int c){
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);

    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height(); y++){
            QColor colorIn = imageIn.pixel(x, y);
            int h, s, v;
            colorIn.getHsv(&h, &s, &v);

            int vOut = v + c; // change value out aming to increasing or decreasing the brightness of the imageIn

            // check thresholding
            if(vOut > 255)
                vOut = 255;
            else if(vOut < 0)
                vOut = 0;

            QColor colorOut = QColor::fromHsv(h, s, vOut);
            imageOut.setPixel(x, y, colorOut.rgb());
        }
    }

    return imageOut;
}

/* Histogram stretch method => increasing or decreasing CONTRAST of an image */
QImage MainWindow::histogramStretchGray(QImage &imageIn, double c){
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);

    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height(); y++){
            int gray = qGray(imageIn.pixel(x, y));

            int grayOut = int((double)gray * c); // change gray out value to increasing or decreasing the contrast of imageIn

            // check thresholding
            if(grayOut > 255)
                grayOut = 255;
            else if(grayOut < 0)
                grayOut = 0;

            imageOut.setPixel(x, y, qRgb(grayOut, grayOut, grayOut));
        }
    }

    return imageOut;
}

QImage MainWindow::histogramStretchColor(QImage &imageIn, double c){
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);

    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height(); y++){
            QColor colorIn = imageIn.pixel(x, y);
            int h, s, v;
            colorIn.getHsv(&h, &s, &v);

            int vOut = int((double)v * c); // change value out to increasing or decreasing the contrast of imageIn

            // check thresholding
            if(vOut > 255)
                vOut = 255;
            else if(vOut < 0)
                vOut = 0;

            QColor colorOut = QColor::fromHsv(h, s, vOut);
            imageOut.setPixel(x, y, colorOut.rgb());
        }
    }

    return imageOut;
}

QImage MainWindow::linearModificationGray(QImage &imageIn){
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);

    int maxValue = -1, minValue = 256;

    // traversing all pixels to find max and min pixels
    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height(); y++){
            int gray = qGray(imageIn.pixel(x, y));
            maxValue = max(gray, maxValue);
            minValue = min(gray, minValue);
        }
    }

    qDebug() << "Max value:" << maxValue;
    qDebug() << "Min value:" << minValue;

    /* traversing all pixels one again and update output pixels following the formula:
    O(x, y) = ((I(x, y) - min(I(x, y))) * 255) / (max(I(x, y)) - min(I(x, y))) */
    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height(); y++){
            int grayIn = qGray(imageIn.pixel(x, y));

            int grayOut = ((grayIn - minValue)*255) / (maxValue - minValue);

            // check thresholding
            if(grayOut > 255)
                grayOut = 255;
            else if(grayOut < 0)
                grayOut = 0;

            imageOut.setPixel(x, y, qRgb(grayOut, grayOut, grayOut));
        }
    }

    return imageOut;
}

QImage MainWindow::linearModificationColor(QImage &imageIn){
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);

    int maxValue = -1, minValue = 256;

    // traversing all pixels to find max and min pixels
    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height(); y++){
            QColor color = imageIn.pixel(x, y);
            int h, s, v;
            color.getHsv(&h, &s, &v);

            maxValue = max(v, maxValue);
            minValue = min(v, minValue);
        }
    }

    qDebug() << "Max value:" << maxValue;
    qDebug() << "Min value:" << minValue;

    /* traversing all pixels one again and update output pixels following the formula:
    O(x, y) = ((I(x, y) - min(I(x, y))) * 255) / (max(I(x, y)) - min(I(x, y))) */
    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height(); y++){
            QColor colorIn = imageIn.pixel(x, y);
            int h, s, v;
            colorIn.getHsv(&h, &s, &v);

            int vOut = ((v - minValue)*255) / (maxValue - minValue);

            // check thresholding
            if(vOut > 255)
                vOut = 255;
            else if(vOut < 0)
                vOut = 0;

            QColor colorOut = QColor::fromHsv(h, s, vOut);
            imageOut.setPixel(x, y, colorOut.rgb());
        }
    }

    return imageOut;
}

QImage MainWindow::histogramEqualizationGray(QImage &imageIn){
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);

    int h[256];
    float hn[256], C[256];
    for(int i = 0; i < 256; i++){
        h[i] = 0;
        hn[i] = 0;
        C[i] = 0;
    }

    // calculating histogram h[x]
    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height(); y++){
            int gray = qGray(imageIn.pixel(x, y));
            h[gray]++;
        }
    }

    // standardized histogram: hn[x] = h[x]/imageSize
    int imageSize = imageIn.width()*imageIn.height();
    for(int x = 0; x < 256; x++){
        hn[x] = (float)h[x] / (float)imageSize;
    }

    // calculating C[x] = C[x-1] + hn[x]
    C[0] = hn[0];
    for(int x = 1; x < 256; x++){
        C[x] = C[x-1] + hn[x];
    }

    /* traversing all pixels one again and update output pixels following the formula: O(x, y) = C(x, y) * 255  */
    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height(); y++){
            int gray = qGray(imageIn.pixel(x, y));

            int grayOut = C[gray] * 255;

            imageOut.setPixel(x, y, qRgb(grayOut, grayOut, grayOut));
        }
    }

    return imageOut;
}

QImage MainWindow::histogramEqualizationColor(QImage &imageIn){
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);

    int h[256];
    float hn[256], C[256];
    for(int i = 0; i < 256; i++){
        h[i] = 0;
        hn[i] = 0;
        C[i] = 0;
    }

    // calculating histogram h[x]
    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height(); y++){
            QColor color = imageIn.pixel(x, y);
            int hue, sat, value;
            color.getHsv(&hue, &sat, &value);
            h[value]++;
        }
    }

    // standardized histogram: hn[x] = h[x]/imageSize
    int imageSize = imageIn.width()*imageIn.height();
    for(int x = 0; x < 256; x++){
        hn[x] = (float)h[x] / (float)imageSize;
    }

    // calculating C[x] = C[x-1] + hn[x]
    C[0] = hn[0];
    for(int x = 1; x < 256; x++){
        C[x] = C[x-1] + hn[x];
    }

    /* traversing all pixels one again and update output pixels following the formula: O(x, y) = C(x, y) * 255  */
    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height(); y++){
            QColor colorIn = imageIn.pixel(x, y);
            int h, s, v;
            colorIn.getHsv(&h, &s, &v);

            int vOut = C[v] * 255;

            QColor colorOut = QColor::fromHsv(h, s, vOut);
            imageOut.setPixel(x, y, colorOut.rgb());
        }
    }

    return imageOut;
}

QImage MainWindow::gradientFilterGrayImageGx(QImage &imageIn){
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut.fill(Qt::white);

    int gray1 = 0, gray2 = 0, grayOut;

    for(int x = 0; x < imageIn.width()-1; x++){
        for(int y = 0; y < imageIn.height(); y++){
            QRgb rgb1 = imageIn.pixel(x, y);
            gray1 = qGray(rgb1);

            QRgb rgb2 = imageIn.pixel(x+1, y);
            gray2 = qGray(rgb2);

            // gx: O(x, y) = I(x+1, y) - I(x, y)
            grayOut = abs(gray2 - gray1);

            imageOut.setPixel(x, y, qRgb(grayOut, grayOut, grayOut));
        }
    }

    return imageOut;
}

QImage MainWindow::gradientFilterGrayImageGy(QImage &imageIn){
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut.fill(Qt::white);

    int gray1 = 0, gray2 = 0, grayOut;

    for(int x = 0; x < imageIn.width(); x++){
        for(int y = 0; y < imageIn.height()-1; y++){
            QRgb rgb1 = imageIn.pixel(x, y);
            gray1 = qGray(rgb1);

            QRgb rgb2 = imageIn.pixel(x, y+1);
            gray2 = qGray(rgb2);

            // gy: O(x, y) = I(x, y+1) - I(x, y)
            grayOut = abs(gray2 - gray1);

            imageOut.setPixel(x, y, qRgb(grayOut, grayOut, grayOut));
        }
    }

    return imageOut;
}

QImage MainWindow::gradientFilterColorImageGx(QImage &imageIn){
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

    return imageOut;
}

QImage MainWindow::gradientFilterColorImageGy(QImage &imageIn){
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

    return imageOut;
}

QImage MainWindow::robertFilterGrayImageGx(QImage &imageIn){
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut.fill(Qt::white);
    int gray1 = 0, gray2 = 0, grayOut;
    for(int x = 0; x < imageIn.width()-1; x++){
        for(int y = 0; y < imageIn.height()-1; y++){
            QRgb rgb1 = imageIn.pixel(x, y+1);
            gray1 = qGray(rgb1);

            QRgb rgb2 = imageIn.pixel(x+1, y);
            gray2 = qGray(rgb2);

            // gx: O(x, y) = I(x+1, y) - I(x, y+1)
            grayOut = abs(gray2 - gray1);

            imageOut.setPixel(x, y, qRgb(grayOut, grayOut, grayOut));
        }
    }

    return imageOut;
}

QImage MainWindow::robertFilterGrayImageGy(QImage &imageIn){
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut.fill(Qt::white);
    int gray1 = 0, gray2 = 0, grayOut;
    for(int x = 0; x < imageIn.width()-1; x++){
        for(int y = 0; y < imageIn.height()-1; y++){
            QRgb rgb1 = imageIn.pixel(x+1, y+1);
            gray1 = qGray(rgb1);

            QRgb rgb2 = imageIn.pixel(x, y);
            gray2 = qGray(rgb2);

            // gy: O(x, y) = I(x, y) - I(x+1, y+1)
            grayOut = abs(gray2 - gray1);

            imageOut.setPixel(x, y, qRgb(grayOut, grayOut, grayOut));
        }
    }

    return imageOut;
}

QImage MainWindow::robertFilterColorImageGx(QImage &imageIn){
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

    return imageOut;
}

QImage MainWindow::robertFilterColorImageGy(QImage &imageIn){
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

    return imageOut;
}

QImage MainWindow::sobelFilterGrayImageGx(QImage &imageIn){
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

                    sumMask += (gray * maskSobel[i+margin][j+margin]);
                }
            }

            imageOut.setPixel(x, y, qRgb(abs(sumMask), abs(sumMask), abs(sumMask)));
        }
    }

    return imageOut;
}

QImage MainWindow::sobelFilterGrayImageGy(QImage &imageIn){
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

                    sumMask += (gray * maskSobel[i+margin][j+margin]);
                }
            }

            imageOut.setPixel(x, y, qRgb(abs(sumMask), abs(sumMask), abs(sumMask)));
        }
    }

    return imageOut;
}

QImage MainWindow::sobelFilterColorImageGx(QImage &imageIn){
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut.fill(Qt::white);
    int maskSize = 3;
    int margin = maskSize/2;
    int maskSobel[maskSize][maskSize] = {{-1, 0, 1},
                                         {-2, 0, 2},
                                         {-1, 0, 1}};

    for(int x = margin; x < imageIn.width()-margin; x++){
        for(int y = margin; y < imageIn.height()-margin; y++){
            int sumR = 0;
            int sumG = 0;
            int sumB = 0;

            for(int i = -margin; i <= margin; i++){
                for(int j = -margin; j <= margin; j++){
                    QColor color = imageIn.pixel(x+j, y+i);
                    int r = color.red();
                    int g = color.green();
                    int b = color.blue();

                    sumR += (r * maskSobel[i+margin][j+margin]);
                    sumG += (g * maskSobel[i+margin][j+margin]);
                    sumB += (b * maskSobel[i+margin][j+margin]);
                }
            }

            imageOut.setPixel(x, y, qRgb(abs(sumR), abs(sumG), abs(sumB)));
        }
    }

    return imageOut;
}

QImage MainWindow::sobelFilterColorImageGy(QImage &imageIn){
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    int maskSize = 3;
    int margin = maskSize/3;
    int maskSobel[maskSize][maskSize] = {{-1, -2, -1},
                                         {0, 0, 0},
                                         {1, 2, 1}};

    for(int x = margin; x < imageIn.width()-margin; x++){
        for(int y = margin; y < imageIn.height()-margin; y++){
            int sumR = 0;
            int sumG = 0;
            int sumB = 0;

            for(int i = -margin; i <= margin; i++){
                for(int j = -margin; j <= margin; j++){
                    QColor color = imageIn.pixel(x+j, y+i);
                    int r = color.red();
                    int g = color.green();
                    int b = color.blue();

                    sumR += (r * maskSobel[i+margin][j+margin]);
                    sumG += (g * maskSobel[i+margin][j+margin]);
                    sumB += (b * maskSobel[i+margin][j+margin]);
                }
            }

            imageOut.setPixel(x, y, qRgb(abs(sumR), abs(sumG), abs(sumB)));
        }
    }

    return imageOut;
}

QImage MainWindow::prewittFilterGrayImageGx(QImage &imageIn){
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

                    sumMask += (gray * maskPrewitt[i+margin][j+margin]);
                }
            }
            imageOut.setPixel(x, y, qRgb(abs(sumMask), abs(sumMask), abs(sumMask)));
        }
    }

    return imageOut;
}

QImage MainWindow::prewittFilterGrayImageGy(QImage &imageIn){
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

    return imageOut;
}

QImage MainWindow::prewittFilterColorImageGx(QImage &imageIn){
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    int maskSize = 3;
    int margin = maskSize/3;
    int maskPrewitt[maskSize][maskSize] = {{-1, 0, 1},
                                           {-1, 0, 1},
                                           {-1, 0, 1}};

    for(int x = margin; x < imageIn.width()-margin; x++){
        for(int y = margin; y < imageIn.height()-margin; y++){
            int sumR = 0;
            int sumG = 0;
            int sumB = 0;

            for(int i = -margin; i <= margin; i++){
                for(int j = -margin; j <= margin; j++){
                    QColor color = imageIn.pixel(x+j, y+i);
                    int r = color.red();
                    int g = color.green();
                    int b = color.blue();

                    sumR += (r * maskPrewitt[i+margin][j+margin]);
                    sumG += (g * maskPrewitt[i+margin][j+margin]);
                    sumB += (b * maskPrewitt[i+margin][j+margin]);
                }
            }

            imageOut.setPixel(x, y, qRgb(abs(sumR), abs(sumG), abs(sumB)));
        }
    }

    return imageOut;
}

QImage MainWindow::prewittFilterColorImageGy(QImage &imageIn){
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    int maskSize = 3;
    int margin = maskSize/3;
    int maskPrewitt[maskSize][maskSize] = {{-1, -1, -1},
                                           {0, 0, 0},
                                           {1, 1, 1}};

    for(int x = margin; x < imageIn.width()-margin; x++){
        for(int y = margin; y < imageIn.height()-margin; y++){
            int sumR = 0;
            int sumG = 0;
            int sumB = 0;

            for(int i = -margin; i <= margin; i++){
                for(int j = -margin; j <= margin; j++){
                    QColor color = imageIn.pixel(x+j, y+i);
                    int r = color.red();
                    int g = color.green();
                    int b = color.blue();

                    sumR += (r * maskPrewitt[i+margin][j+margin]);
                    sumG += (g * maskPrewitt[i+margin][j+margin]);
                    sumB += (b * maskPrewitt[i+margin][j+margin]);
                }
            }

            imageOut.setPixel(x, y, qRgb(abs(sumR), abs(sumG), abs(sumB)));
        }
    }

    return imageOut;
}

QImage MainWindow::meanFilterGrayImage(QImage &imageIn){
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

    return imageOut;
}

QImage MainWindow::meanFilterColorImage(QImage &imageIn){
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut.fill(Qt::white);
    int size = 3;
    int cells = size*size;
    int margin = size/2;
    int sumR, sumG, sumB;

    for(int x = margin; x < imageIn.width()-margin; x++){
        for(int y = margin; y < imageIn.height()-margin; y++){
            sumR = 0; sumG = 0; sumB = 0;

            for(int i = -margin; i <= margin; i++){
                for(int j = -margin; j <= margin; j++){
                    QColor color = imageIn.pixel(x+i, y+j);
                    sumR += color.red();
                    sumG += color.green();
                    sumB += color.blue();
                }
            }

            imageOut.setPixel(x, y, qRgb(sumR/cells, sumG/cells, sumB/cells));
        }
    }

    return imageOut;
}

QImage MainWindow::medianFilterGrayImage(QImage &imageIn){
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

    return imageOut;
}

QImage MainWindow::medianFilterColorImage(QImage &imageIn){
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut.fill(Qt::white);
    int size = 3;
    int cells = size*size;
    int margin = size/2;
    int hR[cells], hG[cells], hB[cells];

    for(int x = margin; x < imageIn.width()-margin; x++){
        for(int y = margin; y < imageIn.height()-margin; y++){
            int r = 0, g = 0, b = 0;

            for(int i = -margin; i <= margin; i++){
                for(int j = -margin; j <= margin; j++){
                    QColor color = imageIn.pixel(x+i, y+j);
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

    return imageOut;
}

QImage MainWindow::dilationSequentialAlgo(QImage &imageIn){
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
    qDebug() << "thresholding for Sequential algorithm:" << thresholding; // result: 88
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

    /* post-segmentation */
    int size = 3;
    int kernel[size][size] = {{0, 1, 0},
                              {1, 1, 1},
                              {0, 1, 0}}; // cross shape
    int margin = size/2;
    int cGray;
    QImage imageOut2(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut2.fill(Qt::white);

    for(int x = margin; x < imageIn.width()-margin; x++){
        for(int y = margin; y < imageIn.height()-margin; y++){
            cGray = qGray(imageOut.pixel(x, y));
            if(cGray == 0){
                for(int i = -margin; i <= margin; i++){
                    for(int j = -margin; j <= margin; j++){
                        if(kernel[i+margin][j+margin]){
                            imageOut2.setPixel(x+i, y+j, qRgb(0, 0, 0));
                        }
                    }
                }
            }
        }
    }

    return imageOut2;
}

QImage MainWindow::erosionSequentialAlgo(QImage &imageIn){
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
    qDebug() << "thresholding for Sequential algorithm:" << thresholding; // result: 88
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

    /* post-segmentation */
    int size = 3;
    int kernel[size][size] = {{0, 1, 0},
                              {1, 1, 1},
                              {0, 1, 0}}; // cross shape
    int margin = size/2;
    int cGray;
    QImage imageOut2(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut2.fill(Qt::white);

    for(int x = margin; x < imageIn.width()-margin; x++){
        for(int y = margin; y < imageIn.height()-margin; y++){
            int ok = 1;
            for(int i = -margin; i <= margin; i++){
                for(int j = -margin; j <= margin; j++){
                    cGray = qGray(imageOut.pixel(x+i, y+j));
                    ok = ok && (kernel[i][j] == 0 || cGray == 0);
                }
            }
            if(ok)
                imageOut2.setPixel(x, y, qRgb(0, 0, 0));
        }
    }

    return imageOut2;
}

QImage MainWindow::openSequentialAlgo(QImage &imageIn){
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
    qDebug() << "thresholding for Sequential algorithm:" << thresholding; // result: 88
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

    int size = 3;
    int kernel[size][size] = {{0, 1, 0},
                              {1, 1, 1},
                              {0, 1, 0}}; // cross shape
    int margin = size/2;
    int cGray;
    QImage imageOut2(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut2.fill(Qt::white);

    /* post-segmentation: erosion */
    for(int x = margin; x < imageIn.width()-margin; x++){
        for(int y = margin; y < imageIn.height()-margin; y++){
            int ok = 1;
            for(int i = -margin; i <= margin; i++){
                for(int j = -margin; j <= margin; j++){
                    cGray = qGray(imageOut.pixel(x+i, y+j));
                    ok = ok && (kernel[i][j] == 0 || cGray == 0);
                }
            }
            if(ok)
                imageOut2.setPixel(x, y, qRgb(0, 0, 0));
        }
    }

    QImage imageOut3(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut3.fill(Qt::white);

    /* post-segmentation: dilation */
    for(int x = margin; x < imageIn.width()-margin; x++){
        for(int y = margin; y < imageIn.height()-margin; y++){
            cGray = qGray(imageOut2.pixel(x, y));
            if(cGray == 0){
                for(int i = -margin; i <= margin; i++){
                    for(int j = -margin; j <= margin; j++){
                        if(kernel[i+margin][j+margin]){
                            imageOut3.setPixel(x+i, y+j, qRgb(0, 0, 0));
                        }
                    }
                }
            }
        }
    }

    return imageOut3;
}

QImage MainWindow::closeSequentialAlgo(QImage &imageIn){
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
    qDebug() << "thresholding for Sequential algorithm:" << thresholding; // result: 88
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

    int size = 3;
    int kernel[size][size] = {{0, 1, 0},
                              {1, 1, 1},
                              {0, 1, 0}}; // cross shape
    int margin = size/2;
    int cGray;
    QImage imageOut2(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut2.fill(Qt::white);

    /* post-segmentation: dilation */
    for(int x = margin; x < imageIn.width()-margin; x++){
        for(int y = margin; y < imageIn.height()-margin; y++){
            cGray = qGray(imageOut.pixel(x, y));
            if(cGray == 0){
                for(int i = -margin; i <= margin; i++){
                    for(int j = -margin; j <= margin; j++){
                        if(kernel[i+margin][j+margin]){
                            imageOut2.setPixel(x+i, y+j, qRgb(0, 0, 0));
                        }
                    }
                }
            }
        }
    }

    QImage imageOut3(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut3.fill(Qt::white);

    /* post-segmentation: erosion */
    for(int x = margin; x < imageIn.width()-margin; x++){
        for(int y = margin; y < imageIn.height()-margin; y++){
            int ok = 1;
            for(int i = -margin; i <= margin; i++){
                for(int j = -margin; j <= margin; j++){
                    cGray = qGray(imageOut2.pixel(x+i, y+j));
                    ok = ok && (kernel[i][j] == 0 || cGray == 0);
                }
            }
            if(ok)
                imageOut3.setPixel(x, y, qRgb(0, 0, 0));
        }
    }

    return imageOut3;
}

QImage MainWindow::dilationOtsuAlgo(QImage &imageIn){
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);

    // initialize variables
    int sumCurrent = 0;
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

    // calculate (x * h(x)) for the overall histogram
    int sum = 0;
    for(int x = 0; x < maxIntensity; x++){
        sum += (x * histogram[x]);
    }

    /* choose thresholding */
    for (int t = 0; t <  maxIntensity; t++){
        // calculate count1 for the current t
        count1 += histogram[t];

        if(count1 == 0)
            continue;

        // calculate count2 for the current t
        count2 = nbPixels - count1;

        sumCurrent += (t*histogram[t]);

        // calculate mean value for the first class
        float mean1 = (float)sumCurrent/(float)count1;
        // calculate mean value for the second class
        float mean2 = ((float)sum - (float)sumCurrent)/(float)count2;

        // calculate the between-class varience
        float variance = (float)count1 * (float)count2 * (mean2 - mean1) * (mean2 - mean1);

        // check if new maximum variance found
        if(variance > maxVariance){
            maxVariance = variance; // update maximum variance
            thresholding = t; // choose the thresholding with the maximum variance
        }
    }

    /* segmentation */
    qDebug() << "thresholding for Otsu algorithm:" << thresholding; // result: 88
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

    /* post-segmentation */
    int size = 3;
    int kernel[size][size] = {{0, 1, 0},
                              {1, 1, 1},
                              {0, 1, 0}}; // cross shape
    int margin = size/2;
    int cGray;
    QImage imageOut2(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut2.fill(Qt::white);

    for(int x = margin; x < imageIn.width()-margin; x++){
        for(int y = margin; y < imageIn.height()-margin; y++){
            cGray = qGray(imageOut.pixel(x, y));
            if(cGray == 0){
                for(int i = -margin; i <= margin; i++){
                    for(int j = -margin; j <= margin; j++){
                        if(kernel[i+margin][j+margin]){
                            imageOut2.setPixel(x+i, y+j, qRgb(0, 0, 0));
                        }
                    }
                }
            }
        }
    }

    return imageOut2;
}

QImage MainWindow::erosionOtsuAlgo(QImage &imageIn){
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);

    // initialize variables
    int sumCurrent = 0;
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

    // calculate (x * h(x)) for the overall histogram
    int sum = 0;
    for(int x = 0; x < maxIntensity; x++){
        sum += (x * histogram[x]);
    }

    /* choose thresholding */
    for (int t = 0; t <  maxIntensity; t++){
        // calculate count1 for the current t
        count1 += histogram[t];

        if(count1 == 0)
            continue;

        // calculate count2 for the current t
        count2 = nbPixels - count1;

        sumCurrent += (t*histogram[t]);

        // calculate mean value for the first class
        float mean1 = (float)sumCurrent/(float)count1;
        // calculate mean value for the second class
        float mean2 = ((float)sum - (float)sumCurrent)/(float)count2;

        // calculate the between-class varience
        float variance = (float)count1 * (float)count2 * (mean2 - mean1) * (mean2 - mean1);

        // check if new maximum variance found
        if(variance > maxVariance){
            maxVariance = variance; // update maximum variance
            thresholding = t; // choose the thresholding with the maximum variance
        }
    }

    /* segmentation */
    qDebug() << "thresholding for Otsu algorithm:" << thresholding; // result: 88
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

    /* post-segmentation */
    int size = 3;
    int kernel[size][size] = {{0, 1, 0},
                              {1, 1, 1},
                              {0, 1, 0}}; // cross shape
    int margin = size/2;
    int cGray;
    QImage imageOut2(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut2.fill(Qt::white);

    for(int x = margin; x < imageIn.width()-margin; x++){
        for(int y = margin; y < imageIn.height()-margin; y++){
            int ok = 1;
            for(int i = -margin; i <= margin; i++){
                for(int j = -margin; j <= margin; j++){
                    cGray = qGray(imageOut.pixel(x+i, y+j));
                    ok = ok && (kernel[i][j] == 0 || cGray == 0);
                }
            }
            if(ok)
                imageOut2.setPixel(x, y, qRgb(0, 0, 0));
        }
    }

    return imageOut2;
}

QImage MainWindow::openOtsuAlgo(QImage &imageIn){
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);

    // initialize variables
    int sumCurrent = 0;
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

    // calculate (x * h(x)) for the overall histogram
    int sum = 0;
    for(int x = 0; x < maxIntensity; x++){
        sum += (x * histogram[x]);
    }

    /* choose thresholding */
    for (int t = 0; t <  maxIntensity; t++){
        // calculate count1 for the current t
        count1 += histogram[t];

        if(count1 == 0)
            continue;

        // calculate count2 for the current t
        count2 = nbPixels - count1;

        sumCurrent += (t*histogram[t]);

        // calculate mean value for the first class
        float mean1 = (float)sumCurrent/(float)count1;
        // calculate mean value for the second class
        float mean2 = ((float)sum - (float)sumCurrent)/(float)count2;

        // calculate the between-class varience
        float variance = (float)count1 * (float)count2 * (mean2 - mean1) * (mean2 - mean1);

        // check if new maximum variance found
        if(variance > maxVariance){
            maxVariance = variance; // update maximum variance
            thresholding = t; // choose the thresholding with the maximum variance
        }
    }

    /* segmentation */
    qDebug() << "thresholding for Otsu algorithm:" << thresholding; // result: 88
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

    int size = 3;
    int kernel[size][size] = {{0, 1, 0},
                              {1, 1, 1},
                              {0, 1, 0}}; // cross shape
    int margin = size/2;
    int cGray;
    QImage imageOut2(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut2.fill(Qt::white);

    /* post-segmentation: erosion */
    for(int x = margin; x < imageIn.width()-margin; x++){
        for(int y = margin; y < imageIn.height()-margin; y++){
            int ok = 1;
            for(int i = -margin; i <= margin; i++){
                for(int j = -margin; j <= margin; j++){
                    cGray = qGray(imageOut.pixel(x+i, y+j));
                    ok = ok && (kernel[i][j] == 0 || cGray == 0);
                }
            }
            if(ok)
                imageOut2.setPixel(x, y, qRgb(0, 0, 0));
        }
    }

    QImage imageOut3(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut3.fill(Qt::white);

    /* post-segmentation: dilation */
    for(int x = margin; x < imageIn.width()-margin; x++){
        for(int y = margin; y < imageIn.height()-margin; y++){
            cGray = qGray(imageOut2.pixel(x, y));
            if(cGray == 0){
                for(int i = -margin; i <= margin; i++){
                    for(int j = -margin; j <= margin; j++){
                        if(kernel[i+margin][j+margin]){
                            imageOut3.setPixel(x+i, y+j, qRgb(0, 0, 0));
                        }
                    }
                }
            }
        }
    }

    return imageOut3;
}

QImage MainWindow::closeOtsuAlgo(QImage &imageIn){
    QImage imageOut(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);

    // initialize variables
    int sumCurrent = 0;
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

    // calculate (x * h(x)) for the overall histogram
    int sum = 0;
    for(int x = 0; x < maxIntensity; x++){
        sum += (x * histogram[x]);
    }

    /* choose thresholding */
    for (int t = 0; t <  maxIntensity; t++){
        // calculate count1 for the current t
        count1 += histogram[t];

        if(count1 == 0)
            continue;

        // calculate count2 for the current t
        count2 = nbPixels - count1;

        sumCurrent += (t*histogram[t]);

        // calculate mean value for the first class
        float mean1 = (float)sumCurrent/(float)count1;
        // calculate mean value for the second class
        float mean2 = ((float)sum - (float)sumCurrent)/(float)count2;

        // calculate the between-class varience
        float variance = (float)count1 * (float)count2 * (mean2 - mean1) * (mean2 - mean1);

        // check if new maximum variance found
        if(variance > maxVariance){
            maxVariance = variance; // update maximum variance
            thresholding = t; // choose the thresholding with the maximum variance
        }
    }

    /* segmentation */
    qDebug() << "thresholding for Otsu algorithm:" << thresholding; // result: 88
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

    int size = 3;
    int kernel[size][size] = {{0, 1, 0},
                              {1, 1, 1},
                              {0, 1, 0}}; // cross shape
    int margin = size/2;
    int cGray;
    QImage imageOut2(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut2.fill(Qt::white);

    /* post-segmentation: dilation */
    for(int x = margin; x < imageIn.width()-margin; x++){
        for(int y = margin; y < imageIn.height()-margin; y++){
            cGray = qGray(imageOut.pixel(x, y));
            if(cGray == 0){
                for(int i = -margin; i <= margin; i++){
                    for(int j = -margin; j <= margin; j++){
                        if(kernel[i+margin][j+margin]){
                            imageOut2.setPixel(x+i, y+j, qRgb(0, 0, 0));
                        }
                    }
                }
            }
        }
    }

    QImage imageOut3(imageIn.width(), imageIn.height(), QImage::Format_ARGB32);
    imageOut3.fill(Qt::white);

    /* post-segmentation: erosion */
    for(int x = margin; x < imageIn.width()-margin; x++){
        for(int y = margin; y < imageIn.height()-margin; y++){
            int ok = 1;
            for(int i = -margin; i <= margin; i++){
                for(int j = -margin; j <= margin; j++){
                    cGray = qGray(imageOut2.pixel(x+i, y+j));
                    ok = ok && (kernel[i][j] == 0 || cGray == 0);
                }
            }
            if(ok)
                imageOut3.setPixel(x, y, qRgb(0, 0, 0));
        }
    }

    return imageOut3;
}

void MainWindow::on_actionOpen_File_triggered(){
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", "../../images",
                                                    "*.* All Files;;*.bmp;;*.jpeg;;*.jpg;;*.png;;*.ppm");
    QImage image(fileName);
    displayImage(image, QFileInfo(fileName).fileName());
}

void MainWindow::on_actionSave_File_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage image = activeWidget->getImage();
    QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(this, "Save Images", "../../image_out",
                                                    "BMP (*.bmp);;PNG (*.png);;JPEG (*.jpg);;All files (*.*)",
                                                    &selectedFilter);
    if(selectedFilter == "BMP (*.bmp)")
        image.save(fileName, "BMP");
    else if(selectedFilter == "PNG (*.png)")
        image.save(fileName, "PNG");
    else if(selectedFilter == "JPEG (*.jpg)")
        image.save(fileName, "JPEG");
    else
        image.save(fileName);
}

void MainWindow::on_actionExit_File_triggered(){
    QApplication::exit();
}

void MainWindow::on_actionShow_Histogram_Gray_Image_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = drawHisGray(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Histogram_Grayscale_Image");
}

void MainWindow::on_actionShow_Histogram_Color_Image_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = drawHisColor(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Histogram_Color_Image");
}

void MainWindow::on_actionHistogram_Slide_Gray_Image_triggered(){
    bool ok;
    int c = QInputDialog::getInt(this, tr("Histogram Slide"),
                               tr("Enter c:"), 25, 0, 100, 1, &ok);
    if(ok){
        QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
        if(activeWindow == 0){
            return;
        }
        ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
        QImage imageIn = activeWidget->getImage();
        QImage imageOut = histogramSlideGray(imageIn, c);
        displayImage(imageOut, activeWindow->windowTitle()+"_Histogram_Slide_Grayscale_Image");
    }
}

void MainWindow::on_actionHistogram_Slide_Color_Image_triggered(){
    bool ok;
    int c = QInputDialog::getInt(this, tr("Histogram Slide"),
                               tr("Enter c:"), 25, 0, 100, 1, &ok);
    if(ok){
        QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
        if(activeWindow == 0){
            return;
        }
        ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
        QImage imageIn = activeWidget->getImage();
        QImage imageOut = histogramSlideColor(imageIn, c);
        displayImage(imageOut, activeWindow->windowTitle()+"_Histogram_Slide_Color_Image");
    }
}

void MainWindow::on_actionHistogram_Stretch_Gray_Image_triggered(){
    bool ok;
    double c = QInputDialog::getDouble(this, tr("Histogram Slide"),
                                 tr("Enter c:"), 37.56, -10000, 10000, 2, &ok);
    if (ok){
        QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
        if(activeWindow == 0){
            return;
        }
        ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
        QImage imageIn = activeWidget->getImage();
        QImage imageOut = histogramStretchGray(imageIn, c);
        displayImage(imageOut, activeWindow->windowTitle()+"_Histogram_Stretch_Grayscale_Image");
    }
}

void MainWindow::on_actionHistogram_Stretch_Color_Image_triggered(){
    bool ok;
    double c = QInputDialog::getDouble(this, tr("Histogram Slide"),
                                 tr("Enter c:"), 37.56, -10000, 10000, 2, &ok);
    if (ok){
        QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
        if(activeWindow == 0){
            return;
        }
        ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
        QImage imageIn = activeWidget->getImage();
        QImage imageOut = histogramStretchColor(imageIn, c);
        displayImage(imageOut, activeWindow->windowTitle()+"_Histogram_Stretch_Color_Image");
    }
}

void MainWindow::on_actionLinear_Modification_Gray_Image_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = linearModificationGray(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Linear_Modification_Grayscale_Image");
}

void MainWindow::on_actionLinear_Modification_Color_Image_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = linearModificationColor(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Linear_Modification_Color_Image");
}

void MainWindow::on_actionHistogram_Equalization_Gray_Image_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = histogramEqualizationGray(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Histogram_Equalization_Grayscale_Image");
}

void MainWindow::on_actionHistogram_Equalization_Color_Image_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = histogramEqualizationColor(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Histogram_Equalization_Color_Image");
}

void MainWindow::on_actionGradient_Filter_Gray_Image_Gx_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = gradientFilterGrayImageGx(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Gradient_Filter_Grayscale_Image_Gx");
}

void MainWindow::on_actionGradient_Filter_Gray_Image_Gy_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = gradientFilterGrayImageGy(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Gradient_Filter_Grayscale_Image_Gy");
}

void MainWindow::on_actionGradient_Filter_Color_Image_Gx_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = gradientFilterColorImageGx(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Gradient_Filter_Color_Image_Gx");
}

void MainWindow::on_actionGradient_Filter_Color_Image_Gy_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = gradientFilterColorImageGy(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Gradient_Filter_Color_Image_Gy");
}

void MainWindow::on_actionRobert_Filter_Gray_Image_Gx_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = robertFilterGrayImageGx(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Robert_Filter_Grayscale_Image_Gx");
}

void MainWindow::on_actionRobert_Filter_Gray_Image_Gy_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = robertFilterGrayImageGy(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Robert_Filter_Grayscale_Image_Gy");
}

void MainWindow::on_actionRobert_Filter_Color_Image_Gx_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = robertFilterColorImageGx(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Robert_Filter_Color_Image_Gx");
}

void MainWindow::on_actionRobert_Filter_Color_Image_Gy_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = robertFilterColorImageGy(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Robert_Filter_Color_Image_Gy");
}

void MainWindow::on_actionSobel_Filter_Gray_Image_Gx_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = sobelFilterGrayImageGx(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Sobel_Filter_Grayscale_Image_Gx");
}

void MainWindow::on_actionSobel_Filter_Gray_Image_Gy_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = sobelFilterGrayImageGy(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Sobel_Filter_Grayscale_Image_Gy");
}

void MainWindow::on_actionSobel_Filter_Color_Image_Gx_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = sobelFilterColorImageGx(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Sobel_Filter_Color_Image_Gx");
}

void MainWindow::on_actionSobel_Filter_Color_Image_Gy_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = sobelFilterColorImageGy(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Sobel_Filter_Color_Image_Gy");
}

void MainWindow::on_actionPrewitt_Filter_Gray_Image_Gx_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = prewittFilterGrayImageGx(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Prewitt_Filter_Grayscale_Image_Gx");
}

void MainWindow::on_actionPrewitt_Filter_Gray_Image_Gy_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = prewittFilterGrayImageGy(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Prewitt_Filter_Grayscale_Image_Gy");
}

void MainWindow::on_actionPrewitt_Filter_Color_Image_Gx_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = prewittFilterColorImageGx(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Prewitt_Filter_Color_Image_Gx");
}

void MainWindow::on_actionPrewitt_Filter_Color_Image_Gy_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = prewittFilterColorImageGy(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Prewitt_Filter_Color_Image_Gy");
}

void MainWindow::on_actionMean_Filter_Gray_Image_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = meanFilterGrayImage(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Mean_Filter_Grayscale_Image");
}

void MainWindow::on_actionMean_Filter_Color_Image_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = meanFilterColorImage(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Mean_Filter_Color_Image");
}

void MainWindow::on_actionMedian_Filter_Gray_Image_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = medianFilterGrayImage(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Median_Filter_Grayscale_Image");
}

void MainWindow::on_actionMedian_Filter_Color_Image_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = medianFilterColorImage(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Median_Filter_Color_Image");
}

void MainWindow::on_actionDilation_Sequential_Algorithm_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = dilationSequentialAlgo(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Dilation_Sequential_Algorithm");
}

void MainWindow::on_actionErosion_Sequential_Algorithm_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = erosionSequentialAlgo(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Erosion_Sequential_Algorithm");
}

void MainWindow::on_actionClose_Sequential_Algorithm_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = closeSequentialAlgo(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Close_Sequential_Algorithm");
}

void MainWindow::on_actionOpen_Sequential_Algorithm_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = openSequentialAlgo(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Open_Sequential_Algorithm");
}

void MainWindow::on_actionDilation_Otsu_Algorithm_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = dilationOtsuAlgo(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Dilation_Otsu_Algorithm");
}

void MainWindow::on_actionErosion_Otsu_Algorithm_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = erosionOtsuAlgo(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Erosion_Otsu_Algorithm");
}

void MainWindow::on_actionClose_Otsu_Algorithm_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = closeOtsuAlgo(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Close_Otsu_Algorithm");
}

void MainWindow::on_actionOpen_Otsu_Algorithm_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = openOtsuAlgo(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Open_Otsu_Algorithm");
}
