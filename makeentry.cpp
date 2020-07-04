#include "makeentry.h"
#include "ui_makeentry.h"
#include<QDate>


makeEntry::makeEntry(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::makeEntry)
{
    ui->setupUi(this);
    ui->entyDateEdit->setDate(QDate::currentDate());
  QSqlQuery query;
  query.exec("SELECT status FROM workHours Where dateTime= (SELECT MAX(dateTime) FROM workHours WHERE AE='AE1')");
  query.next();
    qDebug()<< query.value(0).toString();
    if(query.value(0).toString()=="sta"){
       ui->statusLbl->setText("Engine is in operation!");
       ui->pushButton_3->setEnabled(false);

    }
    else{
        ui->statusLbl->setText("Engine stopped!");
        ui->t1->setEnabled(false);
        ui->t2->setEnabled(false);
        ui->t3->setEnabled(false);
        ui->t4->setEnabled(false);
        ui->t5->setEnabled(false);
        ui->t6->setEnabled(false);
        ui->power->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton->setEnabled(false);

}
    //check status to update statusLbl


  // connect(ui->engineComboBox, &QComboBox::currentIndexChanged, this, &makeEntry::update);
   connect(ui->engineComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(update(int)));
}

makeEntry::~makeEntry()
{
    delete ui;
}



void makeEntry::msWrn(){
    QMessageBox* msbx= new QMessageBox(this);
    msbx->setIcon(QMessageBox::Warning);
    msbx->setText("Entry all measures!");
    //QPushButton *btnCancel =  msbx->addButton( "Ok", QMessageBox::AcceptRole );
     msbx->setAttribute(Qt::WA_DeleteOnClose);
    msbx->setModal(true);
    msbx->show();
}

void makeEntry::addMeas(QString eng){

    QDate curD= QDate::currentDate();
    QSqlQuery add;
    QString addQ="INSERT INTO "+ eng +" VALUES('%1','%2','%3', '%4','%5','%6', '%7', '%8')";
    QString addCQ= addQ.arg(ui->entyDateEdit->date().toString("yyyy-MM-dd"))
            .arg(ui->t1->text().toInt())
            .arg(ui->t2->text().toInt())
            .arg(ui->t3->text().toInt())
            .arg(ui->t4->text().toInt())
            .arg(ui->t5->text().toInt())
            .arg(ui->t6->text().toInt())
            .arg(ui->power->text().toInt());
    add.exec(addCQ);
}

void makeEntry::on_pushButton_clicked()
{
    if(ui->power->text().isEmpty() && ui->t1->text().isEmpty() &&ui->t2->text().isEmpty() &&ui->t3->text().isEmpty() &&
            ui->t4->text().isEmpty() && ui->t5->text().isEmpty() && ui->t6->text().isEmpty()){
        msWrn();
    }
    else{
        if(ui->engineComboBox->currentIndex()==0)
        addMeas("Temperatures");
           else{
            addMeas("Temperatures2");
        }

    }
}

void makeEntry::on_pushButton_2_clicked()//engine stopped
{

stopOkptr.clear();
stopOkptr= QSharedPointer<QPushButton>(new QPushButton("Ok"));
hlyt.addWidget(&edPower);
hlyt.addWidget(&ed1);
hlyt.addWidget(&ed2);
hlyt.addWidget(&ed3);
hlyt.addWidget(&ed4);
hlyt.addWidget(&ed5);
hlyt.addWidget(&ed6);

loaDlbl.setText("Load");
t1lbl.setText("t1");
t2lbl.setText("t2");
t3lbl.setText("t3");
t4lbl.setText("t4");
t5lbl.setText("t5");
t6lbl.setText("t6");

hlyt1.addWidget(&loaDlbl);
hlyt1.addWidget(&t1lbl);
hlyt1.addWidget(&t2lbl);
hlyt1.addWidget(&t3lbl);
hlyt1.addWidget(&t4lbl);
hlyt1.addWidget(&t5lbl);
hlyt1.addWidget(&t6lbl);



    hbx->addWidget(lbl);
    hbx->addWidget(stopTimeEdit);
    stopTimeEdit->setDate(QDate::currentDate());
    stopTimeEdit->setTime(QTime::currentTime());
    hbx->addWidget(stopOkptr.data());
    vlyt.addLayout(hbx);
    vlyt.addLayout(&hlyt1);
    vlyt.addLayout(&hlyt);
    entryTime->setLayout(&vlyt);
    entryTime->resize(260, 60);
        entryTime->show();
        connect(stopOkptr.data(), SIGNAL(clicked()), this, SLOT(workHoursEntryStop()));
        ui-> pushButton_3-> setEnabled(true);
        ui-> pushButton_2->setEnabled(false);
}

