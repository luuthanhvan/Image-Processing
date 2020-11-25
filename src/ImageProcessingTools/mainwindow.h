#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT
    QMdiArea *mdiArea;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void displayImage(QImage &image, QString title);
    int findMaxPixel(int pixels[]);
    int max(int num1, int num2);
    int min(int num1, int num2);

    QImage drawHisGray(QImage &imageIn);
    QImage drawHisColor(QImage &imageIn);

    /* Some methods used to improve the brightness and contrast of an image */
    QImage histogramSlideGray(QImage &imageIn, int c);
    QImage histogramSlideColor(QImage &imageIn, int c);

    QImage histogramStretchGray(QImage &imageIn, double c);
    QImage histogramStretchColor(QImage &imageIn, double c);

    QImage linearModificationGray(QImage &imageIn);
    QImage linearModificationColor(QImage &imageIn);

    QImage histogramEqualizationGray(QImage &imageIn);
    QImage histogramEqualizationColor(QImage &imageIn);

    /* Some filters used to extract the border of an image */
    QImage gradientFilterGrayImageGx(QImage &imageIn);
    QImage gradientFilterGrayImageGy(QImage &imageIn);
    QImage gradientFilterColorImageGx(QImage &imageIn);
    QImage gradientFilterColorImageGy(QImage &imageIn);

    QImage robertFilterGrayImageGx(QImage &imageIn);
    QImage robertFilterGrayImageGy(QImage &imageIn);
    QImage robertFilterColorImageGx(QImage &imageIn);
    QImage robertFilterColorImageGy(QImage &imageIn);

    QImage sobelFilterGrayImageGx(QImage &imageIn);
    QImage sobelFilterGrayImageGy(QImage &imageIn);
    QImage sobelFilterColorImageGx(QImage &imageIn);
    QImage sobelFilterColorImageGy(QImage &imageIn);

    QImage prewittFilterGrayImageGx(QImage &imageIn);
    QImage prewittFilterGrayImageGy(QImage &imageIn);
    QImage prewittFilterColorImageGx(QImage &imageIn);
    QImage prewittFilterColorImageGy(QImage &imageIn);

    /* Some filters used to make an image smooth */
    QImage meanFilterGrayImage(QImage &imageIn);
    QImage meanFilterColorImage(QImage &imageIn);

    QImage medianFilterGrayImage(QImage &imageIn);
    QImage medianFilterColorImage(QImage &imageIn);

    /* Some methods are used for post-segmentation phrase */
    QImage dilationSequentialAlgo(QImage &imageIn);
    QImage erosionSequentialAlgo(QImage &imageIn);
    QImage openSequentialAlgo(QImage &imageIn);
    QImage closeSequentialAlgo(QImage &imageIn);

    QImage dilationOtsuAlgo(QImage &imageIn);
    QImage erosionOtsuAlgo(QImage &imageIn);
    QImage openOtsuAlgo(QImage &imageIn);
    QImage closeOtsuAlgo(QImage &imageIn);

private slots:
    void on_actionOpen_File_triggered();

    void on_actionSave_File_triggered();

    void on_actionExit_File_triggered();

    void on_actionShow_Histogram_Gray_Image_triggered();

    void on_actionShow_Histogram_Color_Image_triggered();

    void on_actionHistogram_Slide_Gray_Image_triggered();

    void on_actionHistogram_Slide_Color_Image_triggered();

    void on_actionHistogram_Stretch_Gray_Image_triggered();

    void on_actionHistogram_Stretch_Color_Image_triggered();

    void on_actionLinear_Modification_Gray_Image_triggered();

    void on_actionLinear_Modification_Color_Image_triggered();

    void on_actionHistogram_Equalization_Gray_Image_triggered();

    void on_actionHistogram_Equalization_Color_Image_triggered();

    void on_actionGradient_Filter_Gray_Image_Gx_triggered();

    void on_actionGradient_Filter_Gray_Image_Gy_triggered();

    void on_actionGradient_Filter_Color_Image_Gx_triggered();

    void on_actionGradient_Filter_Color_Image_Gy_triggered();

    void on_actionRobert_Filter_Gray_Image_Gx_triggered();

    void on_actionRobert_Filter_Gray_Image_Gy_triggered();

    void on_actionRobert_Filter_Color_Image_Gx_triggered();

    void on_actionRobert_Filter_Color_Image_Gy_triggered();

    void on_actionSobel_Filter_Gray_Image_Gx_triggered();

    void on_actionSobel_Filter_Gray_Image_Gy_triggered();

    void on_actionSobel_Filter_Color_Image_Gx_triggered();

    void on_actionSobel_Filter_Color_Image_Gy_triggered();

    void on_actionPrewitt_Filter_Gray_Image_Gx_triggered();

    void on_actionPrewitt_Filter_Gray_Image_Gy_triggered();

    void on_actionPrewitt_Filter_Color_Image_Gx_triggered();

    void on_actionPrewitt_Filter_Color_Image_Gy_triggered();

    void on_actionMean_Filter_Gray_Image_triggered();

    void on_actionMean_Filter_Color_Image_triggered();

    void on_actionMedian_Filter_Gray_Image_triggered();

    void on_actionMedian_Filter_Color_Image_triggered();

    void on_actionDilation_Sequential_Algorithm_triggered();

    void on_actionErosion_Sequential_Algorithm_triggered();

    void on_actionClose_Sequential_Algorithm_triggered();

    void on_actionOpen_Sequential_Algorithm_triggered();

    void on_actionDilation_Otsu_Algorithm_triggered();

    void on_actionErosion_Otsu_Algorithm_triggered();

    void on_actionClose_Otsu_Algorithm_triggered();

    void on_actionOpen_Otsu_Algorithm_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
