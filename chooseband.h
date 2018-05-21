#ifndef CHOOSEBAND_H
#define CHOOSEBAND_H
#include<QLabel>
#include <QMainWindow>
#include<QTreeWidgetItem>
#include<QButtonGroup>
namespace Ui {
class ChooseBand;
}

class ChooseBand : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChooseBand(QWidget *parent = 0);
    ~ChooseBand();

public:
    QList<QTreeWidgetItem *> rootList;
    int GrayBand;//加载显示图像，GrayBand存储用户选择的灰度波段索引，RGBBand存储用户选择的RGB波段索引
    int  RGBBand[3];
    Ui::ChooseBand *ui;

private:
    void switch_stackedwidget();
    void  setSelected(QTreeWidgetItem *,int);
};

#endif // CHOOSEBAND_H
