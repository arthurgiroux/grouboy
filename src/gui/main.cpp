#include "ApplicationData.hpp"
#include "ImageItem.hpp"
#include "TileDebugger.hpp"
#include <QCommandLineParser>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);
    app.setOrganizationDomain("giroux");
    app.setApplicationName("Grouboy");

    QCommandLineParser parser;
    QCommandLineOption romFileOption(QStringList() << "rom-file",
                                     QCoreApplication::translate("main", "The file of the rom to load."),
                                     QCoreApplication::translate("main", "file"));
    parser.addOption(romFileOption);
    parser.process(app);

    QQmlApplicationEngine engine;

    ApplicationData appData;
    appData.setRomFile(parser.value(romFileOption));
    engine.rootContext()->setContextProperty("applicationData", &appData);

    engine.load(QUrl("qrc:/GBQml/qml/main.qml"));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}