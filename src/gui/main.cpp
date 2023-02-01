#include "ImageItem.hpp"
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<ImageItem>("gbemu", 1, 0, "ImageItem");
    engine.load(QUrl("qrc:/GBQml/qml/main.qml"));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}