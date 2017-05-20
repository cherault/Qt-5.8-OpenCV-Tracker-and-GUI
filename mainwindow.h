#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/tracking.hpp>

using namespace std;
using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Mat frame;
    VideoCapture cap;

    bool check=false;

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void Frame();

    const string date();
    const string heure();

    void Square(Mat&, Point, const Scalar&, int, int, int);
    void TriangleDown(Mat&, Point, const Scalar&, int, int, int);
    void TriangleLeft(Mat&, Point, const Scalar&, int, int, int);

    void on_checkBox_clicked(bool checked);

    void on_radioBlanc_clicked();
    void on_radioNoir_clicked();
    void on_radioRouge_clicked();
    void on_radioVert_clicked();
    void on_radioBleu_clicked();

    void on_verticalSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;

    int epais = 40;
    int delta = 40;
    int scale = 10;
    int offset = 20;

    int taille=60;

    Point center;
    Point pt1, pt2;
    Point pt3, pt4;
    Point pt5, pt6;
    Point pt7, pt8;

    Scalar blanc = Scalar(255,255,255);
    Scalar rouge = Scalar(0,0,255);
    Scalar vert = Scalar(0,255,0);
    Scalar bleu = Scalar(255,0,0);
    Scalar noir = Scalar(0,0,0);

    Scalar couleur;

    Point centroid;
    Rect2d roi;
};

#endif // MAINWINDOW_H
