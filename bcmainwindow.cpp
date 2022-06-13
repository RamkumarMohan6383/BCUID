#include <QtCore/QCoreApplication>
#include "bcmainwindow.h"
#include "ui_bcmainwindow.h"
#include <iostream>
#include "printer.h"
#include <QtNetwork/QNetworkConfigurationManager>
#include <QNetworkSession>
#define steps 7
#define dir 0
#define en 3
#define hm_sen 4
#define LED 26
#define init 0
#define range 100
#define BARCODE_GND 21
#define ADC_BASE 100
#define SPI_CHAN 0
#define LED_BASE 200
#define dataPin 21
#define clockPin 22
#define latchPin 23

int filtwave[600];
static QString array[100]={""};
int read_status=0;
int read_wave=0;
int read_intensity=0;
int total_read_point=0;
int current_read_point=0;
double blank_val=0;
int dly=0;
int lagg=0;
int val=0;
double fct=0;
double multiFact=0;
int option=0;
double concen=0;
int w340=0,w405=0,w507=0,w545=0,w572=0,w628=0,w700=0;
double bc_y_val=0;
double absorbance=0;
int line=0;

BCMainWindow::BCMainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::BCMainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->Save_Btn_2->setDisabled(true);
    ui->tabWidget_2->setCurrentIndex(0);
    ui->tabWidget_4->setCurrentIndex(0);
    ui->tabWidget_5->setCurrentIndex(0);
    wiringPiSetup () ;
    mcp3004Setup (ADC_BASE, SPI_CHAN) ;
    sr595Setup (LED_BASE, 16, dataPin, clockPin, latchPin) ;
    pinMode (en, OUTPUT) ;
    pinMode (dir, OUTPUT) ;
    pinMode (steps, OUTPUT) ;
    pinMode (hm_sen, INPUT) ;
    pinMode (BARCODE_GND, OUTPUT) ;
    digitalWrite(en,HIGH);
    digitalWrite(BARCODE_GND,LOW);
    pinMode (LED, PWM_OUTPUT);
    pwmWrite (LED, 0);
    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("/home/pi/git/BCUIDF/bc.db");
    if(!mydb.open())
    {
        qDebug() << "Can't Connect to DB !";
    }
    else
    {
        qDebug() << "Connected Successfully to DB !";
    }
    setWindowFlags(Qt::FramelessWindowHint);

    //Swipe Gesture**************

    QScroller::grabGesture(ui->scrollArea, QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(ui->scrollArea_3, QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(ui->scrollArea_4, QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(ui->scrollArea_5, QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(ui->scrollArea_6, QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(ui->scrollArea_7, QScroller::LeftMouseButtonGesture);

    //Setting & Power Btn Hidden**************

    ui->frame->hide();
    ui->pushButton_23->hide();
    ui->pushButton_25->hide();

    //Date & Time edit********************

    hourPattern = QString("%1:%2");
    this->date = "";
    this->time = "";
    QDate date=QDate::currentDate();
    QString datetext= date.toString("dd / MM / yyyy");
    ui->SysDate_Lbl->setText(datetext);

    timer = new QTimer(this);
    connect(timer ,SIGNAL(timeout()) , this ,SLOT(ShowTime()));
    timer->setTimerType(Qt::PreciseTimer);
    timer->start(1000);

    readtimer = new QTimer(this);
    connect(readtimer ,SIGNAL(timeout()) , this ,SLOT(reading1()));
    readtimer->setTimerType(Qt::PreciseTimer);
    //readtimer->start(1000);

    timer1 = new QTimer(this);
    connect(timer1, SIGNAL(timeout()), this,SLOT(temp()));
    timer1->start(1000);

    //End-Point Plot********************

    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsLine);

    //Wifi Setup*************************

    QStringList list2;
    QProcess process1;
    process1.start("sh",QStringList()<<"-c"<<"sudo iwlist wlan0 scan | grep ESSID");//scan list of wifi networks
    process1.waitForFinished();
    QString data = process1.readAllStandardOutput();
    QString Error= process1.readAllStandardError();
    list2 = (QStringList()<<"------Select-------");//append to dropdownlist
    ui->comboBox_4->addItems(list2);
    QStringList list = data.split("\n");//split data
    for(int i=0;i<list.count()-1;i++)
    {
        QStringList list1 = list.at(i).split("ESSID:");
        QString data1 = list1.at(1);
        list2 = (QStringList()<<data1);
        ui->comboBox_4->addItems(list2);//adding wifi names to dropdownlist
    }
    ;

    QString name;
    //int fact = 0;
    QSqlQuery query;
    query.prepare("select * from test where name = '"+name+"'");
    query.exec();
    while(query.next())
    {
        //  fact = query.value(6).toInt();
    }

    /*for(int i=0;i<200;i++)
    {
        on_pushButton_8_clicked();
//        on_pushButton_9_clicked();
//        on_pushButton_24_clicked();
//        on_pushButton_26_clicked();
//        on_pushButton_128_clicked();
//        on_pushButton_129_clicked();
//        on_pushButton_130_clicked();
        QThread::msleep(400);
//        qDebug() <<i<<"  "<<w340<<"  "<<w405<<"  "<<w507<<"  "<<w545<<"  "<<w572<<"  "<<w628<<"  "<<w700;
        qDebug()<<w340;
    }*/

}

void BCMainWindow::on_hours_sliderMoved(int position)
{
    ui->time->setText(QString(this->hourPattern).arg(position, 2,10,QChar('0')).arg(ui->minutes->value(),2,10,QChar('0')));
    this->time = ui->time->text();
}

void BCMainWindow::on_minutes_sliderMoved(int position)
{
    ui->time->setText(QString(this->hourPattern).arg(ui->hours->value(), 2,10,QChar('0')).arg(position,2,10,QChar('0')));
    this->time = ui->time->text();
}

void BCMainWindow::ShowTime()
{
    QTime time = QTime::currentTime();
    QString time_text = time.toString("hh : mm : ss");
    ui->SysTime_Lbl->setText(time_text);
}

void BCMainWindow::on_calendarWidget_clicked(const QDate &date)
{
    this->date = date.toString("dd/MM/yyyy");
}

void BCMainWindow::on_pushButton_6_clicked()
{
    ui->calendarWidget->selectedDate().toString("dd/MM/yyyy");
    this->date = QString("%1").arg(this->date);
    this->time = QString("%2").arg(this->time);
    ui->SysDate_Lbl->setText(date);
    ui->SysTime_Lbl->setText(time);
    ui->stackedWidget->setCurrentIndex(14);
}

BCMainWindow::~BCMainWindow()
{
    delete ui;
}

void BCMainWindow::on_Home_Btn_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void BCMainWindow::on_Menu_Btn_2_clicked()
{
    if (ui->frame->isVisible())
        ui->frame->setVisible(false);
    else
        ui->frame->setVisible(true);
}

void BCMainWindow::on_Home_Btn_3_clicked()
{
    LED_OFF();
    ui->stackedWidget->setCurrentIndex(1);
}

void BCMainWindow::on_Home_Btn_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void BCMainWindow::on_Home_Btn_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void BCMainWindow::temp()
{
    Pi2c arduino(7); //Create a new object "arduino" using address "0x07"
    char receive[30]; //Create a buffer of char (single bytes) for the data
    //Receive from the Arduino and put the contents into the "receive" char array
    //timer1->stop();
    QThread::msleep(100);
    arduino.i2cRead(receive,30);
    QThread::msleep(100);
    QString str=receive;
    //Print out what the Arduino is sending...
    //qDebug() << "Arduino Says: " << str.mid(0,2);
    ui->ReadBlock_Lbl_2->setText(str.mid(0,4));
    ui->IncuBlock_Lbl_2->setText(str.mid(5,9));
}

void BCMainWindow::on_DataHis_Btn_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(11);
}

void BCMainWindow::on_pushButton_308_clicked()
{
    QPrinter printer;
    QPrintDialog dialog(&printer, this);
    dialog.setWindowTitle("Print Setup");
    if(ui->textEdit->textCursor().hasSelection())
        dialog.addEnabledOption(QAbstractPrintDialog::PrintSelection);
    if(dialog.exec() != QDialog::Accepted)
    {
        return;
    }
}

void BCMainWindow::on_Load_Btn_clicked()
{
    QSqlQueryModel * modal = new QSqlQueryModel();
    QSqlQuery* query=new QSqlQuery(mydb);
    query->prepare("select * from test");
    query->exec();
    modal->setQuery(*query);
    ui->tableView->setModel(modal);
    qDebug() << ( modal->rowCount());
}

void BCMainWindow::on_toolButton_13_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void BCMainWindow::on_toolButton_12_clicked()
{
    ui->stackedWidget->setCurrentIndex(9);
}

void BCMainWindow::on_Home_Btn_6_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void BCMainWindow::on_toolButton_14_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void BCMainWindow::on_Setting_Btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(12);
}

void BCMainWindow::on_Load_Btn_2_clicked()
{
    QSqlQueryModel * modal = new QSqlQueryModel();
    QSqlQuery* query=new QSqlQuery(mydb);
    query->prepare("select * from Reports");
    query->exec();
    modal->setQuery(*query);
    ui->tableView->setModel(modal);
    qDebug() << ( modal->rowCount());
}

void BCMainWindow::on_Home_Btn_7_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void BCMainWindow::on_Home_Btn_8_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
}

