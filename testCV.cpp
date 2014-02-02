#include<stdio.h>
#include<opencv2/objdetect/objdetect.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/contrib/contrib.hpp>
#include"testCV.h"
#define DIST_HEIGHT 10
using namespace cv;

testCV::testCV(){
    hog.setSVMDetector(ocl::HOGDescriptor::getDefaultPeopleDetector());
    faceDetector = new ocl::OclCascadeClassifier();
    bodyDetector = new ocl::OclCascadeClassifier();
    faceDetector->load("/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml");
    bodyDetector->load("/usr/share/opencv/haarcascades/haarcascade_mcs_upperbody.xml");
}

testCV::~testCV(){

}

vector<Rect> testCV::detectFaces(Mat& frame){
    Mat frameGray, dst;
    cvtColor(frame, frameGray, CV_BGR2GRAY);
    ocl::oclMat src(frameGray);
    equalizeHist (frameGray, frameGray);
    vector<Rect> objbuf;
    faceDetector->detectMultiScale(src, objbuf, 1.2, 3, 0|CV_HAAR_SCALE_IMAGE, Size(20,20), Size(0,0));
    for (size_t i = 0; i < objbuf.size(); i++){
        rectangle(frame, objbuf[i], Scalar(255));
    }
    return objbuf;
}

bool inside(Rect x, Rect y){

    return (x.x > y.x && x.y > y.y && (x.x + x.width) >(y.x + y.width) && (x.y + x.height) && (y.y+y.height) );
}

std::vector<Rect> testCV::detectPeople(Mat& frame){
    ocl::HOGDescriptor hog;
    hog.setSVMDetector(ocl::HOGDescriptor::getDefaultPeopleDetector());
    Mat frameGray;
    cvtColor(frame, frameGray, CV_BGR2BGRA);
    ocl::oclMat src(frameGray);

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
            vector<Rect> faces = cv->detectFaces(frame);
            imshow("result", frame);
            waitKey(2);
        }
}
