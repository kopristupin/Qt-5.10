
#include "chart.h"
#include "ui_chart.h"
#include<callout.h>
#include<QPushButton>
#include<QPixmap>
#include<QImage>
#include<QPrinter>


chart::chart(QString str, QWidget *parent):
    tableName(str), QWidget(parent),
    ui(new Ui::chart)
{
    ui->setupUi(this);
    
    QT_CHARTS_USE_NAMESPACE

    QSqlQuery queryTem;
    QString qs={"SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = '"+tableName+"'"};
    queryTem.exec(qs);

    while(queryTem.next())
        columnNames.push_back(queryTem.value(0).toString());

    ch->setTitle( tableName+ " chart");


    queryTem.exec("SELECT MAX(date) FROM "+ tableName);
    qDebug()<< "SELECT MAX(date) FROM"+ tableName;
    queryTem.next();
    QDate maxDate= queryTem.value(0).toDate();
    int tickCount= -maxDate.daysTo(maxDate.addMonths(-1))+1;



    QDateTime dt;
    dt.setDate(maxDate);
    axisX->setRange(dt.addMonths(-1), dt);

   QString qStr="SELECT MAX(Id) FROM " +tableName;
    queryTem.exec(qStr);
    queryTem.next();
    int maxId= queryTem.value(0).toInt();
qDebug()<<" ID:::: "<< maxId;



    axisX->setTickCount(tickCount);
    axisX->setFormat("dd/MM");
    axisX->setTitleText("Date");
    ch->addAxis(axisX, Qt::AlignBottom);

    axisLoad->setRange(0, 750);
    axisLoad->setTickCount(20);
    axisLoad->setTitleText("Load, kWt");
    ch->addAxis(axisLoad, Qt::AlignRight);

    if(tableName=="Pressures"){
        axisY->setRange(140, 180);
        axisY->setTickCount(20);
        axisY->setTitleText(tableName+", MPa");
        ch->addAxis(axisY, Qt::AlignLeft);
    }
    else{
        axisY->setRange(300, 410);
        axisY->setTickCount(20);
        axisY->setTitleText(tableName);
        ch->addAxis(axisY, Qt::AlignLeft);

    }
    QString columnQuery;
    foreach(QString s,  columnNames){
        columnQuery+=s+ ", ";
    }

    int size= columnQuery.length();
    columnQuery.remove(size-6, 6);//removes last comma and Id




    int minId;
  //  int numberMeasures;
    QString minIdS;
    if(maxId>=30){
        minId= maxId-30;
        minIdS= QString::number(minId-1);
      //  numberMeasures=30;
    }
    qDebug()<< "DATE : "<< maxDate.addMonths(-1).toString("yyyy-dd-MM");
    qStr="SELECT "+ columnQuery +" FROM "+ tableName +" WHERE date>='"+ maxDate.addMonths(-1).toString("yyyy-MM-dd")+"'";
    qDebug()<<qStr;
    queryTem.exec(qStr);
    QSqlRecord rec= queryTem.record();

    int i=0;
    numberSeries= columnNames.count()-1;//minus 'Id'

    for(int c=0; c<numberSeries-1; c++)
        toCalculateDynamicLines.push_back(new QMap<int, int>);

    QDate min= maxDate.addMonths(-1);

    while(queryTem.next()){

        i++;
       // queryTem.next();
        QDateTime dtt;
        QVariant v;
        v=queryTem.value(0);
        dtt.setDate(v.toDate());
        int startIndex=1;
qDebug()<< dtt<< " "<<queryTem.value(startIndex).toInt();
        for(int c=0; c<numberSeries-1; c++){
            seriesToDraw.push_back(new QLineSeries);
            seriesToDraw[c]->append(dtt.toMSecsSinceEpoch(),queryTem.value(startIndex).toInt());
            if(queryTem.value(startIndex).toInt()!=0)
                toCalculateDynamicLines[c]->insert(i,queryTem.value(startIndex).toInt());

            startIndex++;
        }

        min.addDays(i);
    }


    if(tableName=="Pressures" && tableName=="Pressures2"){
        for(int c=0; c<numberSeries-1/2;c++){
            seriesToDraw[c*2]->setName("Pz Cyl. №" + QString::number(c+1));
            seriesToDraw[c*2+1]->setName("Pc Cyl. №" + QString::number(c+1));

        }
    }


    else{
        for(int c=0; c<numberSeries-1; c++){
            qDebug()<<c ;
            seriesToDraw[c]->setName("Cyl. №" + QString::number(c+1));
            dynamicLines.push_back(new QLineSeries);
            dynamicLines[c]->append(linearRe(toCalculateDynamicLines[c], maxDate.addMonths(-1), maxDate));
        }
        seriesToDraw[6]->setName("Load");
    }

    QColor c;
    c.setRed(100);
    QObject::connect(this, SIGNAL(changeRange(QDate, QDate)), SLOT(changeDateRange(QDate, QDate)));

    chartView->setRenderHint(QPainter::Antialiasing);
    ui->verticalLayout->addWidget(chartView);

    for(int i=0; i<6; i++){
        chBox[i]= new QCheckBox(QString::number(i+1));
        ui->horizontalLayout->addWidget(chBox[i]);
        connect(chBox[i], SIGNAL(clicked()), this, SLOT(checkChBox()));
    }

    if(tableName=="Temperatures"){
        QPen pen;
        pen.setStyle(Qt::DashLine);
        seriesToDraw[6]->setPen(pen);
        ch->addSeries(seriesToDraw[6]);
        seriesToDraw[6]->attachAxis(axisLoad);
    }






}

