//
// Created by ac on 15-4-26.
//

#ifndef AC_RAY_TRACING_MAINWIDGET_H
#define AC_RAY_TRACING_MAINWIDGET_H

#include <common.h>
#include <QWidget>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>

class MainWidget : public QWidget {
Q_OBJECT

private:
    QGraphicsView *view;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *item;
    QImage *image;

    qint32 width = SCREEN_WIDTH;
    qint32 height = SCREEN_HEIGHT;

public:
    MainWidget(int width, int height);

public slots:
    void changePixel(int width, int height, acRayTracing::Color color);

};

#endif //AC_RAY_TRACING_MAINWIDGET_H

