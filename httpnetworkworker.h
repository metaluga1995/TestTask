#ifndef HTTPNETWORKWORKER_H
#define HTTPNETWORKWORKER_H

#include <QObject>
#include <QtNetwork>

class HttpNetworkWorker : public QObject
{
    Q_OBJECT
public:
    explicit HttpNetworkWorker(QObject *parent = nullptr);

private:
    const QString c_LogStart = "\nlink is loading\n";
    const QString c_logDone = "\nlink opened\n";
    const QString c_lodErr = "\nlink load failed\n";

    void urlLoader(const QUrl* url, QString *data);
    void subStringCounter(const QString *page, const QString *subString, const QUrl *link);
    void htmlSearcher(const QString* page, const int* level);

public slots:
    void loadData(int coreNumber, QUrl link, int level, QString subStr);

signals:
    void finished(int coreNumber);
    void sendHtml(QUrl link, int linkLevel);
    void sendSubstrNumber(int number, QUrl link);
    void sendLog(QString log);
    void done();
};

#endif // HTTPNETWORKWORKER_H
