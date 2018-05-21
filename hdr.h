#ifndef HDR
#define HDR
#include<iostream>
#include<QString>
#include<string>
#include <QList>
class info{
public:
    int height;
    int width;
    int band;
    QList<double> wavelength;
    bool interleave;
    int data_type;
};
#endif // HDR

