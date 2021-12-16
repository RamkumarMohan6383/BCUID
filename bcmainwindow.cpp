#include "bcmainwindow.h"
#include "ui_bcmainwindow.h"
#include <QTimer>
#include <QDateTime>
#include <QDate>
#include <QMessageBox>
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

static int read[20000];
static int filtdata[20000];

static int filt360nm[200];
static int filt405nm[200];
static int filt505nm[200];
static int filt525nm[200];
static int filt570nm[200];
static int filt625nm[200];
static int filt660nm[200];
static int filtwave[200];

static int readbc[200];
//static int filtwave[200];

static int opt=0;
static double bc_y_val=0;




static QString array[100]={""};

BCMainWindow::BCMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BCMainWindow)
{
    ui->setupUi(this);

    QScroller::grabGesture(ui->scrollArea, QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(ui->scrollArea_3, QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(ui->scrollArea_4, QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(ui->scrollArea_5, QScroller::LeftMouseButtonGesture);

    wiringPiSetup () ;
    mcp3004Setup (ADC_BASE, SPI_CHAN) ;
    sr595Setup (LED_BASE, 8, dataPin, clockPin, latchPin) ;
    //softPwmCreate(LED, init,range);
    pinMode (en, OUTPUT) ;
    pinMode (dir, OUTPUT) ;
    pinMode (steps, OUTPUT) ;
    pinMode (hm_sen, INPUT) ;
    pinMode (BARCODE_GND, OUTPUT) ;
    //softPwmWrite(LED,0);
    digitalWrite(en,HIGH);

    digitalWrite(BARCODE_GND,LOW);
    pinMode (LED, PWM_OUTPUT);
    pwmWrite (LED, 0);


    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this,SLOT(temp()));
    timer->start(1000);

      QSqlDatabase mydb=QSqlDatabase::addDatabase("QSQLITE");
      mydb.setDatabaseName("/home/pi/git/BCUID/bcd.db");
      if(mydb.open())
          qDebug()<<"DB Connected";
      else
          qDebug()<<"DB Failed";

    BCMainWindow::makePlot();

    QTimer * timer = new QTimer(this);
    connect(timer ,SIGNAL(timeout()) , this ,SLOT(ShowTime()));
    timer->start();

    QDate date=QDate::currentDate();
    QString datetext= date.toString("dd / MM / yyyy");
    ui->SysDate_Lbl->setText(datetext);
    setWindowFlags(Qt::FramelessWindowHint);
    ui->customPlot->setBackground(QColor(0,0,26));
    ui->customPlot->xAxis->setBasePen(QPen(Qt::white, 1));
    ui->customPlot->yAxis->setBasePen(QPen(Qt::white, 1));
    ui->customPlot->xAxis->setTickPen(QPen(Qt::white, 1));
    ui->customPlot->yAxis->setTickPen(QPen(Qt::white, 1));
    //     ui->customPlot->xAxis->setSubTickPen(QPen(Qt::white, 1));
    //     ui->customPlot->yAxis->setSubTickPen(QPen(Qt::white, 1));
    ui->customPlot->xAxis->setTickLabelColor(Qt::white);
    ui->customPlot->yAxis->setTickLabelColor(Qt::white);
    //     ui->customPlot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    //     ui->customPlot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    //     ui->customPlot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    //     ui->customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    //     ui->customPlot->xAxis->grid()->setSubGridVisible(true);
    //     ui->customPlot->yAxis->grid()->setSubGridVisible(true);
    //     ui->customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    //     ui->customPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    //     ui->customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    //     ui->customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);


    //ui->customPlot_2->setBackground(QColor(0,0,26));
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsLine);

    ;

}
void BCMainWindow::ShowTime()
{
    QTime time = QTime::currentTime();
    QString time_text = time.toString("hh : mm : ss");
    if((time.second() % 2)==0)
    {
        time_text[3]= ' ';
        time_text[8]= ' ';
    }

    ui->SysTime_Lbl->setText(time_text);
}

BCMainWindow::~BCMainWindow()
{
    delete ui;
}

void BCMainWindow::makePlot()
{

}



void BCMainWindow::on_Home_Btn_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void BCMainWindow::on_Menu_Btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

}

