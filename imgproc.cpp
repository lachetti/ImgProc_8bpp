#include "imgproc.h"
#include <cmath>
#include <random>

void imgProc::setSource(QImage load) {

    source = load;
    // Перевод в черно-белый
    for(int i = 0; i<source.width(); i++)
        for(int j = 0;j<source.height();j++){
            QColor col = source.pixelColor(i,j);
            int g = (int)round(col.red()*0.2952 + col.green()*0.5547 + col.blue()*0.148);
            source.setPixelColor(i,j,QColor(g,g,g));
        }
    result = source;
}
void imgProc::setSourceTIFF(QImage load) {

    sourceTIFF = load;

    result = source;
}

// Шумы
void imgProc::noizeAdd(double sig) {
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, sig);
    result = source;
    for(int i = 0; i<result.width(); i++)
        for(int j = 0;j<result.height();j++) {
            int g = toRange0_255(result.pixelColor(i,j).red() + distribution(generator));
            result.setPixelColor(i,j,QColor(g,g,g));
        }
}
void imgProc::noizeMul(double sig) {
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(1.0, (sig / (sig + 1.0)));
    result = source;
    for(int i = 0; i<result.width(); i++)
        for(int j = 0;j<result.height();j++) {
            int g = toRange0_255(result.pixelColor(i,j).red() * distribution(generator));
            result.setPixelColor(i,j,QColor(g,g,g));
        }
}
void imgProc::noizeImp(double ps) {
    std::srand(8);
    result = source;
    for(int i = 0; i<result.width(); i++)
        for(int j = 0;j<result.height(); j++) {
            if (((double)std::rand() / (double)RAND_MAX) < ps) {
                if (((double)std::rand() / (double)RAND_MAX) < 0.5) {
                    result.setPixelColor(i,j,QColor(0,0,0));
                } else {
                    result.setPixelColor(i,j,QColor(255,255,255));
                }
            }
        }
}

// Инверсия
void imgProc::inverce() {
    for(int i = 0; i<result.width(); i++)
        for(int j = 0;j<result.height();j++){
            int r = 255 - result.pixelColor(i,j).red();
            int g = 255 - result.pixelColor(i,j).green();
            int b = 255 - result.pixelColor(i,j).blue();
            result.setPixelColor(i,j,QColor(r,g,b));
        }
}

// Labwork 1 [credited]
void imgProc::setCs(QImage load) {

    Cs = load;
    // Перевод в черно-белый
    for(int i = 0; i<Cs.width(); i++)
        for(int j = 0;j<Cs.height();j++){
            QColor col = Cs.pixelColor(i,j);
            int g = (int)round(col.red()*0.2952 + col.green()*0.5547 + col.blue()*0.148);
            Cs.setPixelColor(i,j,QColor(g,g,g));
        }
}
void imgProc::setAlpha(QImage load) {

    alpha = load;
    // Перевод в черно-белый
    for(int i = 0; i<alpha.width(); i++)
        for(int j = 0;j<alpha.height();j++){
            QColor col = alpha.pixelColor(i,j);
            int g = (int)round(col.red()*0.2952 + col.green()*0.5547 + col.blue()*0.148);
            alpha.setPixelColor(i,j,QColor(g,g,g));
        }
}
void imgProc::CompostingAlpha() {
    // === Переписал на целочисленные значения === //
    // ------------------------------------------------------------------------------------------------------------ //
    for(int i = 0; i<source.width(); i++)
        for(int j = 0;j<source.height();j++){
            int col_Cb = source.pixelColor(i,j).red();
            int col_Cs = Cs.pixelColor(i,j).red();
            int alp = alpha.pixelColor(i, j).red();

            int g = (alp*col_Cb + (255 - alp)*col_Cs) / 255;
            result.setPixelColor(i,j,QColor(g,g,g));
        }
}
void imgProc::FlipV() {
    // === Переписал на свою функцию и применяю на текущей картинке === //
    // ------------------------------------------------------------------------------------------------------------ //
    QColor swapC;
    result = source;
    for(int i = 0; i<result.width(); i++) {
        for(int j = 0;j<result.height()/2;j++){
            swapC = result.pixelColor(i, j);
            result.setPixelColor(i, j, source.pixelColor(i,result.width()-j));
            result.setPixelColor(i, result.width()-j, swapC);
        }
    }
}
void imgProc::FlipH() {
    // === Переписал на свою функцию и применяю на текущей картинке === //
    // ------------------------------------------------------------------------------------------------------------ //
    QColor swapC;
    result = source;
    for(int i = 0; i<result.width()/2; i++) {
        for(int j = 0;j<result.height();j++){
            swapC = result.pixelColor(i, j);
            result.setPixelColor(i, j, source.pixelColor(result.height() - i, j));
            result.setPixelColor(result.height() - i, j, swapC);
        }
    }
}
void imgProc::Transpose() {
    // === Переписал на применение на текущей картинке === //
    // ------------------------------------------------------------------------------------------------------------ //
    QColor swapC;
    result = source;
    for(int i = 1; i<result.width(); i++) {
        for(int j = i+1; j<result.height(); j++){
            swapC = result.pixelColor(i, j);
            result.setPixelColor(i, j, result.pixelColor(j,i));
            result.setPixelColor(j, i, swapC);
        }
    }
}

