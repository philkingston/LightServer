#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include <QDebug>
#include "lightserver.h"
#include "backlight.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Backlight *b = new Backlight();

    QCommandLineParser parser;
    parser.setApplicationDescription("LightServer");
    parser.addHelpOption();

    QCommandLineOption dbgOption(QStringList() << "d" << "debug",
            QCoreApplication::translate("main", "Debug output [default: off]."));
    parser.addOption(dbgOption);
    QCommandLineOption portOption(QStringList() << "p" << "port",
            QCoreApplication::translate("main", "Port for lightserver [default: 1234]."),
            QCoreApplication::translate("main", "port"), QLatin1Literal("1235"));
    parser.addOption(portOption);
    parser.process(a);
    bool debug = parser.isSet(dbgOption);
    int port = parser.value(portOption).toInt();

    LightServer *server = new LightServer(port, b, debug);

    QObject::connect(server, &LightServer::closed, &a, &QCoreApplication::quit);

    return a.exec();
}
