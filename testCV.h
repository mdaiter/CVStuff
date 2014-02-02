#ifndef TESTCV_H
#define TESTCV_H
#include<opencv2/ocl/ocl.hpp>
#include<opencv2/imgproc/imgproc.hpp>
class testCV{
    public:
        std::vector<cv::Rect> detectPeople(cv::Mat&);
        std::vector<cv::Rect> detectFaces(cv::Mat&);
        testCV();
        ~testCV();
    private:
        cv::ocl::OclCascadeClassifier *faceDetector;
        cv::ocl::OclCascadeClassifier *bodyDetector;
        cv::ocl::HOGDescriptor hog;
};
#endif