// Labwork 2 [credited]
void imgProc::calcBrightnessHist() {
    // === Добавил вывод графика и характеристик === //
    // ------------------------------------------------------------------------------------------------------------ //
    brightnessHist.fill(0, 256);
    histMean = 0.0;
    histVariance = 0.0;
    histSkewness = 0.0;
    histKurtosis = 0.0;
    histUniformity = 0.0;
    histEntropy = 0.0;

    for(int i=0; i < result.width(); i++) {
        for(int j=0; j < result.height(); j++){
            brightnessHist[result.pixelColor(i,j).red()]++;
        }
    }
    double p=0.0;
    for (int i=0; i < 256; i++) {
        p = (double)brightnessHist[i] / (double)(result.width() * result.height());
        histMean += (double)i * p;
        histUniformity += p * p;
        histEntropy -= p * log2(p);
    }
    for (int i=0; i < 256; i++) {
        p = (double)brightnessHist[i] / (double)(result.width() * result.height());
        histVariance += (i - histMean) * (i - histMean) * p;
        histSkewness += (i - histMean) * (i - histMean) * (i - histMean) * p;
        histKurtosis += (i - histMean) * (i - histMean) * (i - histMean) * (i - histMean) * p - 3.0;
    }
    histSkewness /= histVariance * sqrt(histVariance);
    histKurtosis /= histVariance * histVariance;
}
void imgProc::calcCoOccurenceMatrix(int R, int C) {
    // === Добавил вывод графика и характеристики === //
    // ------------------------------------------------------------------------------------------------------------ //
    co_occurenceMatrix.resize(256);
    for (int i = 0; i < 256; i++) {
        co_occurenceMatrix[i].fill(0, 256);
    }
    comUniformity = 0;
    comC = C;
    comR = R;

    /*
    // 10 485 760 000 итераций на оригинальном размере
    for(int a=0; a < 256; a++) {
        for(int b=0; b < 256; b++) {
            for(int x=0; x < result.width() - C; x++) {
                for(int y=0; y < result.height() - R; y++) {
                    if ((result.pixelColor(x,y).red() == a) && (result.pixelColor(x+C,y+R).red() == b)) {
                        co_occurenceMatrix[a][b]++;
                    }
                }
            }
            if (max_com < co_occurenceMatrix[a][b]) {
                max_com = co_occurenceMatrix[a][b];
            }
        }
    }
    */
    int max_com = 0;
    for (int i = 0; i<result.width(); i++) {
        for(int j = 0; j<result.height(); j++) {
            if ( ((i+C) < result.width()) && ((j+R) < result.height()) ) {
                int x = result.pixelColor(i,j).red();
                int y = result.pixelColor(i+C,j+R).red();
                co_occurenceMatrix[x][y]++;
                if (max_com < co_occurenceMatrix[x][y]) {
                    max_com = co_occurenceMatrix[x][y];
                }
            }
        }
    }

    com = result.scaled(256, 256, Qt::KeepAspectRatio);
    for(int a=0; a < 256; a++) {
        for(int b=0; b < 256; b++) {
            int newPC = 255 * (1.0 - (double)co_occurenceMatrix[a][b] / (double)max_com);
            com.setPixelColor(a, b, QColor(newPC, newPC, newPC));
        }
    }

    double Nt = (result.width() - C) * (result.height() - R);
    for(int a=0; a < 256; a++) {
        for(int b=0; b < 256; b++) {
            comUniformity += ((double)co_occurenceMatrix[a][b] / Nt) * ((double)co_occurenceMatrix[a][b] / Nt);
        }
    }

}

