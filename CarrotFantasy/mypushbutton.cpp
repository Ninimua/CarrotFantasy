#include "mypushbutton.h"

MyPushButton::MyPushButton(QWidget *parent=0,QPixmap Pic,QString Str):QPushButton(parent)
{
    pic = Pic;
    str = Str;
    QStringLiteral(str);
    setStyleSheet(tr("background-image: url(:/image/07.PNG);"));
}

