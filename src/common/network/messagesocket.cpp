#include "messagesocket.h"

#include <QTcpSocket>
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>

MessageSocket::MessageSocket(QObject *parent) :
    Socket(parent)
{
    connect(this,&Socket::newData,
            this,&MessageSocket::onNewMessage);
}

MessageSocket::~MessageSocket()
{
    close();
}

void MessageSocket::sendMessage(const QString &content)
{
    QJsonObject map;
    QJsonDocument doc;
    map.insert("content", content);
    doc.setObject(map);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    QByteArray buffer = doc.toJson(QJsonDocument::Compact);
#else
    QByteArray buffer = doc.toJson();
#endif

    this->sendData(buffer);
}

void MessageSocket::onNewMessage(const QByteArray &array)
{
    QJsonObject map = QJsonDocument::fromJson(array).object();
    if(!map.contains("content")) return;

    QString string = map["content"].toString();
    emit newMessage(string);
}
