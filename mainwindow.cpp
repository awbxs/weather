#include "mainwindow.h"
#include <QMap>
#include <QTextToSpeech>
#include <QDebug>

#include <QContextMenuEvent>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMenu>
#include <QMessageBox>
#include <QNetworkReply>
#include <QPainter>
#include <QTimer>
#include <QMessageBox>
#include "ui_mainwindow.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPainter>

#define INCREMENT 5//温度每升高或降低坐标的偏移量
#define POINT_RADIUS 2//曲线点的大小
#define TEXT_OFFSET_X 18
#define TEXT_OFFSET_Y 13

#define init_city "贵阳"

QMap<QString,QString> weatherTool::mCityMap = {};

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    //设置窗口属性
    setWindowFlag(Qt::FramelessWindowHint);  // 无边框
    setFixedSize(width(), height());         // 固定窗口大小




    // 右键菜单：退出程序
    mExitMenu = new QMenu(this);
    mExitAct = new QAction();
    mExitAct->setText(tr("退出"));
    mExitAct->setIcon(QIcon(":/res/close.png"));
    mExitMenu->addAction(mExitAct);

    connect(mExitAct, &QAction::triggered, this, [=]() { qApp->exit(0); });
    
    //将控件添加到控件数组
    mWeekList<<ui->lblWeek0<<ui->lblWeek1<<ui->lblWeek2<<ui->lblWeek3<<ui->lblWeek4<<ui->lblWeek5;
    mDateList<<ui->lblDate0<<ui->lblDate1<<ui->lblDate2<<ui->lblDate3<<ui->lblDate4<<ui->lblDate5;
    mTypeList<<ui->lblType0<<ui->lblType1<<ui->lblType2<<ui->lblType3<<ui->lblType4<<ui->lblType5;
    mTypeIconList<<ui->lblTypeIcon0<<ui->lblTypeIcon1<<ui->lblTypeIcon2<<ui->lblTypeIcon3<<ui->lblTypeIcon4<<ui->lblTypeIcon5;
    mAqiList<<ui->lblQuality0<<ui->lblQuality1<<ui->lblQuality2<<ui->lblQuality3<<ui->lblQuality4<<ui->lblQuality5;
    mFlList<<ui->lblFl0<<ui->lblFl1<<ui->lblFl2<<ui->lblFl3<<ui->lblFl4<<ui->lblFl5;
    mFxList<<ui->lblFx0<<ui->lblFx1<<ui->lblFx2<<ui->lblFx3<<ui->lblFx4<<ui->lblFx5;

    mTypeMap.insert("暴雪",":/res/type/BaoXue.png");
    mTypeMap.insert("暴雨",":/res/type/BaoYu.png");
    mTypeMap.insert("暴雨到大暴雨",":/res/type/BaoYuDaoDaBaoYu.png");
    mTypeMap.insert("大暴雨",":/res/type/DaBaoYu.png");
    mTypeMap.insert("大暴雨到特大暴雨",":/res/type/DaBaoYuDaoTeDaBaoYu.png");
    mTypeMap.insert("大到暴雪",":/res/type/DaDaoBaoXue.png");
    mTypeMap.insert("大雪",":/res/type/DaXue.png");
    mTypeMap.insert("大雨",":/res/type/DaYu.png") ;
    mTypeMap.insert("冻雨",":/res/type/DongYu .png");
    mTypeMap.insert("多云",":/res/type/DuoYun.png");
    mTypeMap.insert("浮沉",": /res/type/Fuchen .png");
    mTypeMap.insert("雷阵雨",":/res/type/LeizhenYu.png");
    mTypeMap.insert("雷阵雨伴有冰雹",":/res/type/LeizhenYuBanYouBingBao.png");
    mTypeMap.insert("霾",":/res/type/Mai.png");
    mTypeMap.insert("强沙尘暴",":/res/type/QiangshaChenBao.png");
    mTypeMap.insert("晴",":/res/type/Qing.png");
    mTypeMap.insert("沙尘暴",":/res/type/shachenBao,png");
    mTypeMap.insert("特大暴雨",":/res/type/TeDaBaoYu.png");
    mTypeMap.insert("undefined",":/res/type/undefined.png");
    mTypeMap.insert("雾",":/res/type/wu.png");
    mTypeMap.insert("小到中雪",":/res/type/XiaoDaozhongXue.png");
    mTypeMap.insert("小到中雨" ,":/res/type/XiaoDaozhongYu.png");
    mTypeMap.insert("小雪",":/res/type/XiaoXue.png");
    mTypeMap.insert("小雪",":/res/type/XiaoYu.png");
    mTypeMap.insert("小雨",":/res/type/XiaoXue.png");
    mTypeMap.insert("雪",":/res/type/Xue.png");
    mTypeMap.insert("扬沙",":/res/type/Yangsha.png");
    mTypeMap.insert("阴",":/res/type/Yin.png");
    mTypeMap.insert("雨",":/res/type/Yu.png");
    mTypeMap.insert("雨夹雪",":/res/type/YuJiaxue.png");


    mTypeMap.insert("雨夹雪",":/res/type/YuJiaxue.png");
    mTypeMap.insert("阵雪",":/res/type/ZhenXue.png");
    mTypeMap.insert("阵雨",":/res/type/ZhenYu.png");
    mTypeMap.insert("中到大雪",":/res/type/ZhongDaoDaxue.png");
    mTypeMap.insert("中到大雨",":/res/type/ZhongDaoDaYu.png");
    mTypeMap.insert("中雪",":/res/type/Zhongxue.png");
    mTypeMap.insert("中雨",":/res/type/ZhongYu.png");

    mNetAccessManager = new QNetworkAccessManager(this);
    connect(mNetAccessManager,&QNetworkAccessManager::finished,this,&MainWindow::onReplied);
    //北京城市编码