// Labwork 3 [completed]
void imgProc::rotateImage(double angle) {
    // === Переписал вывод картинки === //
    // ------------------------------------------------------------------------------------------------------------ //
    QVector<QVector<double>> rotate({{cos(angle), sin(angle), 0.0},
                                     {-sin(angle), cos(angle), 0.0},
                                     {0.0, 0.0, 1.0}});

    QVector<QVector<double>> transformInv(rotate);
    QVector<QVector<double>> corners(4);
    QVector<double> tCoord(3);

    result = source.scaled(source.width(), source.height(), Qt::KeepAspectRatio);

    transformInv = matrixInverce3(rotate);
    int newWidth = 0;
    int newHeight = 0;

    corners[0] = matrixMul3(0.0, 0.0, 1.0, rotate);
    corners[1] = matrixMul3(0.0, result.width(), 1.0, rotate);
    corners[2] = matrixMul3(result.height(), result.width(), 1.0, rotate);
    corners[3] = matrixMul3(result.height(), 0.0, 1.0, rotate);

    double maxX = corners[0][0];
    double maxY = corners[0][1];
    double minX = corners[0][0];
    double minY = corners[0][1];

    for (int i=1; i<4; i++) {
        if (corners[i][0] > maxX) {
            maxX = corners[i][0];
        } else if (corners[i][0] < minX) {
            minX = corners[i][0];
        }
        if (corners[i][1] > maxY) {
            maxY = corners[i][1];
        } else if (corners[i][1] < minY) {
            minY = corners[i][1];
        }
    }
    newWidth = round(maxX - minX);
    newHeight = round(maxY - minY);

    QImage newImage(newWidth, newHeight, QImage::Format_Grayscale8);
    result = newImage;
    QVector<double> b(16);
    double f_color = 0.0;

    for (int i = 0; i<result.width(); i++) {
        for(int j = 0; j<result.height(); j++) {
            tCoord = matrixMul3(minX + (double)i, minY + (double)j, 1.0, transformInv);

            if ( (round(tCoord[0])+2 < source.width()) && (round(tCoord[1])+2 < source.height()) &&
                 (round(tCoord[0])-1 > 0) && (round(tCoord[1])-1 > 0)) {
                b = funcB(tCoord[0] - round(tCoord[0]), tCoord[1] - round(tCoord[1]));

                f_color = b[0] * (double)f_r_s(round(tCoord[0]) + 0, round(tCoord[1]) + 0);
                f_color += b[1] * (double)f_r_s(round(tCoord[0]) + 0, round(tCoord[1]) + 1);
                f_color += b[2] * (double)f_r_s(round(tCoord[0]) + 1, round(tCoord[1]) + 0);
                f_color += b[3] * (double)f_r_s(round(tCoord[0]) + 1, round(tCoord[1]) + 1);
                f_color += b[4] * (double)f_r_s(round(tCoord[0]) + 0, round(tCoord[1]) - 1);
                f_color += b[5] * (double)f_r_s(round(tCoord[0]) - 1, round(tCoord[1]) + 0);
                f_color += b[6] * (double)f_r_s(round(tCoord[0]) + 1, round(tCoord[1]) - 1);
                f_color += b[7] * (double)f_r_s(round(tCoord[0]) - 1, round(tCoord[1]) + 1);
                f_color += b[8] * (double)f_r_s(round(tCoord[0]) + 0, round(tCoord[1]) + 2);
                f_color += b[9] * (double)f_r_s(round(tCoord[0]) + 2, round(tCoord[1]) + 0);
                f_color += b[10] * (double)f_r_s(round(tCoord[0]) - 1, round(tCoord[1]) - 1);
                f_color += b[11] * (double)f_r_s(round(tCoord[0]) + 1, round(tCoord[1]) + 2);
                f_color += b[12] * (double)f_r_s(round(tCoord[0]) + 2, round(tCoord[1]) + 1);
                f_color += b[13] * (double)f_r_s(round(tCoord[0]) - 1, round(tCoord[1]) + 2);
                f_color += b[14] * (double)f_r_s(round(tCoord[0]) + 2, round(tCoord[1]) + -1);
                f_color += b[15] * (double)f_r_s(round(tCoord[0]) + 2, round(tCoord[1]) + 2);

                result.setPixelColor(i, j, QColor(f_color,f_color,f_color));
            } else {
                result.setPixelColor(i, j, QColor(0, 0, 0));
            }
        }
    }
    result = result.scaled(source.width(), source.height(), Qt::KeepAspectRatio);
}
QVector<double> imgProc::funcB(double x, double y) {
    QVector<double> b(16);
    b[0] = (x-1)*(x-2)*(x+1)*(y-1)*(y-2)*(y+1)/4.0;
    b[1] = -x*(x-2)*(x+1)*(y-1)*(y-2)*(y+1)/4.0;
    b[2] = -(x-1)*(x-2)*(x+1)*y*(y-2)*(y+1)/4.0;
    b[3] = x*(x-2)*(x+1)*y*(y-2)*(y+1)/4.0;
    b[4] = -x*(x-1)*(x-2)*(y-1)*(y-2)*(y+1)/12.0;
    b[5] = -(x-1)*(x-2)*(x+1)*y*(y-1)*(y-2)/12.0;
    b[6] = x*(x-1)*(x-2)*y*(y-2)*(y+1)/12.0;
    b[7] = x*(x-2)*(x+1)*y*(y-1)*(y-2)/12.0;
    b[8] = x*(x-1)*(x+1)*(y-1)*(y-2)*(y+1)/12.0;
    b[9] = (x-1)*(x-2)*(x+1)*y*(y-1)*(y+1)/12.0;
    b[10] = x*(x-1)*(x-2)*y*(y-1)*(y-2)/36.0;
    b[11] = -x*(x-1)*(x+1)*y*(y-2)*(y+1)/12.0;
    b[12] = -x*(x-2)*(x+1)*y*(y-1)*(y+1)/12.0;
    b[13] = -x*(x-1)*(x+1)*y*(y-1)*(y-2)/36.0;
    b[14] = -x*(x-1)*(x-2)*y*(y-1)*(y+1)/36.0;
    b[15] = x*(x-1)*(x+1)*y*(y-1)*(y+1)/36.0;
    return b;
}
int imgProc::f_r_s(int i, int j) {
    return source.pixelColor(i, j).red();
}
QVector<double> imgProc::matrixMul3(double x, double y, double z, QVector<QVector<double>>& transform) {

    QVector<double> res(3);
    res.fill(0.0, 3);

    if (transform.size() == 3) {
        for(int i=0; i < 3; i++) {
            res[i] = x * transform[i][0] + y * transform[i][1] + z * transform[i][2];
        }
    }

    return res;
}
QVector<QVector<double>> imgProc::matrixInverce3(QVector<QVector<double>>& m) {

    QVector<QVector<double>> res(3);
    res[0].fill(0.0, 3);
    res[1].fill(0.0, 3);
    res[2].fill(0.0, 3);

    double a = m[0][0];
    double b = m[0][1];
    double c = m[0][2];
    double d = m[1][0];
    double r = m[1][1];
    double f = m[1][2];
    double g = m[2][0];
    double h = m[2][1];
    double x = m[2][2];
    double den = b*f*g + c*d*h - a*f*h - c*g*r - b*d*x + a*r*x;

    res[0][0] = (-f*h + r*x) / den;
    res[0][1] = (c*h - b*x) / den;
    res[0][2] = (b*f - c*r) / den;
    res[1][0] = (f*g-d*x) / den;
    res[1][1] = (-c*g+a*x) / den;
    res[1][2] = (c*d-a*f) / den;
    res[2][0] = (d*h - g*r) / den;
    res[2][1] = (b*g - a*h) / den;
    res[2][2] = (-b*d + a*r) / den;

    return res;
}

