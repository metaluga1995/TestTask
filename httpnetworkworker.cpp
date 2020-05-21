#include "httpnetworkworker.h"

HttpNetworkWorker::HttpNetworkWorker(QObject *parent) : QObject(parent)
{

}

void HttpNetworkWorker::urlLoader(const QUrl* url, QString *data)
{
    QSharedPointer<QNetworkAccessManager> manager = QSharedPointer<QNetworkAccessManager>(new QNetworkAccessManager(this));
    QNetworkRequest request;
    emit sendLog(url->toString() + c_LogStart);
    request = QNetworkRequest(*url);
    QNetworkReply *reply(manager->get(request));
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()), Qt::DirectConnection);

    loop.exec();
    if (!reply->error())
    {
        emit sendLog(url->toString() + c_logDone);
        *data = reply->readAll();
    }
    else
    {
        emit sendLog(url->toString() + c_lodErr + reply->errorString() + "\n");
    }
}

void HttpNetworkWorker::loadData(int coreNumber, QUrl link, int level, QString subStr)
{
    QString data;
    urlLoader(&link, &data);
    emit done();

    htmlSearcher(&data, &level);
    subStringCounter(&data, &subStr, &link);

    emit finished(coreNumber);
}


void HttpNetworkWorker::htmlSearcher(const QString *page, const int *level)
{
    QRegExp httpRegExp("(HTTP://[!#$&-;=?-\\[\\]_a-z~]+)");

    int lastPos = 0;
    while( ( lastPos = httpRegExp.indexIn( *page, lastPos ) ) != -1 )
    {
        emit sendHtml( httpRegExp.cap(1), *level);
        lastPos += httpRegExp.matchedLength();
    }
}

void HttpNetworkWorker::subStringCounter(const QString *page, const QString *subString, const QUrl *link)
{
    int count = page->count(*subString);
    emit sendSubstrNumber(count, *link);
}
