#include "order.h"
#include "ui_order.h"
#include<QPushButton>
#include<QTextDocument>
#include<QPrinter>
#include<QPrintDialog>
#include<QSpinBox>

Order::Order(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Order)
{
    ui->setupUi(this);
    QTreeWidgetItem* head= new QTreeWidgetItem;
    head->setText(0, "Name of detail");
    head->setText(1, "Quantity");
    head->setText(2, "Description");
    head->setText(3, "Date of order");




    twgt->setHeaderItem(head);
    twgt->setColumnWidth(0, 120);
    QSqlQuery query;
    QString stringQuery {"SELECT * FROM orders"};
    if(!query.exec(stringQuery)){
        qDebug()<<"ERROR";
    }
    QSqlRecord rec= query.record();


    while(query.next()){
        QTreeWidgetItem* item= new QTreeWidgetItem;
        QString detailI= query.value(rec.indexOf("detail")).toString();
        item->setText(0, detailI);
        QString qnt= query.value(rec.indexOf("quantity")).toString();
        item->setText(1, qnt);
        QString descr= query.value(rec.indexOf("description")).toString();
        item->setText(2, descr);
        QString dateOr= query.value(2).toString();
        item->setText(3, dateOr);
        twgt->addTopLevelItem(item);

    }




    hbl->addSpacing(240);
    hbl->addWidget(del);
hbl->addWidget(save);
    vbl->addWidget(twgt);
    vbl->addLayout(hbl);

    setLayout(vbl);

    twgt->setSelectionMode(QAbstractItemView::MultiSelection);

    connect(del , SIGNAL(clicked()), this, SLOT(test()));
    connect(save, SIGNAL(clicked())  , this, SLOT(saveOrder()));
}

Order::~Order()
{
    delete ui;
}

void Order::test(){
    QList<QTreeWidgetItem*> items= twgt->selectedItems();
    foreach(QTreeWidgetItem* item, items){
        QSqlQuery query;
        QString q={"DELETE FROM orders WHERE detail='"+item->text(0)+"'"};
        query.exec(q);
        delete item;
    }
}
void Order::saveOrder(){
    QString strStream;
    QTextStream out(&strStream);

    const int rowCount = twgt->model()->rowCount();
    const int columnCount = twgt->model()->columnCount();
    QString strTitle;
    QString data;
    out <<  "<!DOCTYPE html>                                                       "
            " <style>                                                               "
            " p {                                                                   "
            "     margin-right: 25%;                                                "
            "    }                                                                  "
            " </style>                                                              "
            " <html>                                                                "
            "  <head>                                                               "
            "  <h1 align= center> AE Order </h1>                               "
            "  </head>                                                              "
            "  <body>                                                                 "
            "   <table width=680 border=1 cellpadding=5 cellspacing=0>      "
            "    <tr>                                                               "
            "     <th>Name </th>                                                 "
            "     <th>Quantity</th> "
            "<th>  Description  </th>                                           "
            " <th>Date</th>                                                        ";
    for (int row = 0; row < rowCount; row++) {
            out << "<tr>";
            for (int column = 0; column < columnCount; column++) {
                if (!twgt->isColumnHidden(column)) {
                    QString data = twgt->model()->data(twgt->model()->index(row, column)).toString().simplified();
                    out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
                }
            }
            out << "</tr>\n";
        }

    out<<
           "  </table>                                                             "

           " <p align=right> Date:</p>                                           "
           " <p align=right> Chief Engineer:                                     "
           " <p align=right> Signature:                                     "

           "  </body>                                                              "
           " </html>                                                               ";






    QTextDocument *document = new QTextDocument();
    document->setHtml(strStream);

    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOrientation(QPrinter::Portrait);

printer.setOutputFileName("C:/Users/USER/Documents/Qt/analizer/testOrder.pdf");

document->print(&printer);






    delete document;
}
