#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "opencv2\highgui\highgui.hpp"
#include <opencv2\imgproc\imgproc.hpp>
#include"hdr.h"
#include"roitool.h"
#include"scrollarea.h"
#include"chooseband.h"
#include "ui_chooseband.h"
#include<QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    RoiTool *roi;   //用于RoiTool 和 ScrollArea之间的信息交换

private slots:
    void on_actionOpen_triggered();
    void on_actionROI_triggered();
    void on_actionclassify_triggered();
    void showSelectedImage(QTreeWidgetItem *,int);

private:
    ScrollArea *new_image ;
    ChooseBand *select;
    info hdr ; //头文件解析
    QByteArray array; //文件的数据
    QImage showImage;//图像显示的QImage对象
    qint64 lsize= 0;  //文件大小
    qint64 image_size=0; //一个波段的图像大小

private:
    Ui::MainWindow *ui;
    void load_rgb();
    void load_gray();
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
