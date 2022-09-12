#include "thirdDialog.h"

ThirdDialog::ThirdDialog(QString table, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ThirdDialog)
{
    ui->setupUi(this);
    updateM(table);

    //--------------connecting function----------------------//
    connect(this->ui->Btn1,SIGNAL(clicked(bool)),this,SLOT(light1()));
    connect(this->ui->Btn2,SIGNAL(clicked(bool)),this,SLOT(light2()));
    connect(this->ui->Btn3,SIGNAL(clicked(bool)),this,SLOT(light3()));
    connect(this->ui->Btn4,SIGNAL(clicked(bool)),this,SLOT(light4()));
    connect(this->ui->Btn5,SIGNAL(clicked(bool)),this,SLOT(light5()));

}

ThirdDialog::~ThirdDialog()
{
    delete ui;
}

//--------------Define Btn Function. When Pressed, light up the display window----------------------//
void ThirdDialog::light1(){
    this->ui->info1->setStyleSheet ("background-color: rgb(215, 54, 54);");
    this->ui->info2->setStyleSheet ("background-color: rgb(0, 0, 0);");
    this->ui->info3->setStyleSheet ("background-color: rgb(0, 0, 0);");
    this->ui->info4->setStyleSheet ("background-color: rgb(0, 0, 0);");
    this->ui->info5->setStyleSheet ("background-color: rgb(0, 0, 0);");
}

void ThirdDialog::light2(){
    this->ui->info1->setStyleSheet ("background-color: rgb(0, 0, 0);");
    this->ui->info2->setStyleSheet ("background-color: rgb(54, 70, 163);");
    this->ui->info3->setStyleSheet ("background-color: rgb(0, 0, 0);");
    this->ui->info4->setStyleSheet ("background-color: rgb(0, 0, 0);");
    this->ui->info5->setStyleSheet ("background-color: rgb(0, 0, 0);");
}

void ThirdDialog::light3(){
    this->ui->info1->setStyleSheet ("background-color: rgb(0, 0, 0);");
    this->ui->info2->setStyleSheet ("background-color: rgb(0, 0, 0);");
    this->ui->info3->setStyleSheet ("background-color: rgb(29, 115, 29);");
    this->ui->info4->setStyleSheet ("background-color: rgb(0, 0, 0);");
    this->ui->info5->setStyleSheet ("background-color: rgb(0, 0, 0);");
}

void ThirdDialog::light4(){
    this->ui->info1->setStyleSheet ("background-color: rgb(0, 0, 0);");
    this->ui->info2->setStyleSheet ("background-color: rgb(0, 0, 0);");
    this->ui->info3->setStyleSheet ("background-color: rgb(0, 0, 0);");
    this->ui->info4->setStyleSheet ("background-color: rgb(235, 235, 0);");
    this->ui->info5->setStyleSheet ("background-color: rgb(0, 0, 0);");
}

void ThirdDialog::light5(){
    this->ui->info1->setStyleSheet ("background-color: rgb(0, 0, 0);");
    this->ui->info2->setStyleSheet ("background-color: rgb(0, 0, 0);");
    this->ui->info3->setStyleSheet ("background-color: rgb(0, 0, 0);");
    this->ui->info4->setStyleSheet ("background-color: rgb(0, 0, 0);");
    this->ui->info5->setStyleSheet ("background-color: rgb(243, 0, 243);");
}

double total = 6048;
//----------------------------- html setting string ---------------------------------------//
QString p2 = " margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;";
QString p3 = "><span style=";
QString p4 = " font-family:'SimSun'; font-size:7pt; font-weight:600; color:#ffffff;";
QString p5 = ">memory consume</span><span style=";
QString p6 = " font-family:'SimSun'; font-size:7pt; color:#ffffff;";
QString p7 = ">:";
QString s1 = QString("%1 %2 %3 %4 %5 %6").arg(p2).arg(p3).arg(p4).arg(p5).arg(p6).arg(p7);
QString p8 = "Byte</span></p><p style=";
QString p9 = "margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;";
QString p10 = "><span style=" ;
QString p11 = "font-family:'SimSun'; font-size:7pt; font-weight:600; color:#ffffff;";
QString p12 = ">percentage</span><span style=";
QString p13 = "font-family:'SimSun'; font-size:7pt; color:#ffffff;";
QString p14 = ">:	";
QString s3 = QString("%1 %2 %3 %4 %5 %6 %7").arg(p8).arg(p9).arg(p10).arg(p11).arg(p12).arg(p13).arg(p14);
QString s5 = "%</span></p></body></html>";