//    getWeatherInfo("101010100");

    connect(ui->btnSearch,SIGNAL(pressed()),this,SLOT(pressed()));


    getWeatherInfo(init_city);

    qDebug()<<"helljj";


    //添加事件过滤器
    //当前窗口
    ui->lblHighCurve->installEventFilter(this);
    ui->lblLowCurve->installEventFilter(this);

//    resizeEvent(this);



}


//void MainWindow::resizeEvent(QResizeEvent *event)
//{
//    MainWindow::resizeEvent(event);

//    // 获取根控件
//    MainWindow *rootWidget = this;

//    // 计算比例因子
//    double scaleFactor = 0.8; // 0.8表示缩小到原来的80%

//    // 遍历所有控件
//    QList<QWidget *> allWidgets = rootWidget->findChildren<QWidget *>();
//    foreach (QWidget *widget, allWidgets) {
//        // 计算新尺寸
//        int newWidth = widget->width() * scaleFactor;
//        int newHeight = widget->height() * scaleFactor;

//        // 设置新尺寸
//        widget->resize(newWidth, newHeight);

//        // 可以根据需要设置新位置
//        // widget->move(newX, newY);
//    }
//}



MainWindow::~MainWindow() { delete ui; }

void MainWindow::contextMenuEvent(QContextMenuEvent* event) {
    mExitMenu->exec(QCursor::pos());
    event->accept();
}


void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos()-mOffset);
}

void MainWindow::getWeatherInfo(QString cityname)
{
    QString citycode = weatherTool::getCityCode(cityname);
    qDebug()<<"citycode : "<<citycode;
    QUrl url("http://t.weather.itboy.net/api/weather/city/"+citycode);
    mNetAccessManager->get(QNetworkRequest(url));
    qDebug()<<"hell";
    qDebug()<<"getWeatherInfo";
}

