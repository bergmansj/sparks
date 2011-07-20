#include "imageconversion.h"

QImage ImageConversion::mat2QImage(const cv::Mat &mat)
{
    IplImage iplImage = (IplImage)mat;

    return IplImage2QImage(&iplImage);
}

cv::Mat ImageConversion::QImage2Mat(const QImage& qImg)
{

    IplImage* iplImage = cvCreateImageHeader( cvSize(qImg.width(), qImg.height()), IPL_DEPTH_8U, 4);
    iplImage->imageData = reinterpret_cast<char*>(const_cast<uchar*>(qImg.bits()));
    cv::Mat mat(iplImage,true);

    return mat;
}

QImage ImageConversion::IplImage2QImage(IplImage* iplImg)
{
    unsigned int h = iplImg->height;
    unsigned int w = iplImg->width;
    unsigned int channels = iplImg->nChannels;
    QImage qImg(w, h, QImage::Format_ARGB32);
    char *data = iplImg->imageData;

    for (unsigned int y = 0; y < h; y++, data += iplImg->widthStep) {
        for (unsigned int x = 0; x < w; x++) {

            unsigned int index = x*channels;
            char r, g, b, a = 0;
            if (channels == 1) {
                r = g = b = data[index];
            }
            else if (channels == 3 || channels == 4) {
                r = data[index + 2];
                g = data[index + 1];
                b = data[index];
            }

            if (channels == 4) {
                a = data[index + 3];
                qImg.setPixel(x, y, qRgba(r, g, b, a));
            }
            else {
                qImg.setPixel(x, y, qRgb(r, g, b));
            }
        }
    }
    return qImg;

}
