#include "mainwindow.h"
#include<QtWidgets>
#include <QApplication>
#include"dialog.h"
#include"archive.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
 //a.setStyle("Fusion");
    MainWindow w;


 w.show();


    return a.exec();
}
