#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imagewidget.h"
#include <QMdiSubWindow>
#include <QFileDialog>

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
    displayImage(imageOut, activeWindow->windowTitle()+"_Histogram");
}

void MainWindow::on_actionShow_Histogram_Color_Image_triggered(){
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if(activeWindow == 0){
        return;
    }
    ImageWidget *activeWidget = (ImageWidget*)activeWindow->widget();
    QImage imageIn = activeWidget->getImage();
    QImage imageOut = drawHisColor(imageIn);
    displayImage(imageOut, activeWindow->windowTitle()+"_Histogram");
}
