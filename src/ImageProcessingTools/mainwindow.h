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
    QImage drawHisGray(QImage &imageIn);
    QImage drawHisColor(QImage &imageIn);

private slots:
    void on_actionOpen_File_triggered();

    void on_actionSave_File_triggered();

    void on_actionExit_File_triggered();

    void on_actionShow_Histogram_Gray_Image_triggered();

    void on_actionShow_Histogram_Color_Image_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
