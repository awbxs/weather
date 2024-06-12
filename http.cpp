#include "http.h"

http::http(QObject *parent) : QObject(parent)
{
    
}

bool http::post_sync(QString url, QMap<QString, QString> header, QByteArray requestData, QByteArray &replyData)
{
    //发送请求的对象
    QNetworkAccessManager manager;

    //请求对象
    QNetworkRequest request;
    request.setUrl(url);
    QMapIterator<QString,QString> it(header);
    while(it.hasNext())
    {
        it.next();
        request.setRawHeader(it.key().toLatin1(),it.value().toLatin1());
    }
//    qDebug()<<"post_sync";
    QNetworkReply *reply = manager.post(request,requestData);

    QEventLoop l;
    //一旦服务器返回，将会返回已给信号
    connect(reply,&QNetworkReply::finished,&l,&QEventLoop::quit);
    l.exec();//死循环，发出信号，结束循环

    if(reply != nullptr && reply->error() == QNetworkReply::NoError)
    {
        qDebug()<<"成功";
        replyData = reply->readAll();//服务器返回数据
        qDebug()<<replyData;
        return true;
    }
    else
    {
        qDebug()<<"请求失败";
        return false;
    }

}
