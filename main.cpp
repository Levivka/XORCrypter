#include "tablemodel.h"
#include "xorqtquick.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    XorQtQuick backend;
    engine.rootContext()->setContextProperty("backend", &backend);

    TableModel tableModel;
    engine.rootContext()->setContextProperty("tableModel", &tableModel);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("XorQtQuick", "Main");

    return app.exec();
}
