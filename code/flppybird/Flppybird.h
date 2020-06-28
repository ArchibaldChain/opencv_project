//
// Flppybird.h
// Created by Archibald Chain on 2020/6/16.
//
#ifndef OPENCV_PROJECT_FLPPYBIRD_H
#define OPENCV_PROJECT_FLPPYBIRD_H
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <experimental/random>
/**
    *@ClassName Flppybird
    *@Description TODO
    *@Date 2020/6/16 12:19
    *@Version 1.0
    */
using namespace std;

class Tube;

void imgCopy(cv::Mat &dsn_img, cv::Mat &copy_img, cv::Point &leftUp);

class Flppybird
{
    /**
     * Vector that using to store Tubes
     */
    vector<Tube> Tubes;

    /**
     * The size of the output frame
     */
    int f_height;
    int f_length;

    /**
     * The upper and lower bound that the gate could appear
     */
    int upperBound;
    int lowerBound;

    /**
     * stores the pics of bird
     */
    vector<cv::Mat> birds;
    int bird_width, bird_height;

    /**
     * The shape of tube
     */
    int tube_width;
    int tube_height;
    int gate_size;

    /**
     * Stores the up and down tubes
     */
    cv::Mat tub1;
    cv::Mat tub2;

    /**
     * The time that call setTubes()
     */
    int count;

public:
    /**
     * Before the loop, get a instance to record the basic information of the frame
     * @param frame the output from camera
     */
    Flppybird(cv::Mat &frame);

    /**
     * call the function in each cap frame
     * @param frame
     */
    void setTubes(cv::Mat &frame);

    /**
     * call the function when there are center trajectory
     * @param frame
     * @param center
     */
    void drawBird(cv::Mat frame, const cv::Point &center);

    /**
     * call it when there are center trajectory
     * @param center
     * @param width
     * @param height
     */
    bool isOver(Mat& frame,const cv::Point &center);

private:
    Tube generateTube();
    void drawTube(Tube &t, cv::Mat &frame);
};

class Tube
{
public:
    /**
     * The Point of the gate of leftUp and rightDown
     */
    int x1, y1, x2, y2;
    cv::Point leftUp, rightDown;

    Tube(int x1, int y1, int x2, int y2);

    cv::Rect getUpTube() const;
    cv::Rect getDownTube(long m) const;
    void moveLeft();
    bool isCrashed(const cv::Point *center, int width, int height) const;
};
#endif //OPENCV_PROJECT_FLPPYBIRD_H
