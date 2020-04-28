#include "querywindow.h"
#include "ui_querywindow.h"
#include "resultwindow.h"
#include<QPushButton>
#include<QButtonGroup>
#include<QDebug>
#include "flights.h"

QButtonGroup* bgGroup;
QString QueryWindow::method;
QString QueryWindow::stcity;
QString QueryWindow::edcity;
QDate QueryWindow::date;

QueryWindow::QueryWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QueryWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("机票查询"));

    bgGroup = new QButtonGroup(this);
    bgGroup->addButton(ui->radioButton);
    bgGroup->addButton(ui->radioButton_2);
    bgGroup->setId(ui->radioButton,0);
    bgGroup->setId(ui->radioButton_2,1);

}

QueryWindow::~QueryWindow()
{
    delete ui;
}

void QueryWindow::on_pushButton_2_clicked()
{
    this->hide();
    emit sendsignal();
}

void QueryWindow::on_pushButton_clicked()
{
    QRadioButton* pbtn=qobject_cast<QRadioButton*>(bgGroup->checkedButton());
    method=pbtn->objectName();
    stcity=ui->textEdit->toPlainText();
    edcity=ui->textEdit_2->toPlainText();
    date=ui->dateEdit->date();
    if(stcity.isEmpty()||edcity.isEmpty())
    {
        QMessageBox::warning(this,tr("航班查询"),tr("请输入出发城市和到达城市"),QMessageBox::Ok);
    }
    else
    {
        this->hide();   //关闭首页界面
        ResultWindow* result = new ResultWindow(this);   //进入订票查询界面
        connect(result,SIGNAL(sendsignal()),this,SLOT(reshow()));
        result->show();
    }

}

void QueryWindow::reshow()
{
    this->show();
}
