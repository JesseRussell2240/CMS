#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>
#include <QSettings>
#include "backend.h"       // C++ 'backend' class to receive and give data to front end GUI


int main(int argc, char *argv[])
{
    //QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);    // Deprecated in Qt v6
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Register QML types with C++ classes to connect them together
    /********************************************************************/
    qmlRegisterType<BackEnd>("io.qt.backend", 1, 0, "BackEnd");                 // 'Audio'is name used inside QML (new QML component in main.qml) with access to the C++ Audio class methods/properties

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
