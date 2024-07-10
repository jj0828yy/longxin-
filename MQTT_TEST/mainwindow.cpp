#include "mainwindow.h"
#include "ui_mainwindow.h"

static QString HostName="iot-06z00a47pokb3ml.mqtt.iothub.aliyuncs.com";
static quint16 Port = 1883;
static QString username = "tance2&izz9L58Kijx" ;
static QString password = "d86f9809cbde2fe945060258e5b372f0f0e74fd22e37336fce3387eff6d965cb";
static QString ClientId = "izz9L58Kijx.tance2|securemode=2,signmethod=hmacsha256,timestamp=1720096812392|";
static QString m_topic_publish= "/sys/izz9L58Kijx/tance2/thing//service/property/set";
static QString m_topic_get= "/sys/izz9L58Kijx/tance2/thing/service/property/set";
static QString Hum= "Hum";
static QString temp= "temp";
static QString LightLux= "LightLux";
static QString SmokeSensor= "SmokeSensor";
static QString PowerSize= "PowerSize";
 //QString topic= "method":"thing.service.property.set","id":"1354728484","params":{"temp":12},"version":"1.0.0";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    t = 0;
    t1 = 0;
    mqtt_client=new QMqttClient;
    chart = new QChart();
    serices0 = new QLineSeries();
    serices1 = new QLineSeries();
    axisX = new QValueAxis();
    axisY = new QValueAxis();

    ConnectOrDisConnect();
    Publish(m_topic_publish);
    InitChart();

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::ConnectOrDisConnect()
{
    connect(ui->pushButton,&QPushButton::clicked,this,[=](){
        if(mqtt_client->state()==QMqttClient::Disconnected)
        {
            mqtt_client->setHostname(HostName);
            mqtt_client->setPort(Port);
            mqtt_client->setUsername(username);
            mqtt_client->setPassword(password);
            mqtt_client->setClientId(ClientId);
            mqtt_client->connectToHost();

            if(mqtt_client->state()==QMqttClient::Disconnected)
            {
                qDebug()<<mqtt_client->error();
            }
            else if(mqtt_client->state()==QMqttClient::Connecting)
            {
                ui->pushButton->setText("断开");
                ui->label->setText("Connect");
                Subcribe();

            }
        }
        else
        {
            mqtt_client->disconnectFromHost();
            ui->pushButton->setText("连接");
            ui->label->setText("DisConnect");

        }
    });

}

void MainWindow::Subcribe()
{
    if(mqtt_client->state()==QMqttClient::Connecting)
    {
        qDebug()<<"connect success";
        mqtt_client->subscribe(m_topic_get);
        connect(mqtt_client, SIGNAL(messageReceived(QByteArray,QMqttTopicName)), this, SLOT(receiveMess(const QByteArray, const QMqttTopicName)));


}
}
void MainWindow::InitChart()
{
    ui->widget->setChart(chart);
    QMargins mgs(5,5,5,5);
    chart->setMargins(mgs);
    chart->setTitle("数据曲线");

    //创建折线序列
    serices0->setName("温度曲线");
    serices1->setName("湿度曲线");
   // serices1->setColor(BLACK_PEN);
    chart->addSeries(serices0);
    chart->addSeries(serices1);
    QPen pen;
    pen.setColor(Qt::red);
    serices0->setPen(pen);


    //创建坐标轴
    axisX->setRange(0,5);
    axisX->setTitleText("time(secs)");

    axisY->setRange(0,100);
    axisY->setTitleText("value");

    chart->setAxisX(axisX,serices0);
    chart->setAxisY(axisY,serices0);

    chart->setAxisX(axisX,serices1);
    chart->setAxisY(axisY,serices1);

  // chart->addAxis((QAbstractAxis*)axisX,Qt::AlignBottom);
  // chart->addAxis((QAbstractAxis*)axisY,Qt::AlignLeft);
}

