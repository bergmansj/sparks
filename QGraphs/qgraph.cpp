#include "qgraph.h"

#include <cmath>
#include <limits>

#include <QRect>
#include <QColor>

QGraph::QGraph(QObject *parent) :
    QObject(parent),
    graph_border(20),
    gridlines_h(10),
    gridlines_v(10),
    device(NULL)
{
    // Set min and max values
    min_x = std::numeric_limits<double>::max( );
    min_y = std::numeric_limits<double>::max( );
    max_x = std::numeric_limits<double>::min( );
    max_y = std::numeric_limits<double>::min( );

    // Set color table (http://www.w3.org/TR/SVG/types.html#ColorKeywords)
    // Unused colours
    //colortable.append("white");       // background
    //colortable.append("black");       // text and outer lines
    //colortable.append("lightgrey");   // gridlines
    //colortable.append("grey");
    //colortable.append("darkgrey");
    // Primary colours
    colortable.append("red");
    colortable.append("green");
    colortable.append("blue");
    colortable.append("yellow");
    // Dark colours
    colortable.append("darkblue");
    colortable.append("darkcyan");
    colortable.append("darkgoldenrod");
    colortable.append("darkgreen");
    colortable.append("darkkhaki");
    colortable.append("darkmagenta");
    colortable.append("darkolivegreen");
    colortable.append("darkorange");
    colortable.append("darkorchid");
    colortable.append("darkred");
    colortable.append("darksalmon");
    colortable.append("darkseagreen");
    colortable.append("darkslateblue");
    colortable.append("darkslategrey");
    colortable.append("darkturquoise");
    colortable.append("darkviolet");
    // medium colours
    colortable.append("mediumaquamarine");
    colortable.append("mediumblue");
    colortable.append("mediumorchid");
    colortable.append("mediumpurple");
    colortable.append("mediumseagreen");
    colortable.append("mediumslateblue");
    colortable.append("mediumspringgreen");
    colortable.append("mediumturquoise");
    colortable.append("mediumvioletred");
    // light colours
    colortable.append("lightblue");
    colortable.append("lightcoral");
    colortable.append("lightcyan");
    colortable.append("lightgoldenrodyellow");
    colortable.append("lightgreen");
    colortable.append("lightpink");
    colortable.append("lightsalmon");
    colortable.append("lightseagreen");
    colortable.append("lightskyblue");
    colortable.append("lightslategrey");
    colortable.append("lightsteelblue");
    colortable.append("lightyellow");
    // other colours
    colortable.append("aliceblue");
    colortable.append("antiquewhite");
    colortable.append("aqua");
    colortable.append("aquamarine");
    colortable.append("azure");
    colortable.append("beige");
    colortable.append("bisque");
    colortable.append("blanchedalmond");
    colortable.append("blueviolet");
    colortable.append("brown");
    colortable.append("burlywood");
    colortable.append("cadetblue");
    colortable.append("chartreuse");
    colortable.append("chocolate");
    colortable.append("coral");
    colortable.append("cornflowerblue");
    colortable.append("cornsilk");
    colortable.append("crimson");
    colortable.append("cyan");
    colortable.append("deeppink");
    colortable.append("deepskyblue");
    colortable.append("dimgrey");
    colortable.append("dodgerblue");
    colortable.append("firebrick");
    colortable.append("floralwhite");
    colortable.append("forestgreen");
    colortable.append("fuchsia");
    colortable.append("gainsboro");
    colortable.append("ghostwhite");
    colortable.append("gold");
    colortable.append("goldenrod");
    colortable.append("greenyellow");
    colortable.append("honeydew");
    colortable.append("hotpink");
    colortable.append("indianred");
    colortable.append("indigo");
    colortable.append("ivory");
    colortable.append("khaki");
    colortable.append("lavender");
    colortable.append("lavenderblush");
    colortable.append("lawngreen");
    colortable.append("lemonchiffon");
    colortable.append("lime");
    colortable.append("limegreen");
    colortable.append("linen");
    colortable.append("magenta");
    colortable.append("maroon");
    colortable.append("midnightblue");
    colortable.append("mintcream");
    colortable.append("mistyrose");
    colortable.append("moccasin");
    colortable.append("navajowhite");
    colortable.append("navy");
    colortable.append("oldlace");
    colortable.append("olive");
    colortable.append("olivedrab");
    colortable.append("orange");
    colortable.append("orangered");
    colortable.append("orchid");
    colortable.append("palegoldenrod");
    colortable.append("palegreen");
    colortable.append("paleturquoise");
    colortable.append("palevioletred");
    colortable.append("papayawhip");
    colortable.append("peachpuff");
    colortable.append("peru");
    colortable.append("pink");
    colortable.append("plum");
    colortable.append("powderblue");
    colortable.append("purple");
    colortable.append("rosybrown");
    colortable.append("royalblue");
    colortable.append("saddlebrown");
    colortable.append("salmon");
    colortable.append("sandybrown");
    colortable.append("seagreen");
    colortable.append("seashell");
    colortable.append("sienna");
    colortable.append("silver");
    colortable.append("skyblue");
    colortable.append("slateblue");
    colortable.append("slategrey");
    colortable.append("snow");
    colortable.append("springgreen");
    colortable.append("steelblue");
    colortable.append("tan");
    colortable.append("teal");
    colortable.append("thistle");
    colortable.append("tomato");
    colortable.append("turquoise");
    colortable.append("violet");
    colortable.append("wheat");
    colortable.append("whitesmoke");
    colortable.append("yellowgreen");

}

