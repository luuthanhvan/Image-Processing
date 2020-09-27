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

private slots:
    void on_btn_browse_clicked();
    void on_btn_hisSlide_gray_clicked();

    void on_btn_hisSlide_Color_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
