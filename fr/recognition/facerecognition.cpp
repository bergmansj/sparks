#include "facerecognition.h"

#include <QFile>
#include <QList>
#include <QRect>
#include <QColor>

#include "../imageconversion.h"

const QString FaceRecognition::IMAGE_EXT = ".png";
const QString FaceRecognition::VERSION = "0.1";
const QString FaceRecognition::APPLICATION = "facedetection";

FaceRecognition::FaceRecognition(const QString &cascadeName,QObject *parent) :
    QObject(parent),
    face_detection(cascadeName),
    face_size(140,200),
    train_image_path("./traindata")
{
    recognizer = new cognition::EigenfaceRecognizer();
    if(!QDir(train_image_path).exists())
        train_image_path.mkpath(train_image_path.path());

}

FaceRecognition::~FaceRecognition()
{
    if (recognizer) delete recognizer;
}

void FaceRecognition::setFaceImageSize(QSize size)
{
    face_size = size;
}

void FaceRecognition::setTrainFolder(QString path)
{
    train_image_path.setPath(path);
}

bool FaceRecognition::isTrained()
{
    return recognizer->train();
}

void FaceRecognition::addTrainingImage(QImage image)
{
    QList<QRect> faces_rects = face_detection.detectFace(image);

    // Create an image of standard size per face
    // Make the image
    foreach(QRect face_rect, faces_rects)
    {
        QImage face = image.copy(face_rect).scaled(face_size,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

        /*
        // Convert face to grayscale image
        face = face.convertToFormat( QImage::Format_Indexed8, Qt::AutoColor );
        for(int j = 0; j < 256; ++j) {
            int grayValue = qGray(face.color(j));
            face.setColor(j, qRgb(grayValue,grayValue,grayValue));
        }
        */

        emit unlabeledFace(face);
    }
}

void FaceRecognition::labeledFace(const QImage& inputImage, const QString& label)
{
    int i = 0;
    QImage image = inputImage;
    // Set image information in the image (see .h files for the defines)
    image.setText("application", APPLICATION);
    image.setText("version", VERSION);
    image.setText("label", label);

    // Generate filename
    QString filename =  "train_" + label.toLower() + "_" + QString::number(i) + IMAGE_EXT;
    QString path = train_image_path.filePath(filename);
    while (QFile::exists(path)) {
        ++i;
        filename =  "train_" + label.toLower() + "_" + QString::number(i) + IMAGE_EXT;
        path = train_image_path.filePath(filename);
    }

    // Save the image
    image.save(path);
}


void FaceRecognition::trainRecognizer()
{
    emit newStatus("Training recognizer");
    QStringList files;
    QString filefilter = "*" + IMAGE_EXT;
    files = train_image_path.entryList(QStringList(filefilter),
                                 QDir::Files | QDir::NoSymLinks);

    // Load all the IMAGE_EXT image files and check if these are correctly labeled
    for (int i = 0; i < files.size(); ++i) {
        QString path = train_image_path.filePath(files.at(i));
        QImage test_image(path);
        test_image.text("");
        QStringList texts = test_image.textKeys();
        bool app_correct = false;
        bool ver_correct = false;
        bool label_present = true;
        if (test_image.text("application")==APPLICATION) app_correct = true;
        if (test_image.text("version")==VERSION) ver_correct = true;
        if (test_image.text("label").isEmpty()) label_present = false;

        if(app_correct && ver_correct && label_present)
        {
            recognizer->addTrainingImage(path.toStdString(), test_image.text("label").toStdString());
        }
    }

    // Train the recognizer
    if(recognizer->train())
        emit newStatus("Training oke");
    else
        emit newStatus("Training FAILED");
}


void FaceRecognition::recognizeFace(const QImage& image)
{
    emit newStatus("Recognizing face");

    // Find the faces
    QList<QRect> faces_rects = face_detection.detectFace(image);


    // Create an image of standard size per face
    // Make the image
    foreach(QRect face_rect, faces_rects)
    {
        QImage face = image.copy(face_rect).scaled(face_size,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

        /*
        // Convert face to grayscale image
        face = face.convertToFormat( QImage::Format_Indexed8, Qt::AutoColor );
        for(int j = 0; j < 256; ++j) {
            int grayValue = qGray(face.color(j));
            face.setColor(j, qRgb(grayValue,grayValue,grayValue));
        }
        */

        // Convert to openCV image
        cv::Mat cvImage = ImageConversion::QImage2Mat(face);
        cv::cvtColor(cvImage, cvImage, CV_BGR2GRAY);
        QString name = QString::fromStdString(recognizer->recognize(cvImage));
        emit newStatus("Now trying to recognize a face.");
        face.setText("application", APPLICATION);
        face.setText("version", VERSION);
        face.setText("label", name);
        emit recognizeFaceResult(face, name);
    }

    if(faces_rects.isEmpty())
    {
        emit newStatus("No faces detected in the test image.");
        return;
    }


}
