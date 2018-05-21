#include "chooseband.h"
#include "ui_chooseband.h"
#include<QTextCursor>
#include <QDebug>
ChooseBand::ChooseBand(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChooseBand)
{
    ui->setupUi(this);
    RGBBand[0]=-1;
    RGBBand[1]=-1;
    RGBBand[2]=-1;
    GrayBand = -1;
    // 当信号和槽都是同一个类里的时候，可以在该类中的构造函数中链接信号和曹
    connect(ui->BtnGray,&QRadioButton::clicked,this,&ChooseBand::switch_stackedwidget);
    connect(ui->BtnRGB,&QRadioButton::clicked,this,&ChooseBand::switch_stackedwidget);
    connect(ui->treeWidget,&QTreeWidget::itemClicked,
            this,&ChooseBand::setSelected);
}

ChooseBand::~ChooseBand()
{
    delete ui;
}

//用户选择灰色单选还是彩色单选，stackedwidget的内容发生变化（翻页+选择波段的文本显示在各标签中）
void ChooseBand:: switch_stackedwidget()
{
    if(ui->BtnGray->isChecked())
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
    if(ui->BtnRGB->isChecked())
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
}


void ChooseBand::setSelected(QTreeWidgetItem *item,int i)
{
    QTreeWidgetItem *parent=item->parent();    //先找父亲，然后根据孩子的索引找到对应行的item
    if(NULL == parent)
        return;
    int col=parent->indexOfChild(item);

    if(ui->BtnGray->isChecked())
    {
         ui->label_2->setText(item->text(i));
         GrayBand=col;
         ui->stackedWidget->setCurrentIndex(0);
    }
    if(ui->BtnRGB->isChecked())
    {
        if(ui->Btn_R->isChecked())
        {
            ui->label_3->setText(item->text(i));
            RGBBand[0]=col;
            ui->Btn_G->setChecked(true);
        }
       else if(ui->Btn_G->isChecked())
        {
            ui->label_4->setText(item->text(i));
            RGBBand[1]=col;
            ui->Btn_B->setChecked(true);
        }
        else
        {
            ui->label_5->setText(item->text(i));
            RGBBand[2]=col;
            ui->Btn_R->setChecked(true);
        }
        ui->stackedWidget->setCurrentIndex(1);
    }
}

