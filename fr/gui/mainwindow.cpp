#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    fr("/usr/share/opencv/haarcascades/haarcascade_frontalface_default.xml")
{
    ui->setupUi(this);

    connect(this, SIGNAL(newStatus(QString)), this, SLOT(statusUpdate(QString)));
    connect(&fr, SIGNAL(newStatus(QString)), this, SLOT(statusUpdate(QString)));

    connect(&fr, SIGNAL(unlabeledFace(QImage)), this, SLOT(labelFace(QImage)));
    connect(&fr, SIGNAL(recognizeFaceResult(QImage,QString)), this, SLOT(recognizeResult(QImage,QString)));

    connect(this, SIGNAL(recognizeNewFaces(QImage)), &fr, SLOT(recognizeFace(QImage)));
    connect(this, SIGNAL(labeledImage(QImage,QString)), &fr, SLOT(labeledFace(QImage,QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    const QStringList files =
        QFileDialog::getOpenFileNames(
                this,
                "Select training images",
                QString::null,
                QString::null);

    // Train with selected files
    for(int i = 0; i < files.size(); ++i) {
        QImage new_image(files.at(i));
        if(!new_image.isNull())
            fr.addTrainingImage(new_image);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QString path;

    path = QFileDialog::getOpenFileName(
        this,
        "Select test image",
        QString::null,
        QString::null);

    // Test with selected file
    QImage new_image(path);
    if(!new_image.isNull())
        emit recognizeNewFaces(new_image);
}

void MainWindow::labelFace(const QImage& face)
{
    unlabeled_images.append(face);
}


void MainWindow::recognizeResult(const QImage& image, const QString& label)
{
    // show image, label
    ui->TextLabel_recognized->setPixmap(QPixmap::fromImage(image));
    ui->TextLabel_label->setText(label);
}

void MainWindow::on_Button_set_label_clicked()
{
    if(ui->TextLabel_unlabeled->text() == "empty") {
        if(unlabeled_images.size() > 0) {
            ui->TextLabel_unlabeled->setText("labeling");
            ui->TextLabel_unlabeled->setPixmap(QPixmap::fromImage(unlabeled_images.first()));
        } else {
            ui->TextLabel_unlabeled->setText("empty");
        }
        return;
    }

    QString label(ui->lineEdit_label->text());

    emit labeledImage(unlabeled_images.first(), label);

    unlabeled_images.removeFirst();
    if(unlabeled_images.size() > 0) {
        ui->TextLabel_unlabeled->setText("labeling");
        ui->TextLabel_unlabeled->setPixmap(QPixmap::fromImage(unlabeled_images.first()));
    } else {
        ui->TextLabel_unlabeled->setText("empty");
        fr.trainRecognizer();
    }
}


void MainWindow::statusUpdate(const QString& status)
{
    ui->label_status->setText(status);
}
