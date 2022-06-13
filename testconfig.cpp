#include "bcmainwindow.h"
#include "ui_bcmainwindow.h"

static QString array[100]={""};

//********************Test Configuration Button**************************

void BCMainWindow::on_TestConfig_Btn_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}
//********************Test Configuration (Add New Test)**************************
void BCMainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    ui->tabWidget_2->setCurrentIndex(0);
    ui->lineEdit_4->setText("");
    ui->lineEdit_5->setText("");
    ui->lineEdit_6->setText("");
    ui->lineEdit_7->setText("");
    ui->lineEdit_8->setText("");
    ui->lineEdit_22->setText("");
    ui->lineEdit_9->setText("");
    ui->lineEdit_10->setText("");
    ui->lineEdit_12->setText("");
    ui->lineEdit_11->setText("");
    ui->lineEdit_88->setText("");
    ui->lineEdit_92->setText("");
    ui->lineEdit_89->setText("");
    ui->lineEdit_95->setText("");
    ui->lineEdit_90->setText("");
    ui->lineEdit_94->setText("");
    ui->lineEdit_91->setText("");
    ui->lineEdit_93->setText("");
    ui->lineEdit_21->setText("");
    ui->lineEdit_23->setText("");
    ui->lineEdit_26->setText("");
    ui->lineEdit_25->setText("");
    ui->lineEdit_28->setText("");
    ui->lineEdit_29->setText("");
    ui->lineEdit_141->setText("");
    ui->lineEdit_142->setText("");

}

void BCMainWindow::on_Save_Btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);


    QString name, unit ,  wave,rctn,temp, fact, blnk, dely, read, lag, linmin, linmax, normin, normax;
    QString mode, nostd, rpt, con1,con2,con3,con4,con5,con6,con7,con8;
    QString ctrl1avg, ctrl1min, ctrl1max , ctrl1bno, ctrl2avg, ctrl2min,ctrl2max, ctrl2bno;

    name=ui->lineEdit_4->text();
    unit=ui->lineEdit_5->text();
    wave = ui->WaveLength_Combo->currentText();
    rctn=ui->ReactionType_Combo->currentText();
    temp=ui->Temp_Combo->currentText();
    fact=ui->lineEdit_6->text();
    blnk=ui->BlankType_Combo->currentText();
    dely=ui->lineEdit_7->text();
    read=ui->lineEdit_8->text();
    lag=ui->lineEdit_22->text();
    linmin=ui->lineEdit_9->text();
    linmax=ui->lineEdit_10->text();
    normin=ui->lineEdit_12->text();
    normax=ui->lineEdit_11->text();
    mode=ui->comboBox->currentText();
    nostd=ui->comboBox_5->currentText();
    rpt=ui->comboBox_6->currentText();
    con1=ui->lineEdit_88->text();
    con2=ui->lineEdit_92->text();
    con3=ui->lineEdit_89->text();
    con4=ui->lineEdit_95->text();
    con5=ui->lineEdit_90->text();
    con6=ui->lineEdit_94->text();
    con7=ui->lineEdit_91->text();
    con8=ui->lineEdit_93->text();
    ctrl1avg=ui->lineEdit_21->text();
    ctrl1min=ui->lineEdit_23->text();
    ctrl1max=ui->lineEdit_141->text();
    ctrl1bno=ui->lineEdit_26->text();
    ctrl2avg=ui->lineEdit_25->text();
    ctrl2min=ui->lineEdit_28->text();
    ctrl1max=ui->lineEdit_142->text();
    ctrl2bno=ui->lineEdit_29->text();



    QSqlQuery qry;

    qry.prepare("insert into test(name,wave,rctn,unit,temp,fact,blnk,dely,read,lag,linmin,linmax,normin,normax,mode,nostd,rpt,con1,con2,con3,con4,con5,con6,con7,con8,"
                "ctrl1avg,ctrl1min,ctrl1max,ctrl1bno,ctrl2avg,ctrl2min,ctrl2max,ctrl2bno) values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

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
    qry.addBindValue(ctrl1max);
    qry.addBindValue(ctrl1bno);
    qry.addBindValue(ctrl2avg);
    qry.addBindValue(ctrl2min);
    qry.addBindValue(ctrl2max);
    qry.addBindValue(ctrl2bno);
    qry.exec();

    qDebug()<<"Ok";

}


//********************Test Configuration (Edit Button)**************************


void BCMainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);

    ui->gridLayout->update();
    ui->gridLayout_6->update();
    ui->gridLayout_8->update();
    ui->tabWidget_6->setCurrentIndex(0);
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

