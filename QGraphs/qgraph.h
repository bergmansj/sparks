#ifndef QGRAPH_H
#define QGRAPH_H

#include <QObject>

#include <QMap>
#include <QSize>
#include <QList>
#include <QImage>
#include <QPoint>
#include <QString>
#include <QPainter>

class QGraph : public QObject
{
    Q_OBJECT
public:
    explicit QGraph(QObject *parent = 0);
    virtual ~QGraph();

    void setDataPoint(const QString&, const double&, const double&);
    QImage getGraph(const int&, const int&);
    void connectGraph(QPaintDevice * device);
    void updateGraph();

    void setMinX(const double&);

signals:

public slots:

private:
    struct DataList {
        QList<QPoint> dplist;
    };

    QList<QString> colortable;

    QMap<QString, QString> colors;
    QMap<QString, DataList*> labels;
    QPaintDevice * device;
    double min_x;
    double min_y;
    double max_x;
    double max_y;

    int graph_border;
    int gridlines_h;
    int gridlines_v;

    void addList(const QString&);
    double round(const double&);
    QPoint hflip(const QPoint&, const QSize&);
};

#endif // QGRAPH_H