//------------Define Updating function. Update display window & shape of VM according to the size of process-------------//
void ThirdDialog::process1(double size){
    //-------------- Update display window-------------------//
    QString p1 = ">     Process 1      </span></p><p style=";
    QString s2 = QString::number(100.0);
    s2.setNum(size);
    double percent = size/total * 100;
    QString s4 = QString::number(percent);
    QString out = p1+s1+s2+s3+s4+s5;
    this->ui->info1->setHtml(out);
    //-------------- Update shape of VM-------------------//
    double h1 = 502*percent/100;
    double h2 = this->ui->Btn2->height();
    double h3 = this->ui->Btn3->height();
    double h4 = this->ui->Btn4->height();
    double h5 = this->ui->Btn5->height();
    double h1r;
    if (h1 > 30){h1r = h1;}
    else if (h1 == 0){h1r = 0;}
    else {h1r = 30;}
    this->ui->Btn1->setGeometry(0,0,141, h1r);
    this->ui->Btn2->setGeometry(0,h1r,141,h2);
    this->ui->Btn3->setGeometry(0,h1r+h2,141,h3);
    this->ui->Btn4->setGeometry(0,h1r+h2+h3,141,h4);
    this->ui->Btn5->setGeometry(0,h1r+h2+h3+h4,141,h5);
    this->ui->Btn1->repaint();
    this->ui->Btn2->repaint();
    this->ui->Btn3->repaint();
    this->ui->Btn4->repaint();
    this->ui->Btn5->repaint();
    this->ui->info1->repaint();
}

void ThirdDialog::process2(double size){
    //-------------- Update display window-------------------//
    QString p1 = ">     Process 2      </span></p><p style=";
    QString s2 = QString::number(100.0);
    s2.setNum(size);
    double percent = size/total * 100;
    QString s4 = QString::number(percent);
    QString out = p1+s1+s2+s3+s4+s5;
    this->ui->info2->setHtml(out);
    //-------------- Update shape of VM-------------------//
    double h1 = this->ui->Btn1->height();
    double h2 = 502*percent/100;
    double h3 = this->ui->Btn3->height();
    double h4 = this->ui->Btn4->height();
    double h5 = this->ui->Btn5->height();
    double h2r;
    if (h2 > 30){h2r = h2;}
    else if (h2 == 0){h2r = 0;}
    else {h2r = 30;}
    this->ui->Btn1->setGeometry(0,0,141, h1);
    this->ui->Btn2->setGeometry(0,h1,141,h2r);
    this->ui->Btn3->setGeometry(0,h1+h2r,141,h3);
    this->ui->Btn4->setGeometry(0,h1+h2r+h3,141,h4);
    this->ui->Btn5->setGeometry(0,h1+h2r+h3+h4,141,h5);
    this->ui->Btn1->repaint();
    this->ui->Btn2->repaint();
    this->ui->Btn3->repaint();
    this->ui->Btn4->repaint();
    this->ui->Btn5->repaint();
    this->ui->info2->repaint();
}

