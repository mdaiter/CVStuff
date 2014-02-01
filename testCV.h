#ifndef TESTCV_H
#define TESTCV_H
#include<opencv2/gpu/gpu.hpp>
#include<opencv2/imgproc/imgproc.hpp>
class testCV{
    public:
        std::vector<cv::Rect> detectPeople(cv::Mat&);
        cv::Rect* detectFaces(cv::Mat&);
        testCV();
        ~testCV();
    private:
        cv::gpu::CascadeClassifier_GPU *faceDetector;
        cv::gpu::CascadeClassifier_GPU *bodyDetector;
        cv::gpu::HOGDescriptor hog;
};
#endif
