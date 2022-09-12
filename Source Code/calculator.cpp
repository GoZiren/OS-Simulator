#include "calculator.h"
#include "ui_calculator.h"

#include <qmath.h>
calculator::calculator(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::calculator)
{
        setFixedSize(500, 400);
        ui->setupUi(this);

        SumSoFar = 0.0;
        FactorSoFar = 0.0;
        waitingForOperand = 1;  //1 means true

        //numberkey
        connect(ui->pushButton_0, SIGNAL(clicked()), this, SLOT(digitClicked()));
        connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(digitClicked()));
        connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(digitClicked()));
        connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(digitClicked()));
        connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(digitClicked()));
        connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(digitClicked()));
        connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(digitClicked()));
        connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(digitClicked()));
        connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(digitClicked()));
        connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(digitClicked()));

        //Four operations key
        connect(ui->pushButton_Addition, SIGNAL(clicked()), this, SLOT(lateradderOperatorClicked()));
        connect(ui->pushButton_Subtraction, SIGNAL(clicked()), this, SLOT(lateradderOperatorClicked()));
        connect(ui->pushButton_Multiplication, SIGNAL(clicked()), this, SLOT(latermulOperatorClicked()));
        connect(ui->pushButton_Division, SIGNAL(clicked()), this, SLOT(latermulOperatorClicked()));

        //"." key
        connect(ui->pushButton_point, SIGNAL(clicked()), this, SLOT(pointClicked()));

        //clearkey
        connect(ui->pushButton_DEL, SIGNAL(clicked()), this, SLOT(DELClicked()));
        connect(ui->pushButton_AC, SIGNAL(clicked()), this, SLOT(clearALL()));

        //equalKey
        connect(ui->pushButton_Equal, SIGNAL(clicked()), this, SLOT(equalClicked()));

}


calculator::~calculator()
{
        delete ui;
}

void calculator::digitClicked()
{
        QPushButton *clicked;
        clicked = qobject_cast<QPushButton *>(sender());

        int value = 0;
        double Pi;
        //    double a = 0;        //temporary variable ,store decimal
        if (clicked->text() == "Pi")
        {
                Pi = getPi();
                ui->lineEdit_output->clear();
        }
        else
        {
                value = clicked->text().toInt();
        }

        if (ui->lineEdit_output->text() == "0" && value == 0)
                return;

        if (waitingForOperand)
        {
                ui->lineEdit_output->clear();
                waitingForOperand = 0;
        }

        if (clicked->text() == "Pi")
        {
                ui->lineEdit_output->setText(QString::number(Pi, 'f', 10));
                waitingForOperand = 1;
        }
        else
        {
                //a=ui->lineEdit_output->text().toDouble() + value;
                ui->lineEdit_output->setText(ui->lineEdit_output->text() + QString::number(value));
        }
}

void calculator::equalClicked()
{
        double a = ui->lineEdit_output->text().toDouble();

        if (!pendingMultiplicativeOperator.isEmpty())
        {
                if (!Calculate(a, pendingMultiplicativeOperator))
                {
                        abortOperation();
                        return;
                }
                a = FactorSoFar;
                FactorSoFar = 0;
                pendingMultiplicativeOperator.clear();
        }
        if (!pendingAdditiveOperator.isEmpty())
        {
                if (!Calculate(a, pendingAdditiveOperator))
                {
                        abortOperation();
                        return;
                }
                pendingAdditiveOperator.clear();
        }
        else
        {
                SumSoFar = a;
        }
        if (SumSoFar >= (10 ^ 6))
        {
                ui->lineEdit_output->setText(QString::number(SumSoFar));
        }
        else
        {
                ui->lineEdit_output->setText(QString::number(SumSoFar, 'f', 10));
        }
        SumSoFar = 0;
        waitingForOperand = 1;
}

void calculator::lateradderOperatorClicked()
{
        QPushButton *clicked = qobject_cast<QPushButton *>(sender());
        QString operatorname = clicked->text();
        double a = ui->lineEdit_output->text().toDouble();

        if (!pendingMultiplicativeOperator.isEmpty())
        {
                if (!Calculate(a, pendingMultiplicativeOperator))
                {
                        abortOperation();
                        return;
                }
                if (FactorSoFar >= (10 ^ 6))
                {
                        ui->lineEdit_output->setText(QString::number(FactorSoFar));
                }
                else
                {
                        ui->lineEdit_output->setText(QString::number(FactorSoFar, 'f', 10));
                }
                a = FactorSoFar;
                FactorSoFar = 0;
                pendingMultiplicativeOperator.clear();
        }

        if (!pendingAdditiveOperator.isEmpty())
        {
                if (!Calculate(a, pendingAdditiveOperator))
                {
                        abortOperation();
                        return;
                }
                if (SumSoFar >= (10 ^ 6))
                {
                        ui->lineEdit_output->setText(QString::number(SumSoFar));
                }
                else
                {
                        ui->lineEdit_output->setText(QString::number(SumSoFar, 'f', 10));
                }
        }
        else
        {
                SumSoFar = a;
        }

        pendingAdditiveOperator = operatorname;

        waitingForOperand = 1;
}

