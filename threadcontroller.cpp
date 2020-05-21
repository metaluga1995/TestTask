#include "threadcontroller.h"
#include <QObject>
#include <QThread>


ThreadController::ThreadController(const int* coresNumber, const QString* substr, const int* searchCycle, QObject *parent) :
        c_NumberOfCores(*coresNumber), c_SubStr(*substr), c_maxSearchCycle(*searchCycle) , QObject(parent)
{
    isStoped = false;
    isPaused = false;
    searchingCount = 0;
}

void ThreadController::createThreads()
{
    threadsList.resize(c_NumberOfCores);
    httpWorker.resize(c_NumberOfCores);

    for(int i = 0; i < c_NumberOfCores; ++i)
    {
        threadsList[i] = QSharedPointer<QThread>(new QThread());
        httpWorker[i] =  QSharedPointer<HttpNetworkWorker>(new HttpNetworkWorker());
        httpWorker[i]->moveToThread(threadsList[i].get());
        isBusy.push_back(i);

        connect(httpWorker[i].get(), SIGNAL(finished(int)), this, SLOT(threadFinished(int)));
        connect(httpWorker[i].get(), SIGNAL(sendLog(QString)), this, SLOT(writeLog(QString)));
        connect(httpWorker[i].get(), SIGNAL(done()), threadsList[i].get(), SLOT(quit()));
        connect(httpWorker[i].get(), SIGNAL(sendHtml(QUrl, int)), this, SLOT(addHtml(QUrl, int)));
        connect(httpWorker[i].get(), SIGNAL(sendSubstrNumber(int, QUrl)), this, SLOT(addSubstrNumber(int, QUrl)));

    }
    dataController = QSharedPointer<DataController>(new DataController());
    connect(dataController.get(), SIGNAL(linkAdded()), this, SLOT(dataHire()));

}

void ThreadController::addHtml(QUrl link, int level)
{
    dataController->addLink(&level, &link);
}

void ThreadController::addSubstrNumber(int number, QUrl link)
{
    QString log = "Substrings found on\n" + link.toString() + "\n" + QString::number(number) + " times\n\n";
    emit updateOutput(log);
}
void ThreadController::threadFinished(int threadNumber)
{
    isBusy.push_back(threadNumber);
    emit updateProgress(double(searchingCount)/c_maxSearchCycle);
    dataHire();
}

void ThreadController::pauseHandler(bool pause)
{
    isPaused = pause;
    if (!isPaused)
    {
        if (dataController->getLinksNumber() > 0)
        {
            int taskCanRun = qMin(isBusy.size(), dataController->getLinksNumber());
            for(int i = 0; i < taskCanRun; ++i)
            {
                dataHire();
            }
        }
        else
        {
            shutDown();
        }
    }
}

void ThreadController::dataHire()
{
    if (!isPaused && !isBusy.empty() && !isStoped && (searchingCount < c_maxSearchCycle))
    {
        QUrl link;
        int level;
        if (dataController->getLink(&link, &level))
        {
            int threadNumber = isBusy.head();
            isBusy.pop_front();
            threadsList[threadNumber]->start();
            ++searchingCount;
            QMetaObject::invokeMethod(httpWorker[threadNumber].get(),"loadData",Qt::QueuedConnection,
                                      Q_ARG(int ,threadNumber), Q_ARG(QUrl, link), Q_ARG(int, level), Q_ARG(QString, c_SubStr));
        }
        else if((isBusy.size() == c_NumberOfCores) && (dataController->getLinksNumber() == 0))
        {
            shutDown();
        }

    }
    else if (((isBusy.size() == c_NumberOfCores) && (dataController->getLinksNumber() == 0 || isStoped)) || (searchingCount >= c_maxSearchCycle))
    {
        shutDown();

    }

}

void ThreadController::stop()
{
    isStoped = true;
}

void ThreadController::writeLog(QString log)
{
    emit updateLog(log);
}

void ThreadController::shutDown()
{
    QString logFinish = "\nFinished\n";
    emit updateProgress(1);
    emit updateLog(logFinish);
    emit finished();
}