void BCMainWindow::on_Menu_Btn_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void BCMainWindow::on_Test_Btn_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);

    QSqlQuery query;
    query.prepare("SELECT name FROM test");
    query.exec();
    int count=0;
    while(query.next())
    {
        QString s=query.value(0).toString();
        array[count]=s;
        count++;
        qDebug()<<s;
    }
    int count1=0;
    const QSize btnSize = QSize(150, 50);
    for (int i=0;i<15 ;i++ )
    {
        for (int j=0;j<5 ;j++ )
        {
            if(count1==count)
               break;
            else
            {
            QPushButton *button = new QPushButton(array[count1]);
            button->setFixedSize(btnSize);
            button->setObjectName(array[count1]);
            connect(button, &QPushButton::clicked, this, &BCMainWindow::cald);
            ui->gridLayout->addWidget(button,i,j);
            //lay->addWidget(button,i,j);
            }
            count1++;
        }

    }

}
void BCMainWindow::on_Test_Btn_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);

    QSqlQuery query;
    query.prepare("SELECT name FROM test");
    query.exec();
    int count=0;
    while(query.next())
    {
        QString s=query.value(0).toString();
        array[count]=s;
        count++;
        qDebug()<<s;
    }
    int count1=0;
    const QSize btnSize = QSize(150, 50);
    for (int i=0;i<15 ;i++ )
    {
        for (int j=0;j<5 ;j++ )
        {
            if(count1==count)
               break;
            else
            {
            QPushButton *button = new QPushButton(array[count1]);
            button->setFixedSize(btnSize);
            button->setObjectName(array[count1]);
            connect(button, &QPushButton::clicked, this, &BCMainWindow::cald);
            ui->gridLayout->addWidget(button,i,j);
            //lay->addWidget(button,i,j);
            }
            count1++;
        }
 }
}
void BCMainWindow::cald()
{
    QObject *senderObj = sender(); // This will give Sender object
    QString senderObjName = senderObj->objectName();
    //qDebug()<< "Button: " << senderObjName;;
    ui->TestName_Lbl->setText(senderObjName);
    ui->stackedWidget->setCurrentIndex(3);
}
void BCMainWindow::on_Save_Btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);

        QString name, unit;
        unsigned int wave, rctn, temp, fact, blnk, dely, read, lag, linmin, linmax, normin, normax;
        unsigned int mode, nostd, rpt, con1,con2,con3,con4,con5,con6,con7,con8;
        unsigned int ctrl1avg, ctrl1min, ctrl1bno, ctrl2avg, ctrl2min, ctrl2bno;

        name=ui->lineEdit_4->text();
        unit=ui->lineEdit_5->text();
        wave=ui->WaveLength_Combo->currentText().toUInt();
        rctn=ui->ReactionType_Combo->currentIndex();
        temp=ui->Temp_Combo->currentIndex();
        fact=ui->lineEdit_6->text().toUInt();
        blnk=ui->BlankType_Combo->currentIndex();
        dely=ui->lineEdit_7->text().toUInt();
        read=ui->lineEdit_8->text().toUInt();
        lag=ui->lineEdit_22->text().toUInt();
        linmin=ui->lineEdit_9->text().toUInt();
        linmax=ui->lineEdit_10->text().toUInt();
        normin=ui->lineEdit_12->text().toUInt();
        normax=ui->lineEdit_11->text().toUInt();
        mode=ui->comboBox->currentIndex();
        nostd=ui->comboBox_5->currentText().toUInt();
        rpt=ui->comboBox_6->currentText().toUInt();
        con1=ui->lineEdit_13->text().toInt();
        con2=ui->lineEdit_14->text().toInt();
        con3=ui->lineEdit_15->text().toInt();
        con4=ui->lineEdit_16->text().toInt();
        con5=ui->lineEdit_18->text().toInt();
        con6=ui->lineEdit_19->text().toInt();
        con7=ui->lineEdit_17->text().toInt();
        con8=ui->lineEdit_20->text().toInt();
        ctrl1avg=ui->lineEdit_21->text().toInt();
        ctrl1min=ui->lineEdit_23->text().toInt();
        ctrl1bno=ui->lineEdit_26->text().toInt();
        ctrl2avg=ui->lineEdit_25->text().toInt();
        ctrl2min=ui->lineEdit_28->text().toInt();
        ctrl2bno=ui->lineEdit_29->text().toInt();



        QSqlQuery qry;

        qry.prepare("insert into test(name,wave,rctn,unit,temp,fact,blnk,dely,read,lag,linmin,linmax,normin,normax,mode,nostd,rpt,con1,con2,con3,con4,con5,con6,con7,con8,"
                    "ctrl1avg,ctrl1min,ctrl1bno,ctrl2avg,ctrl2min,ctrl2bno) values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
        qry.addBindValue(name);
        qry.addBindValue(wave);
        qry.addBindValue(rctn);
        qry.addBindValue(unit);
        qry.addBindValue(temp);
        qry.addBindValue(fact);
        qry.addBindValue(blnk);
        qry.addBindValue(dely);
        qry.addBindValue(read);
        qry.addBindValue(lag);
        qry.addBindValue(linmin);
        qry.addBindValue(linmax);
        qry.addBindValue(normin);
        qry.addBindValue(normax);
        qry.addBindValue(mode);
        qry.addBindValue(nostd);
        qry.addBindValue(rpt);
        qry.addBindValue(con1);
        qry.addBindValue(con2);
        qry.addBindValue(con3);
        qry.addBindValue(con4);
        qry.addBindValue(con5);
        qry.addBindValue(con6);
        qry.addBindValue(con7);
        qry.addBindValue(con8);
        qry.addBindValue(ctrl1avg);
        qry.addBindValue(ctrl1min);
        qry.addBindValue(ctrl1bno);
        qry.addBindValue(ctrl2avg);
        qry.addBindValue(ctrl2min);
        qry.addBindValue(ctrl2bno);
        qry.exec();

}