void BCMainWindow::on_Home_Btn_9_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void BCMainWindow::on_Home_Btn_10_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

int BCMainWindow::readadc( int pin)
{
    unsigned char buff[] = {static_cast<char>(0x6 | ((pin & 0x7) >> 2)),
                            static_cast<char>((pin & 0x7) << 6),
                            static_cast<char>(0)};

    wiringPiSPIDataRW(SPI_CHAN, buff, 3);
    return ((buff[1] & 0xf) << 8) | buff[2];
}

void BCMainWindow::updateTime()
{
}

void BCMainWindow::on_pushButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(14);
    QTcpSocket socket;
    socket.connectToHost("8.8.8.8", 53);
    if (socket.waitForConnected())
    {
        QString text = socket.localAddress().toString();
        ui->label_8->setText(text);
    }
    else
    {
        QMessageBox msg;
        msg.setText("Couldn't connect to the DNS server! No internet connection...");
        msg.setWindowTitle("No internet connection");
        msg.setIcon(QMessageBox::Critical);
        msg.exec();
    }
}

void BCMainWindow::on_toolButton_15_clicked()
{
    ui->stackedWidget->setCurrentIndex(12);
}

void BCMainWindow::on_toolButton_16_clicked()
{
    ui->stackedWidget->setCurrentIndex(12);
}

void BCMainWindow::on_toolButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(17);
}

void BCMainWindow::on_toolButton_2_clicked()
{
    QPrinter printer;
    QPrintDialog dialog(&printer, this);
    dialog.setWindowTitle("Print Setup");
    if(ui->textEdit->textCursor().hasSelection())
        dialog.addEnabledOption(QAbstractPrintDialog::PrintSelection);
    if(dialog.exec() != QDialog::Accepted)
    {
        return;
    }
}

void BCMainWindow::on_toolButton_17_clicked()
{
    ui->stackedWidget->setCurrentIndex(12);
}

void BCMainWindow::on_pushButton_10_clicked()
{
    ui->stackedWidget->setCurrentIndex(15);
    ui->SysDate_Lbl->setText(date);
}

void BCMainWindow::on_pushButton_5_clicked()
{
    QString filter = tr("Database Backup (*.db)");
    QString fileName = QFileDialog::getSaveFileName(this,tr("Backup Database"),QDir::homePath(),filter);
    if(!fileName.isEmpty()){
        QString filePath = QDir::homePath();
        filePath.append("/home/pi/git/BCUID/bc.db");
        QFile sourceFile(filePath);
        if(!sourceFile.open(QIODevice::ReadOnly))
        {
            return;
        }
        QFile destFile("bc.db");
        if(!destFile.open(QIODevice::WriteOnly))
        {
            return;
        }
        QTextStream sourceStream(&sourceFile);
        QTextStream destStream(&destFile);
        QByteArray data;
        sourceStream>>data;
        data = qCompress(data);
        destStream<<data;
        sourceFile.close();
        destFile.close();
    }
}

void BCMainWindow::on_Power_Btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(19);
}

void BCMainWindow::on_pushButton_309_clicked()
{
    ui->stackedWidget->setCurrentIndex(9);
}

void BCMainWindow::on_toolButton_20_clicked()
{
    ui->stackedWidget->setCurrentIndex(12);
}

