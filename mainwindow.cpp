#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<string>
#include <QFileDialog>
#include<QDebug>
#include<QTextEdit>
#include<QCloseEvent>
using namespace std;
using namespace cv;

void func_elm(Mat traindata, Mat &trainlabel,
    Mat testdata,Mat &testlabel,
    int nHiddenNeurons, int nclass);

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

void MainWindow::on_actionOpen_triggered()
{
    info parce_header(string str);//函数的声明

    //打开高光谱数据文件
    QString path=QFileDialog::getOpenFileName(this,
                                              "打开","../../","images(*.img)");
    QFileInfo fileInfo;
    QString filename;
    if(path.isEmpty() == false)
    {
        QFile file1(path);
        fileInfo =file1.fileName();
        filename=fileInfo.fileName();
        if(file1.open(QIODevice::ReadOnly) ==true )
        {
            array=file1.readAll();
            lsize=array.size();
        }
        else return;
    }
    else
        return;

    //处理图像头文件（包含行数、列数、波段数等）
    QString path_hdr =path.replace("img","hdr");    //注：此时path字符串已经变了
    if(path_hdr.isEmpty() == false)
    {
        QFile file(path_hdr);
        bool isOk = file.open(QIODevice::ReadOnly);
        if(isOk == true)
        {
            hdr = parce_header(QString(file.readAll()).toStdString());
            image_size= hdr.height *hdr.width;
        }
        else
            return;
    }
    else
        return;
    qDebug() <<"image size:" <<image_size;
    qDebug() <<"file size:" <<lsize;

    //子类初始化对象
    roi = new RoiTool;
    roi->setAttribute(Qt::WA_QuitOnClose,false);
    new_image = new ScrollArea(this);//this传递mainwindow指针，同一个文件，显示不同波段，也只使用这同一个内存
    new_image->setAttribute(Qt::WA_QuitOnClose,false); //注：Qt::WA_QuitOnClose用于子窗口和主窗口关闭联动
    select =new ChooseBand();
    select->setAttribute(Qt::WA_QuitOnClose,false);

    //在主窗口打开文件时，同时显示可用波段
    //具体实现：QTreeWidget  -- > rootList（QList<QTreeWidgetItem *>） -- > QTreeWidgetItem
    QTreeWidgetItem *imageItem= new QTreeWidgetItem; //列表头
    QTreeWidgetItem *imageItem_band;//列表波段
    imageItem->setText(0,filename);
    select->rootList.append(imageItem);
    for(int i=0;i<hdr.band;i++)
    {
        if(hdr.band ==1)
        {
            imageItem_band = new QTreeWidgetItem(imageItem,QStringList(QString("波段 1")));
            imageItem->addChild(imageItem_band); //添加子节点
            break;
        }
        imageItem_band = new QTreeWidgetItem(
                    imageItem,
                    QStringList(QString("波段 %1 (%L2)")
                                .arg(i+1)
                                .arg(QString::number( hdr.wavelength.at(i), 'f',4 ))
                                )
                    ); //子节点1
        imageItem->addChild(imageItem_band); //添加子节点
    }
    select->ui->treeWidget->insertTopLevelItems(0,select->rootList);
    select->ui->treeWidget->expandAll(); //结点全部展开
    select->show();

    //默认：加载灰色图像，用户双击某一波段，直接显示该图片
    connect(select->ui->treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this,SLOT(showSelectedImage(QTreeWidgetItem *,int)));
    connect(select->ui->Btn_load_band,&QPushButton::clicked,this,&MainWindow::load_gray);
    connect(select->ui->Btn_load_color,&QPushButton::clicked,this,&MainWindow::load_rgb);
    connect(new_image,SIGNAL(sendnums(int)),roi,SLOT(receivenums(int)));
    connect(roi->btn_delregion,&QPushButton::clicked,new_image,&ScrollArea::deleteclass);
}