void MainWindow::parseJson(QByteArray &byteArray)
{
    QJsonParseError err;
    QJsonDocument dec = QJsonDocument::fromJson(byteArray,&err);
    if(err.error != QJsonParseError::NoError){
        return ;
    }
    QJsonObject rootobj = dec.object();
    qDebug()<<rootobj.value("message").toString();

    mToday.date = rootobj.value("date").toString();
    mToday.city = rootobj.value("cityInfo").toObject().value("city").toString();

    //解析昨天
    QJsonObject objData = rootobj.value("data").toObject();
    QJsonObject objYesterday = objData.value("yesterday").toObject();
    mDay[0].week = objYesterday.value("week").toString();
    mDay[0].date = objYesterday.value("ymd").toString();
    mDay[0].type = objYesterday.value("type").toString();

        //获取高温
    QString s =objYesterday.value("high").toString().split(" ").at(1);
//    s.left(s.length()-1);
    QString s1;
    for(int i=0;i<2;i++){
        s1[i]=s[i];
    }
//    qDebug()<<s1;
    mDay[0].high = s1.toInt();
//    qDebug()<<mDay[0].high;
        //获取高温
    s =objYesterday.value("low").toString().split(" ").at(1);
    //    s.left(s.length()-1);
    QString s2;
    for(int i=0;i<2;i++){
        s2[i]=s[i];
    }
//    qDebug()<<s2;
    mDay[0].low = s2.toInt();
//    qDebug()<<mDay[0].low;

    //获取风向和风力
    mDay[0].fl = objYesterday.value("fl").toString();
    mDay[0].fx = objYesterday.value("fx").toString();
    //获取污染指数
    mDay[0].aqi = objYesterday.value("aqi").toDouble();


    //解析forcast中5填数据
    QJsonArray forecastArr = objData.value("forecast").toArray();
    for(int i = 0; i < 5; i++){
        QJsonObject objforecast = forecastArr[i].toObject();

        mDay[i+1].week = objforecast.value("week").toString();
        mDay[i+1].type = objforecast.value("type").toString();
        mDay[i+1].date = objforecast.value("date").toString();
        mDay[i+1].aqi = objforecast.value("aqi").toDouble();
        mDay[i+1].fl = objforecast.value("fl").toString();
        mDay[i+1].fx = objforecast.value("fx").toString();
        mDay[i+1].date1 = objforecast.value("ymd").toString();
        mDay[i+1].natice = objforecast.value("notice").toString();
        qDebug()<<"mDay[i+1].natice "<<mDay[i+1].natice;
            //获取高温
        QString s =objforecast.value("high").toString().split(" ").at(1);
    //    s.left(s.length()-1);
        QString s1;
        for(int j=0;j<2;j++){
            s1[j]=s[j];
        }
    //    qDebug()<<s1;
        mDay[i+1].high = s1.toInt();
        qDebug()<<mDay[i+1].high;
            //获取低温
        s =objforecast.value("low").toString().split(" ").at(1);
        //    s.left(s.length()-1);
        QString s2;
        for(int j=0;j<2;j++){
            s2[j]=s[j];
        }
    //    qDebug()<<s2;
        mDay[i+1].low = s2.toInt();
        qDebug()<<mDay[i+1].low;

        //获取风向和风力

    }

    //解析今天的数据
    mToday.ganmao = objData.value("ganmao").toString();
    mToday.wendu = objData.value("wendu").toString().toInt();
    mToday.shidu = objData.value("shidu").toString();
    mToday.pm25 = objData.value("pm25").toInt();
    mToday.quality = objData.value("quality").toString();
    mToday.type =mDay[1].type;
    mToday.fl =mDay[1].fl;
    mToday.fx =mDay[1].fx;
    mToday.high =mDay[1].high;
    mToday.low =mDay[1].low;
    mToday.natice = mDay[1].natice;
    qDebug()<< "high"<<mToday.high;
    qDebug()<< "mToday.wendu"<<mToday.wendu;
    qDebug()<< mToday.low;
//    speech->stop();//停止当前的播报
//    speech->pause();//暂停当前的播报
//    speech->resume();//重新播报当前内容


    //更新UI界面
    updateUI();

    //绘制高低温
    ui->lblHighCurve->update();
    ui->lblLowCurve->update();

    qDebug()<<"parseJson()";

}

//延时功能
void MainWindow::sleep(unsigned int msec){
//currnentTime 返回当前时间 用当前时间加上我们要延时的时间msec得到一个新的时刻
    QTime reachTime = QTime::currentTime().addMSecs(msec);
    //用while循环不断比对当前时间与我们设定的时间
    while(QTime::currentTime()<reachTime){
    //如果当前的系统时间尚未达到我们设定的时刻，就让Qt的应用程序类执行默认的处理，
    //以使程序仍处于响应状态。一旦到达了我们设定的时刻，就跳出该循环，继续执行后面的语句。
        QApplication::processEvents(QEventLoop::AllEvents,100);
    }
}


