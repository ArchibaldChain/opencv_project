#ifndef OPENCV_TRAJECT_CAP_H
#define OPENCV_TRAJECT_CAP_H
#include <iostream>
#include <opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core.hpp>
#include "./flppybird/Flppybird.h"
#include "hci_control.h"
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

int capMode = 0;

const string pic_path = "F:/opencv_ws/C++Project/code/flppybird/pictures/";
const string bird1 = pic_path + "bird1.png";
const string bird2 = pic_path + "bird2.png";
const string bird3 = pic_path + "bird3.png";
const string tube1 = pic_path + "tm1.png";
const string tube2 = pic_path + "tm2.png";

void imgCopy(cv::Mat &dsn_img, cv::Mat &copy_img, cv::Point &leftUp);
const int redLower[3] = {0, 140, 140};
const int redUpper[3] = {5, 255, 255};
int traject_cap();
void dispInf();

int traject_cap()
{
    void dispInf();
    cv::VideoCapture cap(1);
    if (!cap.isOpened())
        return -1;
    vector<Point> centers;
    Mat imgOriginal, imgHSV, imgThresholded;
    vector<vector<Point>> contours;
    vector<cv::Vec4i> hierarcy;
    int trajFlag = 0;

    cap >> imgOriginal;
    // if (capMode == 1) { //capMode is 1 //todo
    Flppybird fp = Flppybird(imgOriginal); //todo
    // }
    while (true)
    {
        cap >> imgOriginal;
        cv::flip(imgOriginal, imgOriginal, 1);
        cvtColor(imgOriginal, imgHSV, cv::COLOR_BGR2HSV); // BGR to HSV
        // select the cap accroding to threshold
        cv::inRange(imgHSV, Scalar(redLower[0], redLower[1], redLower[2]),
                    Scalar(redUpper[0], redUpper[1], redUpper[2]), imgThresholded);
        // imshow("before", imgThresholded);
        // dilate 6 times and erode 6 times
        cv::dilate(imgThresholded, imgThresholded, Mat(), Point(-1, -1), 6);
        cv::erode(imgThresholded, imgThresholded, Mat(), Point(-1, -1), 6);
        cv::findContours(imgThresholded, contours, hierarcy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
        // imshow("after",imgThresholded);
        // cv::drawContours(imgOriginal, contours, -1, Scalar(0, 255, 0), 2);

        if (contours.size() > 0)
        {
            int maxIndex = 0;
            size_t maxSize = 0;
            for (size_t i = 0; i < contours.size(); ++i)
            {
                if (contours[i].size() > maxSize)
                {
                    maxSize = contours[i].size();
                    maxIndex = i;
                }
            }
            // find the biggest circle

            auto circle = contours[maxIndex];

            cv::Point2f center;
            float radius;
            cv::minEnclosingCircle(circle, center, radius);
            if (capMode == 1)
            {                                                        //todo  capMode 1
                fp.drawBird(imgOriginal, Point(center.x, center.y)); //todo
                if (fp.isOver(imgOriginal, Point(center.x, center.y)))
                {
                    // cap >> imgOriginal;

                    waitKey(1000);
                } //todo
            }
            // get the radius and center of the circle
            if (capMode != 1)
                cv::circle(imgOriginal, center, int(radius), Scalar(0, 255, 255), 2);

            if (radius > 10 && capMode == 2)
            {
                // draw circle around the cap and the center.
                cv::circle(imgOriginal, cursorOri, cursorRad, Scalar(0, 0, 255), 4);
                centers.insert(centers.begin(), center);
                if (centers.size() > 30)
                {
                    centers.pop_back();
                }
                // put centers into the content
                for (size_t i = 0; centers.size() > 0 && (i < centers.size() - 1); ++i)
                {
                    line(imgOriginal, centers[i], centers[i + 1], Scalar(255, 0, 0), 2);
                }
                if (isOpRange(int(center.x), int(center.y)))
                {
                    if (trajFlag == 0)
                    {
                        trajFlag = 1;
                    }
                    else if (trajFlag == 2)
                    {
                        int shape;
                        traDetect(centers, shape);
                        keybdControl(shape);
                        trajFlag = 0;
                    }
                }
                else if (trajFlag == 1)
                {
                    trajFlag = 2;
                }
                // cout << trajFlag << endl;
            }
        }

        if (capMode == 1)
        {                             //todo  faceMode 1
            fp.setTubes(imgOriginal); //todo
        }
        // imshow("Thresholded Image", imgThresholded);
        imshow("Original", imgOriginal);
        if (waitKey(25) == 27)
            break;
    }
    // cv::destroyWindow("Thresholded Image");
    cv::destroyWindow("Original");
    return 0;
}
void dispInf()
{
    cout << "Cap ricognition start..." << endl;
}
#endif // OPENCV_TRAJECT_CAP_H
