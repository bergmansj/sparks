#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QImage>
#include <QString>

#include <QMainWindow>
#include <QFileDialog>

#include "recognition/facerecognition.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void newStatus(const QString&);
    void recognizeNewFaces(const QImage&);
    void labeledImage(const QImage&, const QString&);

private slots:
    void labelFace(const QImage&);
    void recognizeResult(const QImage&, const QString&);
    void statusUpdate(const QString&);

    void on_buttonSelectTrainImages_clicked();
    void on_buttonTrain_clicked();
    void on_buttonSelectTestImage_clicked();
    void on_buttonSetLabel_clicked();
    void on_buttonViewNextResult_clicked();

private:
    Ui::MainWindow *ui;

    FaceRecognition fr;

    QList<QImage> unlabeled_images;
    QList<QImage> recognized_images;
};

#endif // MAINWINDOW_H
