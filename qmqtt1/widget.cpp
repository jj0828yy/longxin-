#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>

static QString HostName="iot-06z00a47pokb3ml.mqtt.iothub.aliyuncs.com";
static quint16 Port = 1883;
static QString username = "tance&izz9L58Kijx" ;
static QString password = "c56ca33ab27ff37611eb6d6108b750c76e8a82d5a6edd9c5c10d65024b58dd27";
static QString ClientId = "izz9L58Kijx.tance|securemode=2,signmethod=hmacsha256,timestamp=1716702113378|";
static QString m_topic_publish= "/sys/izz9L58Kijx/tance/thing/event/property/post";
static QString m_topic_get= "/sys/izz9L58Kijx/tance/thing/event/property/post";
//const QString topic= "{"method":"thing.service.property.set","id":"1354728484","params":{"temp":12},"version":"1.0.0"}";
// /sys/izz9L58Kijx/tance/thing/event/property/post_reply
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    serialPort = new QSerialPort(this);
    mqtt_client=new QMqttClient;
    ConnectOrDisConnect();
   // Publish(m_topic_publish);
    //扫描本机的串口，并且添加到下拉框
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
    ui->comboBox->addItem(info.portName());
    }

    //读取数据
    connect(serialPort, SIGNAL(readyRead()), this, SLOT(readData()));
    //当timer的timeout()信号发出后，触发slotTimerUpdate()槽函数
    connect(timer,SIGNAL(timeout()),this,SLOT(slotTimerUpdate()));

}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked(bool checked)
{

    if (checked){

    //设置要打开的串口的名字
    serialPort->setPortName(ui->comboBox->currentText());
    //设置波特率
    serialPort->setBaudRate(ui->comboBox_2->currentText().toInt());
    //设置停止位
    serialPort->setStopBits(QSerialPort::StopBits(ui->comboBox_3->currentText().toInt()));
    //设置数据位
    serialPort->setDataBits(QSerialPort::DataBits(ui->comboBox_4->currentText().toInt()));
    //设置校验位
    switch (ui->comboBox_5->currentIndex()) {
    case 0:
        serialPort->setParity(QSerialPort::NoParity);
        break;
    case 1:
        serialPort->setParity(QSerialPort::EvenParity);
        break;
    case 2:
        serialPort->setParity(QSerialPort::OddParity);
        break;
    case 3:
        serialPort->setParity(QSerialPort::SpaceParity);
        break;
    case 4:
        serialPort->setParity(QSerialPort::MarkParity);
        break;
    default:
        break;

    }
    //设置流控
    serialPort->setFlowControl(QSerialPort::NoFlowControl);
    if(!serialPort->open(QIODevice::ReadWrite)){
        QMessageBox::about(this,"错误","串口打开失败可能被占用了");
        return;
    }

    ui->comboBox->setEnabled(false);
    ui->comboBox_2->setEnabled(false);
    ui->comboBox_3->setEnabled(false);
    ui->comboBox_4->setEnabled(false);
    ui->comboBox_5->setEnabled(false);
    ui->pushButton->setText("关闭串口");
    timer->stop();
    }else{
        //关闭串口
        serialPort->close();
        ui->comboBox->setEnabled(true);
        ui->comboBox_2->setEnabled(true);
        ui->comboBox_3->setEnabled(true);
        ui->comboBox_4->setEnabled(true);
        ui->comboBox_5->setEnabled(true);
        ui->pushButton->setText("打开串口");
        timer->start(4);
    }
}
//数据处理阶段
void Widget::readData()
{
    int bufferlens = 0;
    QByteArray bufferbegin = "#";
    int index=0;
    QByteArray bufferend = "*";
    int indexend=0;
    QByteArray buf;
    QByteArray receiverBuf;
    QString str;
    timer->start(4);
    buf.append(serialPort->readAll());

    timer->stop();
    ui->textBrowser_5->insertPlainText(buf);
    index = buf.indexOf(bufferbegin,index);
    qDebug() << index;
    indexend = buf.indexOf(bufferend,indexend);
     qDebug() << "jiewei" << indexend;
    if((index<buf.size())&&(indexend<buf.size()))
    {
        bufferlens = indexend - index + 1;
        receiverBuf = buf.mid(index,bufferlens);
    }
   /*char recvdata[receiverBuf.size()];
    memset(recvdata,0,sizeof(recvdata));
    memcpy(recvdata,receiverBuf.data(),bufferlens-1);
    recvdata[receiverBuf.size()-1]=35;
    if(recvdata[0]=='#'&&recvdata[receiverBuf.size()-1]=='*')
    {*/

        str.append(receiverBuf.mid(0,2));

        ui->textBrowser->setText(str);
        //  qDebug() << str;
    //}
    //const char* charArray = receiverBuf.data();
    //if(sizeof(charArray)>5)
   // {
    //{
        //温度的数据
       // if(receiverBuf.at(0)=='S'&&receiverBuf.at(1)=='T'&&receiverBuf.at(5)=='E')
       // {
          //  if(receiverBuf.at(2)=='1')
           // {
                //正温度值
              //  qDebug()<<"温度：+"<<receiverBuf.at(3)<<receiverBuf.at(4)<<"℃";
                // str="+";
                 //str.append(receiverBuf.at());
                // str.append(receiverBuf.at(1));
                 //str.append(charArray[0]);
                 //str.append(charArray[1]);
               ui->textBrowser_4->insertPlainText(receiverBuf);
               // str.append(receiverBuf.mid(0,2));
              //  qDebug() << str;
                //str.append(receiverBuf.at(2));
                //str.append('.');
               // str.append(receiverBuf.mid(5,8));
               // ui->textBrowser->setText(str);
                 //buf.clear();
                receiverBuf.clear();


            }
           // else if(receiverBuf.at(2)=='0')
          //  {
                //负温度值
               // qDebug()<<"温度：-"<<receiverBuf.at(3)<<receiverBuf.at(4)<<"℃";
            // QString
              //  str="-";
            /*    str.append(receiverBuf.at(5));
                str.append(receiverBuf.at(6));
                //str.append('.');
                //str.append(receiverBuf.at(8));
                ui->textBrowser_2->setText(str);
                str1.replace(0,sizeof(str),str);
                str.clear();

                str.append(receiverBuf.at(10));
                str.append(receiverBuf.at(11));
                ui->textBrowser_4->setText(str);
                str.clear();

           // }
         //   else
           // {
                //错误处理
           //     qDebug()<<"温度数据错误！";
           // }
            //清空缓存
            //str.clear();
     //   }
        //湿度的数据
      //  else if(receiverBuf.at(0)=='S'&&receiverBuf.at(1)=='H'&&receiverBuf.at(5)=='E')
       // {
          //  if(receiverBuf.at(2)=='1')
         //   {
                //正湿度值
           //     qDebug()<<"湿度："<<receiverBuf.at(3)<<receiverBuf.at(4)<<"%";

               // str.append(receiverBuf.at(3));
               // str.append(receiverBuf.at(4));
               // str.append('%');
               // memcpy(str1,str,sizeof (str));
              //   str1.replace(0,sizeof(str),str);
               // ui->textBrowser_2->setText(str);*/
           // }
           // else
          //  {
                //错误处理
              //  qDebug()<<"湿度数据错误";
         //   }
            //清空缓存
           // str.clear();
       // }
       // else
        //{
            //清空缓存
           // receiverBuf.clear();
        //}
  // }
