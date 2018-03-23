#ifndef GPSWIDGET_H
#define GPSWIDGET_H

#include <QWidget>
#include "win_qextserialport.h"
#include <QMessageBox>
#include <QTimer>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QtSerialPort/QSerialPortInfo>
#define TIME_OUT 10
#define TIMER_INTERVAL 200
#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

namespace Ui {
class GPSWidget;
}

class GPSWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GPSWidget(QWidget *parent = 0);
    ~GPSWidget();

private slots:
    void on_StartGPSbtn_clicked();

    void on_StopGPSbtn_clicked();

    void readGpsData();

private:
    Ui::GPSWidget *ui;
    void startInit();
    void setComboBoxEnabled(bool status);
    void GpsDisplay();
    QString& UTCtime(QString& u_time);
    QString& UTCdate(QString& u_date);
    QString& alt_position(QString& alt_str);
    QString& lon_position(QString& lon_str);
    int timerdly;
    Win_QextSerialPort *myGpsCom;
    QByteArray GPS_RMC;
    QList<QByteArray> Gps_list;
    QTimer *readTimer;
};

#endif // GPSWIDGET_H