void BCMainWindow::on_pushButton_7_clicked()
{
    ui->stackedWidget->setCurrentIndex(16);
    int intensity=0  , wavelength=0,ABS = 0 , ABSO = 0;
    QSqlQuery query;
    query.prepare("select * from tests where sno = 1");
    query.exec();
    while(query.next())
    {
        intensity=query.value(1).toInt();
        wavelength=query.value(8).toInt();
        ABS=query.value(9).toInt();
        ABSO=query.value(10).toInt();

    }
    QString ity1=QString::number(intensity);
    QString ABS1=QString::number(ABS);
    QString ABSO1=QString::number(ABSO);
    ui->lineEdit_72->setText(ity1);
    ui->lineEdit_83->setText(ABS1);
    ui->lineEdit_139->setText(ABSO1);

    query.prepare("select * from tests where sno = 2");
    query.exec();
    while(query.next())
    {
        intensity=query.value(1).toInt();
        wavelength=query.value(8).toInt();
        ABS=query.value(9).toInt();
        ABSO=query.value(10).toInt();

    }
    QString ity2=QString::number(intensity);
    QString ABS2=QString::number(ABS);
    QString ABSO2=QString::number(ABSO);
    ui->lineEdit_73->setText(ity2);
    ui->lineEdit_84->setText(ABS2);
    ui->lineEdit_137->setText(ABSO2);

    query.prepare("select * from tests where sno = 3");
    query.exec();
    while(query.next())
    {
        intensity=query.value(1).toInt();
        wavelength=query.value(8).toInt();
        ABS=query.value(9).toInt();
        ABSO=query.value(10).toInt();

    }
    QString ity3=QString::number(intensity);
    QString ABS3=QString::number(ABS);
    QString ABSO3=QString::number(ABSO);
    ui->lineEdit_74->setText(ity3);
    ui->lineEdit_80->setText(ABS3);
    ui->lineEdit_86->setText(ABSO3);

    query.prepare("select * from tests where sno = 4");
    query.exec();
    while(query.next())
    {
        intensity=query.value(1).toInt();
        wavelength=query.value(8).toInt();
        ABS=query.value(9).toInt();
        ABSO=query.value(10).toInt();

    }
    QString ity4=QString::number(intensity);
    QString ABS4=QString::number(ABS);
    QString ABSO4=QString::number(ABSO);
    ui->lineEdit_75->setText(ity4);
    ui->lineEdit_82->setText(ABS4);
    ui->lineEdit_87->setText(ABSO4);

    query.prepare("select * from tests where sno = 5");
    query.exec();
    while(query.next())
    {
        intensity=query.value(1).toInt();
        wavelength=query.value(8).toInt();
        ABS=query.value(9).toInt();
        ABSO=query.value(10).toInt();

    }
    QString ity5=QString::number(intensity);
    QString ABS5=QString::number(ABS);
    QString ABSO5=QString::number(ABSO);
    ui->lineEdit_76->setText(ity5);
    ui->lineEdit_79->setText(ABS5);
    ui->lineEdit_140->setText(ABSO5);

    query.prepare("select * from tests where sno = 6");
    query.exec();
    while(query.next())
    {
        intensity=query.value(1).toInt();
        wavelength=query.value(8).toInt();
        ABS=query.value(9).toInt();
        ABSO=query.value(10).toInt();

    }
    QString ity6=QString::number(intensity);
    QString ABS6=QString::number(ABS);
    QString ABSO6=QString::number(ABSO);
    ui->lineEdit_77->setText(ity6);
    ui->lineEdit_81->setText(ABS6);
    ui->lineEdit_136->setText(ABSO6);

    query.prepare("select * from tests where sno = 7");
    query.exec();
    while(query.next())
    {
        intensity=query.value(1).toInt();
        wavelength=query.value(8).toInt();
        ABS=query.value(9).toInt();
        ABSO=query.value(10).toInt();

    }
    QString ity7=QString::number(intensity);
    QString ABS7=QString::number(ABS);
    QString ABSO7=QString::number(ABSO);
    ui->lineEdit_78->setText(ity7);
    ui->lineEdit_85->setText(ABS7);
    ui->lineEdit_138->setText(ABSO7);
}

void BCMainWindow::on_pushButton_11_clicked()
{   
    ui->stackedWidget->setCurrentIndex(20);
    //ui->webView->load(QUrl("https://github.com/RamkumarMohan6383/BCUID/" ));
}

void BCMainWindow::on_toolButton_18_clicked()
{
    ui->stackedWidget->setCurrentIndex(12);
}

void BCMainWindow::download(const QNetworkRequest &request)
{
    qDebug()<<"Download Requested: "<<request.url();
}

void BCMainWindow::unsupportedContent(QNetworkReply * reply)
{
    qDebug()<<"Unsupported Content: "<<reply->url();
}

void BCMainWindow::displayResult(QNetworkReply *reply)
{
    QByteArray buffer = reply->readAll();
    qDebug() << "received reply";
    qDebug() << buffer;
    reply->deleteLater();
}

void BCMainWindow::on_Power_Btn_2_clicked()
{
    qApp->exit();
}

void BCMainWindow::on_pushButton_8_clicked()
{
    QSqlQuery query;
    int intensity = 0 , wavelength = 0 ;
    query.prepare("select * from tests where sno=1");
    query.exec();
    while(query.next())
    {
        intensity=query.value(1).toInt();
    }
    //qDebug()<<intensity;
    filtwave[0]=reading(wavelength,intensity);
    ui->label_114->setNum(filtwave[0]);
    //qDebug()<<filtwave[0];
    w340=filtwave[0];
    QString Blankval;
    Blankval = ui->label_114->text();
    query.prepare("update tests set blankval='"+Blankval+"' where sno=1");
    if(query.exec())
    {
        //qDebug()<<"Update Done";
    }
    else
    {
        qDebug()<<"Error";
    }
}

void BCMainWindow::on_pushButton_9_clicked()
{
    QSqlQuery query;
    int intensity = 0 , wavelength = 1 ;
    query.prepare("select * from tests where sno=2");
    query.exec();
    while(query.next())
    {
        intensity=query.value(1).toInt();
    }
    filtwave[0]=reading(wavelength,intensity);
    ui->label_115->setNum(filtwave[0]);
    w405=filtwave[0];
    QString Blankval;
    Blankval = ui->label_115->text();
    query.prepare("update tests set blankval='"+Blankval+"' where sno=2");
    if(query.exec())
    {
        //qDebug()<<"Update Done";
    }
    else
    {
        qDebug()<<"Error";
    }
}

void BCMainWindow::on_pushButton_24_clicked()
{
    QSqlQuery query;
    int intensity = 0 , wavelength = 2 ;
    query.prepare("select * from tests where sno=3");
    query.exec();
    while(query.next())
    {
        intensity=query.value(1).toInt();
    }
    filtwave[0]=reading(wavelength,intensity);
    ui->label_117->setNum(filtwave[0]);
    w507=filtwave[0];
    QString Blankval;
    Blankval = ui->label_117->text();
    query.prepare("update tests set blankval='"+Blankval+"' where sno=3");
    if(query.exec())
    {
        //qDebug()<<"Update Done";
    }
    else
    {
        qDebug()<<"Error";
    }
}

void BCMainWindow::on_pushButton_26_clicked()
{
    QSqlQuery query;
    int intensity = 0 , wavelength = 3 ;
    query.prepare("select * from tests where sno=4");
    query.exec();
    while(query.next())
    {
        intensity=query.value(1).toInt();
    }
    filtwave[0]=reading(wavelength,intensity);
    ui->label_118->setNum(filtwave[0]);
    w545=filtwave[0];
    QString Blankval;
    Blankval = ui->label_118->text();
    query.prepare("update tests set blankval='"+Blankval+"' where sno=4");
    if(query.exec())
    {
        //qDebug()<<"Update Done";
    }
    else
    {
        qDebug()<<"Error";
    }
}