void BCMainWindow::cald2()
{
    QObject *senderObj = sender(); // This will give Sender object
    QString senderObjName = senderObj->objectName();
    ui->label_16->setText(senderObjName);
    ui->lineEdit_24->setText(senderObjName);
    ui->stackedWidget->setCurrentIndex(6);
    ui->tabWidget_4->setCurrentIndex(0);

    QString unit;
    QSqlQuery query;
    QString  Wave ,Rctn , Blnk , Temp, Fact , Dely, Read, Lag, Linmin, Linmax, Normin, Normax;
    QString Mode , Nostd , Rpt , Con1,Con2,Con3,Con4,Con5,Con6,Con7,Con8;
    QString Ctrl1avg, Ctrl1min,Ctrl1max, Ctrl1bno, Ctrl2avg, Ctrl2min,Ctrl2max, Ctrl2bno , StandAbso;


    QString TestName = ui->label_16->text();

    int  wave =0 ,rctn =0 , temp =0 ,blnk =0,fact = 0,  dely = 0, read = 0, lag = 0, linmin = 0, linmax = 0, normin = 0, normax = 0;
    int  mode = 0, nostd =0, rpt= 0,con1 = 0,con2 = 0,con3 = 0,con4 = 0,con5 = 0,con6 = 0,con7 = 0,con8 = 0;
    int ctrl1avg = 0, ctrl1min = 0,ctrl1max=0, ctrl1bno = 0, ctrl2avg = 0, ctrl2min = 0,ctrl2max=0, ctrl2bno = 0 , StandAbs =0;

    Wave = char (wave);
    Rctn = char (rctn);
    Blnk = char (blnk);
    Temp = char (temp);
    Fact = char (fact);
    Dely=char(dely);
    Read=char(read);
    Lag=char(lag);
    Linmin=char(linmin);
    Linmax=char(linmax);
    Normin=char(normin);
    Normax=char(normax);
    Mode = char (mode);
    Nostd = char (nostd);
    Rpt = char (rpt);
    Con1=char(con1);
    Con2=char(con2);
    Con3=char(con3);
    Con4=char(con4);
    Con5=char(con5);
    Con6=char(con6);
    Con7=char(con7);
    Con8=char(con8);

    Ctrl1avg=char(ctrl1avg);
    Ctrl1min=char(ctrl1min);
    Ctrl1max=char(ctrl1max);
    Ctrl1bno=char(ctrl1bno);
    Ctrl2avg=char(ctrl2avg);
    Ctrl2min=char(ctrl2min);
    Ctrl2max=char(ctrl2max);
    Ctrl2bno=char(ctrl2bno);
    StandAbso=char(StandAbs);

    query.prepare("select * from test where name='"+TestName+"'");
    query.exec();
    while(query.next())
    {
        unit=query.value(4).toString();
        Wave=query.value(2).toString();
        Rctn=query.value(3).toString();
        Temp=query.value(5).toString();
        Fact=query.value(6).toString();
        Blnk=query.value(7).toString();
        Dely=query.value(8).toString();
        Read=query.value(9).toString();
        Lag=query.value(10).toString();
        Linmin=query.value(11).toString();
        Linmax=query.value(12).toString();
        Normin=query.value(13).toString();
        Normax=query.value(14).toString();
        Mode=query.value(15).toString();
        Nostd=query.value(16).toString();
        Rpt=query.value(17).toString();
        Con1=query.value(18).toString();
        Con2=query.value(19).toString();
        Con3=query.value(20).toString();
        Con4=query.value(21).toString();
        Con5=query.value(22).toString();
        Con6=query.value(23).toString();
        Con7=query.value(24).toString();
        Con8=query.value(25).toString();
        Ctrl1avg=query.value(26).toString();
        Ctrl1min=query.value(27).toString();
        Ctrl1max=query.value(35).toString();
        Ctrl1bno=query.value(28).toString();
        Ctrl2avg=query.value(29).toString();
        Ctrl2min=query.value(30).toString();
        Ctrl2max=query.value(34).toString();
        Ctrl2bno=query.value(31).toString();
        StandAbso=query.value(33).toString();

    }

    ui->lineEdit_27->setText(unit);
    ui->WaveLength_Combo_2->setCurrentText(Wave);
    ui->ReactionType_Combo_2->setCurrentText(Rctn);
    ui->Temp_Combo_2->setCurrentText(Temp);
    ui->lineEdit_30->setText(Fact);
    ui->BlankType_Combo_2->setCurrentText(Blnk);
    ui->lineEdit_31->setText(Dely);
    ui->lineEdit_32->setText(Read);
    ui->lineEdit_37->setText(Lag);
    ui->lineEdit_33->setText(Linmin);
    ui->lineEdit_34->setText(Linmax);
    ui->lineEdit_36->setText(Normin);
    ui->lineEdit_35->setText(Normax);
    ui->comboBox_2->setCurrentText(Mode);
    ui->comboBox_7->setCurrentText(Nostd);
    ui->comboBox_8->setCurrentText(Rpt);
    ui->lineEdit_112->setText(Con1);
    ui->lineEdit_116->setText(Con2);
    ui->lineEdit_113->setText(Con3);
    ui->lineEdit_119->setText(Con4);
    ui->lineEdit_114->setText(Con5);
    ui->lineEdit_118->setText(Con6);
    ui->lineEdit_115->setText(Con7);
    ui->lineEdit_117->setText(Con8);
    ui->lineEdit_58->setText(Ctrl1avg);
    ui->lineEdit_59->setText(Ctrl1min);
    ui->lineEdit_143->setText(Ctrl1max);
    ui->lineEdit_60->setText(Ctrl1bno);
    ui->lineEdit_61->setText(Ctrl2avg);
    ui->lineEdit_62->setText(Ctrl2min);
    ui->lineEdit_144->setText(Ctrl2max);
    ui->lineEdit_63->setText(Ctrl2bno);
    ui->lineEdit_104->setText(StandAbso);

}


