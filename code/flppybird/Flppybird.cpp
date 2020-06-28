//
// Flppybird.cpp
// Created by Archibald Chain on 2020/6/16.
//
// #include <w32api/mshtmlc.h>
#include "Flppybird.h"
#include <experimental/random>

const string pic_path = "F:/opencv_ws/C++Project/code/flppybird/pictures/" ;
const string bird1 = pic_path + "bird1.png";
const string bird2 = pic_path + "bird2.png";
const string bird3 = pic_path + "bird3.png";
const string tube1 = pic_path + "tub1.png";
const string tube2 = pic_path + "tub2.png";

void imgCopy(cv::Mat &dsn_img, cv::Mat &copy_img, cv::Point &leftUp);

Flppybird::Flppybird(cv::Mat &frame)
{
    f_height = frame.size[0];
    f_length = frame.size[1];

    int n = 6;
    upperBound = int(f_height / n);
    lowerBound = int(f_height / n * (n - 1));

    cv::Mat b1 = cv::imread(bird1);
    cv::resize(b1, b1, cv::Size(2 * b1.rows, 2 * b1.cols));
    cv::Mat b2 = cv::imread(bird2);
    cv::resize(b2, b2, cv::Size(2 * b2.rows, 2 * b2.cols));
    cv::Mat b3 = cv::imread(bird3);
    cv::resize(b3, b3, cv::Size(2 * b3.rows, 2 * b3.cols));
    birds = {b1, b2, b3};

    bird_width = b1.rows;
    bird_height = b1.cols;

    tube_width = f_height / 15;
    tube_height = f_height / 2;

    gate_size = f_height / 4;

    cv::Size newSize(tube_width, tube_height);
    cv::resize(cv::imread(tube1), tub1, newSize);
    cv::resize(cv::imread(tube2), tub2, newSize);

    count = 0;
}

void Flppybird::setTubes(cv::Mat &frame)
{
    count++;
    if (Tubes.empty())
    {
        Tubes.push_back(generateTube());
        return;
    }

    for (auto &t : Tubes)
    {
        t.moveLeft();
        drawTube(t, frame);
    }

    // when the first one from right entering into the frame
    if (f_length - (Tubes.end() - 1)->x2 > 4 * tube_width)
    {
        Tubes.push_back(generateTube());
    }

    // when the first onw from the left get out of frame
    if (Tubes[0].x2 <= 0)
    {
        auto k = Tubes.begin();
        Tubes.erase(k);
    }
}

void Flppybird::drawBird(cv::Mat frame, const cv::Point &center)
{
    auto bird = birds[count % 3];
    auto leftUp = cv::Point(center.x - bird.cols / 2, center.y - bird.rows / 2);
    imgCopy(frame, bird, leftUp);
}

bool Flppybird::isOver(Mat& frame,const cv::Point &center){
    for (auto &t : Tubes){
        if (t.isCrashed(&center, bird_width, bird_height)){
            cout << "Crashed\n Game Over!\n";
            cv::putText(frame, "Game Over!", cv::Point(20, 240), cv::FONT_HERSHEY_PLAIN, 6, Scalar(0, 0, 255), 8);
            imshow("Original", frame);
            return true;
        }
    }
    return false;
}

Tube Flppybird::generateTube()
{
    int y = std::experimental::randint(upperBound, lowerBound - gate_size);
    int x = f_length;
    return {x, y, x + tube_width, y + gate_size};
}

void Flppybird::drawTube(Tube &t, cv::Mat &frame)
{
    auto rec1 = t.getUpTube();
    auto rec2 = t.getDownTube(f_height);

    cv::rectangle(frame, rec1, cv::Scalar(0, 255, 0), -1);
    cv::rectangle(frame, rec2, cv::Scalar(0, 255, 0), -1);

    // cv::Point topLeft1 = cv::Point(rec1.x, rec1.y - tube_height);
    // imgCopy(frame, tub1, topLeft1);
    // auto topLeft2 = rec2.tl();
    // imgCopy(frame, tub2, topLeft2);
}

inline Tube::Tube(int x1, int y1, int x2, int y2)
{
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
    leftUp = cv::Point(x1, y1);
    rightDown = cv::Point(x2, y2);
}

inline cv::Rect Tube::getUpTube() const
{
    cv::Point lu = cv::Point(x1, 0);
    cv::Point rd = cv::Point(x2, y1);
    return cv::Rect(lu, rd);
}

inline cv::Rect Tube::getDownTube(long m) const
{
    cv::Point lu = cv::Point(x1, y2);
    cv::Point rd = cv::Point(x2, m);
    return cv::Rect(lu, rd);
}

inline void Tube::moveLeft()
{
    int step_len = 5;
    x1 -= step_len;
    x2 -= step_len;
}

bool Tube::isCrashed(const cv::Point *center, int width, int height) const{
    if ((center->y - height / 2) < y1 || (center->y + height / 2) > y2){
        if (((center->x + width / 2) < x2 && (center->x + width / 2) > x1) ||
            ((center->x - width / 2) < x2 && (center->x - width / 2) > x1)){
            return true;
        }
    }
    return false;
}

/**
 * using for copy one small image into a big one in the specific location
 * @param dsn_img destiny image
 * @param copy_img small image
 * @param leftUp the left up point for the postion
 */
void imgCopy(cv::Mat &dsn_img, cv::Mat &copy_img, cv::Point &leftUp)
{
    for (int i = 0; i < copy_img.rows; ++i)
    {
        for (int j = 0; j < copy_img.cols; ++j)
        {
            // using outOfRange to make sure that the copy_img will not be outOfRange
            bool outOfRange =
                    i + leftUp.y < 0 || i + leftUp.y >= dsn_img.rows ||
                    j + leftUp.x < 0 || j + leftUp.y >= dsn_img.cols;

            // using sum to make sure the element at (i,j) is not empty
            long sum = 0;
            auto *array = copy_img.ptr<uchar>(i, j);
            for (int i = 0; i < 3; i++)
            {
                try
                {
                    sum += array[i];
                }
                catch (exception e)
                {
                    cout << e.what();
                }
            }
            if (outOfRange || sum == 0)
            {
                continue;
            }
            //change
            auto *dsn = dsn_img.ptr<uchar>(i + leftUp.y, j + leftUp.x);

            // copy_img and dsn_img must have the same color field
            for (int k = 0; k < 3; k++)
            {
                try
                {
                    dsn[k] = array[k];
                }
                catch (exception e)
                {
                    cout << e.what();
                }
            }
        }
    }
}