chart::~chart()
{
    qDebug()<<"DESTRUCTED";
    delete ui;
}


void chart::checkChBox()
{

    // numberSeries doesn't corespond to number of chBoxes in case of 'Temperatures'

    for(int m=0; m<6; m++){
        if(chBox[m]->isChecked()){
            if(chBox[m]->isChecked()){
                if(tableName=="Pressures"){

                    ch->addSeries(seriesToDraw[2*m]);
                    ch->addSeries(seriesToDraw[2*m+1]);
                    seriesToDraw[2*m+1]->attachAxis(axisY);
                    seriesToDraw[2*m]->attachAxis(axisY);

                }
                else{
                    ch->addSeries(seriesToDraw[m]);
                    ch->addSeries(dynamicLines[m]);
                    seriesToDraw[m]->attachAxis(axisY);
                    dynamicLines[m]->attachAxis(axisY);
                }
            }
        }

        if (!chBox[m]->isChecked()){
            if(tableName=="Pressures"){
                ch->removeSeries(seriesToDraw[2*m]);
                ch->removeSeries(seriesToDraw[2*m+1]);
            }
            else{
                ch->removeSeries(seriesToDraw[m]);
                ch->removeSeries(dynamicLines[m]);
            }
        }
    }
}

void chart::on_setDateB_clicked()
{

    QDate dateMi;
    dateMi=ui->minDate->date();
    QDate dateMa;
    dateMa=ui->maxDate->date();

    emit changeRange(dateMi, dateMa);
}