void BCMainWindow::on_Home_Btn_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void BCMainWindow::on_Home_Btn_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void BCMainWindow::on_TestConfig_Btn_5_clicked()
{
     ui->stackedWidget->setCurrentIndex(4);
}

void BCMainWindow::on_TestConfig_Btn_4_clicked()
{
     ui->stackedWidget->setCurrentIndex(4);
}

void BCMainWindow::on_toolButton_clicked()
{
    ui->scrollAreaWidgetContents->update();
}

void BCMainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void BCMainWindow::on_Home_Btn_4_clicked()
{
   ui->stackedWidget->setCurrentIndex(0);
}

void BCMainWindow::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);
    QSqlQuery query;
    query.prepare("SELECT name FROM test");
    query.exec();
    int count=0;
    while(query.next())
    {
        QString s=query.value(0).toString();
        array[count]=s;
        count++;
        qDebug()<<s;
    }
    int count1=0;
    const QSize btnSize = QSize(150, 50);
    for (int i=0;i<15 ;i++ )
    {
        for (int j=0;j<5 ;j++ )
        {
            if(count1==count)
               break;
            else
            {
            QPushButton *button = new QPushButton(array[count1]);
            button->setFixedSize(btnSize);
            button->setObjectName(array[count1]);
            connect(button, &QPushButton::clicked, this, &BCMainWindow::cald3);
            ui->gridLayout_8->addWidget(button,i,j);
            //lay->addWidget(button,i,j);
            }
            count1++;
        }
 }

}


void BCMainWindow::on_Save_Btn_2_clicked()
{

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


void BCMainWindow::on_TestConfig_Btn_6_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void BCMainWindow::on_DataHis_Btn_5_clicked()
{
  ui->stackedWidget->setCurrentIndex(12);
}

void BCMainWindow::on_DataHis_Btn_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(12);
}

void BCMainWindow::on_pushButton_308_clicked()
{

}

void BCMainWindow::on_Load_Btn_clicked()
{
    BCMainWindow conn;
    QSqlQueryModel * modal = new QSqlQueryModel();

    conn.connOpen();
    QSqlQuery* qry=new QSqlQuery(conn.mydb);

    qry->prepare("select * from test");

    qry->exec();
    modal->setQuery(*qry);
    ui->tableView->setModel(modal);

    conn.connclose();
    qDebug() << ( modal->rowCount());
}

void BCMainWindow::on_toolButton_13_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void BCMainWindow::on_toolButton_12_clicked()
{
      ui->stackedWidget->setCurrentIndex(0);
}

