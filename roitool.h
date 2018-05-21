#ifndef ROITOOL_H
#define ROITOOL_H

#include <QWidget>
#include<QLabel>
#include<QTableWidget>
#include<QTableWidgetItem>
#include<QGridLayout>
#include<QPushButton>
#include<QColor>
#include<QHeaderView>
class RoiTool : public QWidget
{
    Q_OBJECT
public:
    explicit RoiTool(QWidget *parent = 0);
    ~RoiTool();
    QPushButton *btn_delregion;
    QPushButton *btn_goto;
    QPushButton *btn_delpart;
    QColor current_color;
    int current_row;
signals:

public slots:
    void receivenums(int);

private:
    void createRoiTable();
    void addnewcategory();
    void deletecategory();
    void sectionClicked(int);

    void setcurrcolor(int,int);
    QTableWidget *roitable;
    QGridLayout * mainLayout;
    QGridLayout * topLayout;
    QGridLayout * bottomLayout;
    QPushButton *btn_newregion;

};

#endif // ROITOOL_H
