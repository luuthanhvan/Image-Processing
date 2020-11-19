#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void displayImage(QImage &img, QString title);
    void saveImage(QImage &imgOut);

private slots:
    void on_btn_browse_clicked();

    void on_btn_meanFilter_gray_clicked();

    void on_btn_meanFilter_Color_clicked();

    void on_btn_medianFilter_gray_clicked();

    void on_btn_medianFilter_Color_clicked();

    void on_btn_gradientFilter_Gx_Gray_clicked();

    void on_btn_gradientFilter_Gy_Gray_clicked();

    void on_btn_gradientFilter_Gx_Color_clicked();

    void on_btn_gradientFilter_Gy_Color_clicked();

    void on_btn_roberFilter_Gx_Gray_clicked();

    void on_btn_roberFilter_Gy_Gray_clicked();

    void on_btn_roberFilter_Gx_Color_clicked();

    void on_btn_roberFilter_Gy_Color_clicked();

    void on_btn_sobelFilter_Gx_Gray_clicked();

    void on_btn_sobelFilter_Gy_Gray_clicked();

    void on_btn_sobelFilter_Gx_Color_clicked();

    void on_btn_sobelFilter_Gy_Color_clicked();

    void on_btn_autoSegmentation_Gray_clicked();

    void on_btn_autoSegmentation_Otsu_Gray_clicked();


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
