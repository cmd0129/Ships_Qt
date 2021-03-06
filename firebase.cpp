#include "firebase.h"

/*!
 * \file firebase.cpp
 * \brief Plik źródłowy zawierający klasę Firebase, odpowiedzialny za REST API Firebase.
 *
 * Jest to REST API do bazy danych Firebase.
 */

Firebase::Firebase(const QString &hostName,
                   const QString &dbPath,
                   QObject *parent)
   : QObject(parent)
   , manager(new QNetworkAccessManager(this))
   , firebaseToken(QString(""))
{
    host = forceEndChar(hostName.trimmed(), '/');
    host = host.append(dbPath.trimmed());

    connect(manager, &QNetworkAccessManager::finished, this, &Firebase::replyFinished);
}

void Firebase::setValue(QJsonDocument jsonDoc,
                        const QString &verb,
                        const QString& queryString)
{
    QString path = buildPath(queryString);
    QNetworkRequest request(path);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/x-www-form-urlencoded");
    QByteArray jsonBA = jsonDoc.toJson(QJsonDocument::Compact);

    QBuffer *buffer = new QBuffer();
    buffer->open(QBuffer::ReadWrite);
    buffer->write(jsonBA);
    buffer->seek(0);

    QByteArray verbBA = verb.toUtf8();
    manager->sendCustomRequest(request, verbBA, buffer);
    buffer->close();
}

void Firebase::getValue(const QString& queryString)
{
    QNetworkRequest request(buildPath(queryString));
    manager->get(request);
}

void Firebase::listenEvents(const QString& queryString)
{
    open(buildPath(queryString));
}

QString Firebase::getPath(const QString &queryString)
{
    return buildPath(queryString);
}

void Firebase::replyFinished(QNetworkReply *reply)
{
    emit eventResponseReady(reply->readAll());
}

void Firebase::eventFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) {
        return;
    }

    QUrl redirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    if (!redirectUrl.isEmpty()) {
        reply->deleteLater();
        open(redirectUrl);
        return;
    }
    reply->deleteLater();
}

void Firebase::eventReadyRead()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) {
        return;
    }

    QByteArray event = reply->readLine();
    if (event.isEmpty()) {
        return;
    }

    QByteArray data = reply->readAll();

    if (event == "event: keep-alive\n") {
        parseKeepAlive(data);
    }
    else if (event == "event: put\n") {
        parsePut(data);
    }
    else if (event == "event: patch\n") { // dodane by działały eventy "PATCH"
        parsePut(data);
    }
    else {
        qWarning() << "Unknown Firebase event:" << event;
    }
}

void Firebase::open(const QUrl &url)
{
    QNetworkRequest request(url);
    request.setRawHeader("Accept", "text/event-stream");
    QNetworkReply *_reply = manager->get(request);
    connect(_reply, &QNetworkReply::readyRead, this, &Firebase::eventReadyRead);
    connect(_reply, &QNetworkReply::finished, this, &Firebase::eventFinished);
}

void Firebase::parseKeepAlive(QByteArray data)
{
    Q_UNUSED(data)

    emit eventKeepAlive();
}

void Firebase::parsePut(QByteArray data)
{
    data = trimValue(data);

    QJsonParseError error;
    QJsonDocument replyDoc = QJsonDocument::fromJson(data, &error);
    if (!replyDoc.isObject()) {
        qWarning() << "Malformed Firebase put data:" << data << error.error << error.errorString();
        return;
    }

    emit eventPut(replyDoc.object());
}

QString Firebase::buildPath(const QString &queryString)
{
    QString destination=host;

    const int dotJsonLength = 5;
    if (destination.length() <= dotJsonLength
            || destination.right(dotJsonLength) != ".json")
        destination.append(".json");

    if (queryString.length() > 0)
            destination.append(forceStartChar(queryString,'?'));

    return destination;
}

QString Firebase::forceEndChar(const QString &string, char endCh)
{
    if (string[string.length()-1] != endCh)
        return QString(string).append(endCh);

    return string;
}

QString Firebase::forceStartChar(const QString &string, char startCh)
{
    if (string.length() > 0 && string[0] != startCh)
        return QString(string).prepend(startCh);

    return string;
}

QByteArray Firebase::trimValue(const QByteArray &line) const
{
    QByteArray value;
    int index = line.indexOf(':');
    if (index > 0)
        value = line.right(line.size() - index  - 1);

    return value.trimmed();
}