void BCMainWindow::on_pushButton_128_clicked()
{
    QSqlQuery query;
    int intensity = 0 , wavelength = 4 ;
    query.prepare("select * from tests where sno=5");
    query.exec();
    while(query.next())
    {
        intensity=query.value(1).toInt();
    }
    filtwave[0]=reading(wavelength,intensity);
    ui->label_124->setNum(filtwave[0]);
    w572=filtwave[0];
    QString Blankval;
    Blankval = ui->label_124->text();
    query.prepare("update tests set blankval='"+Blankval+"' where sno=5");
    if(query.exec())
    {
        //qDebug()<<"Update Done";
    }
    else
    {
        qDebug()<<"Error";
    }
}

void BCMainWindow::on_pushButton_129_clicked()
{
    QSqlQuery query;
    int intensity = 0 , wavelength = 5 ;
    query.prepare("select * from tests where sno=6");
    query.exec();
    while(query.next())
    {
        intensity=query.value(1).toInt();
    }
    filtwave[0]=reading(wavelength,intensity);
    ui->label_126->setNum(filtwave[0]);
    w628=filtwave[0];
    QString Blankval;
    Blankval = ui->label_126->text();
    query.prepare("update tests set blankval='"+Blankval+"' where sno=6");
    if(query.exec())
    {
        //qDebug()<<"Update Done";
    }
    else
    {
        qDebug()<<"Error";
    }
}

void BCMainWindow::on_pushButton_130_clicked()
{
    QSqlQuery query;
    int intensity = 0 , wavelength = 6 ;
    query.prepare("select * from tests where sno=7");
    query.exec();
    while(query.next())
    {
        intensity=query.value(1).toInt();
    }
    filtwave[0]=reading(wavelength,intensity);
    ui->label_183->setNum(filtwave[0]);
    w700=filtwave[0];
    QString Blankval;
    Blankval = ui->label_183->text();
    query.prepare("update tests set blankval='"+Blankval+"' where sno=7");
    if(query.exec())
    {
        //qDebug()<<"Update Done";
    }
    else
    {
        qDebug()<<"Error";
    }
}


void BCMainWindow::on_Save_Btn_2_clicked()
{
    ui->Save_Btn_2->setDisabled(true);

    QString Date , Time ; //PID, , ANrmlRang , BNrmlRang , ODVal , ResultOD ,  ;
    int PID=0  ,ANrmlRang=0 , BNrmlRang=0  , date = 0 ,time=0;
    double  ODVal=0 , ResultOD =0;
    Date = char (date);
    Time = char (time);
    QString TestName = ui->TestName_Lbl->text();
    PID = ui->PatientInfo_LineEdit->text().toInt();
    ODVal = ui->label_6->text().toDouble();
    ResultOD = ui->label_33->text().toDouble();
    Date= ui->SysDate_Lbl->text();
    Time =ui->SysTime_Lbl->text();
    QSqlQuery query;
    query.prepare("select * from test where name ='"+TestName+"'");
    query.exec();
    while(query.next())
    {
        ANrmlRang = query.value(14).toInt();
        BNrmlRang = query.value(13).toInt();
    }
    query.prepare("insert into Reports(PID , TName , ANrmlRang , BNrmlRang , ODVal , ResultOD, Date ,Time) values(?,?,?,?,?,?,?, ?)");
    query.addBindValue(PID);
    query.addBindValue(TestName);
    query.addBindValue(ANrmlRang);
    query.addBindValue(BNrmlRang);
    query.addBindValue(ODVal);
    query.addBindValue(ResultOD);
    query.addBindValue(Date);
    query.addBindValue(Time);
    query.exec();
}

void BCMainWindow::on_Save_Btn_3_clicked()
{
    ui->Save_Btn_3->setDisabled(true);
    QString  Date , Time ; //PID, , ANrmlRang , BNrmlRang , ODVal , ResultOD ,  ;
    int PID=0  ,ANrmlRang=0 , BNrmlRang=0  , date = 0 ,time=0;
    double  ODVal=0 , ResultOD =0;
    Date = char (date);
    Time = char (time);
    QString TestName = ui->TestName_Lbl_5->text();
    PID = ui->PatientInfo_LineEdit_2->text().toInt();
    ODVal = ui->label_119->text().toDouble();
    Date= ui->SysDate_Lbl->text();
    Time =ui->SysTime_Lbl->text();
    QSqlQuery query;
    query.prepare("select * from test where name ='"+TestName+"'");
    query.exec();
    while(query.next())
    {
        ANrmlRang = query.value(14).toInt();
        BNrmlRang = query.value(13).toInt();
    }
    query.prepare("insert into Reports(PID , TName , ANrmlRang , BNrmlRang , ODVal , ResultOD, Date ,Time) values(?,?,?,?,?,?,?, ?)");
    query.addBindValue(PID);
    query.addBindValue(TestName);
    query.addBindValue(ANrmlRang);
    query.addBindValue(BNrmlRang);
    query.addBindValue(ODVal);
    query.addBindValue(ResultOD);
    query.addBindValue(Date);
    query.addBindValue(Time);
    query.exec();
}

void BCMainWindow::on_Save_Btn_4_clicked()
{

    ui->Save_Btn_4->setDisabled(true);
    QString  Date , Time ; //PID, , ANrmlRang , BNrmlRang , ODVal , ResultOD ,  ;
    int PID=0  ,ANrmlRang=0 , BNrmlRang=0  , date = 0 ,time=0;
    double  ODVal=0 , ResultOD =0;
    Date = char (date);
    Time = char (time);
    QString TestName = ui->TestName_Lbl_6->text();
    PID = ui->PatientInfo_LineEdit_3->text().toInt();
    ODVal = ui->label_132->text().toDouble();
    Date= ui->SysDate_Lbl->text();
    Time =ui->SysTime_Lbl->text();
    QSqlQuery query;
    query.prepare("select * from test where name ='"+TestName+"'");
    query.exec();
    while(query.next())
    {
        ANrmlRang = query.value(14).toInt();
        BNrmlRang = query.value(13).toInt();
    }
    query.prepare("insert into Reports(PID , TName , ANrmlRang , BNrmlRang , ODVal , ResultOD, Date ,Time) values(?,?,?,?,?,?,?, ?)");
    query.addBindValue(PID);
    query.addBindValue(TestName);
    query.addBindValue(ANrmlRang);
    query.addBindValue(BNrmlRang);
    query.addBindValue(ODVal);
    query.addBindValue(ResultOD);
    query.addBindValue(Date);
    query.addBindValue(Time);
    query.exec();
}

