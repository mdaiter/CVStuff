#include<stdio.h>
#include<opencv2/gpu/gpu.hpp>
#include<opencv2/objdetect/objdetect.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/contrib/contrib.hpp>
#include"testCV.h"
#define DIST_HEIGHT 10
using namespace cv;

testCV::testCV(){
    hog.setSVMDetector(gpu::HOGDescriptor::getDefaultPeopleDetector());
    faceDetector = new gpu::CascadeClassifier_GPU("/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml");
    bodyDetector = new gpu::CascadeClassifier_GPU("/usr/share/opencv/haarcascades/haarcascade_mcs_upperbody.xml");
}

testCV::~testCV(){

}

Rect* testCV::detectFaces(Mat& frame){
    Mat frameGray, dst;
    cvtColor(frame, frameGray, CV_BGR2GRAY);
    gpu::GpuMat src(frameGray);
    equalizeHist (frameGray, frameGray);
    gpu::GpuMat objbuf;

    int detections_number = faceDetector->detectMultiScale(src, objbuf, 1.2, 4, Size(20,20));
    printf("Faces found:%d\n", detections_number);
    objbuf.colRange(0, detections_number).download(dst);
    Rect* faces = dst.ptr<Rect>();
    for (int i = 0; i < detections_number; i++){
        rectangle(frame, faces[i], Scalar(255));
    }
    return dst.ptr<Rect>();
}

bool inside(Rect x, Rect y){

    return (x.x > y.x && x.y > y.y && (x.x + x.width) >(y.x + y.width) && (x.y + x.height) && (y.y+y.height) );
}

std::vector<Rect> testCV::detectPeople(Mat& frame){
    gpu::HOGDescriptor hog;
    hog.setSVMDetector(gpu::HOGDescriptor::getDefaultPeopleDetector());
    Mat frameGray;
    cvtColor(frame, frameGray, CV_BGR2BGRA);
    gpu::GpuMat src(frameGray);

    std::vector<Rect> found;
    std::vector<Rect> foundFiltered;
    hog.detectMultiScale(src, found, 0.1, Size(8,8), Size(0,0), 1.05, 2);
    printf("Found size:%d\n", found.size());
    for (size_t i = 0; i < found.size(); i++){
        Rect r = found[i];
        for (size_t j = 0; j < found.size(); j++){
            Rect q = found[j];
            if (!inside(r, q)){
                foundFiltered.push_back(r);
            }
        }
    }
    for (size_t k = 0; k < foundFiltered.size(); k++){
        rectangle(frame, foundFiltered[k].tl(), foundFiltered[k].br(), cv::Scalar(0,255,0), 3);
    }
    return found;
}

int main(){
    testCV *cv = new testCV();
    VideoCapture capture(0);
    cvNamedWindow("result", WINDOW_NORMAL);
        while(true){
            Mat frame, frameGray;
            capture >> frame;
            vector<Rect> people = cv->detectPeople(frame);
            Rect* faces = cv->detectFaces(frame);
            imshow("result", frame);
            waitKey(2);
        }
}