void BCMainWindow::on_Update_Btn_clicked()
{

    QString name, unit;
    QSqlQuery query;

    QString  Fact, Wave, Rctn ,  Temp,Blnk, Dely, Read, Lag, Linmin, Linmax, Normin, Normax;
    QString Mode, Nostd, Rpt, Con1,Con2,Con3,Con4,Con5,Con6,Con7,Con8;
    QString Ctrl1avg, Ctrl1min,Ctrl1max, Ctrl1bno, Ctrl2avg, Ctrl2min,Ctrl2max, Ctrl2bno;

    int wave = 0, rctn = 0, temp = 0, fact = 0, blnk = 0, dely = 0, read = 0, lag = 0, linmin = 0, linmax = 0, normin = 0, normax = 0;
    int mode = 0, nostd = 0, rpt = 0, con1 = 0,con2 = 0,con3 = 0,con4 = 0,con5 = 0,con6 = 0,con7 = 0,con8 = 0;
    int ctrl1avg = 0, ctrl1min = 0,ctrl1max=0, ctrl1bno = 0, ctrl2avg = 0, ctrl2min = 0,ctrl2max=0, ctrl2bno = 0;


    Wave = char(wave);
    Fact=char(fact);
    Rctn=char(rctn);
    Temp=char(temp);
    Blnk =char (blnk);
    Dely=char(dely);
    Read=char(read);
    Lag=char(lag);
    Linmin=char(linmin);
    Linmax=char(linmax);
    Normin=char(normin);
    Normax=char(normax);
    Mode= char(mode);
    Nostd= char(nostd);
    Rpt= char(rpt);
    Con1=char(con1);
    Con2=char(con2);
    Con3=char(con3);
    Con4=char(con4);
    Con5=char(con5);
    Con6=char(con6);
    Con7=char(con7);
    Con8=char(con8);

    Ctrl1avg=char(ctrl1avg);
    Ctrl1min=char(ctrl1min);
    Ctrl1max=char(ctrl1max);
    Ctrl1bno=char(ctrl1bno);
    Ctrl2avg=char(ctrl2avg);
    Ctrl2min=char(ctrl2min);
    Ctrl2max=char(ctrl2max);
    Ctrl2bno=char(ctrl2bno);


    name = ui->lineEdit_24->text();
    unit=ui->lineEdit_27->text();
    Wave=ui->WaveLength_Combo_2->currentText();
    Rctn=ui->ReactionType_Combo_2->currentText();
    Temp=ui->Temp_Combo_2->currentText();
    Fact=ui->lineEdit_30->text();
    Blnk=ui->BlankType_Combo_2->currentText();
    Dely=ui->lineEdit_31->text();
    Read=ui->lineEdit_32->text();
    Lag=ui->lineEdit_37->text();
    Linmin=ui->lineEdit_33->text();
    Linmax=ui->lineEdit_34->text();
    Normin=ui->lineEdit_36->text();
    Normax=ui->lineEdit_35->text();
    Mode=ui->comboBox_2->currentText();
    Nostd=ui->comboBox_7->currentText();
    Rpt=ui->comboBox_8->currentText();
    Con1=ui->lineEdit_112->text();
    Con2=ui->lineEdit_116->text();
    Con3=ui->lineEdit_113->text();
    Con4=ui->lineEdit_119->text();
    Con5=ui->lineEdit_114->text();
    Con6=ui->lineEdit_118->text();
    Con7=ui->lineEdit_115->text();
    Con8=ui->lineEdit_117->text();
    Ctrl1avg=ui->lineEdit_58->text();
    Ctrl1min=ui->lineEdit_59->text();
    Ctrl1max=ui->lineEdit_143->text();
    Ctrl1bno=ui->lineEdit_60->text();
    Ctrl2avg=ui->lineEdit_61->text();
    Ctrl2min=ui->lineEdit_62->text();
    Ctrl2max=ui->lineEdit_144->text();
    Ctrl2bno=ui->lineEdit_63->text();

    query.prepare("update test set name='"+name+"',unit='"+unit+"',wave='"+Wave+"', rctn='"+Rctn+"', temp='"+Temp+"', fact='"+Fact+"', blnk='"+Blnk+"', dely='"+Dely+"', read='"+Read+"', lag='"+Lag+"',linmin='"+Linmin+"', linmax='"+Linmax+"', normin='"+Normin+"', normax='"+Normax+"', mode='"+Mode+"', nostd='"+Nostd+"', rpt='"+Rpt+"', con1='"+Con1+"',con2='"+Con2+"',con3='"+Con3+"',con4='"+Con4+"',con5='"+Con5+"',con6='"+Con6+"',con7='"+Con7+"',con8='"+Con8+"',ctrl1avg='"+Ctrl1avg+"', ctrl1min='"+Ctrl1min+"',ctrl1max='"+Ctrl1max+"', ctrl1bno='"+Ctrl1bno+"', ctrl2avg='"+Ctrl2avg+"', ctrl2min='"+Ctrl2min+"', ctrl2max='"+Ctrl2max+"',ctrl2bno='"+Ctrl2bno+"' where name='"+name+"'");
    if(query.exec())
    {
        QMessageBox::information(this,tr("Update"),tr("Updated"));
        //conn.connclose();
        ui->gridLayout->update();
        ui->gridLayout_6->update();
        ui->gridLayout_8->update();
        qDebug()<<"Update done";
        ui->stackedWidget->setCurrentIndex(0);
    }
    else
    {

    }


}