//}

void Widget::slotTimerUpdate()
{

    QDateTime time = QDateTime::currentDateTime();//获取当前日期和时间
    QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");//格式为年-月-日 小时-分钟-秒 星期
    ui->textBrowser_3->setText(str);
}
//保存数据阶段
void Widget::on_pushButton_2_clicked()
{
    if(str1.size()<1)
    {
        QMessageBox::information(this, "提示","当前数据为空");
        return;
    }
    serialPort->clear();        //清空缓存区
    serialPort->close();        //关闭串口
    QString csvFile = QFileDialog::getExistingDirectory(this);      //获取文件保存路径
    if(csvFile.isEmpty())
       return;
    QDateTime current_date_time =QDateTime::currentDateTime();      //获取系统时间
    QString current_date =current_date_time.toString("MM_dd_hh_mm");    //获取时间字符串
    csvFile += tr("/%1.csv").arg(current_date);
    qDebug()<< csvFile;
    QFile file(csvFile);
    if ( file.exists())
    {
            //如果文件存在执行的操作，此处为空，因为文件不可能存在
    }
    file.open( QIODevice::ReadWrite | QIODevice::Text );    //以读写模式读取文件
    QTextStream out(&file);
    out<<tr("Time,")<<tr("sensor1,")<<tr("sensor2,")<<tr("sensor3,")<<tr("sensor4,")<<tr("sensor5,\n");     //写入表头
    // 创建 CSV 文件
    for (const auto &data : str1) {           //测试格式： *111,222,333,444,555#
        out << data ;        //顺序将缓冲区数据写入文件
    }
    file.close();
         //清空缓存区数据
    QMessageBox::information(this, "提示","数据保存成功");
}
//连接云平台
void Widget::ConnectOrDisConnect()
{
    connect(ui->pushButton_3,&QPushButton::clicked,this,[=](){
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
                ui->pushButton_3->setText("断开");
                ui->label_11->setText("Connect");
               Subcribe();

            }
        }
        else
        {
            mqtt_client->disconnectFromHost();
            ui->pushButton_3->setText("连接");
            ui->label_11->setText("DisConnect");

        }
    });

}

void Widget::Subcribe()
{
    if(mqtt_client->state()==QMqttClient::Connecting)
    {
        qDebug()<<"connect success";
        mqtt_client->subscribe(m_topic_get);
        connect(mqtt_client, SIGNAL(messageReceived(QByteArray,QMqttTopicName)), this, SLOT(receiveMess(QByteArray,QMqttTopicName)));
    }


}

/*void Widget::Publish(QString topic)
{
   // int a = str1.toInt();
    QJsonObject root;
  //  QJsonObject sys;
    QJsonObject params;
  //  QJsonObject Power;
  //  QJsonObject WF;

   // sys.insert("ack",0);
   // params.insert("Power",Power);
   // Power.insert("value","on");
   // Power.insert("time",1524448722000);
    params.insert("temp",str1.toInt());
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
    qDebug()<<byteArray;


    connect(ui->pushButton_4,&QPushButton::clicked,this,[=](){

            mqtt_client->publish(topic, byteArray);
           // QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not publish message"));
        });


}*/

void Widget::receiveMess(QByteArray message, QMqttTopicName name)
{
    Q_UNUSED(name);
    QString msg=message;
    qDebug()<<msg;
    ui->textBrowser_4->insertPlainText(msg);
}

void Widget::on_pushButton_4_clicked()
{
     QString topic = "/sys/izz9L58Kijx/tance/thing/event/property/post";
    // int a = str1.toInt();
     QJsonObject root;
   //  QJsonObject sys;
     QJsonObject params;
   //  QJsonObject Power;
   //  QJsonObject WF;

    // sys.insert("ack",0);
    // params.insert("Power",Power);
    // Power.insert("value","on");
    // Power.insert("time",1524448722000);
     params.insert("temp",str1.toInt());
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
     qDebug()<<byteArray;
     mqtt_client->publish(topic, byteArray);

}

