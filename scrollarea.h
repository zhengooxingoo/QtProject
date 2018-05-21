#ifndef SCROLLAREA_H
#define SCROLLAREA_H
#include "opencv2\highgui\highgui.hpp"
#include <opencv2\imgproc\imgproc.hpp>
#include <QScrollArea>
#include<QLabel>
#include<QPainter>
#include<QPen>
#include<QMouseEvent>
using namespace std;
using namespace cv;

namespace Ui {
class ScrollArea;
}

class ScrollArea : public QScrollArea
{
    Q_OBJECT

public:
    explicit ScrollArea(QWidget *parent = 0);
    ~ScrollArea();
    void set_image(QImage *ppm );
    void deleteclass();
    int num_pixels_circle;
    int tempLabel;
    int nclass;
    vector<int> Label;

private:
    Ui::ScrollArea *ui;
    QLabel *    label;
    QWidget* parent; //传递MainWindow父指针的变量
    QPixmap pix;
    Mat mask;
    Mat original;
    Mat disImage;
    Point startPos;
    Point prevPos;
    QColor color;
    int flag; //判断释放时，是左键释放（0）还是右键释放（1）

    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);

signals:
    void sendnums(int);

};
#endif // SCROLLAREA_H