//主窗口关闭事件重写
void MainWindow::closeEvent(QCloseEvent *event)
{
    switch(  QMessageBox::information( this, "关闭窗口",
                                       "你真的要关闭窗口么？","Yes", "No", 0, 1 ) )
    {
    case 0:
    {
        event->accept();
        break;
    }
    case 1:
    {
        event->ignore();
        break;
    }
    }
}

//用户双击某行的波段，显示灰色图像
void MainWindow::showSelectedImage(QTreeWidgetItem *item,int )
{
    if(select->ui->BtnGray->isChecked())
    {
        QTreeWidgetItem *parent=item->parent();
        if(NULL == parent)
            return;
        int col=parent->indexOfChild(item);
        select->GrayBand =col;
        load_gray();
    }
}

void MainWindow::load_gray()
{
    if(select->GrayBand==-1)
    {
        QMessageBox::critical(this,
                              ("警告"),
                              ("请完成灰度显示的波段选择"));
        return;
    }

    switch(hdr.data_type)
    {
    case 1:
    {
        uchar *band_data= new uchar[image_size];

        memcpy(band_data,(uchar *)(array.data()+select->GrayBand*image_size),image_size);
        Mat temp(hdr.width,hdr.height,CV_8UC1,(void *)band_data);
        equalizeHist(temp,temp);
        showImage= QImage (temp.data,hdr.height,hdr.width,temp.step,
                           QImage::Format_Indexed8);

        new_image->set_image(&showImage);
        new_image->setBackgroundRole(QPalette::Dark);
        new_image->show();
        delete []  band_data;
        band_data= NULL;
        break;
    }
    default: break;
    }
}

void MainWindow::load_rgb()
{
    if(select->RGBBand[0] ==-1 || select->RGBBand[1] ==-1 || select->RGBBand[2] ==-1)
    {
        QMessageBox::critical(this,
                              ("警告"),
                              ("请完成RGB三通道的波段选择"));
        return;
    }

    switch(hdr.data_type)
    {
    case 1:
    {
        uchar *bandR_data= new uchar[image_size];
        memcpy(bandR_data,(uchar *)(array.data()+select->RGBBand[0]*image_size),image_size);
        uchar *bandG_data= new uchar[image_size];
        memcpy(bandG_data,(uchar *)(array.data()+select->RGBBand[1]*image_size),image_size);
        uchar *bandB_data= new uchar[image_size];
        memcpy(bandB_data,(uchar *)(array.data()+select->RGBBand[2]*image_size),image_size);

        Mat channels[3];
        channels[0] = Mat(hdr.width,hdr.height,CV_8UC1,(void *)bandR_data);
        channels[1] = Mat(hdr.width,hdr.height,CV_8UC1,(void *)bandG_data);
        channels[2] = Mat(hdr.width,hdr.height,CV_8UC1,(void *)bandB_data);
        equalizeHist(channels[0],channels[0]);
        equalizeHist(channels[1],channels[1]);
        equalizeHist(channels[2],channels[2]);
        Mat load_rgb;
        merge(channels, 3, load_rgb);

        showImage= QImage (load_rgb.data,hdr.height,hdr.width,load_rgb.step,
                           QImage::Format_RGB888);

        new_image->set_image(&showImage);
        new_image->setBackgroundRole(QPalette::Dark);
        new_image->show();
        delete []  bandR_data;
        bandR_data= NULL;
        delete []  bandG_data;
        bandG_data= NULL;
        delete []  bandB_data;
        bandB_data= NULL;
        break;
    }
    default: break;
    }
}




void MainWindow::on_actionROI_triggered()
{
    roi->show();
}

