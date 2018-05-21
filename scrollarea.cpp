#include "scrollarea.h"
#include "ui_scrollarea.h"
#include <QDebug>
#include "mainwindow.h"
using namespace cv;
Mat QImage2cvMat(QImage image);
QImage cvMat2QImage(const Mat& mat);

ScrollArea::ScrollArea(QWidget *parent) :
    QScrollArea(),          //基类构造函数的初始化，是个重载函数，选择的没有参数的构造函数，保证打开的是个空窗口
    ui(new Ui::ScrollArea),
    parent(parent) //将mainwindow的父窗口传递给成员变量parent
{
    ui->setupUi(this);
    color = qRgb(255,0,0);
    startPos = Point(-1,-1);
    prevPos = Point(-1,-1);
    flag=-1;
    nclass =0;
    mask =Mat(0,0,CV_8UC3);
}

ScrollArea::~ScrollArea()
{
    delete ui;
    if(label)
    {
        delete label;
    }
}

void ScrollArea::set_image(QImage *ppm )
{
    label = new QLabel();
    original = QImage2cvMat(*ppm).clone();
    disImage =  original.clone();
    Label.resize(ppm->height()*ppm->width() , 0);    // 标签初始化

    if (mask.rows == 0) //只有在第一次选训练样本的时候才需要给mask重新初始化
    {
        mask =Mat(ppm->height(), ppm->width(),CV_8UC3,Scalar(255,255,255));
    }
    else
    {
        for (int i = 0; i < mask.cols; i++)
        {
            for (int j = 0; j < mask.rows; j++)
            {
                if (mask.at<Vec3b>(j,i)[0] ==255 && mask.at<Vec3b>(j,i)[1] ==255  && mask.at<Vec3b>(j,i)[2] ==255 )
                {
                }
                else
                {
                    disImage.at<Vec3b>(j,i) = mask.at<Vec3b>(j,i);
                }
            }
        }
    }
    pix = QPixmap::fromImage( cvMat2QImage(disImage) );
    update();
    label->setPixmap(pix);
    this->setWidget(label);
    this->setMaximumHeight(1024);
    this->setMaximumWidth(768);
    this->resize(QSize( ppm->width()+5, ppm->height() +5));
    return;
}

void ScrollArea::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton )
    {
        flag =0;
        startPos =Point(ev->x(),ev->y());
        prevPos =Point(ev->x(),ev->y());
        if(((MainWindow*)parent) ->roi){     //此处将父窗口的roi变量中的current_color  current_row 值传递过来
            color =((MainWindow*)parent) ->roi->current_color;
            tempLabel =((MainWindow *)parent)->roi->current_row + 1;
            if(tempLabel > nclass)
                nclass =tempLabel;
            qDebug() <<"***********************";
            qDebug() <<"class index is:"<<tempLabel;
            qDebug() <<"Paint color. Blue:"<<color.blue()<<" Green:" << color.green() <<"    Red:"<< color.red();
            qDebug() <<"***********************";
        }
    }
    else if(ev->button() == Qt::RightButton && startPos.x >=0 && startPos.y >=0)
    {
        flag =1;
        Rect ccomp;
        floodFill(mask, Point(ev->x(),ev->y()),Scalar(color.blue(),color.green(),color.red()),
                  &ccomp,Scalar(10,10,10),Scalar(10,10,10));

        num_pixels_circle = 0;

        for (int i = 0; i < mask.cols; i++)
        {
            for (int j = 0; j < mask.rows; j++)
            {
                if (mask.at<Vec3b>(j,i)[0]==255  && mask.at<Vec3b>(j,i)[1]== 255 && mask.at<Vec3b>(j,i)[2]== 255)
                {

                }
                else if(mask.at<Vec3b>(j,i)[0]==color.blue()  && mask.at<Vec3b>(j,i)[1]== color.green() && mask.at<Vec3b>(j,i)[2]== color.red())
                {
                    disImage.at<Vec3b>(j,i) = mask.at<Vec3b>(j,i);
                    Label[i+j*mask.cols] = tempLabel;
                    num_pixels_circle ++;
                }
                else
                {
                    disImage.at<Vec3b>(j,i) = mask.at<Vec3b>(j,i);
                }
            }
        }
        pix = QPixmap::fromImage(cvMat2QImage(disImage));
        update();
        label->setPixmap(pix);
        emit sendnums(num_pixels_circle);

        //test!!!!
        int count =0;
        for(int k=0;k<mask.rows *mask.cols;k++){
            if(Label[k] != 0)
                count++;
        }
        qDebug()<<"count"<<count;
        //test!!!!!
    }
}

void ScrollArea::mouseMoveEvent(QMouseEvent *ev)
{
    if(ev->buttons() & Qt::LeftButton)
    {
        line(disImage,Point(ev->x(),ev->y()),prevPos,Scalar(color.blue(),color.green(),color.red()));
        line(mask,Point(ev->x(),ev->y()),prevPos,Scalar(color.blue(),color.green(),color.red()));
        prevPos =Point(ev->x(),ev->y());
        pix = QPixmap::fromImage( cvMat2QImage(disImage) );
        update();
        label->setPixmap(pix);
    }
}

void ScrollArea::mouseReleaseEvent(QMouseEvent *ev)
{
    if(flag == 0)
    {
        line(disImage,Point(ev->x(),ev->y()),startPos,Scalar(color.blue(),color.green(),color.red()));
        line(mask,Point(ev->x(),ev->y()),startPos,Scalar(color.blue(),color.green(),color.red()));
        prevPos =Point(-1,-1);
        pix = QPixmap::fromImage( cvMat2QImage(disImage));
        update();
        label->setPixmap(pix);
    }
    flag =-1;
}

void ScrollArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(QPoint(0,0),pix);
}

void ScrollArea::deleteclass()
{
    if(nclass > 0)
    {
        nclass --;
    }
    if(((MainWindow*)parent) ->roi){
        color =((MainWindow*)parent) ->roi->current_color;
    }

    for(int i=0;i<mask.cols;i++){
        for(int j=0;j<mask.rows;j++){
            if(mask.at<Vec3b>(j,i)[0] ==color.blue() && mask.at<Vec3b>(j,i)[1] ==color.green() && mask.at<Vec3b>(j,i)[2] ==color.red())
            {
                mask.at<Vec3b>(j,i)[0] =mask.at<Vec3b>(j,i)[1]=mask.at<Vec3b>(j,i)[2] =255;
                disImage.at<Vec3b>(j,i) = original.at<Vec3b>(j,i);
                Label[i+j*mask.cols] = 0;
            }
        }
    }
    pix = QPixmap::fromImage( cvMat2QImage(disImage));
    update();
    label->setPixmap(pix);

    //test!!!!!
    int count =0;
    for(int k=0;k<mask.rows *mask.cols;k++){
        if(Label[k] != 0)
            count++;
    }
    qDebug()<<"after delete count:"<<count;
    qDebug()<<"after delete class:"<<nclass;
    //test!!!!!
}
