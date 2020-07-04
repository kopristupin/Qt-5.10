#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

QString s= "engine2";

    QPixmap ndw(":/icons/leftNDW.png");
    QPixmap pres(":/icons/pressure.png");
    QPixmap temp(":/icons/leftTemperatures.png");
    QPixmap addOr(":/icons/addOrder.png");
    QPixmap ord(":/icons/order.png");
    QPixmap makeEn(":/icons/makeEntry.png");
    QPixmap schedule(":/icons/leftSchedule.png");
    QPixmap journal(":/icons/leftJoe.png");
    QPixmap listOfDetails(":/icons/listOfParts.png");
    QPixmap fuelPix(":/icons/fuelCRight.png");

    cLbl->setPixmap(ndw);
    cLbl->setScaledContents(true);
    cLbl->setMinimumSize(251,81);
    cLbl->setMaximumSize(251,81);

    pr->setPixmap(pres);
    pr->setScaledContents(true);
    pr->setMinimumSize(251,81);
    pr->setMaximumSize(251,81);

    tem->setPixmap(temp);
    tem->setScaledContents(true);
    tem->setMinimumSize(251,81);
    tem->setMaximumSize(251,81);

    newOr->setPixmap(addOr);
    newOr->setScaledContents(true);
    newOr->setMinimumSize(251,81);
    newOr->setMaximumSize(251,81);


    order->setPixmap(ord);
    order->setScaledContents(true);
    order->setMinimumSize(251,81);
    order->setMaximumSize(251,81);


    makeEnt->setPixmap(makeEn);
    makeEnt->setScaledContents(true);
    makeEnt->setMinimumSize(251,81);
    makeEnt->setMaximumSize(251,81);


    sched->setPixmap(schedule);
    sched->setScaledContents(true);
    sched->setMinimumSize(251,81);
    sched->setMaximumSize(251,81);

    journalEv->setPixmap(journal);
    journalEv->setScaledContents(true);
    journalEv->setMinimumSize(251,81);
    journalEv->setMaximumSize(251,81);

    listP->setPixmap(listOfDetails);
    listP->setScaledContents(true);
    listP->setMinimumSize(251,81);
    listP->setMaximumSize(251,81);

    fuelConsumption->setPixmap(fuelPix);
    fuelConsumption->setScaledContents(true);
    fuelConsumption->setMinimumSize(251,81);
    fuelConsumption->setMaximumSize(251,81);


    ui->rightL->addWidget(fuelConsumption);
    ui->rightL->addWidget(cLbl);
    ui->testL->addWidget(pr);
    ui->rightL->addWidget(tem);
    ui->testL->addWidget(newOr);
    ui->testL->addWidget(order);
    ui->testL->addWidget(makeEnt);
    ui->rightL->addWidget(sched);
    ui->rightL->addWidget(journalEv);
    ui->testL->addWidget(listP);








    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(UpdateTime()));
    timer->start(1000);

    QPushButton* b1= new QPushButton("Diagram 1");
    b1->move(50, 50);
    QPushButton* b2= new QPushButton;
    QPushButton* b3= new QPushButton;
    QPushButton* b4= new QPushButton;
    QPushButton* b5= new QPushButton;
    QPushButton* b6= new QPushButton;

    QHBoxLayout* lay= new QHBoxLayout;
    lay->addWidget(b1);
    lay->addWidget(b2);
    lay->addWidget(b3);
    lay->addWidget(b4);
    lay->addWidget(b5);
    lay->addWidget(b6);
    lay->setStretch(5,1);


    sch = QSqlDatabase::addDatabase("QODBC");
    sch.setDatabaseName("DRIVER={SQL Server}; SERVER=WIN-J524RTIN700\\SQLEXPRESS; DATABASE=database; Trusted_Connection=Yes;");


    if(!sch.open()){
        qDebug()<<" Temp was not opened!";
    }

    QSqlQuery query;
    QString querStr="SELECT NAME, DATE FROM dataV;";
    query.exec(querStr);
    QSqlRecord rec= query.record();
    while(query.next()){
        QString s;
        s= query.value(rec.indexOf("NAME")).toString()+" "+
                query.value(rec.indexOf("DATE")).toString();
        ui->schWorks->append(s);
        ui->schWorks_2->append(s);
    }

     showLastTemps("Temperatures");// fill labels with last temperatures

     query.exec("SELECT status, dateTime, wh FROM workHours WHERE dateTime= (SELECT MAX(dateTime) FROM workHours WHERE AE='AE1')");
     query.next();
        if(query.value(0).toString()=="sta"){

     QDateTime dt =  query.value(1).toDateTime();
     int wh= query.value(2).toInt();
     wh= wh+ (dt.secsTo(QDateTime::currentDateTime())/3600);
     ui->workHours->setText(QString::number(wh));
     ui->engineStatus->setText("Engine is in operation");
}
         else {
             ui->workHours->setText(query.value(2).toString());
              ui->engineStatus->setText("Engine is out of operation");
         }



    QObject::connect(cLbl, SIGNAL(labelClicked()), this, SLOT(on_NDWorksB_clicked()));
    QObject::connect(pr, SIGNAL(labelClicked()), this, SLOT(on_Pressures_clicked()));
    QObject::connect(tem, SIGNAL(labelClicked()), this, SLOT(on_gr1_clicked()));
    QObject::connect(newOr, SIGNAL(labelClicked()), this, SLOT(on_addOrder_clicked()));
    QObject::connect(order, SIGNAL(labelClicked()), this, SLOT(on_showOrder_clicked()));
    QObject::connect(makeEnt, SIGNAL(labelClicked()), this, SLOT(on_pushButton_clicked()));
    QObject::connect(sched, SIGNAL(labelClicked()), this, SLOT(on_pushButton_3_clicked()));
    QObject::connect(journalEv, SIGNAL(labelClicked()), this, SLOT(on_archieve_clicked()));
    connect(listP, SIGNAL(labelClicked()), this, SLOT(listClicked()));
    connect(fuelConsumption, SIGNAL(labelClicked()), this, SLOT(fuelConsClicked()));



    QObject::connect(dial_ptr.data() , SIGNAL(archiveData(QString,QString, QString, QString)), archives_ptr.data() , SLOT(getData(QString,QString, QString, QString)));


    QObject::connect(this, SIGNAL(dateChanged(QDate)), dial_ptr.data(), SLOT(checkDate(QDate)));
    QObject::connect(dial_ptr.data(), SIGNAL(notDoneWork(QString,QString, QString, QString)), NDWEntry, SLOT(notDoneWorkAdd(QString,QString, QString, QString)));
    QObject::connect(dial_ptr.data(), SIGNAL(notDoneWork(QString,QString, QString, QString)), this, SLOT(changeIconNDW()));
    QObject::connect(ui->comment, SIGNAL(textChanged()), this, SLOT(changeText()));

    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::white);
    this-> setAutoFillBackground(true);
    this->setPalette(pal);




    QFile commentsC("C:/Users/USER/Documents/Qt/analizer/comments.txt");
    commentsC.open(QIODevice::ReadOnly);
    QTextStream cons(&commentsC);
    QString str;
    str= cons.readAll();
    ui->comment->setText(str);

    query.exec("SELECT MAX([date]) FROM Archive WHERE AE='AE1' and NAME='oil replacement'");
    query.next();
    lbltxt->setText(query.value(0).toDate().toString("dd/MM/yyyy"));
    ui->testlyt->addWidget(lbltxt);


    query.exec("SELECT MAX([date]) FROM Archive WHERE AE='AE1' and NAME= 'overhaul'");
    query.next();
    lblOverh->setText(query.value(0).toDate().toString("dd/MM/yyyy"));
    ui->overhLyt->addWidget(lblOverh);

    query.exec("SELECT MAX([date]) FROM Archive WHERE AE='AE1' and NAME= 'turbine overhaul'");
    query.next();
    lblOverhTurb->setText(query.value(0).toDate().toString("dd/MM/yyyy"));
    ui->turbOverhLyt->addWidget(lblOverhTurb);




}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showLastTemps(QString tableNameT){
    QSqlQuery query;
    QString lTStr={"SELECT * FROM "+ tableNameT+" WHERE Id= (SELECT Max(Id) From "+tableNameT+")"};
    qDebug()<< lTStr;
    if(!query.exec(lTStr))
        qDebug()<<"ERROR FROM query labels";
    query.next();
        if(tableNameT=="Temperatures"){
   ui->t1->setText(query.value(1).toString());
   ui->t2->setText(query.value(2).toString());
   ui->t3->setText(query.value(3).toString());
   ui->t4->setText(query.value(4).toString());
   ui->t5->setText(query.value(5).toString());
   ui->t6->setText(query.value(6).toString());
        }
       else{
            ui->t1_2->setText(query.value(1).toString());
            ui->t2_2->setText(query.value(2).toString());
            ui->t3_2->setText(query.value(3).toString());
            ui->t4_2->setText(query.value(4).toString());
            ui->t5_2->setText(query.value(5).toString());
            ui->t6_2->setText(query.value(6).toString());
        }
}




