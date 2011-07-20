#include "facedetection.h"
#include "../imageconversion.h"

#include <QRect>

FaceDetection::FaceDetection(const QString &cascadeName)
{
    setHaarCascadeXML(cascadeName);
}

void FaceDetection::setHaarCascadeXML(const QString &cascadeName)
{
    cascade = (CvHaarClassifierCascade*)cvLoad( cascadeName.toLocal8Bit().constData(), 0, 0, 0 );
}

QList<QRect> FaceDetection::detectFace(const QImage &image)
{
    QList<QRect> faces_list;

    cv::Mat frame = ImageConversion::QImage2Mat(image); // Fill frame with image from QImage

    IplImage iplFrame = (IplImage)(frame);

    if( cascade )
    {
        // Allocate the memory storage
        CvMemStorage * storage = cvCreateMemStorage(0);

        // There can be more than one face in an image. So create a growable sequence of faces.
        // Detect the objects and store them in the sequence
        CvSeq* faces = cvHaarDetectObjects( &iplFrame, cascade, storage,              // http://opencv.itseez.com/modules/objdetect/doc/cascade_classification.html?highlight=cvhaardetectobjects#cvHaarDetectObjects
                                            1.1, 2, CV_HAAR_DO_CANNY_PRUNING,
                                            cvSize(40,40) );

        // Loop the number of faces found.
        int i;
        for( i = 0; i < (faces ? faces->total : 0); i++ )
        {
            // Create a new rectangle for drawing the face
            CvRect* r = (CvRect*)cvGetSeqElem( faces, i );

            // Draw the rectangle in the input image
            QRect r_face(r->x,r->y,r->width,r->height);

            faces_list.append(r_face);
        }

        // Clear the storage
        cvClearMemStorage( storage );
    }

    return faces_list;
}
