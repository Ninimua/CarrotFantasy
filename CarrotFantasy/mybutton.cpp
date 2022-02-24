#include "mybutton.h"

mybutton::mybutton(QString pic1,QString pic2,QString text,QWidget *parent):
    QPushButton(text,parent)
{
    QPixmap pixmap(pic1);

    _picurl1 = "QPushButton{border-image: url(";
    _picurl1 +=pic1;
    _picurl1 +=");}";

    _picurl2 = "QPushButton{border-image: url(";
    _picurl2 +=pic2;
    _picurl2 +=");}";

    resize(pixmap.size());
    setMask(QBitmap(pixmap.mask()));
    setStyleSheet(_picurl1);
}

void mybutton::enterEvent(QEvent *)
{
    setStyleSheet(_picurl2);
}

void mybutton::leaveEvent(QEvent *)
{
    setStyleSheet(_picurl1);
}
