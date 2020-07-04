#include "notdoneworksc.h"
#include "ui_notdoneworksc.h"
#include<QtSql>

notDoneWorksC::notDoneWorksC(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::notDoneWorksC)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(transfer()));
}

notDoneWorksC::~notDoneWorksC()
{
    delete ui;
}

void notDoneWorksC::notDoneWorkAdd(QString w,QString ae, QString des, QString dat){
    QTreeWidgetItem* newEntry= new QTreeWidgetItem;
    newEntry->setText(0,w);
    newEntry->setText(1, ae);
    newEntry->setText(2, des);
    newEntry->setText(3, dat);
    ui->works->addTopLevelItem(newEntry);
}

void notDoneWorksC::transfer(){
   QSqlQuery query;
    QList<QTreeWidgetItem*> selectedItems;
    selectedItems=ui->works->selectedItems();
    foreach(QTreeWidgetItem*  item, selectedItems){
        QString s={" INSERT INTO Archive VALUES('%1','%2', '%3', '%4') "};
        QString qStr= s.arg(item->text(0))
                       .arg(item->text(2))
                       .arg(item->text(3))
                .arg(item->text(1));
        query.exec(qStr);
        QString name= item->text(0);
         QString str={" DELETE FROM dataV WHERE NAME= '"+name+"'"};
         query.exec(str);




        delete item;
    }

}