void BCMainWindow::on_Home_Btn_6_clicked()
{
      ui->stackedWidget->setCurrentIndex(0);
}

void BCMainWindow::on_QC_Btn_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(10);

    QSqlQuery query;
    query.prepare("SELECT name FROM test");
    query.exec();
    int count=0;
    while(query.next())
    {
        QString s=query.value(0).toString();
        array[count]=s;
        count++;
        qDebug()<<s;
    }
    int count1=0;
    const QSize btnSize = QSize(150, 50);
    for (int i=0;i<15 ;i++ )
    {
        for (int j=0;j<5 ;j++ )
        {
            if(count1==count)
               break;
            else
            {
            QPushButton *button = new QPushButton(array[count1]);
            button->setFixedSize(btnSize);
            button->setObjectName(array[count1]);
            connect(button, &QPushButton::clicked, this, &BCMainWindow::cald1);
            ui->gridLayout_2->addWidget(button,i,j);
            //lay->addWidget(button,i,j);
            }
            count1++;
        }
 }
}

void BCMainWindow::on_QC_Btn_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(10);

    QSqlQuery query;
    query.prepare("SELECT name FROM test");
    query.exec();
    int count=0;
    while(query.next())
    {
        QString s=query.value(0).toString();
        array[count]=s;
        count++;
        qDebug()<<s;
    }
    int count1=0;
    const QSize btnSize = QSize(150, 50);
    for (int i=0;i<15 ;i++ )
    {
        for (int j=0;j<5 ;j++ )
        {
            if(count1==count)
               break;
            else
            {
            QPushButton *button = new QPushButton(array[count1]);
            button->setFixedSize(btnSize);
            button->setObjectName(array[count1]);
            connect(button, &QPushButton::clicked, this, &BCMainWindow::cald1);
            ui->gridLayout_2->addWidget(button,i,j);
            //lay->addWidget(button,i,j);
            }
            count1++;
        }
 }
}
void BCMainWindow::cald1()
{
    QObject *senderObj = sender(); // This will give Sender object
        QString senderObjName = senderObj->objectName();
        //qDebug()<< "Button: " << senderObjName;
        ui->label_45->setText(senderObjName);
        ui->stackedWidget->setCurrentIndex(7);
}
void BCMainWindow::cald2()
{
    QObject *senderObj = sender(); // This will give Sender object
        QString senderObjName = senderObj->objectName();
        //qDebug()<< "Button: " << senderObjName;
        ui->lineEdit_24->setText(senderObjName);
        ui->stackedWidget->setCurrentIndex(7);
}
void BCMainWindow::cald3()
{
    QObject *senderObj = sender(); // This will give Sender object
        QString senderObjName = senderObj->objectName();
        //qDebug()<< "Button: " << senderObjName;
        ui->lineEdit_64->setText(senderObjName);
        ui->stackedWidget->setCurrentIndex(9);
}
void BCMainWindow::on_toolButton_14_clicked()
{
     ui->stackedWidget->setCurrentIndex(0);
}

void BCMainWindow::on_Setting_Btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(13);
}

void BCMainWindow::on_Load_Btn_2_clicked()
{
    BCMainWindow conn;
    QSqlQueryModel * modal = new QSqlQueryModel();

    conn.connOpen();
    QSqlQuery* qry=new QSqlQuery(conn.mydb);

    qry->prepare("select * from tests");

    qry->exec();
    modal->setQuery(*qry);
    ui->tableView->setModel(modal);

    conn.connclose();
    qDebug() << ( modal->rowCount());
}




void BCMainWindow::on_pushButton_307_clicked()
{

}

void BCMainWindow::on_pushButton_309_clicked()
{

}

void BCMainWindow::on_Save_Btn_3_clicked()
{

}

void BCMainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);

    QSqlQuery query;
    query.prepare("SELECT name FROM test");
    query.exec();
    int count=0;
    while(query.next())
    {
        QString s=query.value(0).toString();
        array[count]=s;
        count++;
        qDebug()<<s;
    }
    int count1=0;
    const QSize btnSize = QSize(150, 50);
    for (int i=0;i<15 ;i++ )
    {
        for (int j=0;j<5 ;j++ )
        {
            if(count1==count)
               break;
            else
            {
            QPushButton *button = new QPushButton(array[count1]);
            button->setFixedSize(btnSize);
            button->setObjectName(array[count1]);
            connect(button, &QPushButton::clicked, this, &BCMainWindow::cald2);
            ui->gridLayout_6->addWidget(button,i,j);
            //lay->addWidget(button,i,j);
            }
            count1++;
        }
 }
}