void chart::changeDateRange(QDate newMin, QDate newMax){
 QSqlQuery query1;
 query1.exec("SELECT t1 FROM "+tableName +" WHERE date= '"+ newMin.toString("yyyy-MM-dd")+"'");
 qDebug()<< "SELECT t1 FROM "+tableName +" WHERE date= '"+ newMin.toString("yyyy-MM-dd")+"'";
 query1.next();
 qDebug()<<query1.value(0).toInt();
    if(query1.value(0).toInt()==0){
        QMessageBox* msbx= new QMessageBox(QMessageBox::Warning, "Incorrect date", "Date is out of range");
        msbx->show();
    }

    else{

    QDateTime min;
    min.setDate(newMin);
    QDateTime max;
    max.setDate(newMax);
    axisX->setRange(min, max);
    axisX->setTickCount(min.daysTo(max)+1);
    for(int i=0; i<numberSeries-1; i++)
    {
        seriesToDraw[i]->clear();
        toCalculateDynamicLines[i]->clear();
        if(tableName=="Temperatures")
            dynamicLines[i]->clear();
    }


    QString qStr="SELECT * FROM "+ tableName+ " WHERE date>='"+ newMin.toString("yyyy-MM-dd")+"' and date<= '"+ newMax.toString("yyyy-MM-dd") +"' ";
    qDebug()<< "changed: "<< qStr;
    if(!query1.exec(qStr)){
        qDebug()<<"Error: "<<query1.lastError();
    }
    QSqlRecord rec= query1.record();
int newMinId, newMaxId;
    int i=0;
    while(query1.next()){
        if(i==0)
            newMinId== query1.value(8).toInt();

        QDateTime dtt;
        int startIndex=1;
        dtt.setDate(query1.value(rec.indexOf("date")).toDate());

        for(int y=0; y<numberSeries-1; y++){
            seriesToDraw[y]->append(dtt.toMSecsSinceEpoch(),query1.value(startIndex).toInt());
            if(query1.value(startIndex).toInt()!=0)
                toCalculateDynamicLines[y]->insert(i,query1.value(startIndex).toInt());
            startIndex++;
        }
        i++;

    }
    query1.exec("SELECT id FROM Temperatures WHERE date='"+ newMax.toString("yyyy-dd-MM")+"'");
    query1.next();
    newMaxId= query1.value(0).toInt();


    if(tableName=="Temperatures"){
        ch->removeSeries(seriesToDraw[6]);
        ch->addSeries(seriesToDraw[6]);
        seriesToDraw[6]->attachAxis(axisLoad);

        for(int y=0; y<numberSeries-1; y++)
            dynamicLines[y]->append(linearRe(toCalculateDynamicLines[y],newMin, newMax));

    }
}
}

//метод Крамера
QList<QPointF> chart::linearRe( QMap<int, int>* map, QDate min, QDate max){
    int number= map->count();
    int squareSumX=0, sumX=0, sumY=0, sumMultXY=0;
    QMap<int , int>:: iterator it= map->begin();
    while(it!=map->end()){
        squareSumX+= qPow(it.key(), 2);
        sumX+= it.key();
        sumY+= it.value();
        sumMultXY+= it.key() * it.value();
        it++;
    }
    double a,b, determ, determA, determB;
    determ =(squareSumX*number) - ( qPow(sumX, 2));
    determA= (sumMultXY * number) - (sumY * sumX);
    determB=(squareSumX * sumY) - (sumX * sumMultXY);
    a= determA/ determ;
    b= determB/ determ;
    QDateTime minDT, maxDT;
    minDT.setDate(min);
    maxDT.setDate(max);

    QPointF p1(minDT.toMSecsSinceEpoch(), a*1 + b);
    QPointF p2(maxDT.toMSecsSinceEpoch(), a* number  + b);
    QList<QPointF> res;
    res.push_back(p1);
    res.push_back(p2);

    return res;
}

void chart::on_pushButton_clicked()
{
    QPrinter* pprinter= new QPrinter;
    pprinter->setOutputFormat(QPrinter::PdfFormat);
    pprinter->setOrientation(QPrinter::Landscape);
    QString numbers;
    for(int i=0; i<6; i++)
        if(chBox[i]->isChecked())
            numbers+= QString::number(i+1);
    QDate cd= QDate::currentDate();
    QString nameOfSaved={tableName+"_"+numbers +"_"+ cd.toString("dd.MM.yy")};
    pprinter->setOutputFileName("C:/Users/USER/Documents/Qt/analizer/"+ nameOfSaved+".pdf");
    QPainter painter(pprinter);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::NoBrush);
    chartView->scene()->render(&painter);
    painter.drawText(100,450, "Engine (specifcation)");
    painter.end();
}

void chart::keepCallout(QPointF point, bool state)
{

}
