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
    int max(int num1, int num2);
    int min(int num1, int num2);
    QImage drawHisGray(QImage &img);
    void drawHisColor(QImage &img);


private slots:
    void on_btn_browse_clicked();

    void on_btn_hisSlide_gray_clicked();

    void on_btn_hisSlide_Color_clicked();

    void on_btn_hisStretch_gray_clicked();

    void on_btn_hisStretch_Color_clicked();

    void on_btn_segmentation_gray_clicked();

    void on_btn_showHis_gray_clicked();

    void on_btn_showHis_Color_clicked();

    void on_btn_linearModification_gray_clicked();

    void on_btn_histogramEqual_gray_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
