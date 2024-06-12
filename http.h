#ifndef HTTP_H
#define HTTP_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>

#include <QObject>
#include <QMap>
#include <QDebug>


class http : public QObject
{
    Q_OBJECT
public:
    explicit http(QObject *parent = nullptr);
    
    bool post_sync(QString url, QMap<QString,QString> header,QByteArray requestData,QByteArray &replyData);
    
signals:

public slots:
};

#endif // HTTP_H