void MainWindow::on_actionclassify_triggered()
{
    int nclass =new_image->nclass;
    vector<int> Label(new_image->Label);
    Mat Data= Mat(hdr.band,image_size,CV_64FC1);
    double max=-1;
    for(int i=0;i<hdr.band;i++){
        max =-1;
        for(int j=0;j<image_size; j++){
            if(* (array.data()+j*i) >max)
                max =(double)(* (array.data()+j*i)) ;
        }
        for(int  k=0;k<image_size; k++){
            Data.at<double>(i,k) = (* (array.data()+k*i) )/max;
        }
    }

    int TestIndex =count(Label.begin(),Label.end(),0);
    int TrainIndex =image_size -TestIndex;
    qDebug() <<"TrainIndex.size: " <<TrainIndex;
    qDebug() <<"TestIndex.size: " <<TestIndex;


    Mat traindata=Mat(hdr.band,TrainIndex,CV_64FC1,Scalar(0));
    Mat trainlabel =Mat(TrainIndex,1,CV_8UC1);
    Mat testdata=Mat(hdr.band,TestIndex,CV_64FC1,Scalar(0));
    Mat testlabel = Mat(TestIndex,1,CV_8UC1);
    int n1=0;
    int n2 =0;
    for(int i=0;i<image_size;i++){
        if(Label[i] != 0){
            for(int j=0;j<hdr.band;j++){
                traindata.at<double>(j,n1) =Data.at<double>(j,i);
            }
            trainlabel.at<uchar>(n1,0) = Label[i];
            n1++;
        }
        else{
            for(int k=0;k<hdr.band;k++){
                testdata.at<double>(k,n2) =Data.at<double>(k,i);
            }
            n2 ++;
        }
    }
    qDebug() <<"traindata.size()" <<n1;
    qDebug() <<"testdata.size()" <<n2;
    func_elm(traindata, trainlabel,testdata,testlabel,50, nclass);

    int m1=0;
    int m2=0;
    for(int i=0;i<image_size;i++){
         if(Label[i] != 0){
             Label[i] = trainlabel.at<uchar>(m1,0);
             m1++;
         }
         else
         {
             Label[i] = testlabel.at<uchar>(m2,0);
             m2++;
         }
    }
    qDebug() <<"trainlabel.size()" <<m1;
    qDebug() <<"testlabel.size()" <<m2;

    Mat image=Mat(hdr.width,hdr.height,CV_8UC3,Scalar(255,255,255));
    int index_x=0; //row
    int index_y=0;
    for(int k=0;k<image_size;k++){
        index_x = floor(k / hdr.height); //ceil向上取整,floor向下取整
        index_y = k - index_x *hdr.height;
        if(Label[k] == 1){
            image.at<Vec3b>(index_x,index_y)[0] = 0;
            image.at<Vec3b>(index_x,index_y)[1] = 0;
            image.at<Vec3b>(index_x,index_y)[2] = 255;
        }
        else if(Label[k] == 2){
            image.at<Vec3b>(index_x,index_y)[0] = 0;
            image.at<Vec3b>(index_x,index_y)[1] = 255;
            image.at<Vec3b>(index_x,index_y)[2] = 0;
        }
        else if(Label[k] == 3){
            image.at<Vec3b>(index_x,index_y)[0] = 255;
            image.at<Vec3b>(index_x,index_y)[1] = 0;
            image.at<Vec3b>(index_x,index_y)[2] = 0;
        }
        else if(Label[k] == 4){
            image.at<Vec3b>(index_x,index_y)[0] = 0;
            image.at<Vec3b>(index_x,index_y)[1] = 255;
            image.at<Vec3b>(index_x,index_y)[2] = 255;
        }
        else if(Label[k] == 5){
            image.at<Vec3b>(index_x,index_y)[0] = 255;
            image.at<Vec3b>(index_x,index_y)[1] = 255;
            image.at<Vec3b>(index_x,index_y)[2] = 0;
        }
        else{
            image.at<Vec3b>(index_x,index_y)[0] = 255;
            image.at<Vec3b>(index_x,index_y)[1] = 255;
            image.at<Vec3b>(index_x,index_y)[2] = 255;
        }
    }
    GaussianBlur( image, image, Size(3,3), 0, 0, BORDER_DEFAULT );
    imshow("classify window",image);
}