void MainWindow::speak(QString text2)
{
    qDebug()<<"void MainWindow::speak()";
    sleep(1000);
    qDebug()<<"text2"<<text2;
    QTextToSpeech *speech = new QTextToSpeech();
    QString text1 = ui->lblCity->text();
    if(text2 == "今天"){
        QString high = QString :: number(mToday.high);
        QString low = QString :: number(mToday.low);
        qDebug()<<high<< "  "<<low;
        speech->say("这里是" +text1);
         speech->say(", 今天" +QDateTime::fromString( mToday.date,"yyyyMMdd").toString("yyyy年MM月dd日")+ "天气"+ mToday.type);
         speech->say(",最高温" +high+ ",最低温"+low);
         speech->say(",建议"+ mToday.natice );
        /*
        if(mToday.type.indexOf("雨") != -1)
            speech->say(", 今天" +QDateTime::fromString( mToday.date,"yyyyMMdd").toString("yyyy年MM月dd日")+ "今天将降雨，出门请别忘带伞。别被淋成个落汤鸡");//播报msg
        //    else if(mToday.type.indexOf("暴雨") != -1 || mToday.type.indexOf("大雨") != -1 ||mToday.type.indexOf("阵雨") != -1)
        //        speech->say(", 今天" +mToday.type+ "今天将降雨，出门请别忘带伞，淋成个落汤鸡");
        else if(mToday.type.indexOf("雪") != -1)
            speech->say(", 今天" +QDateTime::fromString( mToday.date,"yyyyMMdd").toString("yyyy年MM月dd日")+ "朋友们请做好御寒工作,注意防滑");
        else if(mToday.type.indexOf("晴") != -1)
            speech->say(", 今天" +QDateTime::fromString( mToday.date,"yyyyMMdd").toString("yyyy年MM月dd日")+ "蓝蓝的天空鸟儿飞");
        else if(mDay[2].type.indexOf("云") != -1)
            speech->say(", 明天" +QDateTime::fromString( mToday.date,"yyyyMMdd").toString("yyyy年MM月dd日")+ "出来玩啊");
        else if(mDay[2].type.indexOf("阴") != -1)
            speech->say(", 明天" +QDateTime::fromString( mToday.date,"yyyyMMdd").toString("yyyy年MM月dd日")+ "一抹了");
        speech->say(", 最高温" +high+ ",最低温 "+low);//播报msg
        qDebug()<<"high :"<<high;
        if( mToday.high>28){
            qDebug()<<"high :"<<high;
            speech->say(", 最高温" +high+ ", 请注意避暑，寡求热，热死个人喽，空调西瓜不能少");
        }

        else if(mToday.high >25 && mToday.high <28){
            qDebug()<<"high :"<<high;
            speech->say(", 最高温" +high+ ", 天气合适，还是有点热，空调可以关了");
        }

        else if(mToday.high > 20 && mToday.high <25){
            qDebug()<<"high :"<<high;
            speech->say(", 最高温" +high+ ", 天气微凉，嘎嘎舒服，适合养生");
        }

        else if(mToday.high < 20){
            qDebug()<<"high :"<<high;
            speech->say(", 最高温" +high+ ", 有点凉快了，还可以接受");
        }
        else if(mToday.high < 10){
            qDebug()<<"high :"<<high;
            speech->say(", 最高温" +high+ ", 寡求冷，冻死个人，还不穿厚点");
        }

        */


}
    else if(text2 == "明天"){
        QString high = QString :: number(mDay[2].high);
        QString low = QString :: number(mDay[2].low);
        qDebug()<< " mDay[2].date1 "<<mDay[2].date1;
        speech->say("这里是" +text1);
        speech->say(", 明天" +QDateTime::fromString( mDay[2].date1,"yyyy-MM-dd").toString("yyyy年MM月dd日")+ "天气"+ mDay[2].type);
        speech->say(",最高温" +high+ ",最低温"+low);
        speech->say(",建议"+ mDay[2].natice );
       /*
        qDebug()<<"mDay[2].date "<<mDay[2].date;
        if(mDay[2].type.indexOf("雨") != -1)
            speech->say(", 明天" +QDateTime::fromString(mDay[2].date1,"yyyyMMdd").toString("yyyy年MM月dd日")+ "明天将降雨，出门请别忘带伞。别被淋成个落汤鸡");//播报msg
        //    else if(mToday.type.indexOf("暴雨") != -1 || mToday.type.indexOf("大雨") != -1 ||mToday.type.indexOf("阵雨") != -1)
        //        speech->say(", 今天" +mToday.type+ "今天将降雨，出门请别忘带伞，淋成个落汤鸡");
        else if(mDay[2].type.indexOf("雪") != -1)
            speech->say(", 明天" +QDateTime::fromString(mDay[2].date1,"yyyyMMdd").toString("yyyy年MM月dd日")+ "朋友们请做好御寒工作,注意防滑");
        else if(mDay[2].type.indexOf("晴") != -1)
            speech->say(", 明天" +QDateTime::fromString(mDay[2].date1,"yyyyMMdd").toString("yyyy年MM月dd日")+ "蓝蓝的天空鸟儿飞");
        else if(mDay[2].type.indexOf("云") != -1)
            speech->say(", 明天" +QDateTime::fromString(mDay[2].date1,"yyyyMMdd").toString("yyyy年MM月dd日")+ "出来玩啊");
        else if(mDay[2].type.indexOf("阴") != -1)
            speech->say(", 明天" +QDateTime::fromString(mDay[2].date1,"yyyyMMdd").toString("yyyy年MM月dd日")+  "一抹了");
//        else

        speech->say(", 最高温" +high+ ",最低温 "+low);//播报msg
        qDebug()<<"high :"<<high;
        if( mDay[2].high>28){
            qDebug()<<"high :"<<high;
            speech->say(", 最高温" +high+ ", 请注意避暑，寡求热，热死个人喽，空调西瓜不能少");
        }

        else if(mDay[2].high >25 && mDay[2].high <28){
            qDebug()<<"high :"<<high;
            speech->say(", 最高温" +high+ ", 天气合适，还是有点热，空调可以关了");
        }

        else if(mDay[2].high > 20 && mDay[2].high <25){
            qDebug()<<"high :"<<high;
            speech->say(", 最高温" +high+ ", 天气微凉，嘎嘎舒服，适合养生");
        }

        else if(mDay[2].high < 20){
            qDebug()<<"high :"<<high;
            speech->say(", 最高温" +high+ ", 有点凉快了，还可以接受");
        }
        else if(mDay[2].high < 10){
            qDebug()<<"high :"<<high;
            speech->say(", 最高温" +high+ ", 寡求冷，冻死个人，还不穿厚点");
        }
        */
    }




}

