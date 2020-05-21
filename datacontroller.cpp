#include "datacontroller.h"

DataController::DataController(QObject *parent) : QObject(parent)
{

}


void DataController::addLink(const int *level, const QUrl *link)
{
    if (!fullLinksList.contains(*link))
    {
        fullLinksList.insert(*link);
        links.push(qMakePair(*level+1, *link));
        linkAdded();
    }
}

bool DataController::getLink(QUrl *link, int *level)
{
    if (!links.empty())
    {
        *level = links.top().first;
        *link = links.top().second;
        links.pop();
        return true;
    }
    else
    {
        return false;
    }
}

int DataController::getLinksNumber()
{
    return links.size();
}