void MainWindow::on_archieve_clicked()
{

    archives_ptr->show();
}

void MainWindow::paintEvent(QPaintEvent* ){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

}

void MainWindow::changeIconNDW(){
    QPixmap ndwA(":/icons/NDWatt.png");
    cLbl->setPixmap(ndwA);
}

void MainWindow::changeText(){

}



void MainWindow::on_pushButton_3_clicked()
{
    dial_ptr->show();
}



void MainWindow::on_gr1_clicked()
{
 chart_ptr->show();
  }


void MainWindow::UpdateTime()
{
    ui->timeL->setText((QDate::currentDate().toString("dd/MM/yyyy"))+" "+(QTime::currentTime().toString("hh:mm:ss")));
      ui->timeL_2->setText((QDate::currentDate().toString("dd/MM/yyyy"))+" "+(QTime::currentTime().toString("hh:mm:ss")));
    QString dateC=QDate::currentDate().toString("dd/MM/yyyy");
    QDate date=QDate::currentDate();

    emit dateChanged(date);
}



void MainWindow::on_NDWorksB_clicked()
{
    NDWEntry->show();
}

void MainWindow::on_pushButton_clicked()
{
   makeEntry* me= new makeEntry();
   me->show();

}



void MainWindow::keyPressEvent(QKeyEvent* e){
    switch(e->key()){
    case Qt::Key_Enter:
        qDebug()<<'Z';
    }

}

