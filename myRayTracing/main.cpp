//
// Created by ac on 15-4-25.
//
#include <iostream>
#include <QString>
#include <QtWidgets/qapplication.h>
#include "RayTracer.h"
#include "MainWidget.h"


int main (int argc, char *argv[]) {

    int width = SCREEN_WIDTH, height = SCREEN_HEIGHT;
    if (argc > 3) {
        bool ok;
        width = QString(argv[2]).toInt(&ok, 10);
        if (!ok) {
            width = SCREEN_WIDTH;
        }

        height = QString(argv[3]).toInt(&ok, 10);
        if (!ok) {
            height = SCREEN_HEIGHT;
        }
    }

    acRayTracing::RayTracer rayTracer;
    rayTracer.setScreen(width, height);

    //init QT
    QApplication app(argc, argv);
    MainWidget view(width, height);

    //connect SIGNAL and SLOT
    QObject::connect(&rayTracer, SIGNAL(updateScreenSignal(int, int, acRayTracing::Color)),
                     &view, SLOT(changePixel(int, int, acRayTracing::Color)));

    view.show();
    app.exec();
}
