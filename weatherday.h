#ifndef WEATHERDAY_H
#define WEATHERDAY_H

class Today{
    public:
    Today(){
        date = "2022-10-20";
        city ="广州";
        ganmao ="恋后指数";
        wendu = 0;
        shidu = "0%";
        pm25 = 0;
        quality ="儿微据";
        type = "多云";
        fl ="2级";
        fx ="南风";
        high = 30;
        low = 18;
        natice = "";
    }
    QString date;
    QString city;
    QString ganmao;
    int wendu;
    QString shidu ;
    int pm25;
    QString quality;
    QString type;
    QString fx;
    QString fl;
    int high ;
    int low;
    QString natice;
};

class Day{
    public:
    Day(){
        date = "2022-10-20";
        week ="周五";
        type ="多云";
        high = 0;
        low = 0;
        fx ="而风";
        fl ="2级";
        aqi = 0;
        date1 = "";
        natice = "";
    }
    QString date;
    QString week;
    QString type;
    int high;
    int low;
    QString fx;
    QString fl;
    int aqi;
    QString date1;
    QString natice;
};

#endif // WEATHERDAY_H