void ThirdDialog::process3(double size){
    //-------------- Update display window-------------------//
    QString p1 = ">     Process 3      </span></p><p style=";
    QString s2 = QString::number(100.0);
    s2.setNum(size);
    double percent = size/total * 100;
    QString s4 = QString::number(percent);
    QString out = p1+s1+s2+s3+s4+s5;
    this->ui->info3->setHtml(out);
    //-------------- Update shape of VM-------------------//
    double h1 = this->ui->Btn1->height();
    double h2 = this->ui->Btn2->height();
    double h3 = 502*percent/100;
    double h4 = this->ui->Btn4->height();
    double h5 = this->ui->Btn5->height();
    double h3r;
    if (h3 > 30){h3r = h3;}
    else if (h3 == 0){h3r = 0;}
    else {h3r = 30;}
    this->ui->Btn1->setGeometry(0,0,141, h1);
    this->ui->Btn2->setGeometry(0,h1,141,h2);
    this->ui->Btn3->setGeometry(0,h1+h2,141,h3r);
    this->ui->Btn4->setGeometry(0,h1+h2+h3r,141,h4);
    this->ui->Btn5->setGeometry(0,h1+h2+h3r+h4,141,h5);
    this->ui->Btn1->repaint();
    this->ui->Btn2->repaint();
    this->ui->Btn3->repaint();
    this->ui->Btn4->repaint();
    this->ui->Btn5->repaint();
    this->ui->info3->repaint();
}

void ThirdDialog::process4(double size){
    //-------------- Update display window-------------------//
    QString p1 = ">     Process 4      </span></p><p style=";
    QString s2 = QString::number(100.0);
    s2.setNum(size);
    double percent = size/total * 100;
    QString s4 = QString::number(percent);
    QString out = p1+s1+s2+s3+s4+s5;
    this->ui->info4->setHtml(out);
    //-------------- Update shape of VM-------------------//
    double h1 = this->ui->Btn1->height();
    double h2 = this->ui->Btn2->height();
    double h3 = this->ui->Btn3->height();
    double h4 = 502*percent/100;
    double h5 = this->ui->Btn5->height();

    double h4r;
    if (h4 > 30){h4r = h4;}
    else if (h4 == 0){h4r = 0;}
    else {h4r = 30;}
    this->ui->Btn1->setGeometry(0,0,141, h1);
    this->ui->Btn2->setGeometry(0,h1,141,h2);
    this->ui->Btn3->setGeometry(0,h1+h2,141,h3);
    this->ui->Btn4->setGeometry(0,h1+h2+h3,141,h4r);
    this->ui->Btn5->setGeometry(0,h1+h2+h3+h4r,141,h5);
    this->ui->Btn1->repaint();
    this->ui->Btn2->repaint();
    this->ui->Btn3->repaint();
    this->ui->Btn4->repaint();
    this->ui->Btn5->repaint();
    this->ui->info4->repaint();
}

void ThirdDialog::process5(double size){
    //-------------- Update display window-------------------//
    QString p1 = ">     Process 5      </span></p><p style=";
    QString s2 = QString::number(100.0);
    s2.setNum(size);
    double percent = size/total * 100;
    QString s4 = QString::number(percent);
    QString out = p1+s1+s2+s3+s4+s5;
    this->ui->info5->setHtml(out);
    //-------------- Update shape of VM-------------------//
    double h1 = this->ui->Btn1->height();
    double h2 = this->ui->Btn2->height();
    double h3 = this->ui->Btn3->height();
    double h4 = this->ui->Btn4->height();
    double h5 = 502*percent/100;
    double h5r;
    if (h5 > 30){h5r = h5;}
    else if (h5 == 0){h5r = 0;}
    else {h5r = 30;}
    this->ui->Btn1->setGeometry(0,0,141, h1);
    this->ui->Btn2->setGeometry(0,h1,141,h2);
    this->ui->Btn3->setGeometry(0,h1+h2,141,h3);
    this->ui->Btn4->setGeometry(0,h1+h2+h3,141,h4);
    this->ui->Btn5->setGeometry(0,h1+h2+h3+h4,141,h5r);
    this->ui->Btn1->repaint();
    this->ui->Btn2->repaint();
    this->ui->Btn3->repaint();
    this->ui->Btn4->repaint();
    this->ui->Btn5->repaint();
    this->ui->info5->repaint();
}

