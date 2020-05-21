#ifndef DATACONTROLLER_H
#define DATACONTROLLER_H

#include <QtNetwork>
#include <QObject>
#include <queue>
#include <mutex>

class DataController : public QObject
{
    Q_OBJECT
public:
    explicit DataController(QObject *parent = nullptr);

    int getLinksNumber();
    bool getLink(QUrl *link, int *level);
private:
    QString htmlOutput;
    QString loadStartus;

    std::priority_queue<QPair<int, QUrl>,  QVector<QPair<int, QUrl>>, qGreater<QPair<int, QUrl>>> links;
    QSet<QUrl> fullLinksList;
    QMutex linksMutex;
signals:
    void linkAdded();
public slots:
    void addLink(const int *level, const QUrl *link);
};

#endif // DATACONTROLLER_H
