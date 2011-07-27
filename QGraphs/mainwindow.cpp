#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qgraph.h"
#include <QPixmap>

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
    QGraph graph;

    for(int i = 1; i <= 9; ++i) {
        graph.setDataPoint("line1",(double)(i),(double)i);
        //graph.setDataPoint("line2",(double)(i),(double)i*i);
    }

    graph.setMinX(2);

    QImage img = graph.getGraph(640,480);

    ui->label->setPixmap(QPixmap::fromImage(img));
}