void calculator::latermulOperatorClicked()
{
        QPushButton *clicked = qobject_cast<QPushButton *>(sender());
        QString operatorname = clicked->text();
        double a = ui->lineEdit_output->text().toDouble();

        if (!pendingMultiplicativeOperator.isEmpty())
        {
                if (!Calculate(a, pendingMultiplicativeOperator))
                {
                        abortOperation();
                        return;
                }
                if (FactorSoFar >= (10 ^ 6))
                {
                        ui->lineEdit_output->setText(QString::number(FactorSoFar));
                }
                else
                {
                        ui->lineEdit_output->setText(QString::number(FactorSoFar, 'f', 10));
                }
        }
        else
        {
                FactorSoFar = a;
        }

        pendingMultiplicativeOperator = operatorname;
        waitingForOperand = 1;
}

void calculator::MCclicked()
{
    sumInMemory=0.0;
}
void calculator::MRclicked()
{
    ui->lineEdit_output->setText(QString::number(sumInMemory));
    waitingForOperand=true;
}
void calculator::Maddclicked()
{
    equalClicked();
    sumInMemory+=ui->lineEdit_output->text().toDouble();
}
void calculator::Msubclicked()
{
    equalClicked();
    sumInMemory-=ui->lineEdit_output->text().toDouble();
}

void calculator::pointClicked()
{
        if (waitingForOperand)
        {
                ui->lineEdit_output->setText("0");
        }
        if (!(ui->lineEdit_output->text().contains(".")))
        {
                ui->lineEdit_output->setText(ui->lineEdit_output->text() + tr("."));
        }
        waitingForOperand = 0;
}

void calculator::change()
{
        QString text = ui->lineEdit_output->text();
        double value = ui->lineEdit_output->text().toDouble();

        if (value > 0)
        {
                text.prepend(tr("-"));
        }
        else  if (value < 0)
        {
                text.remove(0, 1);
        }
        ui->lineEdit_output->setText(text);
}

void calculator::Adsclicked()
{
    double value = ui->lineEdit_output->text().toDouble();
    if (value < 0)
        ui->lineEdit_output->setText(ui->lineEdit_output->text().remove(0, 1));
}

void calculator::DELClicked()
{
        QString text = ui->lineEdit_output->text();
        text.chop(1);       //delete the last number
        if (text.isEmpty())
        {
                text = '0';
                waitingForOperand = 1;
        }
        ui->lineEdit_output->setText(text);
}

void calculator::clearALL()
{
        SumSoFar = 0;
        FactorSoFar = 0;
        ui->lineEdit_output->setText("0");
        pendingAdditiveOperator.clear();
        pendingMultiplicativeOperator.clear();
        waitingForOperand = 1;
}

void calculator::unaryfunction(){}

double calculator::getPi()
{
        return pi;
}

void calculator::on_pushButton_Tensquare_clicked(){}

bool calculator::Calculate(double rightOperand, const QString &pendingOperator)
{
        if (pendingOperator == QObject::tr("+"))
        {
                SumSoFar += rightOperand;
        }
        else if (pendingOperator == QObject::tr("-"))
        {
                SumSoFar -= rightOperand;
        }
        else if (pendingOperator == QObject::tr("*"))
        {
                FactorSoFar *= rightOperand;
        }
        else if (pendingOperator == QObject::tr("/"))
        {
                if (rightOperand == 0.0)
                        return 0;
                FactorSoFar /= rightOperand;
        }
        else if (pendingOperator == QObject::tr("x^y"))
        {
                SumSoFar = pow(SumSoFar, rightOperand);
        }
        else if (pendingOperator == QObject::tr("*10^x"))
        {
                SumSoFar = SumSoFar*pow(10, rightOperand);
        }
        return 1;
}

void calculator::abortOperation()
{
        clearALL();
        ui->lineEdit_output->setText(QObject::tr("Math ERROR"));
}
