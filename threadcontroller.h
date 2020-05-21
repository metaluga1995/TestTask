#ifndef THREADCONTROLLER_H
#define THREADCONTROLLER_H

#include <QObject>
#include "httpnetworkworker.h"
#include <datacontroller.h>

class ThreadController : public QObject
{
    Q_OBJECT
private:
    const int c_NumberOfCores;
    const int c_maxSearchCycle;
    const QString c_SubStr;

    bool isPaused;
    bool isStoped;
    int searchingCount;

    QVector<QSharedPointer<QThread> > threadsList;
    QVector<QSharedPointer<HttpNetworkWorker> > httpWorker;
    QQueue<int> isBusy;

    QSharedPointer<DataController> dataController;

    void shutDown();

public:
    explicit ThreadController(const int* coresNumber, const QString* substr, const int* searchCycle, QObject *parent = nullptr);
    void createThreads();

signals:
    void updateOutput(QVariant outpuString);
    void updateLog(QVariant logString);
    void updateProgress(QVariant progress);
    void finished();

private slots:
    void threadFinished(int threadNumber);
    void addSubstrNumber(int number, QUrl link);

public slots:
    void pauseHandler(bool pause);
    void dataHire();
    void addHtml(QUrl link, int level);
    void writeLog(QString log);
    void stop();

};

#endif // THREADCONTROLLER_H
