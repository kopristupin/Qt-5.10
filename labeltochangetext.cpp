#include "labeltochangetext.h"
#include<QDebug>
labelToChangeText::labelToChangeText(QString str): nameStr(str)
{
nameStr= str;
edit.setDate(QDate::currentDate());
this->setCursor(Qt::PointingHandCursor);
this->setText(QDate::currentDate().toString("dd/MM/yyyy"));
QFont font("Times New Roman", 16);
this->setFont(font);
connect(&bt, &QPushButton::clicked, this , &labelToChangeText::setT);

}

void labelToChangeText::mouseDoubleClickEvent( QMouseEvent * e ){
    bt.setText("Ok");
    lyt.addWidget(&edit);
    lyt.addWidget(&bt);
    wgt.setLayout(&lyt);
    wgt.show();

}

void labelToChangeText::setT(){
    this->setText(edit.date().toString("dd/MM/yyyy"));
    QSqlQuery query;
    query.exec("INSERT INTO Archive VALUES('"+nameStr+"','w', +'"+ edit.date().toString("yyyy/MM/dd")+"')");
    wgt.close();
}


