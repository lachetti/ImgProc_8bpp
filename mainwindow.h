#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_ApplyButton_clicked();

    void on_filterSize_valueChanged(int value);

    void on_useResult_clicked();

    void on_openButton_clicked();

    void on_reload_clicked();

    void on_noizeAdd_clicked();

    void on_noizeMul_clicked();

    void on_noizeImp_clicked();

    void on_inverce_clicked();

    void plotBrightHist();

    void on_openTIFF_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
