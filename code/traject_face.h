#ifndef OPENCV_TRAJECT_FACE_H
#define OPENCV_TRAJECT_FACE_H
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <cmath>

#include "./flppybird/Flppybird.h" //todo

int faceMode;

using cv::cvtColor;
using cv::imread;
using cv::imshow;
using cv::Mat;
using cv::Point;
using cv::Rect;
using cv::Scalar;
using cv::waitKey;
using std::cout;
using std::endl;
using std::vector;

int traject_face();
void dispInfo();

int traject_face()
{
    dispInfo();
    cv::VideoCapture cap(1);
    if (!cap.isOpened())
    {
        cout << "Camera opening failed!" << endl;
        return -1;
    }

    cv::CascadeClassifier face_cascade;
    std::string face_xml_filename =
        "F:/Program Files/opencv/opencv/sources/data/haarcascades/haarcascade_frontalface_default.xml";
    if (!face_cascade.load(face_xml_filename))
    {
        std::cout << "File not found!\n";
        return -1;
    }

    Mat imgOriginal, imgGray;
    vector<Point> pts;
    cap >> imgOriginal;

    Flppybird fp = Flppybird(imgOriginal); //todo

    while (true)
    {
        cap >> imgOriginal;
        cv::flip(imgOriginal, imgOriginal, 1);              // flip horizontally
        cvtColor(imgOriginal, imgGray, cv::COLOR_BGR2GRAY); // convert to gray

        vector<Rect> faces;
        face_cascade.detectMultiScale(imgGray, faces, 1.3, 5);

        Point center;
        for (auto f : faces)
        {
            center = Point(f.x + f.width / 2, f.y + f.height / 2);

            // when controlling is 0, default
            if (faceMode == 0)
            {
                cv::rectangle(imgOriginal, f, Scalar(0, 0, 251), 3);
                pts.insert(pts.begin(), center);
                if (pts.size() > 10)
                {
                    pts.pop_back();
                }
                for (size_t i = 0; i < pts.size() - 1; ++i)
                {
                    line(imgOriginal, pts[i], pts[i + 1], Scalar(0, 0, 255), 3);
                }
            }

            if (faceMode == 1)
            {
                fp.drawBird(imgOriginal, center); //todo
                if (fp.isOver(imgOriginal, center))
                {
                    waitKey(1000);
                } //todo
            }
        }

        if (faceMode == 1)
        {
            fp.setTubes(imgOriginal); //todo
        }

        imshow("Original", imgOriginal); //显示最终图像
        if (waitKey(25) == 27)
            break;
    }
    cv::destroyWindow("Original");
    return 0;
}

void dispInfo()
{
    cout << "Face ricognition start..." << endl;
}
#endif //OPENCV_TRAJECT_FACE_H