void MainWindow::mousePressEvent(QMouseEvent *event)
{

    if(event->button() == Qt::MidButton)
    {
        audio = new Audio;
        audio->startAudio("D:\\1.wav");
    }
    else
    {
        mOffset = event->globalPos()-this->pos();
    }
}


void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::MidButton){
        audio->stopAudio();
        speech m_speech;
        QString text = m_speech.speechIdentify("D:\\1.wav");
        qDebug()<<"text: "<<text;
        if(text.mid(0,2) == "今天" && text.indexOf("天气")){
            getWeatherInfo(init_city);
            speak("今天");
            qDebug()<<"1";
        }
        else if(text.mid(0,2) == "明天" && text.indexOf("天气")){
            getWeatherInfo(init_city);
            speak("明天");
            qDebug()<<"2";

        }
//        urlString.contains("deviceinfo",Qt::CaseSensitive)
        else if(text.contains("今天",Qt::CaseSensitive)  && text.indexOf("天气")){
            getWeatherInfo(text.mid(0,2));
            speak("今天");
            qDebug()<<"3";
        }
        else if(text.contains("明天",Qt::CaseSensitive)  && text.indexOf("天气")){
            getWeatherInfo(text.mid(0,2));
            speak("明天");
            qDebug()<<"4";
            ui->lblCity->text();
        }

    }

}

//QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
void MainWindow::updateUI()
{
    //更新日期
    ui->lblDate->setText(QDateTime::fromString( mToday.date,"yyyyMMdd").toString("yyyy/MM/dd")+" "+mDay[1].week);
    //更新城市
    ui->lblCity->setText(mToday.city);

    //更新今天
    ui->lblTypeIcon->setPixmap(mTypeMap[mToday.type]);
    //温度
    ui->lblTemp->setText(QString::number(mToday.wendu));
    //类型
    ui->lblType->setText(mToday.type);
    //感冒指数
    ui->lblGanMao->setText(mToday.ganmao);
    //风向和风力
    ui->lblWindFl->setText(mToday.fl);
    ui->lblWindFx->setText(mToday.fx);
    //湿度和PM
    ui->lblPM25->setText(QString::number(mToday.pm25));
    ui->lblShiDu->setText(mToday.shidu);

    //更新最高温最低温
    QString high = QString :: number(mToday.high);
    QString low = QString :: number(mToday.low);
    ui->lblLowHigh->setText(low+"° ~ "+high+" °");

    QString s ;
    //更新六天
    for(int i=0; i<6; i++){
        mWeekList[i]->setText("周" + mDay[i].week.right(1));
        mWeekList[0]->setText("昨天");
        mWeekList[1]->setText("今天");
        mWeekList[2]->setText("明天");

        QStringList ymdList = mDay[i].date.split("-");
        qDebug()<<"ymdList  "<<ymdList;

        if(i == 0){
            mDateList[i]->setText(ymdList[1] + "/" + ymdList[2]);
             s = ymdList[1];
        }
        else
            mDateList[i]->setText(s + "/" + ymdList[0]);

        //更新天气类型
        mTypeList[i]->setText(mDay[i].type);
        mTypeIconList[i]->setPixmap(mTypeMap[mDay[i].type]);
        qDebug()<<mDay[i].type;

        //更新空气质量
        if(mDay[i].aqi>=0 && mDay[i].aqi<50){
            mAqiList[i]->setText("优");
            mAqiList[i]->setStyleSheet("background-color: rgb(121,184,8);");
        }
        else if(mDay[i].aqi>=50 && mDay[i].aqi<100){
            mAqiList[i]->setText("良");
            mAqiList[i]->setStyleSheet("background-color: rgb(255,187,23);");
        }
        else if(mDay[i].aqi>=100 && mDay[i].aqi<150){
            mAqiList[i]->setText("轻度");
            mAqiList[i]->setStyleSheet("background-color: rgb(155,87,97);");
        }
        else if(mDay[i].aqi>=150 && mDay[i].aqi<200){
            mAqiList[i]->setText("中度");
            mAqiList[i]->setStyleSheet("background-color: rgb(235,17,27);");
        }
        else if(mDay[i].aqi>=200 && mDay[i].aqi<250){
            mAqiList[i]->setText("重度");
            mAqiList[i]->setStyleSheet("background-color: rgb(170,0,0);");
        }
        else{
            mAqiList[i]->setText("严重");
            mAqiList[i]->setStyleSheet("background-color: rgb(110,0,0);");
        }

        //更新风力和风向
        mFxList[i]->setText(mDay[i].fx);
        mFlList[i]->setText(mDay[i].fl);

        qDebug()<<"UPDateUI";

    }


}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->lblHighCurve && event->type() == QEvent::Paint)
    {
        paintHighCurve();
    }
    else if(watched == ui->lblLowCurve && event->type() == QEvent::Paint)
    {
        paintLowCurve();
    }
     qDebug()<<"jjj";
    return QWidget::eventFilter(watched,event);


}

