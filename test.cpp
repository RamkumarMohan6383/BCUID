#include "bcmainwindow.h"
#include "ui_bcmainwindow.h"
static QString array[100]={""};



void BCMainWindow::on_Test_Btn_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->tabWidget->setCurrentIndex(0);
    ui->gridLayout->update();
    ui->gridLayout_6->update();
    ui->gridLayout_8->update();
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
                connect(button, &QPushButton::clicked, this,&BCMainWindow::cald);
                ui->gridLayout->addWidget(button,i,j);
                count1++;
            }
        }
    }
}

void BCMainWindow::cald()
{
    QString name, unit , Fact , Rctn , EndPoint , TwoPoint, Kinetic , Blnk , Calibrate , Wave;
    unsigned int fact = 0  , blnk = 0 , Cal = 0 , wave=0;
    Fact = char (fact);
    Blnk =  char (blnk);
    Calibrate = char (Cal);
    Wave = char (wave);
    QObject *senderObj = sender(); // This will give Sender object
    QString senderObjName = senderObj->objectName();
    QString TestName=senderObjName;
    ui->label_33->setText("");
    ui->label_5->setText("");
    ui->label_6->setText("");
    ui->label_208->setText("");
    ui->label_120->setText("");
    ui->label_122->setText("");
    ui->label_119->setText("");
    ui->label_106->setText("");
    ui->label_121->setText("");
    ui->label_123->setText("");
    ui->label_125->setText("");
    ui->label_127->setText("");
    ui->label_128->setText("");
    ui->label_204->setText("");
    ui->label_195->setText("");
    ui->label_194->setText("");
    ui->label_198->setText("");
    ui->PatientInfo_LineEdit->setText("");
    ui->PatientInfo_LineEdit_2->setText("");
    ui->PatientInfo_LineEdit_3->setText("");
    name=ui->TestName_Lbl->text();
    name=ui->TestName_Lbl_5->text();
    name=ui->TestName_Lbl_6->text();
    QSqlQuery query;
    query.prepare("select * from test where name='"+TestName+"'");
    query.exec();
    while(query.next())
    {
        unit=query.value(4).toString();
        Rctn=query.value(3).toString();
        Fact = query.value(6).toString();
        Blnk = query.value(32).toString();
        Calibrate = query.value(33).toString();
        Wave = query.value(2).toString();

    }
    ui->Unit_lineEdit->setText(unit);
    ui->label->setText(Blnk);
    ui->label_196->setText(Fact);
    ui->label_76->setText(Fact);
    if(Rctn == "EndPoint" )
    {
        ui->stackedWidget->setCurrentIndex(2);
        ui->TestName_Lbl->setText(TestName);
        clearGraph_end();
        ui->RunSample_Btn->setEnabled(false);
        ui->RunSample_Btn_2->setEnabled(false);
        ui->RunSample_Btn_3->setEnabled(false);
        ui->label_5->setVisible(false);
        ui->label_6->setVisible(false);
        ui->label_203->setVisible(false);
        ui->label_204->setVisible(false);
        ui->TestName_Lbl->setVisible(true); // End_Point Btn Hide
        ui->Save_Btn_2->setVisible(true);
        ui->Patient_Info_Lbl_4->setVisible(true);
        ui->pushButton_117->setVisible(true);
        ui->PatientInfo_LineEdit->setVisible(true);
        ui->RunBlank_Btn->setVisible(true);
        ui->label_193->setVisible(true);
        ui->label->setVisible(true);
        ui->RunCal_Btn->setVisible(true);
        ui->label_100->setVisible(true);
        ui->label_34->setVisible(true);
        ui->label_196->setVisible(true);
        ui->label_197->setVisible(true);
        ui->Save_Btn_5->setVisible(true);
        ui->RunSample_Btn->setVisible(true);
        ui->label_33->setVisible(true);
        ui->Unit_lineEdit->setVisible(true);
        ui->Print_Btn->setVisible(true);
        ui->label_5->setVisible(false);
        ui->label_6->setVisible(false);
        ui->label_207->setVisible(false);
        ui->label_209->setVisible(false);
        ui->label_7->setVisible(false);
        ui->label_32->setVisible(false);
        ui->label_12->setVisible(false);
        ui->label_205->setVisible(false);
        ui->label_206->setVisible(false);
        ui->label_34->setText("");
        ui->label_196->setText("");
        ui->label_208->setVisible(false);
        // Two_Point Btn Hide

        ui->TestName_Lbl_5->setVisible(false);
        ui->Save_Btn_3->setVisible(false);
        ui->Patient_Info_Lbl_5->setVisible(false);
        ui->pushButton_202->setVisible(false);
        ui->PatientInfo_LineEdit_2->setVisible(false);
        ui->RunBlank_Btn_2->setVisible(false);
        ui->RunCal_Btn_2->setVisible(false);
        ui->label_197->setVisible(false);
        ui->Save_Btn_6->setVisible(false);
        ui->RunSample_Btn_2->setVisible(false);
        ui->Print_Btn_2->setVisible(false);
        ui->label_120->setVisible(false);
        ui->label_122->setVisible(false);
        ui->label_72->setVisible(false);
        ui->label_119->setVisible(false);
        ui->TestName_Lbl_6->setVisible(false); //Kinetic Btn _Hide
        ui->Save_Btn_4->setVisible(false);
        ui->Patient_Info_Lbl_6->setVisible(false);
        ui->pushButton_203->setVisible(false);
        ui->PatientInfo_LineEdit_3->setVisible(false);
        ui->RunBlank_Btn_3->setVisible(false);
        ui->RunCal_Btn_3->setVisible(false);
        ui->label_98->setVisible(false);
        ui->label_75->setVisible(false);
        ui->label_197->setVisible(false);
        ui->Save_Btn_7->setVisible(false);
        ui->RunSample_Btn_3->setVisible(false);
        ui->Print_Btn_3->setVisible(false);
        ui->label_106->setVisible(false);
        ui->label_121->setVisible(false);
        ui->label_123->setVisible(false);
        ui->label_125->setVisible(false);
        ui->label_97->setVisible(false);
        ui->label_127->setVisible(false);
        ui->label_98->setVisible(false);
        ui->label_128->setVisible(false);
        ui->label_99->setVisible(false);
        ui->label_132->setVisible(false);
        on_pushButton_225_clicked();
//        if(Wave==340)
//            digitalWrite (LED_BASE + 8,HIGH) ;

//        else if(Wave==405)

//            digitalWrite (LED_BASE + 9,HIGH) ;

//        else if(Wave==507)

//            digitalWrite (LED_BASE + 10,HIGH) ;

//        else if(Wave==545)

//            digitalWrite (LED_BASE + 11,HIGH) ;

//        else if(Wave==572)

//            digitalWrite (LED_BASE + 12,HIGH) ;

//        else if(Wave==628)

//            digitalWrite (LED_BASE + 13,HIGH) ;

//        else if(Wave==700)

//            digitalWrite (LED_BASE + 14,HIGH) ;

//        qDebug()<<Wave;

        if(Fact == "")
        {
            ui->RunCal_Btn->setVisible(true);
            ui->RunCal_Btn_2->setVisible(false);
            ui->RunCal_Btn_3->setVisible(false);
            ui->label_100->setText("Std OD");
            ui->label_34->setText(Calibrate);
            ui->label_197->setVisible(true);
            ui->label_196->setVisible(true);
            ui->label_76->setVisible(false);
            ui->RunSample_Btn->setEnabled(false);
            ui->RunSample_Btn_2->setEnabled(false);
            ui->RunSample_Btn_3->setEnabled(false);
        }
        else
        {
            ui->RunCal_Btn->setVisible(true);
            ui->RunCal_Btn_2->setVisible(false);
            ui->RunCal_Btn_3->setVisible(false);
            ui->label_76->setVisible(true);
            ui->label_100->setText("Factor");
            ui->label_196->setText(Fact);
            ui->label_76->setText(Fact);
            ui->label_196->setVisible(false);
            ui->label_197->setVisible(false);
        }




    }

    else if(Rctn == "TwoPoint" )
    {

        ui->stackedWidget->setCurrentIndex(2);
        ui->TestName_Lbl_5->setText(TestName);
        clearGraph_end();
        ui->RunSample_Btn->setEnabled(false);
        ui->RunSample_Btn_2->setEnabled(false);
        ui->RunSample_Btn_3->setEnabled(false);
        ui->RunCal_Btn_2->setEnabled(true);
        ui->label_75->setVisible(false);
        ui->label_208->setVisible(false);
        ui->label_209->setVisible(false);
        ui->label_72->setVisible(false);
        ui->label_119->setVisible(false);
        ui->TestName_Lbl_5->setVisible(true); // Two_Point Btn Hide
        ui->Save_Btn_3->setVisible(true);
        ui->Patient_Info_Lbl_5->setVisible(true);
        ui->pushButton_202->setVisible(true);
        ui->PatientInfo_LineEdit_2->setVisible(true);
        ui->RunBlank_Btn_2->setVisible(true);
        ui->RunCal_Btn_2->setVisible(true);
        ui->label_197->setVisible(true);
        ui->Save_Btn_6->setVisible(true);
        ui->RunSample_Btn_2->setVisible(true);
        ui->Print_Btn_2->setVisible(true);
        ui->label_120->setVisible(true);
        ui->label_122->setVisible(true);
        ui->label_34->setText("");
        ui->label_196->setText("");
        ui->label_203->setVisible(false);
        ui->label_204->setVisible(false);
        ui->TestName_Lbl->setVisible(false); // End_Point Btn Hide
        ui->Save_Btn_2->setVisible(false);
        ui->Patient_Info_Lbl_4->setVisible(false);
        ui->pushButton_117->setVisible(false);
        ui->PatientInfo_LineEdit->setVisible(false);
        ui->RunBlank_Btn->setVisible(false);
        ui->label->setVisible(true);
        ui->RunCal_Btn->setVisible(false);
        ui->label_100->setVisible(true);
        ui->label_34->setVisible(true);
        ui->label_197->setVisible(false);
        ui->label_207->setVisible(false);
        ui->label_196->setVisible(true);
        ui->Save_Btn_5->setVisible(false);
        ui->RunSample_Btn->setVisible(false);
        ui->label_33->setVisible(true);
        ui->Unit_lineEdit->setVisible(true);
        ui->Print_Btn->setVisible(false);
        ui->label_5->setVisible(false);
        ui->label_6->setVisible(false);
        ui->label_7->setVisible(false);
        ui->TestName_Lbl_6->setVisible(false); //Kinetic Btn _Hide
        ui->Save_Btn_4->setVisible(false);
        ui->Patient_Info_Lbl_6->setVisible(false);
        ui->pushButton_203->setVisible(false);
        ui->PatientInfo_LineEdit_3->setVisible(false);
        ui->RunBlank_Btn_3->setVisible(false);
        ui->RunCal_Btn_3->setVisible(false);
        ui->label_98->setVisible(false);
        ui->label_197->setVisible(false);
        ui->Save_Btn_7->setVisible(false);
        ui->RunSample_Btn_3->setVisible(false);
        ui->Print_Btn_3->setVisible(false);
        ui->label_106->setVisible(false);
        ui->label_121->setVisible(false);
        ui->label_123->setVisible(false);
        ui->label_125->setVisible(false);
        ui->label_97->setVisible(false);
        ui->label_127->setVisible(false);
        ui->label_98->setVisible(false);
        ui->label_128->setVisible(false);
        ui->label_99->setVisible(false);
        ui->label_132->setVisible(false);
        on_pushButton_225_clicked();
        if(Fact == "")
        {
            ui->RunCal_Btn_2->setVisible(true);
            ui->label_203->setText("ΔA/min");
            ui->label_100->setText("Std OD");
            ui->label_34->setText(Calibrate);
            ui->label_197->setVisible(true);
            ui->label_203->setVisible(false);
            ui->label_204->setVisible(false);
            ui->label_205->setVisible(false);
            ui->label_206->setVisible(false);
            ui->label_76->setVisible(false);
            ui->RunSample_Btn->setEnabled(false);
            ui->RunSample_Btn_2->setEnabled(false);
            ui->RunSample_Btn_3->setEnabled(false);
        }
        else
        {
            ui->RunCal_Btn_2->setVisible(true);
            ui->label_100->setText("Factor");
            ui->label_196->setText(Fact);
            ui->label_76->setText(Fact);
            ui->label_197->setVisible(false);
            ui->label_203->setVisible(false);
            ui->label_204->setVisible(false);
            ui->label_76->setVisible(true);
            ui->label_205->setVisible(false);
            ui->label_206->setVisible(false);
            ui->label_196->setVisible(false);
        }
    }
    else if(Rctn == "Kinetic" )
    {
        ui->stackedWidget->setCurrentIndex(2);
        ui->TestName_Lbl_6->setText(TestName);
        clearGraph_end();
        ui->RunSample_Btn->setEnabled(false);
        ui->RunSample_Btn_2->setEnabled(false);
        ui->RunSample_Btn_3->setEnabled(false);
        ui->label_194->setVisible(false);
        ui->label_195->setVisible(false);
        ui->label_198->setVisible(false);
        ui->label_75->setVisible(false);
        ui->label_205->setVisible(false);
        ui->label_206->setVisible(false);
        ui->label_208->setVisible(false);
        ui->label_209->setVisible(false);
        ui->label_97->setVisible(false);
        ui->label_98->setVisible(false);
        ui->label_127->setVisible(false);
        ui->label_128->setVisible(false);
        ui->TestName_Lbl_6->setVisible(true);  //Kinetic Btn _Hide
        ui->Save_Btn_4->setVisible(true);
        ui->Patient_Info_Lbl_6->setVisible(true);
        ui->pushButton_203->setVisible(true);
        ui->PatientInfo_LineEdit_3->setVisible(true);
        ui->RunBlank_Btn_3->setVisible(true);
        ui->RunCal_Btn_3->setVisible(true);
        ui->label_98->setVisible(true);
        ui->label_197->setVisible(false);
        ui->label_207->setVisible(false);
        ui->Save_Btn_7->setVisible(true);
        ui->RunSample_Btn_3->setVisible(true);
        ui->Print_Btn_3->setVisible(true);
        ui->label_106->setVisible(false);
        ui->label_121->setVisible(false);
        ui->label_123->setVisible(false);
        ui->label_12->setVisible(true);
        ui->label_32->setVisible(true);
        ui->label_125->setVisible(false);
        ui->label_97->setVisible(true);
        ui->label_127->setVisible(true);
        ui->label_98->setVisible(true);
        ui->label_128->setVisible(true);
        ui->label_99->setVisible(true);
        ui->label_132->setVisible(true);
        ui->TestName_Lbl->setVisible(true); // End_Point Btn Hide
        ui->Save_Btn_2->setVisible(false);
        ui->Patient_Info_Lbl_4->setVisible(false);
        ui->pushButton_117->setVisible(false);
        ui->PatientInfo_LineEdit->setVisible(false);
        ui->RunBlank_Btn->setVisible(false);
        ui->label->setVisible(true);
        ui->RunCal_Btn->setVisible(false);
        ui->label_100->setVisible(true);
        ui->label_34->setVisible(true);
        ui->label_197->setVisible(true);
        ui->label_196->setVisible(true);
        ui->Save_Btn_5->setVisible(false);
        ui->RunSample_Btn->setVisible(false);
        ui->label_33->setVisible(true);
        ui->Unit_lineEdit->setVisible(true);
        ui->Print_Btn->setVisible(false);
        ui->label_5->setVisible(false);
        ui->label_6->setVisible(false);
        ui->label_7->setVisible(false);
        ui->TestName_Lbl_5->setVisible(false); // Two_Point Btn Hide
        ui->Save_Btn_3->setVisible(false);
        ui->Patient_Info_Lbl_5->setVisible(false);
        ui->pushButton_202->setVisible(false);
        ui->PatientInfo_LineEdit_2->setVisible(false);
        ui->RunBlank_Btn_2->setVisible(false);
        ui->RunCal_Btn_2->setVisible(false);
        ui->label_197->setVisible(false);
        ui->Save_Btn_6->setVisible(false);
        ui->RunSample_Btn_2->setVisible(false);
        ui->Print_Btn_2->setVisible(false);
        ui->label_120->setVisible(false);
        ui->label_122->setVisible(false);
        ui->label_72->setVisible(false);
        ui->label_119->setVisible(false);
        ui->label_34->setText("");
        ui->label_196->setText("");
        on_pushButton_225_clicked();
        if(Fact == "")
        {
            ui->RunCal_Btn_3->setVisible(true);
            ui->label_197->setVisible(true);
            ui->label_205->setVisible(true);
            ui->label_206->setVisible(true);
            ui->label_100->setText("Std OD");
            ui->label_203->setText("ΔA/min");
            ui->label_34->setText(Calibrate);
            ui->label_76->setVisible(false);
            ui->RunSample_Btn->setEnabled(false);
            ui->RunSample_Btn_2->setEnabled(false);
            ui->RunSample_Btn_3->setEnabled(false);
        }
        else
        {
            ui->RunCal_Btn_3->setVisible(true);
            ui->label_100->setText("Factor");
            ui->label_76->setText(Fact);
            ui->label_76->setVisible(true);
            ui->label_99->setVisible(false);
            ui-> label_203->setText("ΔA/min");
            ui->label_132->setVisible(false);
            ui->label_196->setVisible(false);
        }
    }

}

/*void BCMainWindow::createActionsAndConnections()
{
    QAction* recentFileAction = 0;
        for(auto i = 0; i < maxFileNr; ++i){
            recentFileAction = new QAction(this);
            recentFileAction->setVisible(false);
            QObject::connect(recentFileAction, &QAction::triggered,
                             this, BCMainWindow::openRecent);
            recentFileActionList.append(recentFileAction);
        }
}*/