void makeEntry::on_pushButton_3_clicked()// started
{

    startOkptr.clear();
    startOkptr= QSharedPointer<QPushButton>(new QPushButton("Ok"));

    lytt->addWidget(lblS);
    lytt->addWidget(startTimeEdit);
    startTimeEdit->setDate(QDate::currentDate());
    startTimeEdit->setTime(QTime::currentTime());

     lytt->addWidget(startOkptr.data());
    startTime->setLayout(lytt);
        startTime->show();
        connect(startOkptr.data(), SIGNAL(clicked()), this, SLOT(workHoursEntry()));

ui-> pushButton_3-> setEnabled(false);
ui-> pushButton_2->setEnabled(true);
}

void makeEntry::workHoursEntry(){
    QString eng;
    if(ui->engineComboBox->currentIndex()==0)
            eng="AE1";
    else
        eng="AE2";
    QSqlQuery query;
        qDebug()<<"Start";
        query.exec("SELECT wh FROM workHours Where dateTime= (SELECT MAX(dateTime) FROM workHours WHERE AE='"+eng+"')");
        query.next();
        int n= query.value(0).toInt();
        query.exec("INSERT INTO workHours VALUES ('"+startTimeEdit->dateTime().toString("yyyy-dd-MM hh:mm")+"',"+ QString::number(n) +","+ "'sta',+'"+eng+"')");
    query.exec("INSERT INTO Archive VALUES( 'started', 'engine started', '"+startTimeEdit->dateTime().toString("yyyy-dd-MM hh:mm") +"',+'"+eng+"')");

        ui->statusLbl->setText("Engine is in operation!");
        QString tabName;
            if(eng=="AE1")
                    tabName="Temperatures";
            else
                tabName="Temperatures2";

            query.exec("SELECT date FROM "+ tabName+"  WHERE Id= (SELECT Max(Id) AS LastId FROM "+ tabName+")");
            query.next();
           QDate date(query.value(0).toDate());
           QDate date_= startTimeEdit->date();
           int counter= date.daysTo(date_)-1;

            for(int i=1; i<=counter; i++)
                 query.exec("INSERT INTO "+ tabName +" VALUES('"+ date.addDays(i).toString("yyyy-MM-dd") +"',0,0,0,0,0,0,0)");



}

