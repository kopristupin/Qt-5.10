#include "archive.h"
#include "ui_archive.h"
#include"dialog.h"
#include<mainwindow.h>
#include<QDebug>
#include<QtSql/QSqlRecord>
#include<QtSql/QSqlError>
#include<QDate>




Archive::Archive(QString tabN, QWidget *parent) :
    tableName(tabN), QDialog(parent),
    ui(new Ui::Archive)
{
    ui->setupUi(this);

QSqlQuery queryAr;

     QString nA, dA, dateA, AE;
queryAr.exec("SELECT  * FROM "+tableName+" ORDER BY DATE;");
QSqlRecord recor = queryAr.record();
     while(queryAr.next()){

            nA= queryAr.value(recor.indexOf("NAME")).toString();
            dA= queryAr.value(recor.indexOf("DESCR")).toString();
            dateA= queryAr.value(recor.indexOf("DATE")).toString();
            AE= queryAr.value(recor.indexOf("AE")).toString();

            QTreeWidgetItem* newArch= new QTreeWidgetItem;
            newArch->setText(0, nA);
            newArch->setText(1, AE);
            newArch->setText(2, dA);
            newArch->setText(3, dateA);
            ui->doneWorks->addTopLevelItem(newArch);
            listOfAllParts.append(newArch);

        }
}

Archive::~Archive()
{
    delete ui;
}

void Archive::getData(QString n,QString AE, QString d, QString da){
    QTreeWidgetItem* newArch= new QTreeWidgetItem;
    newArch->setText(0, n);
    newArch->setText(1, AE);
    newArch->setText(2, d);
    newArch->setText(3, da);
    ui->doneWorks->addTopLevelItem(newArch);
}

void Archive::getDateChange(QString dateChanged){
    qDebug()<<"From archive: "<< dateChanged;
}

void Archive::on_searchEdit_textChanged(const QString &arg1)
{
    QList<QTreeWidgetItem*> listOfParts;
    listOfParts= ui->doneWorks->findItems(arg1,Qt::MatchContains);

    if(arg1==0)
        foreach(QTreeWidgetItem* item, listOfAllParts)
                item->setHidden(false);
    else{
        foreach(QTreeWidgetItem* item, listOfAllParts)
               if(!listOfParts.contains(item))
                       item->setHidden(true);
    }
}
