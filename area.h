#ifndef AREA_H
#define AREA_H

class Area {
public:
    QColor color;

    int GM_00;
    int GM_01;
    int GM_10;
    int CGM_02;
    int CGM_11;
    int CGM_20;

    Area() {
        GM_00 = 1;
        GM_01 = 0;
        GM_10 = 0;
        CGM_02 = 0;
        CGM_11 = 0;
        CGM_20 = 0;
    }

    Area(QVector<QPoint> &points_in, QColor color_in) {
        points = points_in;
        color = color_in;

        GM_00 = 1;
        GM_01 = 0;
        GM_10 = 0;
        CGM_02 = 0;
        CGM_11 = 0;
        CGM_20 = 0;

        xLeft = points[0].x();
        yBottom = points[0].y();
        for (int i=1; i<points.size(); i++) {
            if (xLeft > points[i].x()) {
                xLeft = points[i].x();
            }
            if (yBottom > points[i].y()) {
                yBottom = points[i].y();
            }
        }
        calcMoments();
    }

private:
    QVector<QPoint> points;
    int xLeft;
    int yBottom;

    int calc_GM(int p, int q) {
        int Moment = 0;
        int x_deg_p = 1;
        int y_deg_q = 1;
        int x = 0;
        int y = 0;

        for (int i = 0; i<points.size(); i++) {
            x = points[i].x() - xLeft;
            y = points[i].y() - yBottom;
            // возведение x в степень p
            x_deg_p = 1;
            for (int deg = 0; deg<p; deg++) {
                x_deg_p *= x;
            }
            // возведение y в степень q
            y_deg_q = 1;
            for (int deg = 0; deg<q; deg++) {
                y_deg_q *= y;
            }

            Moment += (double)x_deg_p * (double)y_deg_q;
        }
        return Moment;
    }
    int calc_CGM(int p, int q) {
        int Moment = 0;
        int x_deg_p = 1;
        int y_deg_q = 1;
        int x = 0;
        int y = 0;
        int xc = GM_10 / GM_00;
        int yc = GM_01 / GM_00;

        for (int i = 0; i<points.size(); i++) {
            x = points[i].x() - xLeft;
            y = points[i].y() - yBottom;
            // возведение x в степень p
            x_deg_p = 1;
            for (int deg = 0; deg<p; deg++) {
                x_deg_p *= x - xc;
            }
            // возведение y в степень q
            y_deg_q = 1;
            for (int deg = 0; deg<q; deg++) {
                y_deg_q *= y - yc;
            }

            Moment += (double)x_deg_p * (double)y_deg_q;
        }
        return Moment;
    }
    void calcMoments() {
        GM_00 = calc_GM(0, 0);
        GM_01 = calc_GM(0, 1);
        GM_10 = calc_GM(1, 0);
        CGM_02 = calc_CGM(0, 2);
        CGM_11 = calc_CGM(1, 1);
        CGM_20 = calc_CGM(2, 0);
    }
};

#endif // AREA_H
