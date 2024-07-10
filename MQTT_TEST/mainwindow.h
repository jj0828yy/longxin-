#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "QDebug"
#include "QLabel"
#include <QtCharts>
#include "QFile"
#include "QtMqtt/qmqttclient.h"
#include "QJsonObject"
#include "QJsonDocument"
#include "QJsonArray"
#include "QMessageBox"

using namespace QtCharts;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void init_mqtt();

    void sendTopic(QString data);

    void ConnectOrDisConnect();

    void Subcribe();

    void Publish(QString topic);

public slots:
    void receiveMess(const QByteArray &, const QMqttTopicName &);

private:
    QMqttClient *mqtt_client;
    Ui::MainWindow *ui;
private:
    qreal t,t1;
    QChart *chart;
    QValueAxis *axisX,*axisY;
    QLineSeries *serices0;
    QLineSeries *serices1;
    QList <qreal> listvalue;
    QList <qreal> listvalue1;
    void InitChart();

};
#endif // MAINWINDOW_H