QGraph::~QGraph()
{
    QMap<QString, DataList*>::const_iterator i = labels.constBegin();
     while (i != labels.constEnd()) {
         QString key = i.key();
         DataList *dl = labels.value(key);
         delete dl;
         ++i;
     }
}

void QGraph::setMinX(const double& x)
{
    min_x = x;
}

void QGraph::setDataPoint(const QString& key, const double& x, const double& y)
{
    // Check if a line with this key exists and get id
    // Or create line with this key and get id
    DataList *dl = NULL;
    if (!labels.contains(key)) {
        addList(key);
    }
    dl = labels.value(key);
    // Add datapoint
    QPoint dp;
    dp.setX(x);
    dp.setY(y);
    dl->dplist.append(dp);
    // Adjust min and max values
    min_x = std::min(min_x,x);
    min_y = std::min(min_y,y);
    max_x = std::max(max_x,x);
    max_y = std::max(max_y,y);

    updateGraph();
}

void QGraph::addList(const QString& key)
{
    // Add key to datalist
    DataList *dl = NULL;
    dl = new DataList();
    labels[key] = dl;
    // Add key to colour list
    colors[key] = colortable.first();
    colortable.removeFirst();
}

QImage QGraph::getGraph(const int& width, const int& height)
{
    QSize size;
    //size.setWidth((int)(max_x-min_x));
    //size.setHeight((int)(max_y-min_y));
    size.setWidth(width);
    size.setHeight(height);
    QImage img(size,QImage::Format_RGB32);
    img.fill(0);
    device = &img;

    updateGraph();

    device = NULL;
    return img;
}

void QGraph::connectGraph(QPaintDevice * ext_device)
{
    device = ext_device;
}

void QGraph::updateGraph()
{
    if(device == NULL)
        return;

    QPainter paint(device);
    QSize p_size(device->width(),device->height());
    QRect rect(0,0,device->width(),device->height());
    paint.fillRect(rect,QColor("white"));

    if((p_size.width() < graph_border*2) || (p_size.height() < graph_border*2)) {
        paint.end();
        return;
    }

    // Define scale values
    double div_x = max_x - min_x;
    double div_y = max_y - min_y;
    double scale_x = (p_size.width() - 2 * graph_border ) / div_x;
    double scale_y = (p_size.height() - 2 * graph_border ) / div_y;

    // Draw the grid and values
    double div_per_gridline_h = (p_size.height() - 2 * graph_border) / gridlines_h;
    for(int i = 0; i < gridlines_h; ++i) {
        QString color = "lightgrey";
        if(i == 0)
            color = "black";
        paint.setPen(QColor(color));
        QPoint pt1, pt2;
        pt1.setX(graph_border);
        pt2.setX(p_size.width()-graph_border);
        int cy;
        cy = graph_border + (i * div_per_gridline_h);
        pt1.setY(cy);
        pt2.setY(cy);
        paint.drawLine(hflip(pt1,p_size),hflip(pt2,p_size));
        // Draw text
        QString text;
        text.setNum((div_x / gridlines_h) * i + min_x);
        paint.setPen(QColor("black"));
        QPoint tp(1,cy+(graph_border/2));
        tp = hflip(tp, p_size);
        QRect r(tp.x(), tp.y(), graph_border-2,graph_border-2);
        paint.drawText(r,Qt::AlignLeft,text);
    }
    double div_per_gridline_v = (p_size.width() - 2 * graph_border) / gridlines_v;
    for(int i = 0; i <= gridlines_v; ++i) {
        QString color = "lightgrey";
        if(i == 0)
            color = "black";
        paint.setPen(QColor(color));
        QPoint pt1, pt2;
        pt1.setY(graph_border);
        pt2.setY(p_size.height()-graph_border);
        int cx;
        cx = graph_border + (i * div_per_gridline_v);
        pt1.setX(cx);
        pt2.setX(cx);
        paint.drawLine(hflip(pt1,p_size),hflip(pt2,p_size));
        // Draw text
        QString text;
        text.setNum((div_y / gridlines_v) * i + min_y);
        paint.setPen(QColor("black"));
        QPoint tp(cx-(graph_border/2),graph_border-1);
        tp = hflip(tp, p_size);
        QRect r(tp.x(), tp.y(), graph_border-2,graph_border-2);
        paint.drawText(r,Qt::AlignLeft,text);
    }

    // Do some magic to draw the lines
    // Do per line
    QMap<QString, DataList*>::const_iterator i = labels.constBegin();
    while (i != labels.constEnd()) {
        QString key = i.key();
        DataList* dl = i.value();
        paint.setPen(QColor(colors.value(key)));

        // Do per datapoint
        int index = 1;
        while(index < dl->dplist.size()) {
            QPoint p1 = dl->dplist.at(index-1);
            if(p1.x() < min_x) {
                dl->dplist.removeAt(index-1);
                continue;
            }
            QPoint dp1;
            dp1.setX( (int)round(p1.x() * scale_x) + graph_border);
            dp1.setY( (int)round(p1.y() * scale_y) + graph_border);
            QPoint p2 = dl->dplist.at(index);
            QPoint dp2;
            dp2.setX( (int)round(p2.x() * scale_x) + graph_border);
            dp2.setY( (int)round(p2.y() * scale_y) + graph_border);
            paint.drawLine(hflip(dp1,p_size),hflip(dp2,p_size));

            ++index;
        }

        ++i;
    }

    paint.end();
}

double QGraph::round(const double& x)
{
    if(x>=0.5){return ceil(x);}else{return floor(x);}
}

QPoint QGraph::hflip(const QPoint& point, const QSize& size)
{
    int x = point.x();
    int y = std::abs(point.y() - size.height());
    QPoint new_point(x,y);
    return new_point;
}