void BCMainWindow::on_toolButton_3_clicked()
{
    QSqlQuery query;
    QString mulfact;
    double fact = 0.0;
    mulfact = char (fact);
    mulfact = ui->lineEdit_147->text();
    query.prepare("update tests set mulfact='"+mulfact+"' where sno=1");
    if(query.exec())
    {
        qDebug()<<"Update Done";
    }
    else
    {
        qDebug()<<"Error";
    }
}
void BCMainWindow::on_pushButton_131_clicked()
{
    ui->stackedWidget->setCurrentIndex(18);
    QString mulfact;
    QSqlQuery query;
    query.prepare("select * from tests where sno =1");
    query.exec();
    while(query.next())
    {
        mulfact=query.value(12).toString();
    }
    qDebug()<<mulfact;

    ui->lineEdit_147->setText(mulfact);
}

int BCMainWindow::reading(int wave, int intensity)
{
    int blank[300];
    filtwave[0]=0;
    pwmWrite (LED, intensity);
    digitalWrite (LED_BASE + wave,HIGH) ;
    QThread::msleep(200);
    for(int i=0;i<300;i++)
    {
        blank[i]=readadc(6-wave);
        //qDebug()<<blank[i];
        QThread::msleep(1);
        if(i>=250)
            filtwave[0]+=blank[i];
    }
    QThread::msleep(10);
    filtwave[0]=filtwave[0]/50;
    digitalWrite (LED_BASE + wave,LOW) ;
    pwmWrite (LED, 0);
    return filtwave[0];
}

void BCMainWindow::reading1()
{
    int blank[300];
    filtwave[0]=0;
    pwmWrite (LED, read_intensity);
    digitalWrite (LED_BASE + read_wave,HIGH) ;
    QThread::msleep(200);
    for(int i=0;i<300;i++)
    {
        blank[i]=readadc(6-read_wave);
        QThread::msleep(1);
        if(i>=250)
            filtwave[0]+=blank[i];
    }
    QThread::msleep(10);
    //digitalWrite (LED_BASE + read_wave,LOW) ;
    filtwave[0]=filtwave[0]/50;
    pwmWrite (LED, 0);
    read_status=1;
    current_read_point=current_read_point+10;
    update_func();
    qDebug()<<QTime::currentTime()<<" "<<filtwave[0]<<" "<<read_wave<<" "<<read_intensity;
    ui->label_12->setText(QDateTime::fromTime_t(current_read_point).toUTC().toString("hh:mm:ss"));
}

void BCMainWindow::update_func()
{
    if(option==1)                           //Two-Point Calibrate
    {
        double transmission=0;
        double layer=0;
        absorbance=0;
        layer = absorbance;
        ui->label_32->setNum(current_read_point);
        transmission=blank_val/filtwave[0];
        absorbance=log10(transmission);
        addPoint(current_read_point,absorbance);
        qDebug()<<current_read_point;
        if(current_read_point==dly)
        {
            ui->label_120->setText(QString::number(absorbance, 'f', 3));
            ui->label_120->setVisible(true);
        }

        else if(current_read_point==total_read_point)
        {
            ui->label_122->setText(QString::number(absorbance, 'f', 3));
            ui->label_122->setVisible(true);
        }
        qDebug()<<current_read_point;
        qDebug()<<total_read_point;
        if(total_read_point==current_read_point)
        {
            readtimer->stop();
            calc();
            ui->RunCal_Btn_2->setEnabled(true);
        }
    }
    else if(option==2)                          //Two-Point Sample
    {
        double transmission=0 ;
        absorbance=0;
        ui->label_32->setNum(current_read_point);
        transmission=blank_val/filtwave[0];
        absorbance=log10(transmission);
        addPoint(current_read_point,absorbance);
        if(current_read_point==dly)
            ui->label_120->setText(QString::number(absorbance, 'f', 3));
        else if(current_read_point==total_read_point)
            ui->label_122->setText(QString::number(absorbance, 'f', 3));
        if(total_read_point==current_read_point)
        {
            readtimer->stop();
            calc();
            ui->RunSample_Btn_2->setEnabled(true);
        }
    }
    else if(option==3)                              //Kinetic Calibrate
    {
        double transmission=0;
        absorbance=0;
        ui->label_32->setNum(current_read_point);
        transmission=blank_val/filtwave[0];
        absorbance=log10(transmission);
        addPoint(current_read_point,absorbance);
        if(current_read_point==dly)
        {
            if(val==2)
            {
                ui->label_194->setText(QString::number((absorbance), 'f', 3));
                ui->label_194->setVisible(true);
                ui->label_106->setVisible(false);
            }
            else if (val==3)
            {
                ui->label_106->setText(QString::number((absorbance), 'f', 3));
                ui->label_106->setVisible(true);
                ui->label_194->setVisible(false);
            }

        }
        else if(current_read_point==dly+lagg)
        {
            if(val==2)
            {
                ui->label_195->setText(QString::number((absorbance), 'f', 3));
                ui->label_195->setVisible(true);
                ui->label_121->setVisible(false);
            }
            else if (val==3)
            {
                ui->label_121->setText(QString::number((absorbance), 'f', 3));
                ui->label_121->setVisible(true);
                ui->label_195->setVisible(false);
            }
        }
        else if(current_read_point==dly+(lagg*2))
        {
            if(val==2)
            {
                ui->label_198->setText(QString::number((absorbance), 'f', 3));
                ui->label_198->setVisible(true);
                ui->label_123->setVisible(false);
            }
            else if (val==3)
            {
                ui->label_123->setText(QString::number((absorbance), 'f', 3));
                ui->label_123->setVisible(true);
                ui->label_198->setVisible(false);
            }
        }
        else if(current_read_point==dly+(lagg*3))
        {
            ui->label_125->setText(QString::number((absorbance), 'f', 3));
            ui->label_125->setVisible(true);
        }
        if(total_read_point==current_read_point)
        {
            readtimer->stop();
            calc();
            ui->RunCal_Btn_3->setEnabled(true);
        }
    }
    else if(option==4)                              //Kinetic Sample
    {
        ui->label_203->setVisible(true);
        ui->label_203->setText("ΔA");
        double transmission=0;
        absorbance=0;
        ui->label_32->setNum(current_read_point);
        transmission=blank_val/filtwave[0];
        absorbance=log10(transmission);
        addPoint(current_read_point,absorbance);
        if(current_read_point==dly)
        {
            if(val==2)
            {
                ui->label_194->setText(QString::number((absorbance), 'f', 3));
                ui->label_194->setVisible(true);
                ui->label_106->setVisible(false);
            }
            else if (val==3)
            {
                ui->label_106->setText(QString::number((absorbance), 'f', 3));
                ui->label_106->setVisible(true);
                ui->label_194->setVisible(false);
            }

        }
        else if(current_read_point==dly+lagg)
        {
            if(val==2)
            {
                ui->label_195->setText(QString::number((absorbance), 'f', 3));
                ui->label_195->setVisible(true);
                ui->label_121->setVisible(false);
            }
            else if (val==3)
            {
                ui->label_121->setText(QString::number((absorbance), 'f', 3));
                ui->label_121->setVisible(true);
                ui->label_195->setVisible(false);
            }
        }
        else if(current_read_point==dly+(lagg*2))
        {
            if(val==2)
            {
                ui->label_198->setText(QString::number((absorbance), 'f', 3));
                ui->label_198->setVisible(true);
                ui->label_123->setVisible(false);
            }
            else if (val==3)
            {
                ui->label_123->setText(QString::number((absorbance), 'f', 3));
                ui->label_123->setVisible(true);
                ui->label_198->setVisible(false);
            }
        }
        else if(current_read_point==dly+(lagg*3))
        {
            ui->label_125->setText(QString::number((absorbance), 'f', 3));
            ui->label_125->setVisible(true);
        }
        if(total_read_point==current_read_point)
        {
            readtimer->stop();
            calc();
            ui->RunSample_Btn_3->setEnabled(true);
        }
    }
}

