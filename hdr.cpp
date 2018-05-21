#include"hdr.h"
using namespace std;
#include<QDebug>
#include<QRegExp>

info parce_header(string str)
{
    info result;
    char  *str1 =new char[50];
    size_t index1,index2,len;

    index1 = str.find("samples = ");
    index2 = str.find("lines");
    len = (string("samples = ")).length();
    str.copy(str1, index2 -index1- len, index1 + len);
    result.height = atoi(str1);

    index1 = str.find("lines   = ");
    index2 = str.find("bands");
    len = (string("lines   = ")).length();
    str.copy(str1, index2 - index1 - len, index1 + len);
    result.width = atoi(str1);

    index1 = str.find("bands   = ");
    index2 = str.find("header offset");
    len = (string("bands   = ")).length();
    str.copy(str1, index2 - index1 - len, index1 + len);
    result.band = atoi(str1);

    index1 = str.find("data type = ");
    index2 = str.find("interleave");
    len = ((string)("data type = ")).length();
    str.copy(str1, index2 - index1 - len, index1 + len);
    result.data_type = atoi(str1);


    if (str.find("bsq") != string::npos)
        result.interleave = true;
    else
        result.interleave = false;

    index1=QString::fromStdString(str).indexOf("wavelength = {\n");
    if(index1 != -1)
    {
        QString nums = QString::fromStdString(str.substr(index1));
        QRegExp numReg("\\d+\\.?\\d+");
        int pos = 0;
        while ((pos = numReg.indexIn(nums, pos)) != -1) {
            result.wavelength << numReg.cap(0).toDouble();
            pos += numReg.matchedLength();
        }
    }
    else
        result.wavelength << 0;
    delete [] str1;
    return result;
}
