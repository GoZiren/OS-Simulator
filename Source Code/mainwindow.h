#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "secdialog.h"
#include "thirdDialog.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


/* Main login window for operating system
in this page, user will be asked to input username,
passport, and root path*/
class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_pushButton_Login_clicked();



private:
    Ui::MainWindow *ui;
    SecDialog *secDialog;
    ThirdDialog *thirdDialog;
};
#endif // MAINWINDOW_H
