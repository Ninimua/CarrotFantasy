#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QPushButton>
#include <QWidget>
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QBitmap>
#include <QString>
#include <QPainter>

class mybutton : public QPushButton
{
    Q_OBJECT

public:
    mybutton(QString pic1,QString pic2,QString s,QWidget *);
private:
    QString _picurl1;
    QString _picurl2;
protected:
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);
};

#endif // MYBUTTON_H