void MainWindow::on_save_clicked()
{

    QFile comments("C:/Users/USER/Documents/Qt/analizer/comments.txt");
    QString s= ui->comment->toPlainText();
    comments.open(QIODevice::Append);
    QTextStream ts(&comments);
    ts<< s;
    comments.close();

}

void MainWindow::on_wipe_clicked()
{
    remove("comments.txt");
    QFile comments("C:/Users/USER/Documents/Qt/analizer/comments.txt");
    comments.open(QIODevice::WriteOnly);
    ui->comment-> clear();
    comments.close();
}

void MainWindow::on_showOrder_clicked()
{
    ord->show();
}

void MainWindow::on_Pressures_clicked()
{

   chartPr_ptr->show();


}

void MainWindow::listClicked(){
       listOfParts* lst= new listOfParts;
   // QPushButton* showCh= new QPushButton("Show changes");
      QHBoxLayout* acceptL=new QHBoxLayout;
       acceptL->addStretch(2);
       QPushButton* showOr= new QPushButton("Show changes");
       acceptL->addWidget(showOr);
       lst->lyt->addLayout(acceptL);
    connect(showOr, SIGNAL(clicked()),lst, SLOT(showChanged()));

    lst->show();
}

void MainWindow::on_addOrder_clicked()
{
    listOfParts* addOrder= new listOfParts;
    QHBoxLayout* acceptL= new QHBoxLayout;

    acceptL->addStretch(2);
    //acceptL->addWidget(add);

QTreeWidgetItem* head= new QTreeWidgetItem;
head->setText(0, "Part");
head->setText(1, "Quantity");
head->setText(2, "Date of last supply");
head->setText(3, "Q-ty to order");
addOrder->twgt->setHeaderItem(head);

QHBoxLayout* hb= new QHBoxLayout;
QPushButton* showOr= new QPushButton("Show order");
hb->addStretch(1);
hb->addWidget(showOr);
addOrder->lyt->addLayout(hb);

connect(showOr, SIGNAL(clicked()), addOrder, SLOT(showChanged()));


addOrder->show();

}

