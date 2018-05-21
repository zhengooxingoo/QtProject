#include "roitool.h"
#include"QTextcodec"
#include<QDebug>
#include<QColor>
RoiTool::RoiTool(QWidget *parent) : QWidget(parent)
{
    current_color = qRgb(255,0,0);
    current_row = 0;
    createRoiTable();

    mainLayout = new QGridLayout;
    setLayout(mainLayout);
    mainLayout->addWidget(roitable,0,0);
    bottomLayout = new QGridLayout;
    btn_newregion =new QPushButton(QString::fromLocal8Bit ("新加类"));
    bottomLayout->addWidget(btn_newregion,0,0);
    btn_delregion = new QPushButton(QString::fromLocal8Bit ("删除类"));
    bottomLayout->addWidget(btn_delregion,0,1);
    btn_goto =new QPushButton(QString::fromLocal8Bit ("转到块"));
    bottomLayout->addWidget(btn_goto,1,0);
    btn_delpart =new QPushButton(QString::fromLocal8Bit ("删除块"));
    bottomLayout->addWidget(btn_delpart,1,1);
    mainLayout->addLayout(bottomLayout,1,0);

    addnewcategory();//默认情况下，有一个新添加的类
    connect(btn_newregion,&QPushButton::clicked,this,&RoiTool::addnewcategory);
    connect(btn_delregion,&QPushButton::clicked,this,&RoiTool::deletecategory);
    connect(roitable->verticalHeader() , &QHeaderView::sectionClicked ,
            this,&RoiTool::sectionClicked);

    setWindowTitle(QString::fromLocal8Bit ("提取ROI"));
    resize(700, 300);
}

RoiTool::~RoiTool()
{

}

void RoiTool::createRoiTable()
{
    roitable = new QTableWidget(0,4);
    roitable->setSelectionBehavior(QAbstractItemView::SelectRows);
    QStringList headers; //设置列名
    headers<<QString::fromLocal8Bit ("类别");
    headers<<QString::fromLocal8Bit ("颜色");
    headers<<QString::fromLocal8Bit ("像素数");
    headers<<QString::fromLocal8Bit ("多边形块数");
    roitable->setHorizontalHeaderLabels(headers);
    roitable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
}

void RoiTool::addnewcategory()
{
    int row = roitable->rowCount();

    QTableWidgetItem *categoryitem = new QTableWidgetItem();
    categoryitem->setFlags(categoryitem->flags() | Qt::ItemIsEditable);

    QTableWidgetItem *coloritem = new QTableWidgetItem();
    switch(row)
    {
    case 0:
        coloritem->setText(QString::fromLocal8Bit ("红色"));
        break;
    case 1:
        coloritem->setText(QString::fromLocal8Bit ("绿色"));
        break;
    case 2:
        coloritem->setText(QString::fromLocal8Bit ("蓝色"));
        break;
    case 3:
        coloritem->setText(QString::fromLocal8Bit ("黄色"));
        break;
    case 4:
        coloritem->setText(QString::fromLocal8Bit ("青色"));
        break;
    default:
        break;
    }
    coloritem->setFlags(coloritem->flags() | Qt::ItemIsEditable);

    QTableWidgetItem *pixelsitem = new QTableWidgetItem("0");
    pixelsitem->setFlags(pixelsitem->flags() | Qt::ItemIsEditable);

    QTableWidgetItem *polygonsitem = new QTableWidgetItem("0");
    polygonsitem->setFlags(polygonsitem->flags() | Qt::ItemIsEditable);

    roitable->insertRow(row);
    roitable->setItem(row,0,categoryitem);
    roitable->setItem(row,1,coloritem);
    roitable->setItem(row,2,pixelsitem);
    roitable->setItem(row,3,polygonsitem);
}

void RoiTool::deletecategory()
{
//    qDebug() <<"delete current color"<<current_color.blue() <<current_color.green() <<current_color.red() ;
    qDebug() << "delete current row"<<current_row;
     qDebug() <<"test the default  delete current row:"<< roitable->currentRow();
    roitable->removeRow(current_row);
}

void RoiTool::setcurrcolor(int row,int col )
{
    if(  roitable->item(row,col)->text() == QString::fromLocal8Bit ("红色"))
        current_color = qRgb(255,0,0);
      else if(roitable->item(row,col)->text() ==  QString::fromLocal8Bit ("绿色"))
        current_color = qRgb(0,255,0);
      else if(roitable->item(row,col)->text() == QString::fromLocal8Bit ("蓝色"))
        current_color = qRgb(0,0,255);
      else if (roitable->item(row,col)->text() == QString::fromLocal8Bit ("黄色"))
        current_color = qRgb(255,255,0);
      else if(roitable->item(row,col)->text() == QString::fromLocal8Bit ("青色"))
        current_color = qRgb(0,255,255);
      else
        current_color = qRgb(255,0,0);
//    qDebug() <<"Current color. Blue: " <<current_color.blue()
//            <<"   Green:"<< current_color.green()
//           <<"  Red:" << current_color.red();
}

void RoiTool::sectionClicked(int row)
{
    setcurrcolor(row, 1);
    current_row = row;
    qDebug() << "sectionClicked row:" <<row ;
}

void RoiTool::receivenums(int num_pixels_circle)
{
//    int num_pixels = roitable->item(current_row,2)->text().toInt() ;
    roitable->item(current_row,2)->setText(QString::number(num_pixels_circle));
    int num_blocks = roitable->item(current_row,3)->text().toInt();
    roitable->item(current_row,3)->setText(QString::number(++num_blocks));
}
