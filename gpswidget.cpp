#include "gpswidget.h"
#include "ui_gpswidget.h"

GPSWidget::GPSWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GPSWidget)
{
    ui->setupUi(this);
    startInit();
    ui->statusBar->setText(tr("串口关闭"));
}

GPSWidget::~GPSWidget()
{
    delete ui;
}

void GPSWidget::on_StartGPSbtn_clicked()
{
    QString portName = ui->portNameComboBoxGPS->currentText();
    myGpsCom = new Win_QextSerialPort(portName, QextSerialBase::Polling);
    if (myGpsCom->open(QIODevice::ReadOnly)) {
        ui->statusBar->setText(tr("串口打开成功"));
    }
    else {
        ui->statusBar->setText(tr("串口打开失败"));
        return;
    }
    myGpsCom->setBaudRate((BaudRateType)ui->baudRateComboBoxGPS->currentIndex());
    myGpsCom->setDataBits((DataBitsType)ui->dataBitsComboBoxGPS->currentIndex());
    myGpsCom->setParity((ParityType)ui->parityComboBoxGPS->currentIndex());
    myGpsCom->setStopBits((StopBitsType)ui->stopBitsComboBoxGPS->currentIndex());
    myGpsCom->setFlowControl(FLOW_OFF);
    myGpsCom->setTimeout(TIME_OUT);
    setComboBoxEnabled(false);
    readTimer->start(TIMER_INTERVAL);
    ui->StartGPSbtn->setEnabled(false);
    ui->StopGPSbtn->setEnabled(true);
}

void GPSWidget::on_StopGPSbtn_clicked()
{
    myGpsCom->close();
    delete myGpsCom;
    readTimer->stop();
    ui->statusBar->setText(tr("串口关闭"));
    setComboBoxEnabled(true);
    ui->StartGPSbtn->setEnabled(true);
    ui->StopGPSbtn->setEnabled(false);
}

void GPSWidget::startInit()
{
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
    if (infos.isEmpty()) {
        ui->portNameComboBoxGPS->addItem(tr("无串口"));
    }
    foreach (QSerialPortInfo info, infos) {
        ui->portNameComboBoxGPS->addItem(info.portName());
    }
    ui->StartGPSbtn->setEnabled(true);
    ui->StopGPSbtn->setEnabled(false);
    timerdly = TIMER_INTERVAL;
    readTimer = new QTimer(this);
    connect(readTimer, SIGNAL(timeout()), this, SLOT(readGpsData()));
}

void GPSWidget::setComboBoxEnabled(bool status)
{
    ui->portNameComboBoxGPS->setEnabled(status);
    ui->baudRateComboBoxGPS->setEnabled(status);
    ui->dataBitsComboBoxGPS->setEnabled(status);
    ui->parityComboBoxGPS->setEnabled(status);
    ui->stopBitsComboBoxGPS->setEnabled(status);
}

void GPSWidget::GpsDisplay()
{
    QString alt_str;
    QString lon_str;
    QString u_date;
    QString u_time;
    ui->altitudedisplay->setText(alt_position(alt_str));
    ui->longtitudedisplay->setText(lon_position(lon_str));
    ui->speedstatus->setText(Gps_list[7]);
    ui->datedisplay->setText(UTCtime(u_date));
    ui->timedisplay->setText(UTCtime(u_time));
    if (Gps_list[2].contains("A")) {
        ui->statusdisplay->setText(tr("GPS 运行中"));
    }
    else {
        ui->statusdisplay->setText(tr("GPS 无信号"));
    }
}

QString& GPSWidget::UTCtime(QString &u_time)
{
    u_time.clear();
    QByteArray Utime = Gps_list[1];
    u_time = QString::number((Utime.mid(0, 2).toInt()) + 8) + ":" + Utime.mid(2, 2) + ":" + Utime.mid(4, 2);
    return u_time;
}

QString& GPSWidget::UTCdate(QString& u_date)
{
    u_date.clear();
    QByteArray Udate = Gps_list[9];
    u_date = "20"
            + Udate.mid(4, 2) + tr("年")
            + Udate.mid(2, 2) + tr("月")
            + Udate.mid(0, 2) + tr("日");
    return u_date;
}

QString& GPSWidget::alt_position(QString& alt_str)
{
    alt_str.clear();
    QByteArray altitude = Gps_list[3];
    float SecNum = altitude.mid(5, 4).toInt() * 60 / 10000;
    QString str = QString::number(SecNum);
    if (Gps_list[4] == "N") {
        alt_str = tr("北纬")
                + altitude.mid(0, 2) + tr("度")
                + altitude.mid(2, 2) + tr("分")
                + str.mid(0, 2) + tr("秒");
    }
    else {
        alt_str = tr("南纬")
                + altitude.mid(0, 2) + tr("度")
                + altitude.mid(2, 2) + tr("分")
                + str.mid(0, 2) + tr("秒");
    }
    return alt_str;
}

QString& GPSWidget::lon_position(QString& lon_str)
{
    lon_str.clear();
    QByteArray longitude = Gps_list[5];
    float SecNum = longitude.mid(6, 4).toInt() * 60 / 10000;
    QString str = QString::number(SecNum);
    if (Gps_list[6] == "W") {
        lon_str = tr("西经")
                + longitude.mid(0, 3) + tr("度")
                + longitude.mid(3, 2) + tr("分")
                + str.mid(0, 2) + tr("秒");
    }
    else {
        lon_str = tr("东经")
                + longitude.mid(0, 3) + tr("度")
                + longitude.mid(3, 2) + tr("分")
                + str.mid(0, 2) + tr("秒");
    }
    return lon_str;
}

void GPSWidget::readGpsData()
{
    QByteArray GPS_Data = myGpsCom->readAll();
    if(!GPS_Data.isEmpty()) {
        ui->textEditGPSData->append(GPS_Data);
        if (GPS_Data.contains("$GPRMC")) {
            GPS_Data.remove(0, GPS_Data.indexOf("$GPRMC"));
            if (GPS_Data.contains("*")) {
                GPS_RMC = GPS_Data.left(GPS_Data.indexOf("*"));
                Gps_list.clear();
                Gps_list<<GPS_RMC.split(',');
                GpsDisplay();
            }
        }
    }
}
