#include <QImage>
#include "area.h"

class imgProc
{

public:
    QImage source;
    QImage result;

    QImage sourceTIFF;

    QImage Cs;
    QImage alpha;

    // === ЛР №2
    QVector<int> brightnessHist;
    QImage com;
    double histMean;
    double histVariance;
    double histKurtosis;
    double histSkewness;
    double histUniformity;
    double histEntropy;
    QVector<QVector<int>> co_occurenceMatrix;
    int comR;
    int comC;
    double comUniformity;
    QVector<Area> areas;

    imgProc() {}
    void setSource(QImage load);
    void setSourceTIFF(QImage load);

    // === Шумы
    void noizeAdd(double sig);
    void noizeMul(double sig);
    void noizeImp(double ps);

    // === Инверсия
    void inverce();

    // === ЛР №1
    void setCs(QImage load);
    void setAlpha(QImage load);
    void CompostingAlpha();         // Наложение
    void FlipV();                   // Отразить по вертикали
    void FlipH();                   // Отразить по горизонтали
    void Transpose();               // Транспонировать

    // === ЛР №2
    /* Гистограмма яркости – столбчатая диаграмма, которая для каждой
     * градации яркости показывает сколько раз в изображении присутствует
     * пиксел данной яркости.
    */
    void calcBrightnessHist();
    /*Матрица совместной встречаемости ( co-occurrence matrix ) содержит
     * количество случаев совместного появления определенных пар значений
     * пикселов, находящихся на заданном расстоянии dr и dc . В литературе
     * co-occurrence matrix также называют матрицей вхождений и
     * гистограммой второго порядка.
     */
    void calcCoOccurenceMatrix(int R, int C);

    // === ЛР №3
    void rotateImage(double angle);

    // === ЛР №4
    void FloydStenbergESA(int bbp);

    // === ЛР №5
    void LowPassFilterSpatial();    // ФНЧ (усредняющий)
    void LowPassFilterGauss();      // ФНЧ (Гауссиан)
    void HighPassFilterPrevitt();   // ФВЧ (Оператор Превитта)
    void HighPassFilterSobel();     // ФВЧ (Оператор Собеля)
    void SharpenFilter();           // ФВЧ Повышение резкости
    void UserFilter(QVector<QVector<double>>& userKernel);              // ядро от пользователя
    void BilateralFilter(int S, int sigD, int sigR);

    // === ЛР №6
    void MedianBlur(QVector<QVector<int>>& userAperture);            // Медианный фильтр

    // === ЛР №7
    void ThBinarize(int threshold); // Алгоритм бинаризации
    void floodFill4();
    void floodFill8();
    void floodFill8_clear(int bufferSize);
    /*
    int calc_GM(int p, int q, int index);
    int calc_CGM(int p, int q, int index);
    void calc_Moments();
    */

private:

    int toRange0_255(int x);

    // === ЛР №3
    QVector<double> matrixMul3(double x, double y, double z, QVector<QVector<double>>& transform);
    QVector<QVector<double>> matrixInverce3(QVector<QVector<double>>& m);
    QVector<double> funcB(double x, double y);
    int f_r_s(int i, int j);

    // === ЛР №5
    void ApplyKernel(QImage& src, QImage& res,QVector<QVector<double>>& kernel);
    QVector<QVector<double>> NormalizeKernel(QVector<QVector<double>>& kernel);

    double bilaterial_v(int i, int j, int sigD);
    double bilaterial_w(double x, int sigR);

    // === ЛР №7
    int p_function_image(int x, int y);

};




















