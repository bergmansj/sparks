#ifndef IMAGECONVERSION_H
#define IMAGECONVERSION_H

#include <QImage>

#include <opencv/cv.h>
#include <opencv2/opencv.hpp>

// See: https://umanga.wordpress.com/2010/04/19/how-to-covert-qt-qimage-into-opencv-iplimage-and-wise-versa/

class ImageConversion
{
public:
    static QImage mat2QImage(const cv::Mat& mat);
    static cv::Mat QImage2Mat(const QImage& qImg);
    static QImage IplImage2QImage(IplImage *iplImg);
};

#endif // IMAGECONVERSION_H
