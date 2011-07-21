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
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_Button_set_label_clicked();

    void labelFace(const QImage&);
    void recognizeResult(const QImage&, const QString&);
    void statusUpdate(const QString&);

private:
    Ui::MainWindow *ui;

    FaceRecognition fr;

    QList<QImage> unlabeled_images;
};

#endif // MAINWINDOW_H
