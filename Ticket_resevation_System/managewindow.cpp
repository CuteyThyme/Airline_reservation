#include "managewindow.h"
#include "ui_managewindow.h"
#include "flights.h"

#include<QSqlDatabase>
#include<QMessageBox>
#include<QSqlQuery>
#include<QVariantList>
#include<QSqlError>
#include<QDebug>
#include<QSqlTableModel>
#include<QString>
#include<QSqlQueryModel>
#include<QStandardItem>
#include<QDateTime>

Flight *flight=new Flight();
static LinkList L;
static QSqlTableModel *model;
ManageWindow::ManageWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ManageWindow)
{
    ui->setupUi(this);
    setWindowTitle("航班管理");
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("1124533854");
    db.setDatabaseName("airlines");
    db.open();
    if(db.open() ){
          qDebug() << "success to connect mysql:";
          model = new QSqlTableModel(ui->tableView);
          // model->setEditStrategy(QSqlTableModel::OnManualSubmit);
          model->setTable("tickets");
                 model->select();
                 ui->tableView->setModel(model);
                 ui->tableView->verticalHeader()->hide();
                 ui->tableView->setColumnWidth(0,70);
                 ui->tableView->setColumnWidth(1,70);
                 ui->tableView->setColumnWidth(2,70);
                 ui->tableView->setColumnWidth(3,100);
                 ui->tableView->setColumnWidth(4,100);
                 ui->tableView->setColumnWidth(5,130);
                 ui->tableView->setColumnWidth(6,70);
                 ui->tableView->setColumnWidth(7,70);
                 ui->tableView->setColumnWidth(8,70);
                 ui->tableView->setColumnWidth(9,70);
                 model->setEditStrategy(QSqlTableModel::OnManualSubmit);

       }

    else
    {
        qDebug()<<"fail to connect mysql:";
    }


}

ManageWindow::~ManageWindow()
{
    delete ui;
}

void ManageWindow::on_pushButton_4_clicked()
{
    this->hide();
    emit sendsignal();
}

void ManageWindow::on_pushButton_clicked()
{
    /*QSqlTableModel *model = new QSqlTableModel;
    model->setTable("tickets"); //指定要做模型的表
    model->select();
    //model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableView->setModel(model);*/
    QString flightId = ui->lineEdit_2->text();
    if(flightId.isEmpty())
    {
        QMessageBox::warning(this,tr("添加航班"),tr("请输入航班号!"),QMessageBox::Ok);
    }
    else
    {
        bool flag=true;
        for(int i=0;i<ui->tableView->model()->rowCount();i++)
        {
             QAbstractItemModel *amodel=ui->tableView->model();
             QModelIndex index=amodel->index(i,0);
             QString id=amodel->data(index).toString();
             if(QString::compare(id,flightId)==0)
             {
                 flag=false;
                 break;
             }

          }
        if(flag)
        {
            int rowNum = model->rowCount(); //获得表的行数
            model->insertRow(rowNum); //添加一行

            QString st=ui->start->text();
            QString ed=ui->end->text();
            //flight->Insert(Flight::L,flightId);
            model->setData(model->index(rowNum,0),flightId);
            model->setData(model->index(rowNum,1),st);
            model->setData(model->index(rowNum,2),ed);
        }
        else
        {
            QMessageBox::warning(this,tr("添加航班"),tr("无法添加，已存在此航班!"),QMessageBox::Ok);
        }


    }
   /* model->submitAll();
    model->setTable("tickets");
    model->select();
    ui->tableView->setModel(model);*/
}


void ManageWindow::on_pushButton_5_clicked()
{
   /* QSqlTableModel *model = new QSqlTableModel;
    model->setTable("tickets");
    model->select();
    ui->tableView->setModel(model);*/
    QString flightId=ui->lineEdit_2->text();
    if(flightId.isEmpty())
    {
        QMessageBox::warning(this,tr("删除航班"),tr("请输入航班号!"),QMessageBox::Ok);
    }
    else
    {
        bool flag=false;
        int row;
        for(int i=0;i<ui->tableView->model()->rowCount();i++)
        {
            QAbstractItemModel *amodel=ui->tableView->model();
            QModelIndex index=amodel->index(i,0);
            QString id=amodel->data(index).toString();
            if(QString::compare(id,flightId)==0)
            {
                flag=true;
                row = i;
                break;
            }
        }
        if(flag)
        {
            model->removeRow(row);
            QMessageBox::StandardButton reply;
               reply = QMessageBox::information(this, tr("删除航班"),tr("确定删除该航班吗？"),
                                               QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
               //撤销删除
               if (reply == QMessageBox::Yes)
               {
                   model->submitAll();

               }
               else if (reply == QMessageBox::No)
               {
                   model->revertAll();
               }
        }
        else
        {
              QMessageBox::warning(this,tr("删除航班"),tr("无法删除，不存在该航班!"),QMessageBox::Ok);
        }
    }


}

void ManageWindow::on_pushButton_6_clicked()
{
    /*QSqlTableModel *model = new QSqlTableModel;
    model->setTable("tickets");
    model->select();
    ui->tableView->setModel(model);*/
    //将数据库中的内容读入链表中
    model->submitAll();

}


void ManageWindow::on_pushButton_7_clicked()
{

   QSqlQuery query;
   QString str=QString("select * from tickets");
   query.exec(str);
   L=flight->Init();
   while(query.next())  //头插法建表
   {

       LinkNode* newNode=new LinkNode;
       (newNode->ticket).no=query.value(0).toString();
       (newNode->ticket).beg=query.value(1).toString();
       (newNode->ticket).ed=query.value(2).toString();
       (newNode->ticket).date=query.value(3).toDate();
       (newNode->ticket).beg_tim=query.value(4).toTime();
       (newNode->ticket).end_tim=query.value(5).toTime();
       (newNode->ticket).tk=query.value(6).toInt();
       (newNode->ticket).num=query.value(7).toInt();
       (newNode->ticket).money=query.value(8).toInt();
       (newNode->ticket).dis=query.value(9).toInt();
       newNode->next=L->next;
       L->next=newNode;

   }
   LinkNode *p=L->next;
   while(p)
   {
       qDebug()<<"航班号: "<<(p->ticket).no<<" 起点: "<<(p->ticket).beg<<" 终点："<<(p->ticket).ed<<" 起飞时间："
              <<(p->ticket).beg_tim<<" 到达时间："<<(p->ticket).end_tim<<" 总票数: "<<(p->ticket).tk<<" 剩余票数: "
             <<(p->ticket).num<<" 票价: "<<(p->ticket).money<<" 航程："<<(p->ticket).dis;
       p=p->next;
   }
}
