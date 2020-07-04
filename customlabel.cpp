#include "customlabel.h"
#include<qDebug>


customLabel::customLabel(QWidget *parent) : QLabel(parent)
{
qDebug()<< "FROM :"<< this->pos().ry();
}



void customLabel::mouseMoveEvent(QMouseEvent *me){
    qDebug()<<"OK!!!!";
}

void customLabel::enterEvent(QEvent *event){
        if(anchorY==0)
            anchorY= this->pos().y();

    QPropertyAnimation* anim= new QPropertyAnimation(this,  "pos");
    anim->setDuration(1500);
    anim->setStartValue(this->pos());
    anim->setEndValue(QPoint(this->pos().x(), anchorY+15));
    anim->setEasingCurve(QEasingCurve::OutExpo);
    anim->setLoopCount(1);
    anim->start();
}

void customLabel::leaveEvent(QEvent *ev){


    QPropertyAnimation* anim= new QPropertyAnimation(this,  "pos");
    anim->setDuration(1500);
    anim->setStartValue(this->pos());
    anim->setEndValue(QPoint(this->pos().x(), anchorY));
    anim->setEasingCurve(QEasingCurve::OutExpo);
    anim->setLoopCount(1);
    anim->start();
}

void customLabel::mousePressEvent(QMouseEvent *ev){
    qDebug()<<"clicked";
    emit labelClicked();
}



