#include <iostream>
#include "surface.h"
#include "raytracer.h"
#include "scene.h"
#include "MainWidget.h"
#include <QApplication>

#define WIDTH 400
#define HEIGHT 300


int main(int argc, char *argv[]) {

    qint32 width = WIDTH, height = HEIGHT;
    if (argc > 3) {
        bool ok;
        width = QString(argv[2]).toInt(&ok, 10);
        if (!ok) {
            width = WIDTH;
        }

        height = QString(argv[3]).toInt(&ok, 10);
        if (!ok) {
            height = HEIGHT;
        }
    }


    Raytracer::Surface *surface = new Raytracer::Surface(width, height);
    surface->Clear(0);

    // prepare renderer
    Raytracer::Engine *tracer = new Raytracer::Engine();
    tracer->GetScene()->InitScene();
    tracer->SetTarget(surface->GetBuffer(), width, height);

    //init QT
    QApplication app(argc, argv);
    MainWidget view(width, height);

    //connect SIGNAL and SLOT
    QObject::connect(tracer, SIGNAL(updateScreenSignal(Pixel * )),
            &view, SLOT(updateOneRowSlot(Pixel * )));


    view.show();

    //renderer
    tracer->InitRender();
    tracer->Render();


    return app.exec();
}