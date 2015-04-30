//
// Created by ac on 15-4-26.
//

#include "MainWidget.h"


void MainWidget::changePixel(int width, int height, acRayTracing::Color color) {

    QPainter painter(image);
    color *= 256;
    painter.setPen(QColor((int) color.x, (int) color.y, (int) color.z));
    painter.drawPoint(width, height);

    item->setPixmap(QPixmap::fromImage(*image));
    scene->removeItem(item);
    scene->addItem(item);
}

MainWidget::MainWidget(int width, int height) {
    this->width = width;
    this->height = height;

    image = new QImage(this->width, this->height, QImage::Format_ARGB32);
    QPainter painter(image);
    for (int i = 0; i < this->height; ++i) {
        for (int j = 0; j < this->width; ++j) {
            painter.setPen(QColor(i % 255, j % 255, i * j % 255));
            painter.drawPoint(j, i);
        }
    }
    item = new QGraphicsPixmapItem(QPixmap::fromImage(*image));
    scene = new QGraphicsScene();
    view = new QGraphicsView(scene, this);
    scene->addItem(item);
}
