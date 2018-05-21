#ifndef OPENCV_QT_H
#define OPENCV_QT_H
#include "opencv2\highgui\highgui.hpp"
#include <opencv2\imgproc\imgproc.hpp>
#include<QImage>

using namespace cv;
class opencv_qt
{
public:
    opencv_qt();
    ~opencv_qt();

    Mat QImage2cvMat(QImage image);
    QImage cvMat2QImage(const cv::Mat& mat)  ;
};

#endif // OPENCV_QT_H