void BCMainWindow::calc()
{
    if(option==1)              //Two-Point Calibrate calc
    {
        double od1 , od2, dod , dod1 ,dod5,fact;
        od1=ui->label_120->text().toDouble(); //1st Read
        od2=ui->label_122->text().toDouble(); //2nd Read
        dod=od2-od1;
        dod1=abs(dod); //Cal value 1
        ui->label_119->setText(QString::number(dod1, 'f', 3));
        ui->label_34->setText(QString::number(dod1, 'f', 3));   //calibrate OD
        dod5=ui->label_34->text().toDouble();
        fact = concen / dod5; // Factor OD
        if(fact<100)
        {
            ui->label_196->setText(QString::number(fact, 'f' , 2)); // Calibrate Factor Value
            ui->label_76->setText(QString::number(fact, 'f' , 2)); // Calibrate Factor Value
        }
        else if(fact>100)
        {
            ui->label_196->setText(QString::number(fact, 'f' , 0)); // Calibrate Factor Value
            ui->label_76->setText(QString::number(fact, 'f' , 0)); // Calibrate Factor Value
        }
    }

    else if(option==2)          //Two-Point Sample Calc
    {
        if(fct == 0)
        {
            double od1,od2,od4,dod,dod1, dod4 ,dod5,dod6;
            od1=ui->label_120->text().toDouble();
            od2=ui->label_122->text().toDouble();
            dod=od2-od1; // Samp OD1
            dod1=abs(dod);
            ui->label_119->setText(QString::number(dod1, 'f', 3));
            dod6=ui->label_119->text().toDouble();
            od4=ui->label_196->text().toDouble(); // Factor Value
            dod4= dod6 * od4 ; //Sample OD * Factor Value
            dod5 = abs(dod4); // Result
            ui->label_33->setText(QString::number(dod5, 'f', 2));

        }
        else
        {
            double od1,od2,dod,dod1, dod4 ,dod5,dod6;
            od1=ui->label_120->text().toDouble();
            od2=ui->label_122->text().toDouble();
            dod=od2-od1; // Samp OD 1
            dod1=abs(dod);
            ui->label_119->setText(QString::number(dod1, 'f', 3));
            dod6=ui->label_119->text().toDouble();
            dod4=  dod6 * fct; //Sample OD * Fact Value
            dod5 = abs(dod4);//Result
            if(dod5<100)
            {
                ui->label_33->setText(QString::number(dod5, 'f' , 2));
            }
            else if(dod5>100)
            {
                ui->label_33->setText(QString::number(dod5, 'f' , 0));
            }
        }

        ui->RunCal_Btn_2->setEnabled(true);
        ui->RunBlank_Btn_2->setEnabled(true);
        ui->Save_Btn_6->setEnabled(true);
        ui->Print_Btn_2->setEnabled(true);
    }
    else if(option==3)           //Kinetic Calibrate -Calc
    {
        double od1 , od2 , od3 , od4 ,od5,od6,od7, dod =0.0, dod1 , dod2 , dod3 , dod4 ,dod5 , dod6 , dod7 , dod8 ,dod9,dod10 ,dod11,dod12,fact;
        od1=ui->label_106->text().toDouble();    //A1
        od2=ui->label_121->text().toDouble();    //A2
        od3=ui->label_123->text().toDouble();    //A3
        od4=ui->label_125->text().toDouble();    //A4
        od5=ui->label_194->text().toDouble();    //A2
        od6=ui->label_195->text().toDouble();    //A3
        od7=ui->label_198->text().toDouble();    //A4
        if(val==2)
        {
            ui->label_99->setVisible(false);
            ui->label_132->setVisible(false);
            ui->label_106->setVisible(false);
            ui->label_121->setVisible(false);
            ui->label_123->setVisible(false);
            ui->label_125->setVisible(false);
            ui->label_194->setVisible(true);
            ui->label_195->setVisible(true);
            ui->label_198->setVisible(true);
            dod1=od6-od5;
            dod2=abs(dod1);
            ui->label_127->setText(QString::number(dod2, 'f', 3));
            dod3=od7-od6;
            dod4=abs(dod3);
            ui->label_128->setText(QString::number(dod4, 'f', 3));
            dod= (dod2+dod4)/2;
        }
        else if(val==3)
        {
            ui->label_99->setVisible(true);
            ui->label_125->setVisible(true);
            ui->label_132->setVisible(true);
            ui->label_106->setVisible(true);
            ui->label_121->setVisible(true);
            ui->label_123->setVisible(true);
            ui->label_194->setVisible(false);
            ui->label_195->setVisible(false);
            ui->label_198->setVisible(false);
            dod5=od2-od1;
            dod6=abs(dod5);
            ui->label_127->setText(QString::number(dod6, 'f', 3));
            dod7=od3-od2;
            dod8=abs(dod7);
            ui->label_128->setText(QString::number(dod8, 'f', 3));
            dod9=od4-od3;
            dod10=abs(dod9);
            ui->label_132->setText(QString::number(dod10, 'f', 3));
            dod = (dod6+dod8+dod10)/3; //Avg
        }
        dod11=abs(dod); // Cal OD Value
        ui->label_34->setText(QString::number(dod11, 'f', 3));
        dod12=ui->label_34->text().toDouble();
        fact = concen / dod12;//need to check

        if(fact<100)
        {
            ui->label_196->setText(QString::number(fact, 'f' , 2)); // Calibrate Factor Value
            ui->label_76->setText(QString::number(fact, 'f' , 2)); // Calibrate Factor Value
        }
        else if(fact>100)
        {
            ui->label_196->setText(QString::number(fact, 'f' , 0)); // Calibrate Factor Value
            ui->label_76->setText(QString::number(fact, 'f' , 0)); // Calibrate Factor Value
        }

    }
    else if(option==4)                  //Kinetic Sample Calc
    {
        ui->label_204->setText("");
        double od1 , od2 , od3 , od4 ,od5,od6,od7, dod =0.0, dod1=0.0, dod2 , dod3 , dod4 ,dod5 , dod6 , dod7 , dod8 ,dod9,dod10 ,dod11,dod12,dod14,dod15,factor;
        double dod16=0.0, dod17=0.0,dod18=0.0 , dod19=0.0 ,dod20=0.0 , dod21=0.0 , dod22=0.0 , dod23=0.0 ,dod24=0.0,dod25=0.0,dod26=0.0, dod27 ,dod29,dod30;
        od1=ui->label_106->text().toDouble();    //A1
        od2=ui->label_121->text().toDouble();    //A2
        od3=ui->label_123->text().toDouble();    //A3
        od4=ui->label_125->text().toDouble();    //A4
        od5=ui->label_194->text().toDouble();    //A2
        od6=ui->label_195->text().toDouble();    //A3
        od7=ui->label_198->text().toDouble();    //A4
        if(fct == 0)
        {
            if(val==2)
            {
                ui-> label_203->setText("ΔA/min");
                ui->label_106->setVisible(false);
                ui->label_121->setVisible(false);
                ui->label_123->setVisible(false);
                ui->label_125->setVisible(false);
                ui->label_194->setVisible(true);
                ui->label_195->setVisible(true);
                ui->label_198->setVisible(true);
                dod1=od6-od5;
                dod2=abs(dod1);
                ui->label_127->setText(QString::number(dod2, 'f', 3));
                dod3=od7-od6;
                dod4=abs(dod3);
                ui->label_128->setText(QString::number(dod4, 'f', 3));
                dod= (dod2+dod4)/2;
            }
            else if(val==3)
            {
                ui->label_106->setVisible(true);
                ui->label_121->setVisible(true);
                ui->label_123->setVisible(true);
                ui->label_125->setVisible(true);
                ui->label_194->setVisible(false);
                ui->label_195->setVisible(false);
                ui->label_198->setVisible(false);
                ui-> label_203->setText("ΔA/min");
                dod5=od2-od1;
                dod6=abs(dod5);
                ui->label_127->setText(QString::number(dod6, 'f', 3));
                dod7=od3-od2;
                dod8=abs(dod7);
                ui->label_128->setText(QString::number(dod8, 'f', 3));
                dod9=od4-od3;
                dod10=abs(dod9);
                ui->label_132->setText(QString::number(dod10, 'f', 3));
                dod = (dod6+dod8+dod10)/3; //Avg
            }
            dod11=abs(dod);
            ui->label_204->setText(QString::number(dod11, 'f', 3));
            dod12=ui->label_204->text().toDouble();
            factor = ui->label_196->text().toDouble(); // Factor Value
            dod14= dod12*factor; // Sample OD * Factor Value
            dod15 = abs(dod14);
            if(dod15<100)
            {
                ui->label_33->setText(QString::number(dod15, 'f' , 2));
            }
            else if(dod15>100)
            {
                ui->label_33->setText(QString::number(dod15, 'f' , 0));
            }
        }
        else
        {
            if(val==2)
            {
                ui->label_106->setVisible(false);
                ui->label_121->setVisible(false);
                ui->label_123->setVisible(false);
                ui->label_125->setVisible(false);
                ui->label_194->setVisible(true);
                ui->label_195->setVisible(true);
                ui->label_198->setVisible(true);
                ui-> label_203->setText("ΔA/min");
                dod16=od6-od5;
                dod17=abs(dod16);
                ui->label_127->setText(QString::number(dod17, 'f', 3));
                dod18=od7-od6;
                dod19=abs(dod18);
                ui->label_128->setText(QString::number(dod19, 'f', 3));
                dod= (dod17+dod19)/2;
            }
            else if(val==3)
            {
                ui->label_106->setVisible(true);
                ui->label_121->setVisible(true);
                ui->label_123->setVisible(true);
                ui->label_125->setVisible(true);
                ui->label_194->setVisible(false);
                ui->label_195->setVisible(false);
                ui->label_198->setVisible(false);
                ui-> label_203->setText("ΔA/min");
                dod20=od2-od1;
                dod21=abs(dod20);
                ui->label_127->setText(QString::number(dod21, 'f', 3));
                dod22=od3-od2;
                dod23=abs(dod22);
                ui->label_128->setText(QString::number(dod23, 'f', 3));
                dod24=od4-od3;
                dod25=abs(dod24);
                ui->label_132->setText(QString::number(dod25, 'f', 3));
                dod = (dod21+dod23+dod25)/3;
            }
            dod26=abs(dod);
            ui->label_204->setText(QString::number(dod26, 'f', 3));
            dod27=ui->label_204->text().toDouble();
            dod29= dod27 * fct; // Sample OD * Fact
            dod30= abs(dod29);
            if(dod30<100)
            {
                ui->label_33->setText(QString::number(dod30, 'f' , 2));
            }
            else if(dod30>100)
            {
                ui->label_33->setText(QString::number(dod30, 'f' , 0));
            }
        }
    }
}