void MainWindow::paintHighCurve()
{
    QPainter painter(ui->lblHighCurve);
    //抗锯齿
    painter.setRenderHint(QPainter::Antialiasing,true);

    //1.获取x坐标
    int pointX[6] = {0};
    for(int i=0;i<6;i++){
        pointX[i] = mWeekList[i]->pos().x()+mWeekList[i]->width()/2;
    }
    //2.获取y坐标
    int tempSum = 0;
    int tempAve = 0;
    for(int i=0;i<6;i++){
        tempSum += mDay[i].high;
    }
    tempAve = tempSum/6;

    int pointY[6] = {0};
    int yCenter = ui->lblHighCurve->height()/2;
    for(int i=0;i<6;i++){
        pointY[i] = yCenter - ((mDay[i].high-tempAve)*INCREMENT);
    }


    //3,绘制
    //初始化画笔
    QPen pen = painter.pen();
    pen.setWidth(1);
    pen.setColor(QColor(255,170,0));

    painter.setPen(pen);//设置画笔
    painter.setBrush(QColor(255,170,0));//设置画刷
    //画点
    for(int i=0;i<6;i++){
        painter.drawEllipse(QPoint(pointX[i],pointY[i]),POINT_RADIUS,POINT_RADIUS);

        //显示温度文本
        painter.drawText(pointX[i]-TEXT_OFFSET_X,pointY[i]-TEXT_OFFSET_Y,QString::number(mDay[i].high)+"°");

    }

    //画线
    for(int i=0; i<5; i++){
        if(i ==0 ){
            pen.setStyle(Qt::DotLine);
            painter.setPen(pen);
            painter.drawLine(pointX[i],pointY[i],pointX[i+1],pointY[i+1]);
        }
        else{
            pen.setStyle(Qt::SolidLine);
            painter.setPen(pen);
            painter.drawLine(pointX[i],pointY[i],pointX[i+1],pointY[i+1]);
        }

    }




}

void MainWindow::paintLowCurve()
{
    QPainter painter(ui->lblLowCurve);
    //抗锯齿
    painter.setRenderHint(QPainter::Antialiasing,true);

    //1.获取x坐标
    int pointX[6] = {0};
    for(int i=0;i<6;i++){
        pointX[i] = mWeekList[i]->pos().x()+mWeekList[i]->width()/2;
    }
    //2.获取y坐标
    int tempSum = 0;
    int tempAve = 0;
    for(int i=0;i<6;i++){
        tempSum += mDay[i].low;
    }
    tempAve = tempSum/6;

    int pointY[6] = {0};
    int yCenter = ui->lblLowCurve->height()/2;
    for(int i=0;i<6;i++){
        pointY[i] = yCenter - ((mDay[i].low-tempAve)*INCREMENT);
    }


    //3,绘制
    //初始化画笔
    QPen pen = painter.pen();
    pen.setWidth(1);
    pen.setColor(QColor(0,255,255));

    painter.setPen(pen);//设置画笔
    painter.setBrush(QColor(0,255,255));//设置画刷
    //画点
    for(int i=0;i<6;i++){
        painter.drawEllipse(QPoint(pointX[i],pointY[i]),POINT_RADIUS,POINT_RADIUS);

        //显示温度文本
        painter.drawText(pointX[i]-TEXT_OFFSET_X,pointY[i]-TEXT_OFFSET_Y,QString::number(mDay[i].low)+"°");

    }

    //画线
    for(int i=0; i<5; i++){
        if(i ==0 ){
            pen.setStyle(Qt::DotLine);
            painter.setPen(pen);
            painter.drawLine(pointX[i],pointY[i],pointX[i+1],pointY[i+1]);
        }
        else{
            pen.setStyle(Qt::SolidLine);
            painter.setPen(pen);
            painter.drawLine(pointX[i],pointY[i],pointX[i+1],pointY[i+1]);
        }

    }
}



void MainWindow::pressed()
{
    QString text = ui->leCity->text();
    getWeatherInfo(text);

}

void MainWindow::onReplied(QNetworkReply *reply)
{
    //解析数据
    qDebug()<<"onReplied";
    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug()<<"status_code: "<<status_code;

    if(reply->error()!=QNetworkReply::NoError || status_code != 200){
        qDebug()<<reply->errorString().toLatin1().data();
        QMessageBox::warning(this,"天气","请求失败",QMessageBox::Ok);
    }
    else{
        /*
        static QString serialBuffer;

        QByteArray serialBufferTmp=m_serialPort->readAll();

        serialBuffer=serialBuffer.append(serialBufferTmp);
        */
        static QString serialBuffer;
        QByteArray byteArray = reply->readAll();
        serialBuffer = serialBuffer.append(byteArray);
        qDebug()<<"read all: "<<serialBuffer;
        parseJson(byteArray);

    }



}


//void MainWindow::mousePressEvent(QMouseEvent *event){
//    if(event->button() == Qt::MidButton){
//        speech m_speech;
//        QString text2 = m_speech.speechIdentify("D:\\1.wav");
//        qDebug()<<"text2"<<text2;
//    }
//}
