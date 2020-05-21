#ifndef CONNECTOR_H
#define CONNECTOR_H


#include <QObject>
#include <QThread>
#include <QQuickWindow>
#include "threadcontroller.h"
class Connector : public QObject
{
    Q_OBJECT
public:
    explicit Connector(int argc, char *argv[] ,QObject *parent = nullptr);
private:
    QSharedPointer<ThreadController> threadController;
    void windowInit(int argc, char *argv[]);
    QQuickWindow *window;
private slots:
    void startInformation(QString startURL, int coreNumber, int linksNumber, QString subStr);
    void pauseHandler(bool isPaused);
    void stop();
    void finished();
signals:
    void sendPause(bool isPaused);
    void sendStop();
    void uiStop();
};

#endif // CONNECTOR_H