void BCMainWindow::on_toolButton_19_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void BCMainWindow::on_pushButton_132_clicked()
{
    for(int i=3;i>=1;i--)
    {
        if(i==3)
        {
            ui->label_10->setVisible(true);
            ui->label_11->setVisible(false);
            ui->label_68->setVisible(false);
            qDebug()<<"3rd";
            ui->label_10->setText(QString::number(i));
            QThread::msleep(1000);
            qDebug()<<i;

        }
        else if(i==2)
        {
            ui->label_11->setVisible(true);
            ui->label_10->setVisible(false);
            ui->label_68->setVisible(false);
            qDebug()<<"2nd";
            ui->label_11->setText(QString::number(i));
            QThread::msleep(1000);
            qDebug()<<i;


        }

        else if(i==1)
        {
            ui->label_68->setVisible(true);
            ui->label_11->setVisible(false);
            ui->label_10->setVisible(false);

            qDebug()<<"1st";
            ui->label_68->setText(QString::number(i));
            QThread::msleep(1000);
            qDebug()<<i;

        }
    }

}





//            else if(j==2)
//            {
//                QThread::msleep(1000);
//                ui->label_10->setText(QString::number(j));
//                ui->label_10->hide();
//                ui->label_68->hide();
//            }
//            else if(j==1)
//            {
//                QThread::msleep(1000);
//                ui->label_10->setText(QString::number(j));
//                ui->label_10->hide();
//                ui->label_68->hide();
//            }





//       ui->label_10->setText(QString::number(i));
//       i--;
//       QThread::msleep(1000);
//       ui->label_11->setText(QString::number(i));
//       i--;
//       QThread::msleep(1000);
//       ui->label_68->setText(QString::number(i));
//    }