void BCMainWindow::on_Update_Btn_clicked()
{
    BCMainWindow conn;
    QString name, unit;

    if(!conn.connOpen())
    {
        qDebug()<<"failed to open the database";
        return;
    }

    conn.connOpen();
    QSqlQuery qry;
    qry.prepare("update test set name='"+name+"',unit='"+unit+"' where name='"+name+"'");
    if(qry.exec())
    {
        QMessageBox::information(this,tr("Update"),tr("Updated"));
               conn.connclose();
               ui->gridLayout->update();
               ui->stackedWidget->setCurrentIndex(2);
        }
    else
    {
        QMessageBox::critical(this,tr("error::"),qry.lastError().text());
    }


}

void BCMainWindow::on_Delete_Btn_clicked()
{
    BCMainWindow conn;
    QString name;
    name=ui->lineEdit_64->text();
    if(!conn.connOpen())
    {
        qDebug()<<"failed to open the database";
        return;
    }
    conn.connOpen();
    QSqlQuery qry;
    qry.prepare("Delete from test where name='"+name+"'");

    if(qry.exec())
    {
        QMessageBox::information(this,tr("Delete"),tr("Delete"));
         conn.connclose();
          ui->gridLayout->update();
               ui->tableView->update();
               ui->gridLayout_2->update();
               ui->gridLayout_6->update();
               ui->gridLayout_8->update();
                 ui->stackedWidget->setCurrentIndex(2);
        }
    else
    {
        QMessageBox::critical(this,tr("error::"),qry.lastError().text());
    }
}

void BCMainWindow::on_Home_Btn_7_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void BCMainWindow::on_Home_Btn_8_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void BCMainWindow::on_pushButton_17_clicked()
{
   ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_17->text());
}

void BCMainWindow::on_pushButton_23_clicked()
{

}

void BCMainWindow::on_pushButton_13_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_13->text());
}

void BCMainWindow::on_pushButton_14_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_14->text());

}

void BCMainWindow::on_pushButton_25_clicked()
{
   // ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_2->text());

}

void BCMainWindow::on_pushButton_26_clicked()
{
   // ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_26->text());

}

void BCMainWindow::on_pushButton_24_clicked()
{
   // ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_24->text());

}

void BCMainWindow::on_pushButton_12_clicked()
{
    ui->stackedWidget->setCurrentIndex(14);
}

void BCMainWindow::on_pushButton_47_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(2);
}

void BCMainWindow::on_pushButton_71_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(2);
}

void BCMainWindow::on_pushButton_46_clicked()
{
     ui->stackedWidget_2->setCurrentIndex(1);
}

void BCMainWindow::on_pushButton_56_clicked()
{
     ui->stackedWidget_2->setCurrentIndex(0);
}

void BCMainWindow::on_pushButton_18_clicked()
{
    QString text=ui->lineEdit->text();
    ui->PatientInfo_LineEdit->setText(text);
    ui->stackedWidget->setCurrentIndex(3);

}

void BCMainWindow::on_pushButton_15_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_15->text());
}

void BCMainWindow::on_pushButton_22_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_22->text());
}

void BCMainWindow::on_pushButton_37_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_37->text());
}

void BCMainWindow::on_pushButton_36_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_36->text());
}

void BCMainWindow::on_pushButton_35_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_35->text());
}

void BCMainWindow::on_pushButton_34_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_34->text());
}

void BCMainWindow::on_pushButton_33_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_33->text());
}

void BCMainWindow::on_pushButton_29_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_29->text());
}

void BCMainWindow::on_pushButton_27_clicked()
{
         ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_27->text());
}

void BCMainWindow::on_pushButton_21_clicked()
{
         ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_21->text());
}

void BCMainWindow::on_pushButton_31_clicked()
{
         ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_31->text());
}

void BCMainWindow::on_pushButton_41_clicked()
{
         ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_41->text());
}

