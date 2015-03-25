//
// Created by ac on 15-3-18.
//

#ifndef _CG_MAINWIDGET_H_
#define _CG_MAINWIDGET_H_


#include <QObject>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QImage>
#include "common.h"

class MainWidget : public QWidget {
Q_OBJECT
private:
    QGraphicsView *view;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *item;
    QImage *image;

    qint32 width;
    qint32 height;
public:
    MainWidget(qint32, qint32);

public slots:

    void updateOneRowSlot(Pixel *);

};


#endif //_CG_MAINWIDGET_H_
