#ifndef WEATHERTOOL_H
#define WEATHERTOOL_H
#include <QFile>
#include <QMap>
#include <QDebug>
#include <QJsonArray>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonParseError>
class weatherTool{

public:
    static QString getCityCode(QString cityName){

        qDebug()<<"cityname :"<<cityName;

        if(mCityMap.isEmpty()){
            initCityMap();
        }
        QMap<QString,QString>::iterator it = mCityMap.find(cityName);

        if(it == mCityMap.end()) {
            it = mCityMap.find((cityName+"市"));

        }
//        qDebug()<<"it "<<it.value();
        if(it != mCityMap.end()) {
            return it.value();
        }
        else{
            return NULL;
        }
    }

private:

    static QMap<QString,QString>mCityMap;

    static void initCityMap(){
        //读取文件
        QFile file("D:/QTcode/object/weather/Weather/citycode.json");
        file.open(QFile::ReadOnly);
        QByteArray Json = file.readAll();
        file.close();

        //解析，并且读到map
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(Json,&err);
        if(err.error != QJsonParseError::NoError){
            return ;
        }

        if(!doc.isArray()){
            return ;
        }

       QJsonArray cities = doc.array();
       for(int i=0;i<cities.size();i++){
            QString city_name = cities[i].toObject().value("city_name").toString();
            QString city_code = cities[i].toObject().value("city_code").toString();
            if(city_code.size()>0){
                mCityMap.insert(city_name,city_code);
            }




       }
    }

};


#endif // WEATHERTOOL_H
