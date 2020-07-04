#include "checking.h"
#include "ui_checking.h"

checking::checking(QString str, QWidget *parent) :
   nameOfSection(str), QDialog(parent),
    ui(new Ui::checking)
{
    ui->setupUi(this);
    ui->label_2->setText(nameOfSection);
}

checking::~checking()
{
    delete ui;
}


QDate checking::strDate(){
    QDate time;
    time=ui->dateEdit->date();
    QString date= time.toString();
    return time;
}

QString checking::AEdate(){
    QString AEnum;
    AEnum= ui->spinBox->text();
    return AEnum;
}

QString checking::stringName(){
    QString name;
    name= ui->plainTextEdit_2->text();
    return name;
}

QString checking::stringFrom (){
    QString str;
    str=ui->plainTextEdit->text();
    return str;
}

