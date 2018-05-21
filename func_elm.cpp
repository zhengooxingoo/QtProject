#include<iostream>
#include<opencv2\opencv.hpp>
using namespace std;
using namespace cv;

void func_elm(Mat traindata, Mat &trainlabel,
    Mat testdata,Mat &testlabel,
    int nHiddenNeurons, int nclass){

    int nTrainData = traindata.cols;
    int nInputNeurons = traindata.rows;
    int nTestData = testdata.cols;


    Mat targets = Mat::zeros(nclass, nTrainData, CV_64FC1);

    for (int i = 0; i < nTrainData; i++){
        targets.at<double>(trainlabel.at<uchar>(i,0)-1 , i) = 1;
    }

    // generate random input weight matrix
    Mat inW=Mat(nHiddenNeurons,nInputNeurons,CV_64FC1);
    randu(inW, Scalar(double(-1)), Scalar(double(1)) );

    Mat bias = Mat(nHiddenNeurons, 1, CV_64FC1);
    randu(bias, Scalar(double(0)), Scalar(double(1)));

    Mat preH = Mat(nHiddenNeurons,nTrainData,CV_64FC1);
    Mat biasM =Mat(nHiddenNeurons,nTrainData,CV_64FC1);
    preH= inW * traindata;
    repeat(bias, 1, nTrainData, biasM);
    preH = preH + biasM;
    exp(-preH, preH);
    Mat H = Mat(nHiddenNeurons,nTrainData,CV_64FC1);
    H =1.0 / (1.0 + preH);//按位除法A/B     除法A*B.inv()
    Mat pinv= Mat(nTrainData,nHiddenNeurons,CV_64FC1);
    invert(H, pinv, cv::DECOMP_SVD);
    Mat outW = Mat(nHiddenNeurons,nclass,CV_64FC1);
    outW = (targets * pinv).t();

    Mat scores=Mat(nTrainData,nclass,CV_64FC1);
    scores = H.t() * outW;

    for(int i=0;i<nTrainData;i++){
        int max =0;
        int index =0;
        for(int j=0;j<nclass;j++){
            if(scores.at<double>(i,j) >max){
                max = scores.at<double>(i,j) ;
                index =j;
            }
        }
        trainlabel.at<uchar>(i,0) = index+1;
    }

    //test
    Mat test_preH= Mat(nHiddenNeurons,nTestData,CV_64FC1);;
    Mat test_biasM= Mat(nHiddenNeurons,nTestData,CV_64FC1);;
    test_preH = inW * testdata;
    repeat(bias, 1, nTestData, test_biasM);
    test_preH = test_preH + test_biasM;
    exp(-test_preH, test_preH);
    Mat test_H= Mat(nHiddenNeurons,nTestData,CV_64FC1);;
    test_H = 1.0 / (1.0 + test_preH);
    Mat test_scores=Mat(nTestData,nclass,CV_64FC1);
    test_scores = test_H.t() * outW;

    for(int i=0;i<nTestData;i++){
        int max =0;
        int index =0;
        for(int j=0;j<nclass;j++){
            if(test_scores.at<double>(i,j) >max){
                max = test_scores.at<double>(i,j) ;
                index =j;
            }
        }
        testlabel.at<uchar>(i,0) = index+1;
    }
    return ;
}