//********************Test Configuration (Delete Button)**************************



void BCMainWindow::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
    ui->gridLayout->update();
    ui->gridLayout_6->update();
    ui->gridLayout_8->update();
    ui->tabWidget_7->setCurrentIndex(0);
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

void BCMainWindow::cald3()
{
    QObject *senderObj = sender(); // This will give Sender object
    QString senderObjName = senderObj->objectName();

    ui->lineEdit_38->setText(senderObjName);
    ui->stackedWidget->setCurrentIndex(8);
    ui->tabWidget_5->setCurrentIndex(0);

    QString name, unit;
    QSqlQuery query;
    QString  Wave ,Rctn , Blnk , Temp, Fact , Dely, Read, Lag, Linmin, Linmax, Normin, Normax;
    QString Mode , Nostd , Rpt , Con1,Con2,Con3,Con4,Con5,Con6,Con7,Con8;
    QString Ctrl1avg, Ctrl1min,Ctrl1max, Ctrl1bno, Ctrl2avg, Ctrl2min,Ctrl2max, Ctrl2bno , StandAbso;


    QString TestName = ui->lineEdit_38->text();

    int  wave =0 ,rctn =0 , temp =0 ,blnk =0,fact = 0,  dely = 0, read = 0, lag = 0, linmin = 0, linmax = 0, normin = 0, normax = 0;
    int  mode = 0, nostd =0, rpt= 0,con1 = 0,con2 = 0,con3 = 0,con4 = 0,con5 = 0,con6 = 0,con7 = 0,con8 = 0;
    int ctrl1avg = 0, ctrl1min = 0,ctrl1max=0, ctrl1bno = 0, ctrl2avg = 0, ctrl2min = 0,ctrl2max=0, ctrl2bno = 0 , StandAbs = 0;

    Wave = char (wave);
    Rctn = char (rctn);
    Blnk = char (blnk);
    Temp = char (temp);
    Fact = char (fact);
    Dely=char(dely);
    Read=char(read);
    Lag=char(lag);
    Linmin=char(linmin);
    Linmax=char(linmax);
    Normin=char(normin);
    Normax=char(normax);
    Mode = char (mode);
    Nostd = char (nostd);
    Rpt = char (rpt);
    Con1=char(con1);
    Con2=char(con2);
    Con3=char(con3);
    Con4=char(con4);
    Con5=char(con5);
    Con6=char(con6);
    Con7=char(con7);
    Con8=char(con8);

    Ctrl1avg=char(ctrl1avg);
    Ctrl1min=char(ctrl1min);
    Ctrl1max=char(ctrl1max);
    Ctrl1bno=char(ctrl1bno);
    Ctrl2avg=char(ctrl2avg);
    Ctrl2min=char(ctrl2min);
    Ctrl2max=char(ctrl2max);
    Ctrl2bno=char(ctrl2bno);
    StandAbso= char(StandAbs);

    query.prepare("select * from test where name='"+TestName+"'");
    query.exec();
    while(query.next())
    {
        unit=query.value(4).toString();
        Wave=query.value(2).toString();
        Rctn=query.value(3).toString();
        Temp=query.value(5).toString();
        Fact=query.value(6).toString();
        Blnk=query.value(7).toString();
        Dely=query.value(8).toString();
        Read=query.value(9).toString();
        Lag=query.value(10).toString();
        Linmin=query.value(11).toString();
        Linmax=query.value(12).toString();
        Normin=query.value(13).toString();
        Normax=query.value(14).toString();
        Mode=query.value(15).toString();
        Nostd=query.value(16).toString();
        Rpt=query.value(17).toString();
        Con1=query.value(18).toString();
        Con2=query.value(19).toString();
        Con3=query.value(20).toString();
        Con4=query.value(21).toString();
        Con5=query.value(22).toString();
        Con6=query.value(23).toString();
        Con7=query.value(24).toString();
        Con8=query.value(25).toString();
        Ctrl1avg=query.value(26).toString();
        Ctrl1min=query.value(27).toString();
        Ctrl1max=query.value(35).toString();
        Ctrl1bno=query.value(28).toString();
        Ctrl2avg=query.value(29).toString();
        Ctrl2min=query.value(30).toString();
        Ctrl2max=query.value(34).toString();
        Ctrl2bno=query.value(31).toString();
        StandAbso=query.value(33).toString();

    }

    ui->lineEdit_39->setText(unit);
    ui->WaveLength_Combo_3->setCurrentText(Wave);
    ui->ReactionType_Combo_3->setCurrentText(Rctn);
    ui->Temp_Combo_3->setCurrentText(Temp);
    ui->lineEdit_40->setText(Fact);
    ui->BlankType_Combo_3->setCurrentText(Blnk);
    ui->lineEdit_41->setText(Dely);
    ui->lineEdit_54->setText(Read);
    ui->lineEdit_64->setText(Lag);
    ui->lineEdit_55->setText(Linmin);
    ui->lineEdit_56->setText(Linmax);
    ui->lineEdit_57->setText(Normin);
    ui->lineEdit_65->setText(Normax);
    ui->comboBox_3->setCurrentText(Mode);
    ui->comboBox_9->setCurrentText(Nostd);
    ui->comboBox_10->setCurrentText(Rpt);
    ui->lineEdit_128->setText(Con1);
    ui->lineEdit_132->setText(Con2);
    ui->lineEdit_129->setText(Con3);
    ui->lineEdit_135->setText(Con4);
    ui->lineEdit_130->setText(Con5);
    ui->lineEdit_134->setText(Con6);
    ui->lineEdit_131->setText(Con7);
    ui->lineEdit_133->setText(Con8);
    ui->lineEdit_66->setText(Ctrl1avg);
    ui->lineEdit_67->setText(Ctrl1min);
    ui->lineEdit_145->setText(Ctrl1max);
    ui->lineEdit_68->setText(Ctrl1bno);
    ui->lineEdit_69->setText(Ctrl2avg);
    ui->lineEdit_70->setText(Ctrl2min);
    ui->lineEdit_144->setText(Ctrl2max);
    ui->lineEdit_71->setText(Ctrl2bno);
    ui->lineEdit_120->setText(StandAbso);

}

void BCMainWindow::on_Delete_Btn_clicked()
{
    //BCMainWindow conn;
    QString TestName=ui->lineEdit_38->text();
    //    if(!conn.connOpen())
    //     {
    //         qDebug()<<"failed to open the database";
    //         return;
    //     }
    //     conn.connOpen();
    QSqlQuery query;
    query.prepare("Delete from test where name='"+TestName+"'");
    if(query.exec())
    {
        QMessageBox::information(this,tr("Delete"),tr("Delete"));
        // conn.connclose();
        ui->gridLayout->update();
        ui->tableView->update();
        ui->gridLayout_2->update();
        ui->gridLayout_6->update();
        ui->gridLayout_8->update();
        ui->stackedWidget->setCurrentIndex(0);
    }
    else
    {
        QMessageBox::critical(this,tr("error::"),query.lastError().text());
    }
}