// Labwork 4 [credited]
void imgProc::FloydStenbergESA(int bbp) {

    int toneCount = 1;
    for (int i=0; i<bbp; i++) {
        toneCount *= 2;
    }
    QVector<int> newColorTable(toneCount);
    for (int i=0; i<newColorTable.size()-1; i++) {
        newColorTable[i] = 256 / (toneCount - 1) * i;
    }
    newColorTable[newColorTable.size()-1] = 255;

    QVector<QVector<double>> kernel({{0.0, 0.0, 7.0},
                                     {3.0, 5.0, 1.0}});
    for (int i=0; i<kernel.size(); i++)
        for (int j=0; j<kernel[i].size(); j++) {
            kernel[i][j] /= 16.0;
        }

    result = source;
    int col = 1;
    int curPC = 0;
    int newPC = 0;
    int errLeft = 0;
    int errRight = 0;
    for (int i = 0; i<result.width(); i++)
        for(int j = 0; j<result.height(); j++) {
            curPC = result.pixelColor(i, j).red();
            col = 1;
            while ((col<toneCount)&&(curPC > newColorTable[col])) {
                col++;
            }
            errLeft = curPC - newColorTable[col-1];
            errRight = curPC - newColorTable[col];
            if (-errRight <= errLeft) {
                newPC = newColorTable[col];
                result.setPixelColor(i, j, QColor(newPC, newPC, newPC));

                newPC = toRange0_255(result.pixelColor(i,j+1).red() + errRight*kernel[0][2]);
                result.setPixelColor(i,j+1, QColor(newPC, newPC, newPC));

                newPC = toRange0_255(result.pixelColor(i+1,j-1).red() + errRight*kernel[1][0]);
                result.setPixelColor(i+1,j-1, QColor(newPC, newPC, newPC));

                newPC = toRange0_255(result.pixelColor(i+1,j).red() + errRight*kernel[1][1]);
                result.setPixelColor(i+1,j, QColor(newPC, newPC, newPC));

                newPC = toRange0_255(result.pixelColor(i+1,j+1).red() + errRight*kernel[1][2]);
                result.setPixelColor(i+1,j+1, QColor(newPC, newPC, newPC));
            } else if (-errRight > errLeft) {
                newPC = newColorTable[col-1];
                result.setPixelColor(i, j, QColor(newPC, newPC, newPC));

                newPC = toRange0_255(result.pixelColor(i,j+1).red() + errLeft*kernel[0][2]);
                result.setPixelColor(i,j+1, QColor(newPC, newPC, newPC));

                newPC = toRange0_255(result.pixelColor(i+1,j-1).red() + errLeft*kernel[1][0]);
                result.setPixelColor(i+1,j-1, QColor(newPC, newPC, newPC));

                newPC = toRange0_255(result.pixelColor(i+1,j).red() + errLeft*kernel[1][1]);
                result.setPixelColor(i+1,j, QColor(newPC, newPC, newPC));

                newPC = toRange0_255(result.pixelColor(i+1,j+1).red() + errLeft*kernel[1][2]);
                result.setPixelColor(i+1,j+1, QColor(newPC, newPC, newPC));
            }
        }
}
int imgProc::toRange0_255(int x) {
    if (x > 255) {
        x = 255;
    } else if (x < 0) {
        x = 0;
    }
    return x;
}

