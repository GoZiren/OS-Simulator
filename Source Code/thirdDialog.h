#ifndef ThirdDialog_H
#define ThirdDialog_H

#include <QMainWindow>
#include "ui_thirdDialog.h"
#include<QtDebug>
#include<QString>
#include<QStandardItemModel>
#include <QTableView>
#include <QList>
#include<iostream>
QT_BEGIN_NAMESPACE
namespace Ui { class ThirdDialog; }
QT_END_NAMESPACE



class ThirdDialog : public QMainWindow
{
    Q_OBJECT

public:
    ThirdDialog(QString table, QWidget *parent = nullptr);
    ~ThirdDialog();
    void process1(double size);
    void process2(double size);
    void process3(double size);
    void process4(double size);
    void process5(double size);
    void updatePM(QString table);
    void updateVM(QString table);
    void updateM(QString table);

private slots:
    void light1();
    void light2();
    void light3();
    void light4();
    void light5();

private:
    Ui::ThirdDialog *ui;
};
#endif // ThirdDialog_H