void MainWindow::receiveMess(const QByteArray &message, const QMqttTopicName &topic)
{
    QString content;
    QString msg;
    QString wendu;
    QString shidu;
    QString guangzhao;
    QString dianliang;
    QString yanwu;
    QString top;
    content = QDateTime::currentDateTime().toString();
     ui->textBrowser->setText(content);
    top = topic.name();
     ui->textBrowser_7->setText(top);
    msg = message;
    if(msg!=' ')
    {
      QString bwendu = "temp";
      QString bshidu = "Hum";
      QString bguangzhao = "LightLux";
      QString bdianliang = "PowerSize";
      QString byanwu = "SmokeSensor";
      QString end = "value";
      int swendu = msg.indexOf(bwendu,0);
      int sshidu = msg.indexOf(bshidu,0);
      int sguangzhao = msg.indexOf(bguangzhao,0);
      int sdianliang = msg.indexOf(bdianliang,0);
     // int syanwu = msg.indexOf(byanwu,0);
      int indexend = msg.indexOf(end,0);

      if((swendu>0)&&(indexend>0))
      {
          wendu = msg.mid(indexend+7,2);
          ui->textBrowser_2->setText(wendu);
      }
      else if((sshidu>0)&&(indexend>0))
      {
          shidu = msg.mid(indexend+7,2);
          ui->textBrowser_3->setText(shidu);
      }

      else if((sguangzhao>0)&&(indexend>0))
      {
          guangzhao = msg.mid(indexend+7,3);
          ui->textBrowser_5->setText(guangzhao);
      }

      else if((sdianliang>0)&&(indexend>0))
      {
          dianliang = msg.mid(indexend+7,2);
          ui->textBrowser_6->setText(dianliang);
      }

      else //((syanwu>0)&&(indexend>0))
      {
          yanwu = msg.mid(indexend+7,2);
          ui->textBrowser_4->setText(yanwu);
      }

      qreal a,b;

      qreal value = wendu.toInt();
      qreal value1 = shidu.toInt();
      if(value!=0)
      {
          a=value;
      serices0->append(t,a);
      if(t>50)
          axisX->setRange(t-50,t);
      if(listvalue.size()<=10)
         {
           listvalue.push_front(a);

         }
      else
      {
          listvalue.pop_back();
          listvalue.push_front(a);
      }
    //  qreal minvalue = *std::min_element(listvalue.begin(),listvalue.end());
    //  qreal maxvalue = *std::max_element(listvalue.begin(),listvalue.end());
    //  axisY->setRange(minvalue-1,maxvalue+1);
      t += 0.1;
      }
      else if(value1!=0)
      {

          b=value1;

      serices1->append(t1,b);
      if(t1>50)
          axisX->setRange(t1-50,t1);
      if(listvalue1.size()<=10)
         {

           listvalue1.push_front(b);
         }
      else
      {

          listvalue1.pop_back();
          listvalue1.push_front(b);
      }
    //  qreal minvalue = *std::min_element(listvalue.begin(),listvalue.end());
    //  qreal maxvalue = *std::max_element(listvalue.begin(),listvalue.end());
    //  axisY->setRange(minvalue-1,maxvalue+1);
      t1 += 0.1;
      }

    }
     qDebug() << content;
}

void MainWindow::Publish(QString topic)
{
    QJsonObject root;
  //  QJsonObject sys;
    QJsonObject params;
  //  QJsonObject Power;
  //  QJsonObject WF;

   // sys.insert("ack",0);
   // params.insert("Power",Power);
   // Power.insert("value","on");
   // Power.insert("time",1524448722000);
    params.insert("Hum",66);
   // WF.insert("value","23.6");

    root.insert("id","1354728484");
    root.insert("version","1.0.0");
    //root.insert("sys",sys);
    root.insert("params",params);
    root.insert("methon","thing.service.property.set");

    QJsonDocument document;
    document.setObject(root);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);

    connect(ui->pushButton2,&QPushButton::clicked,this,[=](){
         qDebug()<<byteArray;
            mqtt_client->publish(topic, byteArray);
           // QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not publish message"));
        });

}