// Labwork 5 [complited]
void imgProc::LowPassFilterSpatial(){

    QVector<QVector<double>> kernel({{1, 1, 1},
                                    { 1, 1, 1},
                                    { 1, 1, 1}});
    kernel = NormalizeKernel(kernel);

    result = source;
    ApplyKernel(source, result, kernel);
}
void imgProc::LowPassFilterGauss(){

    QVector<QVector<double>> kernel({{1, 4, 7, 4, 1},
                                     {4, 16, 26, 16, 4},
                                     {7, 26, 41, 26, 7},
                                     {4, 16, 26, 16, 4},
                                     {1, 4, 7, 4, 1}});

    kernel = NormalizeKernel(kernel);

    result = source;
    ApplyKernel(source, result, kernel);
}
void imgProc::HighPassFilterPrevitt() {

    QVector<QVector<double>> kernel({{-1,-1,-1},
                                    {0, 0, 0},
                                    {1, 1, 1}});
    result = source;
    ApplyKernel(source, result, kernel);
}
void imgProc::HighPassFilterSobel() {

    QVector<QVector<double>> kernel({{-1,-2,-1},
                                    {0, 0, 0},
                                    {1, 2, 1}});
    result = source;
    ApplyKernel(source, result, kernel);
}
void imgProc::SharpenFilter(){

    QVector<QVector<double>> kernel({{-1,-1,-1},
                                    {-1, 9,-1},
                                    {-1,-1,-1}});
    result = source;
    ApplyKernel(source, result, kernel);
}
void imgProc::UserFilter(QVector<QVector<double>>& userKernel){

    QVector<QVector<double>> kernel(userKernel);
    for (int i=0; i<userKernel.size(); i++)
        for (int j=0; j<userKernel[i].size(); j++) {
            kernel[i][j] = userKernel[i][j];
        }

    result = source;
    ApplyKernel(source, result, kernel);
}
void imgProc::BilateralFilter(int S, int sigD, int sigR) {
    // === Релизовал билатеральный фильтр === //
    // ------------------------------------------------------------------------------------------------------------ //
    QVector<QVector<double>> kernel(S);
    for (int i=0; i<S; i++) {
        kernel[i].fill(0.0, S);
        for (int j=0; j<S; j++) {
            kernel[i][j] = bilaterial_v(i - S/2, j - S/2, sigD);
        }
    }
    double w = 0.0;
    for (int r = S/2; r<result.width()-S/2; r++)
        for (int c = S/2; c<result.height()-S/2; c++) {

            double num = 0.0;   // числитель
            double den = 0.0;   // знаменатель
            for (int i=0; i<S; i++)
                for(int j=0; j<S; j++) {
                    w = bilaterial_w((double)result.pixelColor(r+i-S/2, c+j-S/2).red() - (double)result.pixelColor(r,c).red(), sigR);
                    num += (double)result.pixelColor(r+i-S/2, c+j-S/2).red() * kernel[i][j] * w;
                    den += kernel[i][j] * w;
                }
            int currPC = toRange0_255((int)round(num / den));
            result.setPixelColor(r,c,QColor(currPC,currPC,currPC));
        }
}
double imgProc::bilaterial_v(int i, int j, int sigD) {  // Пространственное ядро весов
    return exp(-1.0 * (i*i + j*j) / (2.0 * (double)(sigD*sigD)));
}
double imgProc::bilaterial_w(double x, int sigR) {  // Функция весов в зависимости от фотометрического расстояния
    return exp(-1.0 * (x*x) / (2.0 * (double)(sigR*sigR)));
}
void imgProc::ApplyKernel(QImage& src, QImage& res, QVector<QVector<double>>& kernel){
    int size = kernel.size();

    for (int r = size/2; r<src.width()-size/2; r++)
        for (int c = size/2; c<src.height()-size/2; c++) {
            double final = 0.0;
            for (int i = 0; i<size; i++)
                for(int j = 0; j<size; j++) {
                    final += (double)src.pixelColor(i+r-size/2, j+c-size/2).red() * kernel[i][j];
                }
            int currPC = toRange0_255((int)round(final));

            res.setPixelColor(r, c, QColor(currPC, currPC, currPC));
        }
}
QVector<QVector<double>> imgProc::NormalizeKernel(QVector<QVector<double>>& kernel) {
    double kerNorm = 0.0;

    for (int i=0; i<kernel.size(); i++)
        for (int j=0; j<kernel[i].size(); j++) {
            kerNorm += kernel[i][j];
        }
    for (int i=0; i<kernel.size(); i++)
        for (int j=0; j<kernel[i].size(); j++) {
            kernel[i][j] /= kerNorm;
        }
    return kernel;
}

