#include "roclient.h"

ROClient::ROClient(QQmlContext* context) :context(context)
{

}

void ROClient::connect(QString text)
{
    QUrl url("tcp://" + text);
    qInfo() << "Connectiong to: " << url.toString();
    if(!connection.connectToNode(url))
    {
        qCritical() << "Failed to connect: " << connection.lastError();
    }
    data.reset(connection.acquire<DisplayDataReplica>("mainData"));
    auto f = [=](){
        qInfo() << "Remote object initialized successfully";
        context->setContextProperty("mainData",data.get());
        emit connected();
    };
    if (data->isInitialized())
    {
        f();
        return;
    }
    QObject::connect(data.get(),&DisplayDataReplica::initialized,f);
}