void ThirdDialog::updatePM(QString items){
    QStandardItemModel* model = new QStandardItemModel();
    QStringList labels = QString("ini_pos,starting \n address,size,occupy").split(",");
    model->setHorizontalHeaderLabels(labels);

    //定义item
    QStringList itemset = items.split(",");
    QStandardItem* item = 0;
    for(int i = 0; i < itemset.length()/4; i++){
        item = new QStandardItem(QString("%1").arg(itemset[i*4+1]));
        model->setItem(i,0,item);
        model->item(i,0)->setTextAlignment(Qt::AlignHCenter);
        item = new QStandardItem(QString("%1").arg(itemset[i*4+2]));
        model->setItem(i,1,item);
        model->item(i,1)->setTextAlignment(Qt::AlignHCenter);
        item = new QStandardItem(QString("%1").arg(itemset[i*4+3]));
        model->setItem(i,2,item);
        model->item(i,2)->setTextAlignment(Qt::AlignHCenter);
        item = new QStandardItem(QString("%1").arg(itemset[i*4+4]));
        model->setItem(i,3,item);
        model->item(i,3)->setTextAlignment(Qt::AlignHCenter);
    }
    this->ui->pm->setModel(model);
    this->ui->pm->repaint();
    this->ui->pm->show();
}

void ThirdDialog::updateVM(QString table){
    QStringList itemset = table.split(",");
    QStringList sizes;
    for (int i = 0; i < itemset.length()/4; i++){
        if (itemset[i*4+4] == "1"){
            sizes.append(itemset[i*4+3]);
        }
    }

    int num_process = sizes.length(); 
    if (num_process == 0){
        ThirdDialog::process1(double(0));
        ThirdDialog::process2(double(0));
        ThirdDialog::process3(double(0));
        ThirdDialog::process4(double(0));
        ThirdDialog::process5(double(0));
    }
    else if (num_process == 1){
        int size1 = sizes.at(0).toInt();
        ThirdDialog::process1(double(size1));
        ThirdDialog::process2(double(0));
        ThirdDialog::process3(double(0));
        ThirdDialog::process4(double(0));
        ThirdDialog::process5(double(0));
    }
    else if (num_process == 2){
        int size1 = sizes.at(0).toInt();
        int size2 = sizes.at(1).toInt();
        ThirdDialog::process1(double(size1));
        ThirdDialog::process2(double(size2));
        ThirdDialog::process3(double(0));
        ThirdDialog::process4(double(0));
        ThirdDialog::process5(double(0));
    }
    else if (num_process == 3){
        int size1 = sizes.at(0).toInt();
        int size2 = sizes.at(1).toInt();
        int size3 = sizes.at(2).toInt();
        ThirdDialog::process1(double(size1));
        ThirdDialog::process2(double(size2));
        ThirdDialog::process3(double(size3));
        ThirdDialog::process4(double(0));
        ThirdDialog::process5(double(0));
    }
    else if (num_process == 4){
        int size1 = sizes.at(0).toInt();
        int size2 = sizes.at(1).toInt();
        int size3 = sizes.at(2).toInt();
        int size4 = sizes.at(3).toInt();
        ThirdDialog::process1(double(size1));
        ThirdDialog::process2(double(size2));
        ThirdDialog::process3(double(size3));
        ThirdDialog::process4(double(size4));
        ThirdDialog::process5(double(0));
    }
    else {
        int size1 = sizes.at(0).toInt();
        int size2 = sizes.at(1).toInt();
        int size3 = sizes.at(2).toInt();
        int size4 = sizes.at(3).toInt();
        int size5 = sizes.at(4).toInt();
        ThirdDialog::process1(double(size1));
        ThirdDialog::process2(double(size2));
        ThirdDialog::process3(double(size3));
        ThirdDialog::process4(double(size4));
        ThirdDialog::process5(double(size5));
    }
}

void ThirdDialog::updateM(QString table){
    ThirdDialog::updatePM(table);
    ThirdDialog::updateVM(table);
}