// Labwork 6 [complited]
void imgProc::MedianBlur(QVector<QVector<int>>& userAperture) {
    // === Релизовал инициализацию апертуры пользователем и добавил возможность зашумления картинки === //
    // ------------------------------------------------------------------------------------------------------------ //
    int size = 0;
    for (int i=0; i<userAperture.size(); i++) {
        for (int j=0; j<userAperture[i].size(); j++) {
            size += userAperture[i][j];
        }
    }
    if (size <= 0) {
        size = 9;
        userAperture.clear();
        userAperture.fill({1, 1, 1}, 3);
    }
    QVector<int> range;
    result = source;

    for (int r = userAperture.size()/2; r<result.width()-userAperture.size()/2; r++) {
        for (int c = userAperture.size()/2; c<result.height()-userAperture.size()/2; c++) {

            for (int i = 0; i<userAperture.size(); i++) {
                for (int j = 0; j<userAperture.size(); j++) {
                    for (int k=0; k<userAperture[i][j]; k++) {
                        range.push_back(result.pixelColor(r+i-userAperture.size()/2, c+j-userAperture.size()/2).red());
                    }
                }
            }
            std::sort(range.begin(), range.end());
            result.setPixelColor(r, c, QColor(range[size/2],range[size/2],range[size/2]));
            range.clear();
        }
    }
}

