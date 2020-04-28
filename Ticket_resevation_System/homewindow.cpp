#include "homewindow.h"
#include "querywindow.h"
#include "ui_homewindow.h"
#include "managewindow.h"
#include<QPushButton>

HomeWindow::HomeWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HomeWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("订票系统首页"));

}

HomeWindow::~HomeWindow()
{
    delete ui;
}


void HomeWindow::on_pushButton_clicked()
{
    this->hide();   //关闭首页界面
    QueryWindow* query = new QueryWindow(this);   //进入订票查询界面
    connect(query,SIGNAL(sendsignal()),this,SLOT(reshow()));
    query->show();
}

void HomeWindow::reshow()
{
    this->show();
}

void HomeWindow::on_pushButton_2_clicked()
{
    this->hide();   //关闭首页界面
    ManageWindow* manage = new ManageWindow(this);   //进入订票查询界面
    connect(manage,SIGNAL(sendsignal()),this,SLOT(reshow()));
    manage->show();
}