//int count=4;
//    int i=3;
//    if(i<=1)
//    {
//        ui->label_10->setText("");
//        qDebug()<<i;
//        QThread::msleep(1000);
//        ui->label_10->setText(QString::number(i));
//        i--;
//    }

//    int count=1;
//    while(count<=3)
//    {
//        qDebug()<<count;
//        ui->label_10->setText(QString::number(count, 'f', 0));
//        count++;
//        QThread::msleep(1000);

//    }


//    for(int i=1 ; i<=3;i++)
//    {
//        on_pushButton_132_clicked();
//    }
//    ui->label_10->setText("");
//    int n=3;
//    while(n>=1)
//    {
//        qDebug()<<n;
//ui->label_10->setText(QString::number(n, 'i', 0));
//        QThread::msleep(1000);
//        n--;

//    }

//    QElapsedTimer timer;

//       int count = 1;
//       timer.start();
//       do {
//           count *= 2;
//           slowOperation2(count);
//       } while (timer.restart() < 250);

//       return count;

//    QString n ;
//    int frt=1, sec=2 , thi=3;

//    if(frt<=2)
//    {
//        qDebug()<<frt;
//        ui->label_10->setText(QString::number(frt, 'f', 0));
//        QThread::msleep(1000);
//        frt++;
//         }
//        else if(sec==2)
//        {
//            qDebug()<<sec;
//            ui->label_10->setText(QString::number(sec, 'f', 0));
//            QThread::msleep(1000);
//        }
//        else if(thi==3)
//        {
//            qDebug()<<thi;
//            ui->label_10->setText(QString::number(thi, 'f', 0));
//            QThread::msleep(1000);
//        }


//    for(int i=5;i>=1;i--)
//    {
//        qDebug()<<i;
//        QThread::msleep(1000);

//    }

//n= char(i);
//    for(int i=5;i>=1;i++)
//    {
//        for(int j=5;j>=1;j--)
//        {
//            //        n= char(i);
//            qDebug()<<j;
//            //ui->label_10->setText("");
//            ui->label_10->setText(QString::number(j, 'f', 0));
//            QThread::msleep(1000);

//            //        ui->label_10->setText(n);
//            //ui->label_10->setText("");
//        }
//     }


void BCMainWindow::on_pushButton_134_clicked()
{
    ui->stackedWidget->setCurrentIndex(20);
}

void BCMainWindow::on_pushButton_135_clicked()
{
    ui->lineEdit->setText("");
}

void BCMainWindow::on_pushButton_225_clicked()
{
    QObject *senderObj = sender(); // This will give Sender object
    QString senderObjName = senderObj->objectName();
    QString TestName=senderObjName;
    unsigned int wave=0;
    QSqlQuery query;
    query.prepare("select * from test where name='"+TestName+"'");
    query.exec();
    while(query.next())
    {
        wave = query.value(2).toUInt();
    }
    qDebug()<<"Enter";
    if(wave==340)
        digitalWrite (LED_BASE + 8,HIGH) ;
    else if(wave==405)

        digitalWrite (LED_BASE + 9,HIGH) ;
    else if(wave==507)

        digitalWrite (LED_BASE + 10,HIGH) ;
    else if(wave==545)

        digitalWrite (LED_BASE + 11,HIGH) ;

    else if(wave==572)

        digitalWrite (LED_BASE + 12,HIGH) ;

    else if(wave==628)

        digitalWrite (LED_BASE + 13,HIGH) ;

    else if(wave==700)

        digitalWrite (LED_BASE + 14,HIGH) ;

    qDebug()<<wave;

    /*digitalWrite (LED_BASE + 8,HIGH) ;
    QThread::msleep(1000);
    digitalWrite (LED_BASE + 9,HIGH) ;
    QThread::msleep(1000);
    digitalWrite (LED_BASE + 10,HIGH) ;
    QThread::msleep(1000);
    digitalWrite (LED_BASE + 11,HIGH) ;
    QThread::msleep(1000);
    digitalWrite (LED_BASE + 12,HIGH) ;
    QThread::msleep(1000);
    digitalWrite (LED_BASE + 13,HIGH) ;
    QThread::msleep(1000);
    digitalWrite (LED_BASE + 14,HIGH) ;
    QThread::msleep(1000);
    digitalWrite (LED_BASE + 8,LOW) ;
    digitalWrite (LED_BASE + 9,LOW) ;
    digitalWrite (LED_BASE + 10,LOW) ;
    digitalWrite (LED_BASE + 11,LOW) ;
    digitalWrite (LED_BASE + 12,LOW) ;
    digitalWrite (LED_BASE + 13,LOW) ;
    digitalWrite (LED_BASE + 14,LOW) ;*/


}
void BCMainWindow::LED_OFF()
{
    qDebug()<<"OUT";

    digitalWrite (LED_BASE + 8,LOW) ;
    digitalWrite (LED_BASE + 9,LOW) ;
    digitalWrite (LED_BASE + 10,LOW) ;
    digitalWrite (LED_BASE + 11,LOW) ;
    digitalWrite (LED_BASE + 12,LOW) ;
    digitalWrite (LED_BASE + 13,LOW) ;
    digitalWrite (LED_BASE + 14,LOW) ;

    /*QObject *senderObj = sender(); // This will give Sender object
    QString senderObjName = senderObj->objectName();
    QString TestName=senderObjName;
    unsigned int wave=0;
    QSqlQuery query;
    query.prepare("select * from test where name='"+TestName+"'");
    query.exec();
    while(query.next())
    {
        wave = query.value(2).toUInt();
    }
    if(wave==340)
        digitalWrite (LED_BASE + 8,LOW) ;

    else if(wave==405)

        digitalWrite (LED_BASE + 9,LOW) ;

    else if(wave==507)

        digitalWrite (LED_BASE + 10,LOW) ;

    else if(wave==545)

        digitalWrite (LED_BASE + 11,LOW) ;

    else if(wave==572)

        digitalWrite (LED_BASE + 12,LOW) ;

    else if(wave==628)

        digitalWrite (LED_BASE + 13,LOW) ;

    else if(wave==700)

        digitalWrite (LED_BASE + 14,LOW) ;

    qDebug()<<wave;*/
}

void BCMainWindow::on_pushButton_226_clicked()
{
    digitalWrite (LED_BASE + 8,LOW) ;
    digitalWrite (LED_BASE + 9,LOW) ;
    digitalWrite (LED_BASE + 10,LOW) ;
    digitalWrite (LED_BASE + 11,LOW) ;
    digitalWrite (LED_BASE + 12,LOW) ;
    digitalWrite (LED_BASE + 13,LOW) ;
    digitalWrite (LED_BASE + 14,LOW) ;
}
