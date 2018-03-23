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
    void on_StartGPSbtn_clicked();              // Open the port

    void on_StopGPSbtn_clicked();               // Close the port

    void readGpsData();                         // Parse GPS data

private:
    Ui::GPSWidget *ui;
    void startInit();                           // Read available port and set timer to listen
    void setComboBoxEnabled(bool status);       // Set/reset port parameters
    void GpsDisplay();                          // Display GPS data
    QString& UTCtime(QString& u_time);          // Process time data
    QString& UTCdate(QString& u_date);          // Process date data
    QString& alt_position(QString& alt_str);    // Process alititude data
    QString& lon_position(QString& lon_str);    // Process longitude data
    int timerdly;
    Win_QextSerialPort *myGpsCom;
    QByteArray GPS_RMC;
    QList<QByteArray> Gps_list;
    QTimer *readTimer;
};

#endif // GPSWIDGET_H
