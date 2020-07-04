#include "listofparts.h"
#include "ui_listofparts.h"

listOfParts::listOfParts(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::listOfParts)
{
    ui->setupUi(this);

    QTreeWidgetItem* head= new QTreeWidgetItem;
    head->setText(0, "Part");
    head->setText(1, "Quantity");
    head->setText(2, "Date of supply");
    head->setText(3, "Taken from storage");

    twgt->setHeaderItem(head);
    twgt->setColumnWidth(0, 120);
    QSqlQuery query;
    QString stringQuery {"SELECT * FROM listOfParts"};
    if(!query.exec(stringQuery)){
        qDebug()<<"ERROR";
    }
    QSqlRecord rec= query.record();

int n=0;
    while(query.next()){


        QTreeWidgetItem* item= new QTreeWidgetItem;
        QString detailI= query.value(rec.indexOf("name")).toString();
        item->setText(0, detailI);
        QString qnt= query.value(rec.indexOf("quantity")).toString();
        item->setText(1, qnt);
        QString descr= query.value(rec.indexOf("date")).toString();
        item->setText(2, descr);

       // item->setText(3, dateOr);


        twgt->addTopLevelItem(item);
        QSpinBox* bx= new QSpinBox;
        qsbVector.push_back(bx);
        twgt->setItemWidget(twgt->topLevelItem(n),3, bx);
listOfAllParts.append(item);
n++;
    }

    searchLay->addWidget(searchL);
    searchLay->addWidget(textSearch);
    searchLay->addStretch(1);

lyt->addLayout(searchLay);
    lyt->addWidget(twgt);
    setLayout(lyt);

    connect(textSearch, SIGNAL(textChanged(QString)),this, SLOT(showResult(QString)));

}


listOfParts::~listOfParts()
{
    delete ui;
}

void listOfParts::showResult(QString query){
    QList<QTreeWidgetItem*> listOfParts;
    listOfParts= twgt->findItems(query,Qt::MatchContains);

    if(query==0)
        foreach(QTreeWidgetItem* item, listOfAllParts)
                item->setHidden(false);
    else{
        foreach(QTreeWidgetItem* item, listOfAllParts)
               if(!listOfParts.contains(item))
                       item->setHidden(true);
    }
}

void listOfParts::showChanged(){
    QVector<int> listOfOrderedParts;
    for(int i=0; i<twgt->topLevelItemCount(); i++)
        if(qsbVector[i]->value()!=0){
            listOfOrderedParts.append(i);
        }
QVBoxLayout lytt;
QTreeWidgetItem* head= new QTreeWidgetItem;
head->setText(0, "Part");
head->setText(1, "Quantity");
head->setText(2, "Date of supply");
head->setText(3, "Q-ty to order");
widgetList->setHeaderItem(head);

//addTopLevelItems(QList<*QtreeWidgetItem>) doesnt work(

int b=0;
foreach(int i, listOfOrderedParts){
QTreeWidgetItem* item= new QTreeWidgetItem;
item->setText(0, twgt->topLevelItem(i)->text(0));
item->setText(1, twgt->topLevelItem(i)->text(1));
item->setText(2, twgt->topLevelItem(i)->text(2));
item->setText(3, QString::number(qsbVector[i]->value()));
  widgetList->addTopLevelItem(item);
  b++;
}

QHBoxLayout* hl= new QHBoxLayout;
hl->addSpacing(350);
hl->addWidget(add);
lytt.addWidget(widgetList);
lytt.addLayout(hl);
 wgt.setLayout(&lytt);
 wgt.resize(450, 200);
 wgt.show();

 connect(add, SIGNAL(clicked()), this , SLOT(saveOrder()));

}

void listOfParts::saveOrder(){
    qDebug()<<"DONE";
    QDate d= QDate::currentDate();
    QVector<QString> listOfPartsInDb;
    QSqlQuery q;
    QString getAlreadyOrderedParts={"SELECT detail FROM orders"};
    q.exec(getAlreadyOrderedParts);
    while(q.next()){
        QString toPush= q.value(0).toString().trimmed();
        listOfPartsInDb.push_back(toPush);
        qDebug()<< toPush;
    }



    for(int i=0; i<widgetList->topLevelItemCount(); i++){

        if(listOfPartsInDb.contains(widgetList->topLevelItem(i)->text(0)))
        {
           qDebug()<<"ADDED";
            QString updateStr={"SELECT quantity FROM orders WHERE detail= '"+widgetList->topLevelItem(i)->text(0) +"'"};

                    q.exec(updateStr);
                    q.next();
                     int quantityInDb= q.value(0).toInt();
            qDebug()<< "Quantity from DB: "<<  quantityInDb;
            quantityInDb= quantityInDb+widgetList->topLevelItem(i)->text(3).toInt();
            qDebug()<<"Quantity to DB: "<<  quantityInDb;
            updateStr={"UPDATE orders SET quantity ="+ QString::number(quantityInDb) +" WHERE detail= '"+widgetList->topLevelItem(i)->text(0)+"'"};
            q.exec(updateStr);
        }

        else{
            qDebug()<<" INSERTED";
            QString qStr={" INSERT INTO orders VALUES ('%1','%2','%3','%4')"};
            QString addOr= qStr.arg( widgetList->topLevelItem(i)->text(0))
                    .arg("o")
                    .arg(d.toString("yyyy-MM-dd"))
                    .arg(widgetList->topLevelItem(i)->text(3).toInt());
            q.exec(addOr);
        }
    }
}
