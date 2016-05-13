#include "UtilityWidgets.h"

//
RangeDialog::RangeDialog(float *lowVal, float *highVal, QDialog *parent, QString title, QString discribe)
    :QDialog(parent),
      _lowVal(lowVal),
      _highVal(highVal)
{
    ledtLow =   new QLineEdit(tr("-1"));
    ledtHigh    =   new QLineEdit(tr("-1"));
    QPushButton* btnOK  =   new QPushButton(tr("OK"));
    QPushButton* btnCancel  =   new QPushButton(tr("cancel"));

    connect(btnOK,SIGNAL(pressed()),this,SLOT(on_btnOK()));
    connect(btnCancel,SIGNAL(pressed()),this,SLOT(close()));

    QHBoxLayout* hbl    =   new QHBoxLayout;
    hbl->addWidget(ledtLow);
    hbl->addWidget(new QLabel(tr(" - ")));
    hbl->addWidget(ledtHigh);
    QHBoxLayout* vbl    =   new QHBoxLayout;
    vbl->addStretch();
    vbl->addWidget(btnOK);
    vbl->addWidget(btnCancel);
    QVBoxLayout* layout =   new QVBoxLayout;
    layout->addWidget(new QLabel(discribe));
    layout->addLayout(hbl);
    layout->addLayout(vbl);
    this->setLayout(layout);

    this->setWindowTitle(title);
}
NumberDialog::NumberDialog(float* value,QDialog *parent,QString title,QString discribe)
             :QDialog(parent),
               _value(value)
{
    ledValue=new QLineEdit(tr("-1"));
    QPushButton* btnOK  =   new QPushButton(tr("OK"));
    QPushButton* btnCancel  =   new QPushButton(tr("cancel"));

    connect(btnOK,SIGNAL(pressed()),this,SLOT(on_btnOK()));
    connect(btnCancel,SIGNAL(pressed()),this,SLOT(close()));


    QHBoxLayout* hbl    =   new QHBoxLayout;
    hbl->addWidget(ledValue);
    QHBoxLayout* vbl    =   new QHBoxLayout;
    vbl->addStretch();
    vbl->addWidget(btnOK);
    vbl->addWidget(btnCancel);
    QVBoxLayout* layout =   new QVBoxLayout;
    layout->addWidget(new QLabel(discribe));
    layout->addLayout(hbl);
    layout->addLayout(vbl);
    this->setLayout(layout);

    this->setWindowTitle(title);
}
void RangeDialog::on_btnOK(){
    //no error check!
    *_lowVal =   ledtLow->text().toFloat();
    *_highVal   =   ledtHigh->text().toFloat();
    qDebug()<<*_lowVal;
    this->close();
}
void NumberDialog::on_btnOK(){
    *_value=ledValue->text().toFloat();
    this->close();
}
