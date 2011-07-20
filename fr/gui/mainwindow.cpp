#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
}
