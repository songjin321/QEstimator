#pragma once

//some utility user interfaces defined here

#include <QtWidgets>

//dialog ask for range
class RangeDialog:  public QDialog
{
    Q_OBJECT
public:
    RangeDialog(float* lowVal,float* highVal,
            QDialog* parent = nullptr,
                QString title = "",
                QString discribe = "input range: "
            );
private slots:
    void on_btnOK();

private:
    QLineEdit* ledtLow;
    QLineEdit* ledtHigh;

    float* _lowVal;
    float* _highVal;

};

class NumberDialog:public QDialog
{
    Q_OBJECT
public:
     NumberDialog(float *value,QDialog *parent,QString title,QString discribe);
private slots:
    void on_btnOK();
private:
    QLineEdit* ledValue;
    float* _value;
};











