#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "imgproc.h"

static imgProc process; // static object of my class

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->MomentsView->setColumnWidth(0, 25);
    ui->histBright->xAxis->setRange(-1, 256);
    ui->histBright->xAxis->setTickLabelFont(QFont(QFont().family(), 7));

    std::srand(9);
    int imgN = (int)(((double)rand() / (double)RAND_MAX) * 32) + 1;
    QString src_file("/home/lachetti/albums/" + QString::number(imgN) + ".jpg");
    QImage src_load(src_file);
    process.setSource(src_load.scaled(ui->src->width(), ui->src->height(), Qt::KeepAspectRatio));
    process.result = process.source;
    ui->src->setPixmap(QPixmap::fromImage(process.source));
    ui->res->setPixmap(QPixmap::fromImage(process.result));
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ApplyButton_clicked()
{
    ui->processing->setDisabled(true);

    QApplication::processEvents();

    if (ui->Labwork_1->isChecked()) {            // Лаба №1
        switch(ui->Labwork_1_method->currentIndex()) {
        case 0: {   // Наложение
            if (process.Cs.isNull()) {
                QString Cs_file("/home/lachetti/albums/1.jpg");
                QImage Cs_load(Cs_file);
                process.setCs(Cs_load.scaled(ui->src->width(), ui->src->height(), Qt::KeepAspectRatio));
            }
            if (process.alpha.isNull()) {
                QString alpha_file("/home/lachetti/albums/8.jpg");
                QImage alpha_load(alpha_file);
                process.setAlpha(alpha_load.scaled(ui->src->width(), ui->src->height(), Qt::KeepAspectRatio));
            }
            process.CompostingAlpha();
            break;
        }
        case 1: {   // Отражение по вертикали
            process.FlipV();
            break;
        }
        case 2: {   // Отражение по горизонтали
            process.FlipH();
            break;
        }
        case 3: {   // Транспонирование
            process.Transpose();
            break;
        }
        }
    }

    if (ui->Labwork_2->isChecked()) {            // Лаба №2
        // Матрица совместной встречаемости
        process.calcCoOccurenceMatrix(ui->r_cooc->text().toInt(), ui->c_cooc->text().toInt());
        ui->com->setPixmap(QPixmap::fromImage(process.com.scaled(ui->com->width(), ui->com->height(), Qt::KeepAspectRatio)));
        ui->lab2_com_uniformity->setText(QString::number(process.comUniformity));
    }

    if (ui->Labwork_3->isChecked()) {            // Лаба №3
        // Поворот с кубической интерполяцией
        process.rotateImage(ui->angle_dg->text().toDouble());
    }

    if (ui->Labwork_4->isChecked()) {            // Лаба №4
        // Алгоритм рассеяния ошибки Флойда-Стенберга
        process.FloydStenbergESA(ui->to_bbp->text().toInt());
    }

    if (ui->Labwork_5->isChecked()) {            // Лаба №5
        switch(ui->Labwork_5_method->currentIndex()) {
        case 0: {
            // ФНЧ Усредняющий
            process.LowPassFilterSpatial();
            break;
        }
        case 1: {
            // ФНЧ Гауссиан
            process.LowPassFilterGauss();
            break;
        }
        case 2: {
            // ФВЧ Превитта
            process.HighPassFilterPrevitt();
            break;
        }
        case 3: {
            // ФВЧ Собеля
            process.HighPassFilterSobel();
            break;
        }
        case 4: {
            // Повышение резкости
            process.SharpenFilter();
            break;
        }
        case 5: {
            // Билатеральный фильтр
            process.BilateralFilter(3, 10, 100);//(int S, int sigD, int sigR);
            break;
        }
        case 6: {
            // Пользовательский фильтр
            QVector<QVector<double>> kernel(3);
            for(int i=0; i < 3; i++) {
                kernel[i].fill(0.0, 3);
                for (int j=0; j < 3; j++) {
                    kernel[i][j] = ui->userKernel->item(i, j)->text().toDouble();
                }
            }

            process.UserFilter(kernel);
            break;
        }
        }
    }

    if(ui->Labwork_6->isChecked()) {            // Лаба №6
        // Ранговая фильтрация изображения
        // Медианный фильтр
        QVector<QVector<int>> aperture(3);
        for(int i=0; i < 3; i++) {
            aperture[i].fill(0.0, 3);
            for (int j=0; j < 3; j++) {
                aperture[i][j] = ui->userAperture->item(i, j)->text().toDouble();
            }
        }
        process.MedianBlur(aperture);
    }

    if(ui->Labwork_7->isChecked()) {            // Лаба №7
        switch(ui->Labwork_7_method->currentIndex()) {
        case 0: {
            // Бинаризация изображения по порогу
            process.ThBinarize(ui->threshold->text().toInt());
            break;
        }
        case 1: {
            // Разметка 4-х связных областей
            process.floodFill4();

            for(int i = 0; i<process.areas.size(); i++) {
                ui->MomentsView->insertRow(i);
                ui->MomentsView->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
                ui->MomentsView->item(i,0)->setBackground(process.areas[i].color);
                ui->MomentsView->setItem(i, 1, new QTableWidgetItem(QString::number(process.areas[i].GM_00)));
                ui->MomentsView->setItem(i, 2, new QTableWidgetItem(QString::number(process.areas[i].GM_01)));
                ui->MomentsView->setItem(i, 3, new QTableWidgetItem(QString::number(process.areas[i].GM_10)));
                ui->MomentsView->setItem(i, 4, new QTableWidgetItem(QString::number(process.areas[i].CGM_02)));
                ui->MomentsView->setItem(i, 5, new QTableWidgetItem(QString::number(process.areas[i].CGM_11)));
                ui->MomentsView->setItem(i, 6, new QTableWidgetItem(QString::number(process.areas[i].CGM_20)));
            }
            break;
        }
        case 2: {
            // Разметка 8-х связных областей
            process.floodFill8();

            for(int i = 0; i<process.areas.size(); i++) {
                ui->MomentsView->insertRow(i);
                ui->MomentsView->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
                ui->MomentsView->item(i,0)->setBackground(process.areas[i].color);
                ui->MomentsView->setItem(i, 1, new QTableWidgetItem(QString::number(process.areas[i].GM_00)));
                ui->MomentsView->setItem(i, 2, new QTableWidgetItem(QString::number(process.areas[i].GM_01)));
                ui->MomentsView->setItem(i, 3, new QTableWidgetItem(QString::number(process.areas[i].GM_10)));
                ui->MomentsView->setItem(i, 4, new QTableWidgetItem(QString::number(process.areas[i].CGM_02)));
                ui->MomentsView->setItem(i, 5, new QTableWidgetItem(QString::number(process.areas[i].CGM_11)));
                ui->MomentsView->setItem(i, 6, new QTableWidgetItem(QString::number(process.areas[i].CGM_20)));
            }
            break;
        }
        case 3: {
            // Разметка 8-х связных областей с подавлением
            process.floodFill8_clear(ui->threshold->text().toInt());
            break;
        }
        }
    }
    ui->res->setPixmap(QPixmap::fromImage(process.result)); // process.result.scaled(ui->res->size(), Qt::KeepAspectRatio)
    ui->processing->setDisabled(false);

    plotBrightHist();
}

