#include "speech.h"

speech::speech(QObject *parent) : QObject(parent)
{

}

QString speech::speechIdentify(QString filename)
{
     QString accessToken;
    //获取access——token
    QString tokenUrl = QString(baiduToken).arg(client_id).arg(client_secret);
    QMap<QString, QString> header;
    header.insert(QString("Content-Type"),QString("audio/pcm;rate=16000"));

    QByteArray requsetData;//发送内容
    QByteArray replyData;//服务器返回

    
    
    
    http m_http;
    bool result = m_http.post_sync(tokenUrl,header,requsetData,replyData);
    
    if(result)
    {
        QString key = "access_token";
        accessToken = getJsonValue(replyData,key);
        qDebug()<<"accessToken"<<accessToken;
    }
    else
    {
//        this->close();
    }
    QString speechUrl = QString(baidu1SpeechUrl).arg(QHostInfo::localHostName()).arg(accessToken);
    qDebug()<<"speechUrl  "<<speechUrl;
    //打开文件，转化成QByearray
    QFile file;
    file.setFileName(filename);
    file.open(QIODevice::ReadOnly);
    requsetData = file.readAll();
    file.close();

    replyData.clear();


    result = m_http.post_sync(speechUrl,header,requsetData,replyData);

    qDebug()<<"replyData: "<<replyData;

    if(result)
    {
        QString key = "result";
        QString text = getJsonValue(replyData,key);
        qDebug()<<"text: "<<text;
        return text;
    }
    else
    {
        QMessageBox::warning(NULL,"识别提示","识别失败");
    }
    return "";
}

QString speech::getJsonValue(QByteArray ba, QString key)
{
//    QByteArray
    QJsonParseError err;

    QJsonDocument jsonDocument = QJsonDocument::fromJson(ba,&err);

    if(err.error == QJsonParseError::NoError)
    {
        if(jsonDocument.isObject()){
            QJsonObject jsonObj = jsonDocument.object();
            if(jsonObj.contains(key)){
                QJsonValue jsonVal = jsonObj.value(key);
                if(jsonVal.isString()){
                    qDebug()<<"jsonVal.toString():"<<jsonVal.toString();
                    return jsonVal.toString();
                }

                else if(jsonVal.isArray())
                {
                    QJsonArray arr = jsonVal.toArray();
                    QJsonValue jv = arr.at(0);
                    return jv.toString();
                }
            }
        }
    }
    return "";
}