void BCMainWindow::on_pushButton_19_clicked()
{
         ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_19->text());
}

void BCMainWindow::on_pushButton_32_clicked()
{
         ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_32->text());
}

void BCMainWindow::on_pushButton_20_clicked()
{
         ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_20->text());
}

void BCMainWindow::on_pushButton_30_clicked()
{
         ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_30->text());
}

void BCMainWindow::on_pushButton_28_clicked()
{
         ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_28->text());
}

void BCMainWindow::on_pushButton_40_clicked()
{
         ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_40->text());
}

void BCMainWindow::on_pushButton_39_clicked()
{
         ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_39->text());
}

void BCMainWindow::on_pushButton_38_clicked()
{
         ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_38->text());
}

void BCMainWindow::on_pushButton_42_clicked()
{
         ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_42->text());
}

void BCMainWindow::on_pushButton_43_clicked()
{
         ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_43->text());
}

void BCMainWindow::on_pushButton_16_clicked()
{
         ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_16->text());
}

void BCMainWindow::on_pushButton_44_clicked()
{
         ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_44->text());
}

void BCMainWindow::on_pushButton_49_clicked()
{
         ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_49->text());
}

void BCMainWindow::on_pushButton_48_clicked()
{

         ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_48->text());
}

void BCMainWindow::on_pushButton_67_clicked()
{
    QString text=ui->lineEdit->text();
    ui->PatientInfo_LineEdit->setText(text);
    ui->stackedWidget->setCurrentIndex(3);

}

void BCMainWindow::on_pushButton_93_clicked()
{
    QString text=ui->lineEdit->text();
    ui->PatientInfo_LineEdit->setText(text);
    ui->stackedWidget->setCurrentIndex(3);

}

void BCMainWindow::on_pushButton_45_clicked()
{
    ui->lineEdit->backspace();
}

void BCMainWindow::on_pushButton_50_clicked()
{

}

void BCMainWindow::on_pushButton_80_clicked()
{
       ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_80->text());
}

void BCMainWindow::on_pushButton_59_clicked()
{
       ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_59->text());
}

void BCMainWindow::on_pushButton_81_clicked()
{
       ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_81->text());
}

void BCMainWindow::on_pushButton_82_clicked()
{
       ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_82->text());
}

void BCMainWindow::on_pushButton_51_clicked()
{
       ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_51->text());
}

void BCMainWindow::on_pushButton_55_clicked()
{
       ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_55->text());
}

void BCMainWindow::on_pushButton_61_clicked()
{
       ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_61->text());
}

void BCMainWindow::on_pushButton_58_clicked()
{
       ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_58->text());
}

void BCMainWindow::on_pushButton_79_clicked()
{
       ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_79->text());
}

void BCMainWindow::on_pushButton_73_clicked()
{
       ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_73->text());
}

void BCMainWindow::on_pushButton_63_clicked()
{
       ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_63->text());
}

void BCMainWindow::on_pushButton_66_clicked()
{
       ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_66->text());
}

void BCMainWindow::on_pushButton_76_clicked()
{
       ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_76->text());
}

void BCMainWindow::on_pushButton_62_clicked()
{
       ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_62->text());
}

void BCMainWindow::on_pushButton_77_clicked()
{
       ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_77->text());
}

void BCMainWindow::on_pushButton_78_clicked()
{
       ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_78->text());
}

void BCMainWindow::on_pushButton_68_clicked()
{
       ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_68->text());
}

void BCMainWindow::on_pushButton_52_clicked()
{
       ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_52->text());
}

void BCMainWindow::on_pushButton_65_clicked()
{
       ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_65->text());
}

void BCMainWindow::on_pushButton_53_clicked()
{
       ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_53->text());
}

void BCMainWindow::on_pushButton_75_clicked()
{
       ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_75->text());
}

void BCMainWindow::on_pushButton_64_clicked()
{
       ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_64->text());
}

void BCMainWindow::on_pushButton_69_clicked()
{
       ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_69->text());
}

void BCMainWindow::on_pushButton_74_clicked()
{
       ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_74->text());
}

void BCMainWindow::on_pushButton_57_clicked()
{
       ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_57->text());
}

void BCMainWindow::on_pushButton_54_clicked()
{
       ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_54->text());
}

void BCMainWindow::on_pushButton_70_clicked()
{
       ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_70->text());
}

