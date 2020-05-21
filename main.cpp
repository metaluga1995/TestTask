#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "connector.h"

int main(int argc, char *argv[])
{
    QScopedPointer<Connector> connector = QScopedPointer<Connector>(new Connector(argc, argv));

    return 0;//app.exec();
}
