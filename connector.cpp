#include <QObject>
#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QDebug>
#include "connector.h"

Connector::Connector(int argc, char *argv[] ,QObject *parent) : QObject(parent)
{
    windowInit(argc, argv);

}

void Connector::windowInit(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QObject *topLevel = engine.rootObjects().value(0);
    window = qobject_cast<QQuickWindow *>(topLevel);

    connect(window, SIGNAL(sendParamethers(QString, int, int , QString)), this, SLOT(startInformation(QString, int, int, QString)));
    connect(window, SIGNAL(sendPauseCommand(bool)), this, SLOT(pauseHandler(bool)));
    connect(window, SIGNAL(sendStopCommand()), this, SLOT(stop()));
    connect(this, SIGNAL(uiStop()), window, SLOT(stopButton()));
    app.exec();
}

void Connector::startInformation(QString startURL, int coreNumber, int linksNumber, QString subStr)
{
   QString test = startURL;

   threadController = QSharedPointer<ThreadController>(new ThreadController(&coreNumber, &subStr, &linksNumber));
   connect(threadController.get(), SIGNAL(updateLog(QVariant)), window, SLOT(updateLog(QVariant)));
   connect(threadController.get(), SIGNAL(updateOutput(QVariant)), window, SLOT(updateOutput(QVariant)));
   connect(threadController.get(), SIGNAL(updateProgress(QVariant)), window, SLOT(updateProgress(QVariant)));
   connect(this, SIGNAL(sendStop()), threadController.get() ,SLOT(stop()));
   connect(threadController.get(), SIGNAL(finished()), this, SLOT(finished()));

   connect(this, SIGNAL(sendPause(bool)), threadController.get(), SLOT(pauseHandler(bool)));

   threadController->createThreads();
   threadController->addHtml(startURL, 0);
}

void Connector::pauseHandler(bool isPaused)
{
    emit sendPause(isPaused);
}

void Connector::stop()
{
    emit sendStop();
}

void Connector::finished()
{
    threadController.reset();
    emit uiStop();
}