void makeEntry::workHoursEntryStop(){
    QString eng;
    if(ui->engineComboBox->currentIndex()==0)
            eng="AE1";
    else
        eng="AE2";

    if(!(edPower.text().isEmpty() && ed1.text().isEmpty() &&  ed2.text().isEmpty() && ed3.text().isEmpty() &&
            ed4.text().isEmpty() && ed5.text().isEmpty() && ed6.text().isEmpty()))
                  {

    qDebug()<<"stop";
  QSqlQuery query;
  query.exec("SELECT dateTime, wh FROM workHours Where dateTime= (SELECT MAX(dateTime) FROM workHours WHERE AE='"+eng+"')");
  query.next();
 QDateTime dateTime= query.value(0).toDateTime();
  int n= query.value(1).toInt();
int hours= dateTime.secsTo(QDateTime::currentDateTime())/3600;
query.exec("INSERT INTO workHours VALUES('"+ stopTimeEdit->dateTime().toString("yyyy-dd-MM hh:mm")+"'," +QString::number(n+ hours)+",'sto', '"+eng+"')");
qDebug()<<"INSERT INTO workHours VALUES('"+ stopTimeEdit->dateTime().toString("yyyy-dd-MM hh:mm")+"'," +QString::number(n+ hours)+",'sto', '"+eng+"')";

QString descr= {"t1 "+ ed1.text()+ ","+"t2 " + ed2.text() + ","+ "t3 "+ ed3.text()+ "," + "t4 "+ ed4.text()+ ","+"t5 "+  ed5.text()+ ","+"t6 "+ ed6.text()+ ","+"power "+ edPower.text()};


query.exec("INSERT INTO Archive VALUES ( 'stopped', '"+ descr +"','"+ stopTimeEdit->dateTime().toString("yyyy-dd-MM hh:mm")+"', '"+eng+"')");
qDebug()<<"INSERT INTO Archive VALUES ( 'stopped', '"+ descr +"','"+ stopTimeEdit->dateTime().toString("yyyy-dd-MM hh:mm")+"', '"+eng+"')";

ui->statusLbl->setText("Engine stopped!");
    }

        else{

        msBx->show();
    }

}

void makeEntry::update(int index){
    if(index==1){
        QSqlQuery query;
        query.exec("SELECT status FROM workHours Where dateTime= (SELECT MAX(dateTime) FROM workHours WHERE AE='AE2')");
        query.next();


        if(query.value(0).toString()=="sta"){
           ui->statusLbl->setText("Engine is in operation!");
           ui->pushButton->setEnabled(true);
           ui->pushButton_3->setEnabled(false);
           ui->pushButton_2->setEnabled(true);

           ui->t1->setEnabled(true);
           ui->t2->setEnabled(true);
           ui->t3->setEnabled(true);
           ui->t4->setEnabled(true);
           ui->t5->setEnabled(true);
           ui->t6->setEnabled(true);
        ui->power->setEnabled(true);

        }
        else{
            ui->statusLbl->setText("Engine stopped!");
            ui->t1->setEnabled(false);
            ui->t2->setEnabled(false);
            ui->t3->setEnabled(false);
            ui->t4->setEnabled(false);
            ui->t5->setEnabled(false);
            ui->t6->setEnabled(false);
            ui->power->setEnabled(false);
            ui->pushButton_2->setEnabled(false);
            ui->pushButton->setEnabled(false);

    }
    }
    else{
        QSqlQuery query;
        query.exec("SELECT status FROM workHours Where dateTime= (SELECT MAX(dateTime) FROM workHours WHERE AE='AE1') ");
        query.next();


        if(query.value(0).toString()=="sta"){
              ui->pushButton->setEnabled(true);
           ui->statusLbl->setText("Engine is in operation!");
           ui->pushButton_3->setEnabled(false);
           ui->pushButton_2->setEnabled(true);

           ui->t1->setEnabled(true);
           ui->t2->setEnabled(true);
           ui->t3->setEnabled(true);
           ui->t4->setEnabled(true);
           ui->t5->setEnabled(true);
           ui->t6->setEnabled(true);
        ui->power->setEnabled(true);

        }
        else{
            ui->statusLbl->setText("Engine stopped!");
            ui->t1->setEnabled(false);
            ui->t2->setEnabled(false);
            ui->t3->setEnabled(false);
            ui->t4->setEnabled(false);
            ui->t5->setEnabled(false);
            ui->t6->setEnabled(false);
            ui->power->setEnabled(false);
            ui->pushButton_2->setEnabled(false);
            ui->pushButton->setEnabled(false);

    }


    }
}
