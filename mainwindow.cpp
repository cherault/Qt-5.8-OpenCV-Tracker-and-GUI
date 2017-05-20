#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <QDebug>
#include <QMessageBox>
#include <qprogressbar.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    cap.open(0);

    Ptr<Tracker> tracker = Tracker::create("KCF");

    cap >> frame;

    roi = selectROI(frame);
    tracker->init(frame, roi);

    while(true)
    {
        cap >> frame;

        tracker->update(frame, roi);

        Rect2d zoneLOCK = Rect2d(frame.cols/2-(taille/2), frame.rows/2-(taille/2), taille, taille);
        Rect2d zoneTRACK = Rect2d(frame.cols/2-120, frame.rows/2-120, 240, 240);

        centroid = Point(roi.x + (roi.width/2), roi.y + (roi.height/2));
        rectangle(frame, roi, rouge,1);
        circle(frame, centroid, 2, rouge, 2);

        Square(frame, Point(frame.cols/2, frame.rows/2), couleur, taille, 1, 8);

        center = Point(frame.cols/2, frame.rows/2);
        circle(frame, center, 1, couleur, 2);

        pt1 = Point(0, frame.rows/2);
        pt2 = Point(frame.cols/2 - offset, frame.rows/2);
        line(frame, pt1, pt2, couleur, 1);

        pt3 = Point(frame.cols/2 + offset, frame.rows/2);
        pt4 = Point(frame.cols, frame.rows/2);
        line(frame, pt3, pt4, couleur, 1);

        pt5 = Point(frame.cols/2, 0);
        pt6 = Point(frame.cols/2, frame.rows/2 - offset);
        line(frame, pt5, pt6, couleur, 1);

        pt7 = Point(frame.cols/2, frame.rows/2 + offset);
        pt8 = Point(frame.cols/2, frame.rows);
        line(frame, pt7, pt8, couleur, 1);

        if(check)
        {
            rectangle(frame, Point(0,0), Point(frame.cols, epais), noir, -1);
            rectangle(frame, Point(0, frame.rows), Point(frame.cols, frame.rows-epais), noir, -1);
            rectangle(frame, Point(0,0), Point(epais+20, frame.rows), noir, -1);
            rectangle(frame, Point(frame.cols-epais, 0), Point(frame.cols, frame.rows), noir, -1);
        }

        for(int i = 0;i<frame.rows; i+=delta)
        {
            for(int j=0; j<frame.cols; j+=delta)
            {
                line(frame, Point(0, i), Point(scale, i), blanc,1);
                line(frame, Point(j, frame.rows), Point(j, frame.rows-scale), blanc,1);
            }
        }

        TriangleDown(frame, Point(centroid.x, centroid.y +(frame.rows-centroid.y)-offset), blanc, 20, 1, 8);
        TriangleLeft(frame, Point(centroid.x-(centroid.x-offset), centroid.y), blanc, 20, 1, 8);

        stringstream x, y;
        x << centroid.x;
        y << centroid.y;

        putText(frame, x.str(), Point(centroid.x-10, centroid.y +(frame.rows-centroid.y)-offset-5),1,1, blanc, 1);
        putText(frame, y.str(), Point(centroid.x-(centroid.x-offset-5), centroid.y+5),1,1, blanc, 1);

        if(centroid.inside(zoneTRACK))
        {
            Square(frame, Point(frame.cols/2, frame.rows/2), vert, 240, 1, 8);
            circle(frame, center, 1, rouge, 2);
            line(frame, center, centroid, bleu, 2);
        }

        if(centroid.inside(zoneLOCK))
        {
            Square(frame, Point(frame.cols/2, frame.rows/2), rouge, taille, 3, 8);
            circle(frame, center, 1, rouge, 2);
            line(frame, center, centroid, bleu, 2);
        }

        if(centroid.inside(zoneTRACK))
            putText(frame, "ON",Point(430,25),1,1, blanc,2);
        else
            putText(frame, "OFF",Point(430,25),1,1, blanc,1);

        if(centroid.inside(zoneLOCK))
            putText(frame, "ON",Point(570,25),1,1,blanc,2);
        else
            putText(frame, "OFF",Point(570,25),1,1,blanc,1);

        putText(frame, date(),Point(60,25),1,1,blanc,1);
        putText(frame, heure(),Point(200,25),1,1,blanc,1);
        putText(frame, "M.TRACK: ", Point(350,25),1,1,blanc,1);
        putText(frame, "M.LOCK: ", Point(500,25),1,1,blanc,1);

        Frame();

        waitKey(32);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    destroyAllWindows();
    close();
    exit(0);
}

