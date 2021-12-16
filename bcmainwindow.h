#ifndef BCMAINWINDOW_H
#define BCMAINWINDOW_H

#include <QMainWindow>
#include <wiringPi.h>
#include <QDebug>
#include <mcp3004.h>
#include <wiringPiSPI.h>
#include <softPwm.h>
#include <Iir.h>
#include <QSqlDatabase>
#include <math.h>
#include <sr595.h>
#include <QDateTime>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QScroller>
#include <QtSql>
#include <QString>
#include <QSqlQuery>
#include <QTimer>
#include <QThread>
#include "pi2c.h"


namespace Ui {
class BCMainWindow;
}

class BCMainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit BCMainWindow(QWidget *parent = nullptr);
    ~BCMainWindow();

    void addPoint(double x, double y);
    void clearData();
    void plot();
    void plot_end();
    void plot_two();
    void plot_kinetic();

public:
    QSqlDatabase mydb;

    void connclose()
    {
        mydb.close();

    }
    bool connOpen()
    {
        QSqlDatabase mydb = QSqlDatabase :: addDatabase("QSQLITE");
        mydb.setDatabaseName("/home/pi/git/BCUID/bcd.db");
        if(!mydb.open())
        {
            qDebug()<<("Failed topen the Database");
            return false;

         }
        else
        {
                qDebug()<<("Connected...");
                return true;


         }
    }
private:

    Ui::BCMainWindow *ui;
    QTimer *timer;
    QVector<double> qv_x, qv_y;


private slots:

    void ShowTime();

    int readadc(int);

    void makePlot();

     void cald();
     void cald1();
     void cald2();
     void cald3();

     void on_Home_Btn_2_clicked();
     void on_Menu_Btn_clicked();

     void on_Test_Btn_4_clicked();
     void on_Save_Btn_clicked();
     void on_Home_Btn_3_clicked();
     void on_Home_Btn_5_clicked();

     void on_TestConfig_Btn_5_clicked();
     void on_TestConfig_Btn_4_clicked();
     void on_toolButton_clicked();
     void on_pushButton_clicked();
     void on_Home_Btn_4_clicked();
     void on_pushButton_3_clicked();
     void on_Save_Btn_2_clicked();
     void temp();
     void on_Test_Btn_5_clicked();
     void on_TestConfig_Btn_6_clicked();
     void on_DataHis_Btn_5_clicked();
     void on_DataHis_Btn_4_clicked();
     void on_pushButton_308_clicked();
     void on_Load_Btn_clicked();
     void on_toolButton_13_clicked();
     void on_toolButton_12_clicked();
     void on_Home_Btn_6_clicked();
     void on_QC_Btn_5_clicked();
     void on_QC_Btn_4_clicked();
     void on_Setting_Btn_clicked();
     void on_Load_Btn_2_clicked();
     void on_toolButton_14_clicked();
     void on_Menu_Btn_2_clicked();
     void on_pushButton_307_clicked();
     void on_pushButton_309_clicked();
     void on_Save_Btn_3_clicked();
     void on_pushButton_2_clicked();
     void on_Update_Btn_clicked();
     void on_Delete_Btn_clicked();
     void on_Home_Btn_7_clicked();
     void on_Home_Btn_8_clicked();
     void on_pushButton_17_clicked();
     void on_pushButton_23_clicked();
     void on_pushButton_13_clicked();
     void on_pushButton_14_clicked();
     void on_pushButton_25_clicked();
     void on_pushButton_26_clicked();
     void on_pushButton_24_clicked();
     void on_pushButton_12_clicked();
     void on_pushButton_47_clicked();
     void on_pushButton_71_clicked();
     void on_pushButton_46_clicked();
     void on_pushButton_56_clicked();
     void on_pushButton_18_clicked();
     void on_pushButton_15_clicked();
     void on_pushButton_22_clicked();
     void on_pushButton_37_clicked();
     void on_pushButton_36_clicked();
     void on_pushButton_35_clicked();
     void on_pushButton_34_clicked();
     void on_pushButton_33_clicked();
     void on_pushButton_29_clicked();
     void on_pushButton_27_clicked();
     void on_pushButton_21_clicked();
     void on_pushButton_31_clicked();
     void on_pushButton_41_clicked();
     void on_pushButton_19_clicked();
     void on_pushButton_32_clicked();
     void on_pushButton_20_clicked();
     void on_pushButton_30_clicked();
     void on_pushButton_28_clicked();
     void on_pushButton_40_clicked();
     void on_pushButton_39_clicked();
     void on_pushButton_38_clicked();
     void on_pushButton_42_clicked();
     void on_pushButton_43_clicked();
     void on_pushButton_16_clicked();
     void on_pushButton_44_clicked();
     void on_pushButton_49_clicked();
     void on_pushButton_48_clicked();
     void on_pushButton_67_clicked();
     void on_pushButton_93_clicked();
     void on_pushButton_45_clicked();
     void on_pushButton_50_clicked();
     void on_pushButton_80_clicked();
     void on_pushButton_59_clicked();
     void on_pushButton_81_clicked();
     void on_pushButton_82_clicked();
     void on_pushButton_51_clicked();
     void on_pushButton_55_clicked();
     void on_pushButton_61_clicked();
     void on_pushButton_58_clicked();
     void on_pushButton_79_clicked();
     void on_pushButton_73_clicked();
     void on_pushButton_63_clicked();
     void on_pushButton_66_clicked();
     void on_pushButton_76_clicked();
     void on_pushButton_62_clicked();
     void on_pushButton_77_clicked();
     void on_pushButton_78_clicked();
     void on_pushButton_68_clicked();
     void on_pushButton_52_clicked();
     void on_pushButton_65_clicked();
     void on_pushButton_53_clicked();
     void on_pushButton_75_clicked();
     void on_pushButton_64_clicked();
     void on_pushButton_69_clicked();
     void on_pushButton_74_clicked();
     void on_pushButton_57_clicked();
     void on_pushButton_54_clicked();
     void on_pushButton_70_clicked();
     void on_pushButton_104_clicked();
     void on_pushButton_99_clicked();
     void on_pushButton_107_clicked();
     void on_pushButton_86_clicked();
     void on_pushButton_91_clicked();
     void on_pushButton_101_clicked();
     void on_pushButton_109_clicked();
     void on_pushButton_106_clicked();
     void on_pushButton_96_clicked();
     void on_pushButton_111_clicked();
     void on_pushButton_112_clicked();
     void on_pushButton_97_clicked();
     void on_pushButton_95_clicked();
     void on_pushButton_100_clicked();
     void on_pushButton_85_clicked();
     void on_pushButton_94_clicked();
     void on_pushButton_92_clicked();
     void on_pushButton_83_clicked();
     void on_pushButton_87_clicked();
     void on_pushButton_116_clicked();
     void on_pushButton_110_clicked();
     void on_pushButton_115_clicked();
     void on_pushButton_108_clicked();
     void on_pushButton_102_clicked();
     void on_pushButton_105_clicked();
     void on_pushButton_90_clicked();
     void on_pushButton_98_clicked();
     void on_pushButton_88_clicked();
     void on_pushButton_113_clicked();
     void on_pushButton_84_clicked();
     void on_pushButton_114_clicked();
     void on_pushButton_103_clicked();


     void on_RunBlank_Btn_clicked();
     void on_pushButton_89_clicked();
     void on_pushButton_60_clicked();
     void on_RunSample_Btn_clicked();
     void on_RunCal_Btn_clicked();
     void on_Home_Btn_9_clicked();
     void on_Home_Btn_10_clicked();
};

#endif // BCMAINWINDOW_H
