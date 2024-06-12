#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <weatherday.h>
#include <weathertool.h>
#include <QEvent>

#include <audio.h>
#include <speech.h>
#include <http.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void contextMenuEvent(QContextMenuEvent* event);
    //鼠标按下去
    void mousePressEvent(QMouseEvent* event);

    //鼠标移动
    void mouseMoveEvent(QMouseEvent* event);

    void getWeatherInfo(QString citycode);

    void parseJson(QByteArray& byteArray);

    void updateUI();

    //重写eventFilter()
    bool eventFilter(QObject* watched,QEvent* event);
    //绘制高低温
    void paintHighCurve();
    void paintLowCurve();

    void speak(QString text);

//    int mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void sleep(unsigned int msec);

public slots:
    void pressed();

   private:
    Ui::MainWindow* ui;

    QMenu* mExitMenu;   // 右键退出的菜单
    QAction* mExitAct;  // 退出的行为

    QPoint mOffset;//窗口移动时，鼠标与窗口左上角的偏移

    QNetworkAccessManager* mNetAccessManager;

    void onReplied(QNetworkReply* reply);

    Today mToday;
    Day mDay[6];
    //星期和日期
    QList<QLabel*>mWeekList;
    QList<QLabel*>mDateList;
    //天气和天气图标
    QList<QLabel*>mTypeList;
    QList<QLabel*>mTypeIconList;
    
    //天气污染指数
    QList<QLabel*>mAqiList;
    
    //风力和风向
    QList<QLabel*>mFlList;
    QList<QLabel*>mFxList;

    QMap<QString,QString> mTypeMap;

     Audio *audio;

    QString init_city;
    void resizeEvent(QResizeEvent);
};
#endif  // MAINWINDOW_H