void MainWindow::Frame()
{
    cv::resize(frame, frame, Size(ui->label->width(), ui->label->height()));
    cvtColor(frame, frame,CV_BGR2RGB);

    QImage imgFrame= QImage((uchar*) frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
    QPixmap pixFrame = QPixmap::fromImage(imgFrame);

    ui->label->setPixmap(pixFrame);
}

const string MainWindow::date()
{
    time_t     now = time(0);
    struct tm  tstruct;
     char       buf[80];
    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), "%d-%m-%Y", &tstruct);

    return buf;
}

const string MainWindow::heure()
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), "%X", &tstruct);

    return buf;
}

void MainWindow::Square(Mat& img, Point position, const Scalar& color, int markerSize, int thickness, int line_type)
{
    line(img, Point(position.x-(markerSize/2), position.y-(markerSize/2)), Point(position.x+(markerSize/2), position.y-(markerSize/2)), color, thickness, line_type);
    line(img, Point(position.x+(markerSize/2), position.y-(markerSize/2)), Point(position.x+(markerSize/2), position.y+(markerSize/2)), color, thickness, line_type);
    line(img, Point(position.x+(markerSize/2), position.y+(markerSize/2)), Point(position.x-(markerSize/2), position.y+(markerSize/2)), color, thickness, line_type);
    line(img, Point(position.x-(markerSize/2), position.y+(markerSize/2)), Point(position.x-(markerSize/2), position.y-(markerSize/2)), color, thickness, line_type);
}

void MainWindow::TriangleDown(Mat& img, Point position, const Scalar& color, int markerSize, int thickness, int line_type)
{
    line(img, Point(position.x+(markerSize/2), position.y), Point(position.x, position.y+(markerSize/2)), color, thickness, line_type);
    line(img, Point(position.x, position.y+(markerSize/2)), Point(position.x-(markerSize/2), position.y), color, thickness, line_type);
    line(img, Point(position.x-(markerSize/2), position.y), Point(position.x+(markerSize/2), position.y), color, thickness, line_type);
}

void MainWindow::TriangleLeft(Mat& img, Point position, const Scalar& color, int markerSize, int thickness, int line_type)
{
    line(img, Point(position.x, position.y+(markerSize/2)), Point(position.x-(markerSize/2), position.y), color, thickness, line_type);
    line(img, Point(position.x-(markerSize/2), position.y), Point(position.x, position.y-(markerSize/2)), color, thickness, line_type);
    line(img, Point(position.x, position.y-(markerSize/2)), Point(position.x, position.y+(markerSize/2)), color, thickness, line_type);
}

void MainWindow::on_checkBox_clicked(bool checked)
{
    if(checked==true)
        check=true;
    else
        check=false;
}

void MainWindow::on_radioBlanc_clicked()
{
    couleur = blanc;
}

void MainWindow::on_radioNoir_clicked()
{
    couleur = noir;
}

void MainWindow::on_radioRouge_clicked()
{
    couleur = rouge;
}

void MainWindow::on_radioVert_clicked()
{
    couleur = vert;
}

void MainWindow::on_radioBleu_clicked()
{
    couleur = bleu;
}

void MainWindow::on_verticalSlider_valueChanged(int value)
{
    taille=value;
}
