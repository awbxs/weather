#ifndef SPEECH_H
#define SPEECH_H

#include <QObject>
#include <http.h>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QHostInfo>
#include <QFile>
#include <QMessageBox>
//POST http://vop.baidu.com/server_api?dev_pid=1537&cuid=******&token=1.a6b7dbd428f731035f771b8d********.86400.1292922000-2346678-124328
//https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=&client_secret=0rDSjzQ20XUj5itV6WRtznPQSzr5pVw2&
//获取token
const QString baiduToken = "https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=%1&client_secret=%2&";

const QString client_id = "SpCBVbS1Zw2Nc2CQGN6SKKQx";
const QString client_secret = "CK3S8NL3LOPOhNi3xAWQIhtCzf2swtgF";

//语言识别相关

const QString baidu1SpeechUrl = "https://vop.baidu.com/pro_api?dev_pid=80001&cuid=%1&token=%2";



class speech : public QObject
{
    Q_OBJECT
public:
    explicit speech(QObject *parent = nullptr);

    QString speechIdentify(QString filename);

    QString getJsonValue(QByteArray ba,QString key);

signals:

public slots:
};

#endif // SPEECH_H
