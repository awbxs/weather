#include "audio.h"

//#include <audio.h>


Audio::Audio(QObject *parent) : QObject(parent)
{


}

void Audio::startAudio(QString filename)
{
    QAudioDeviceInfo dev = QAudioDeviceInfo::defaultInputDevice();
    if(dev.isNull())
    {
        QMessageBox::warning(NULL,"QAudioDeviceInfo","录音设备不存在");

    }
    else
    {
        QAudioFormat m_format;
        m_format.setSampleRate(16000);
        //设置通道
        m_format.setChannelCount(1);

        //设置位深
        m_format.setSampleSize(16);

        //设置编码
        m_format.setCodec("audio/wav");

        //判断设备是否支持这个格式
        if(!dev.isFormatSupported(m_format))
        {
            m_format =  dev.nearestFormat(m_format);
        }

        //打开文件
        m_file = new QFile;
        m_file->setFileName(filename);
        m_file->open(QIODevice::WriteOnly);

        //创建录音对象
        m_audio = new QAudioInput(m_format,this);
        m_audio->start(m_file);


    }
    QAudioFormat m_format;


}

void Audio::stopAudio()
{
    //停止录音
    m_audio->stop();
    //关闭文件
    m_file->close();
    //删除对象
    delete m_file;
    m_file = NULL;

}