void MainWindow::fuelConsClicked(){
    fuel_ptr.clear();
    fuel_ptr=QSharedPointer<FuelConsumption>(new FuelConsumption);
    fuel_ptr->show();

}




void MainWindow::on_tabWidget_tabBarClicked(int index)
{


    if(index==1){
        ui->rightL_2->addWidget(fuelConsumption);
        ui->rightL_2->addWidget(cLbl);
         ui->testL_2->addWidget(pr);
        ui->rightL_2->addWidget(tem);
         ui->testL_2->addWidget(newOr);
         ui->testL_2->addWidget(order);
         ui->testL_2->addWidget(makeEnt);
        ui->rightL_2->addWidget(sched);
        ui->rightL_2->addWidget(journalEv);
         ui->testL_2->addWidget(listP);
 ui->testlyt_2->addWidget(lbltxt);
 ui->overhLyt_2->addWidget(lblOverh);
 ui->turbOverhLyt_2->addWidget(lblOverhTurb);

         QSqlQuery query;
         query.exec("SELECT MAX([date]) FROM Archive WHERE AE='AE2' and NAME='oil replacement'");
         query.next();
         lbltxt->setText(query.value(0).toDate().toString("dd/MM/yyyy"));

         query.exec("SELECT MAX([date]) FROM Archive WHERE AE='AE2' and NAME= 'overhaul'");
         query.next();
         lblOverh->setText(query.value(0).toDate().toString("dd/MM/yyyy"));

         query.exec("SELECT MAX([date]) FROM Archive WHERE AE='AE2' and NAME= 'turbine overhaul'");
         query.next();
         lblOverhTurb->setText(query.value(0).toDate().toString("dd/MM/yyyy"));


query.exec("SELECT status, dateTime, wh FROM workHours Where dateTime= (SELECT MAX(dateTime) FROM workHours WHERE AE='AE2')");
query.next();

    if(query.value(0).toString()=="sta"){
        ui->engineStatus_2->setText("Engine is in operation");
    QDateTime dt =  query.value(1).toDateTime();
    int wh= query.value(2).toInt();
    wh= wh+ (dt.secsTo(QDateTime::currentDateTime())/3600);
    ui->workHours_2->setText(QString::number(wh));
    }
    else{
        ui->engineStatus_2->setText("Engine is out of operation");
        ui->workHours->setText(query.value(2).toString());
    }

    chart_ptr.clear();
    chart_ptr=QSharedPointer<chart>(new chart("Temperatures2", nullptr));

    chartPr_ptr.clear();
    chartPr_ptr= QSharedPointer<chart>(new chart("Pressures2", nullptr));

    showLastTemps("Temperatures2");

}
    else{





        ui->rightL->addWidget(fuelConsumption);
        ui->rightL->addWidget(cLbl);
        ui->testL->addWidget(pr);
        ui->rightL->addWidget(tem);
        ui->testL->addWidget(newOr);
        ui->testL->addWidget(order);
        ui->testL->addWidget(makeEnt);
        ui->rightL->addWidget(sched);
        ui->rightL->addWidget(journalEv);
        ui->testL->addWidget(listP);

 ui->testlyt->addWidget(lbltxt);
 ui->overhLyt->addWidget(lblOverh);
  ui->turbOverhLyt->addWidget(lblOverhTurb);



        QSqlQuery query;
        query.exec("SELECT MAX([date]) FROM Archive WHERE AE='AE1' and NAME='oil replacement'");
        query.next();
        lbltxt->setText(query.value(0).toDate().toString("dd/MM/yyyy"));

        query.exec("SELECT MAX([date]) FROM Archive WHERE AE='AE1' and NAME= 'overhaul'");
        query.next();
        lblOverh->setText(query.value(0).toDate().toString("dd/MM/yyyy"));

        query.exec("SELECT MAX([date]) FROM Archive WHERE AE='AE1' and NAME= 'turbine overhaul'");
        query.next();
        lblOverhTurb->setText(query.value(0).toDate().toString("dd/MM/yyyy"));


        chart_ptr.clear();
        chart_ptr=QSharedPointer<chart>(new chart("Temperatures", nullptr));

        chartPr_ptr.clear();
        chartPr_ptr= QSharedPointer<chart>(new chart("Pressures", nullptr));


        showLastTemps("Temperatures");
    }
}