void BCMainWindow::on_pushButton_104_clicked()
{
       ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_104->text());
}

void BCMainWindow::on_pushButton_99_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_99->text());
}

void BCMainWindow::on_pushButton_107_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_107->text());
}

void BCMainWindow::on_pushButton_86_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_86->text());
}

void BCMainWindow::on_pushButton_91_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_91->text());
}

void BCMainWindow::on_pushButton_101_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_101->text());
}

void BCMainWindow::on_pushButton_109_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_109->text());
}

void BCMainWindow::on_pushButton_106_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_106->text());
}

void BCMainWindow::on_pushButton_96_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_96->text());
}

void BCMainWindow::on_pushButton_111_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_111->text());
}

void BCMainWindow::on_pushButton_112_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_112->text());
}

void BCMainWindow::on_pushButton_97_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_97->text());
}

void BCMainWindow::on_pushButton_95_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_95->text());
}

void BCMainWindow::on_pushButton_100_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_100->text());
}

void BCMainWindow::on_pushButton_85_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_85->text());
}

void BCMainWindow::on_pushButton_94_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_94->text());
}

void BCMainWindow::on_pushButton_92_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_92->text());
}

void BCMainWindow::on_pushButton_83_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_83->text());
}

void BCMainWindow::on_pushButton_87_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_87->text());
}

void BCMainWindow::on_pushButton_116_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_116->text());
}

void BCMainWindow::on_pushButton_110_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_110->text());
}

void BCMainWindow::on_pushButton_115_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_115->text());
}

void BCMainWindow::on_pushButton_108_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_108->text());
}

void BCMainWindow::on_pushButton_102_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_102->text());
}

void BCMainWindow::on_pushButton_105_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_105->text());
}

void BCMainWindow::on_pushButton_90_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_90->text());
}

void BCMainWindow::on_pushButton_98_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_98->text());
}

void BCMainWindow::on_pushButton_88_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_88->text());
}

void BCMainWindow::on_pushButton_113_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_113->text());
}

void BCMainWindow::on_pushButton_84_clicked()
{
     ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_84->text());
}

void BCMainWindow::on_pushButton_114_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(0);
}

void BCMainWindow::on_pushButton_103_clicked()
{

}

void BCMainWindow::on_RunBlank_Btn_clicked()
{
    double samplingrate=0,cutoff_frequency=0;
    QSqlQuery query;
    query.prepare("select samprate, cutoff from FIA where sno=1");
    query.exec();
    while(query.next())
    {

        samplingrate=query.value(0).toDouble();
        cutoff_frequency=query.value(1).toDouble();

    }

    QThread::sleep(1);
    const int order = 2; // 4th order (=2 biquads)
    Iir::Butterworth::LowPass<order> fwave;
    //const float samplingrate = 1500; // Hz
    //const float cutoff_frequency = 4; // Hz
    fwave.setup (samplingrate, cutoff_frequency);
    int wavelength=0;
    wavelength=ui->comboBox->currentIndex();
    //qDebug()<<wavelength;
    int blank[200];
    digitalWrite (LED_BASE + 6-wavelength,HIGH) ;
    QThread::msleep(200);
    for(int i=0;i<3;i++)
    {
        for(int i=0;i<200;i++)
        {
            blank[i]=readadc(wavelength);
            QThread::msleep(1);
            filtwave[i]=fwave.filter(blank[i]);

        }


        //QThread::msleep(200);

        QThread::msleep(10);
        for(int i=100;i<200;i++)
        {
            filtwave[0]+=filtwave[i];

        }

    }
    digitalWrite (LED_BASE + 6-wavelength,LOW) ;
    filtwave[0]=filtwave[0]/100;

    ui->label_33->setNum(filtwave[0]);



}

void BCMainWindow::on_pushButton_89_clicked()
{
    ui->lineEdit->backspace();
}

void BCMainWindow::on_pushButton_60_clicked()
{
    ui->lineEdit->backspace();
}

void BCMainWindow::on_RunSample_Btn_clicked()
{

}

void BCMainWindow::on_RunCal_Btn_clicked()
{

}

void BCMainWindow::on_Home_Btn_9_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void BCMainWindow::on_Home_Btn_10_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