// Labwork 7 [completed]
void imgProc::ThBinarize(int threshold) {
    // ===  === //
    // ------------------------------------------------------------------------------------------------------------ //
    result = source;
    for(int i = 0; i<result.width(); i++)
        for(int j = 0; j<result.height(); j++){
            int intens = result.pixelColor(i, j).red();
            if(intens<threshold)
                result.setPixelColor(i, j, QColor(0,0,0));
            else
                result.setPixelColor(i, j, QColor(255,255,255));
        }
}
void imgProc::floodFill4() {
    // ===  === //
    // ------------------------------------------------------------------------------------------------------------ //
    int dx4[4] = {0, 1, 0, -1};
    int dy4[4] = {1, 0, -1, 0};
    QColor oldColor(0,0,0);
    QColor newColor;
    QPoint currentPoint;

    result = source;
    areas.clear();


    QVector<QPoint> stack;
    QVector<QPoint> buffer;
    for (int r = 0; r<result.width(); r++) {
        for (int c = 0; c<result.height(); c++) {
            if (result.pixelColor(r, c) == oldColor) {
                stack.push_back(QPoint(r, c));
                newColor = QColor(rand() % 256, rand() % 256, rand() % 256);
                while (stack.size() > 0) {
                    currentPoint = stack.last();
                    stack.pop_back();

                    buffer.push_back(currentPoint);
                    result.setPixelColor(currentPoint, QColor(newColor));

                    for (int neib=0; neib<4; neib++) {
                        int nX = currentPoint.x() + dx4[neib];
                        int nY = currentPoint.y() + dy4[neib];
                        if ((nX>0) && (nX<result.width()) && (nY>0) && (nY<result.height()) &&
                                (result.pixelColor(nX, nY) == oldColor)) {
                            stack.push_back(QPoint(nX, nY));
                        }
                    }
                }
                areas.push_back(Area(buffer, newColor));
                buffer.clear();
            }
        }
    }
}
void imgProc::floodFill8() {
    // ===  === //
    // ------------------------------------------------------------------------------------------------------------ //
    int dx8[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
    int dy8[8] = {1, 1, 1, 0, -1, -1, -1, 0};
    QColor oldColor(0,0,0);
    QColor newColor;
    QPoint currentPoint;

    result = source;
    areas.clear();

    QVector<QPoint> stack;
    QVector<QPoint> buffer;
    for (int r = 0; r<result.width(); r++) {
        for (int c = 0; c<result.height(); c++) {

            if (result.pixelColor(r, c) == oldColor) {
                stack.push_back(QPoint(r, c));
                newColor = QColor(rand() % 256, rand() % 256, rand() % 256);

                while (stack.size() > 0) {
                    currentPoint = stack.last();
                    stack.pop_back();

                    buffer.push_back(currentPoint);
                    result.setPixelColor(currentPoint, QColor(newColor));

                    for (int neib=0; neib<8; neib++) {
                        int nX = currentPoint.x() + dx8[neib];
                        int nY = currentPoint.y() + dy8[neib];
                        if ((nX>0) && (nX<result.width()) && (nY>0) && (nY<result.height()) &&
                                (result.pixelColor(nX, nY) == oldColor)) {
                            stack.push_back(QPoint(nX, nY));
                        }
                    }

                }
                areas.push_back(Area(buffer, newColor));
                buffer.clear();

            }
        }
    }
}
void imgProc::floodFill8_clear(int eps) {
    // ===  === //
    // ------------------------------------------------------------------------------------------------------------ //
    int dx8[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
    int dy8[8] = {1, 1, 1, 0, -1, -1, -1, 0};
    QColor oldColor(0,0,0);
    QColor newColor;
    QPoint currentPoint;

    result = source;
    areas.clear();

    QVector<QPoint> stack;
    QVector<QPoint> buffer;
    for (int r = 0; r<result.width(); r++) {
        for (int c = 0; c<result.height(); c++) {

            if (result.pixelColor(r, c) == oldColor) {
                stack.push_back(QPoint(r, c));
                newColor = QColor(rand() % 256, rand() % 256, rand() % 256);

                while (stack.size() > 0) {
                    currentPoint = stack.last();
                    stack.pop_back();

                    buffer.push_back(currentPoint);
                    result.setPixelColor(currentPoint, QColor(newColor));

                    for (int neib=0; neib<8; neib++) {
                        int nX = currentPoint.x() + dx8[neib];
                        int nY = currentPoint.y() + dy8[neib];
                        if ((nX>0) && (nX<result.width()) && (nY>0) && (nY<result.height()) &&
                                (result.pixelColor(nX, nY) == oldColor)) {
                            stack.push_back(QPoint(nX, nY));
                        }
                    }

                }
                areas.push_back(Area(buffer, newColor));
                buffer.clear();

            }
        }
    }
}
/*
int imgProc::p_function_image(int x, int y) {
    return 1;
}
int imgProc::calc_GM(int p, int q, int index) {
    double Moment = 0.0;
    int x_deg_p = 1;
    int y_deg_q = 1;

    for (int i = 0; i<areas[index].size(); i++) {
        // возведение x в степень p
        x_deg_p = 1;
        for (int deg = 0; deg<p; deg++) { x_deg_p *= areas[index][i].x(); }
        // возведение y в степень q
        y_deg_q = 1;
        for (int deg = 0; deg<q; deg++) { y_deg_q *= areas[index][i].y(); }

        Moment += (double)x_deg_p * (double)y_deg_q * p_function_image(areas[index][i].x(), areas[index][i].y());
    }

    return Moment;
}
int imgProc::calc_CGM(int p, int q, int index) {
    double Moment = 0.0;
    int x_deg_p = 1;
    int y_deg_q = 1;
    int xc = GM_10[index]/GM_00[index];
    int yc = GM_01[index]/GM_00[index];

    for (int i = 0; i<areas[index].size(); i++) {
        // возведение x в степень p
        x_deg_p = 1;
        for (int deg = 0; deg<p; deg++) { x_deg_p *= areas[index][i].x() - xc; }
        // возведение y в степень q
        y_deg_q = 1;
        for (int deg = 0; deg<q; deg++) { y_deg_q *= areas[index][i].y() - yc; }

        Moment += (double)x_deg_p * (double)y_deg_q * p_function_image(areas[index][i].x(), areas[index][i].y());
    }

    return Moment;
}
void imgProc::calc_Moments() {
    GM_00.fill(0, areas.size());
    GM_01.fill(0, areas.size());
    GM_10.fill(0, areas.size());
    CGM_02.fill(0, areas.size());
    CGM_11.fill(0, areas.size());
    CGM_20.fill(0, areas.size());
    for(int i = 0; i<areas.size(); i++) {
        GM_00[i] = calc_GM(0, 0, i);
        GM_01[i] = calc_GM(0, 1, i);
        GM_10[i] = calc_GM(1, 0, i);
        CGM_02[i] = calc_CGM(0, 2, i);
        CGM_11[i] = calc_CGM(1, 1, i);
        CGM_20[i] = calc_CGM(2, 0, i);
    }
}
*/