void MainWindow::on_openButton_clicked()
{
    QString src_file = QFileDialog::getOpenFileName(this, tr("Open Image"));
    QImage src_load(src_file);
    process.setSource(src_load.scaled(ui->src->width(), ui->src->height(), Qt::KeepAspectRatio));

    ui->src->setPixmap(QPixmap::fromImage(process.source));
    ui->res->setPixmap(QPixmap::fromImage(process.result));

    plotBrightHist();
}

void MainWindow::on_useResult_clicked()
{
    process.source = process.result;
    ui->src->setPixmap(QPixmap::fromImage(process.source));
}

void MainWindow::on_reload_clicked()
{
    int imgN = (int)(((double)rand() / (double)RAND_MAX) * 32) + 1;
    QString src_file("/home/lachetti/albums/" + QString::number(imgN) + ".jpg");
    QImage src_load(src_file);
    process.setSource(src_load.scaled(ui->src->width(), ui->src->height(), Qt::KeepAspectRatio));
    process.result = process.source;
    ui->src->setPixmap(QPixmap::fromImage(process.source));
    ui->res->setPixmap(QPixmap::fromImage(process.result));

    plotBrightHist();
}

void MainWindow::on_noizeAdd_clicked()
{
    process.noizeAdd(ui->noizeAdd_in->text().toDouble());
    ui->res->setPixmap(QPixmap::fromImage(process.result));

    plotBrightHist();
}

void MainWindow::on_noizeMul_clicked()
{
    process.noizeMul(ui->noizeMul_in->text().toDouble());
    ui->res->setPixmap(QPixmap::fromImage(process.result));

    plotBrightHist();
}

void MainWindow::on_noizeImp_clicked()
{
    process.noizeImp(ui->noizeImp_in->text().toDouble());
    ui->res->setPixmap(QPixmap::fromImage(process.result));

    plotBrightHist();
}

void MainWindow::on_inverce_clicked()
{
    process.inverce();
    ui->res->setPixmap(QPixmap::fromImage(process.result));

    plotBrightHist();
}

void MainWindow::on_filterSize_valueChanged(int value)
{
    ui->filterSizeInfo->setText(QString::number(value*2+1));
}

void MainWindow::plotBrightHist() {
    // Вывод гистограммы яркости
    QVector<double> xHist256;
    QVector<double> yHist256;

    process.calcBrightnessHist();
    for (int i=0; i<256; i++) {
        xHist256.push_back((double)i);
        yHist256.push_back((double)process.brightnessHist[i]);
    }

    // Статистические характеристики
    ui->lab2_mean->setText(QString::number(process.histMean));
    ui->lab2_variance->setText(QString::number(process.histVariance));
    ui->lab2_kurtosis->setText(QString::number(process.histKurtosis));
    ui->lab2_skewness->setText(QString::number(process.histSkewness));
    ui->lab2_uniformity->setText(QString::number(process.histUniformity));

    // Построение гистограммы яркости
    ui->histBright->clearGraphs();
    ui->histBright->yAxis->setVisible(false);
    ui->histBright->legend->setVisible(false);
    ui->histBright->addGraph();
    ui->histBright->graph(0)->setPen(QColor(0, 0, 0, 255));         // Задаем цвет
    ui->histBright->graph(0)->setLineStyle(QCPGraph::lsImpulse);    // График в виде импульсных тиков
    ui->histBright->graph(0)->setAntialiased(false);                // Отключаем сглаживание, по умолчанию включено
    ui->histBright->graph(0)->setData(xHist256, yHist256);

    ui->histBright->rescaleAxes();
    ui->histBright->xAxis->setRange(-1, 256);
    ui->histBright->replot();
}

void MainWindow::on_openTIFF_clicked()
{
    QString src_file = QFileDialog::getOpenFileName(this, tr("Open Image"));
    QImage src_load(src_file);
    process.setSourceTIFF(src_load.scaled(ui->src->width(), ui->src->height(), Qt::KeepAspectRatio));

    ui->src->setPixmap(QPixmap::fromImage(process.sourceTIFF));
    ui->res->setPixmap(QPixmap::fromImage(process.sourceTIFF));
}
