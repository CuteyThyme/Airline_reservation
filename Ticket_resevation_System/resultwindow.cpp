#include "resultwindow.h"
#include "ui_resultwindow.h"
#include "querywindow.h"
#include "flights.h"

#include<QSqlDatabase>
#include<QMessageBox>
#include<QSqlQuery>
#include<QVariantList>
#include<QSqlError>
#include<QDebug>
#include<QSqlQueryModel>
#include<QTableView>
#include<QSqlRecord>
#include<QMessageBox>
#include<QModelIndex>
#include<QSqlTableModel>
#include<QDate>
#include<QStandardItemModel>

#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<cstring>

const int MAXV=100;
const int INF=100000000;

Flight *fflight=new Flight();
static LinkList L;
int n;
set<QString> cityName;  //存储所有城市
map<QString,int> citymp;  //将城市名映射成数字
int G[MAXV][MAXV],d[MAXV],weight[MAXV];
bool visit[MAXV]={false};
vector<int> pre[MAXV];
vector<int> path,tempPath;

ResultWindow::ResultWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ResultWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("查询结果"));
    fill(G[0],G[0]+MAXV*MAXV,INF);
    QSqlDatabase db = linkDataBase();

    if(db.open() ){
        Result();
          QSqlQueryModel *model = new QSqlQueryModel(ui->tableView);
          QString query;
          if(QString::compare(QueryWindow::method,"radioButton")==0)
              query=QString("select 航班号,起点,终点,起飞日期,起飞时间,到达时间,票价,航程 from tickets where 起点='" + QueryWindow::stcity + "'and 终点='" +QueryWindow::edcity+"'and 起飞日期='" +QueryWindow::date.toString("yyyy/MM/dd")+"' order by 航程");
          else
              query=QString("select 航班号,起点,终点,起飞日期,起飞时间,到达时间,票价,航程 from tickets where 起点='" + QueryWindow::stcity + "'and 终点='" +QueryWindow::edcity+"'and 起飞日期='" +QueryWindow::date.toString("yyyy/MM/dd")+"'order by 票价");
          model->setQuery(query);
          QSqlQuery sql;
          sql.exec(query);
          if(sql.size()!=0)
          {
              ui->tableView->setModel(model);
              ui->tableView->verticalHeader()->hide();
              ui->tableView->setColumnWidth(0,90);
              ui->tableView->setColumnWidth(1,90);
              ui->tableView->setColumnWidth(2,90);
              ui->tableView->setColumnWidth(3,100);
              ui->tableView->setColumnWidth(4,100);
              ui->tableView->setColumnWidth(5,100);
              ui->tableView->setColumnWidth(6,90);
              ui->tableView->setColumnWidth(7,90);

              ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
              ui->label_2->hide();
              ui->listView_2->hide();

          }
          else
          {
              QMessageBox::information(this,"查询结果","无满足条件的航班，请参考推荐方案！",QMessageBox::Ok,QMessageBox::Ok);
              ui->tableView->hide();
              ui->label->hide();
              ui->label_2->show();
              ui->listView_2->show();

              Dijsktra(citymp[QueryWindow::stcity]);
              DFS(citymp[QueryWindow::edcity]);
              QStandardItemModel *ItemModel = new QStandardItemModel(ui->listView_2);
              QStringList strList;
              if(QString::compare(QueryWindow::method,"radioButton")==0)
              {
                  strList.append("参考方案(航程最短): ");
              }
              else
              {
                  strList.append("参考方案(费用最少): ");
              }
              QSqlQuery qquery;
              strList.append("航班号  出发城市  到达城市  起飞日期  起飞时间  到达时间  总票数  剩余票数  票价  航程");
              for(int i=path.size()-1;i>0;i--)
              {
                  QString start,end;
                  for(auto it:citymp)
                  {
                      if(it.second==path[i])
                          start=it.first;
                      if(it.second==path[i-1])
                          end=it.first;

                  }
                  strList.append(start+"->"+end);
                  QString str=QString("select * from tickets where 起点='" + start + "'and 终点='" + end +"'");
                  qquery.exec(str);
                  qquery.next();
                  qDebug()<<qquery.value(3)<<qquery.value(4)<<qquery.value(5)<<qquery.value(6)<<qquery.value(7)<<qquery.value(8)<<qquery.value(9);
                  strList.append(qquery.value(0).toString()+"     "+qquery.value(1).toString()+"        "+qquery.value(2).toString()+"   "
                                  +qquery.value(3).toString()+"  "+qquery.value(4).toString()+"  "+qquery.value(5).toString()+"   "+
                                 qquery.value(6).toString()+"     "+qquery.value(7).toString()+"      "+qquery.value(8).toString()+"      "+qquery.value(9).toString());
              }
              int nCount = strList.size();
              for(int i = 0; i < nCount; i++)
              {
                  QString string = static_cast<QString>(strList.at(i));
                  QStandardItem *item = new QStandardItem(string);
                  ItemModel->appendRow(item);
              }
              ui->listView_2->setModel(ItemModel);
              ui->listView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
              ui->listView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
          }

}
}

ResultWindow::~ResultWindow()
{
    delete ui;
}

QSqlDatabase ResultWindow::linkDataBase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("1124533854");
    db.setDatabaseName("airlines");
    return db;
}

void ResultWindow::on_pushButton_clicked()
{
    this->close();
    emit sendsignal();
}

void ResultWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
       Purchase(index);
}

void ResultWindow::Purchase(const QModelIndex &index)
{
    QMessageBox::StandardButton reply;
       reply = QMessageBox::information(this, tr("机票购买"),tr("确定购买该航班次的票吗？"),
                                       QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
       //确定购买机票，则相应的机票数减1
       if (reply == QMessageBox::Yes)
       {
           QSqlTableModel *model = new QSqlTableModel;
           model->setTable("tickets"); //指定要做模型的表
           model->select();
           model->setEditStrategy(QSqlTableModel::OnManualSubmit);
           QSqlRecord record=model->record(index.row());
           QString flightId =  record.value("航班号").toString();
           int ticketnum = record.value("剩余票数").toInt();
           if(ticketnum>0)
           {
               int newticketnum=ticketnum-1;
               model->setData(model->index(index.row(),7),newticketnum);
               model->submitAll();
               QMessageBox::information(this,"机票购买","机票购买成功",QMessageBox::Ok,QMessageBox::Ok);
               LinkNode *p =L->next;
               while(p)
               {
                   if(QString::compare((p->ticket).no,flightId)==0)
                    {
                       (p->ticket).num= (p->ticket).num-1;
                       qDebug()<<(p->ticket).num;
                        break;
                  }
                   p=p->next;
               }
           }
           else if(ticketnum==0)
           {
               QMessageBox::information(this,"机票购买","剩余票数不足",QMessageBox::Ok,QMessageBox::Ok);
           }
       };
}
void ResultWindow::Result()
{
    QSqlQuery query;
    QString str=QString("select * from tickets");
    query.exec(str);
    L=fflight->Init();
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
        cityName.insert((newNode->ticket).beg);
        cityName.insert((newNode->ticket).ed);
    }
    int i=0;
    for(auto it:cityName)
    {
        citymp[it]=i;
        i++;
    }
    LinkNode *p=L->next;
    while(p)
    {
        G[citymp[(p->ticket).beg]][citymp[(p->ticket).ed]]=(p->ticket).dis;
        G[citymp[(p->ticket).ed]][citymp[(p->ticket).beg]]=(p->ticket).dis;
        p=p->next;
    }
   /* LinkNode *p=L->next;
    while(p)
    {
        qDebug()<<"航班号: "<<(p->ticket).no<<" 起点: "<<(p->ticket).beg<<" 终点："<<(p->ticket).ed<<" 起飞时间："
               <<(p->ticket).beg_tim<<" 到达时间："<<(p->ticket).end_tim<<" 总票数: "<<(p->ticket).tk<<" 剩余票数: "
              <<(p->ticket).num<<" 票价: "<<(p->ticket).money<<" 航程："<<(p->ticket).dis;
        p=p->next;
    }*/
    for(auto it:citymp)
    {
        qDebug()<<it.first<<" :"<<it.second;
    }
}

void ResultWindow::Dijsktra(int s)
{
    fill(d,d+MAXV,INF);
    n=citymp.size();
        d[s]=0;
        for(int i=0;i<=n;i++)
        {
           int u=-1,min=INF;
           for(int j=0;j<=n;j++)
           {
               if(visit[j]==false&&d[j]<min)
               {

                    u=j;
                    min=d[j];
               }
           }
            if(u==-1)
                return;
            visit[u]=true;
            qDebug()<<"u: "<<u;
            for(int v=0;v<=n;v++)
            {
                if(visit[v]==false&&G[u][v]!=INF)
                {
                    if(G[u][v]+d[u]<d[v])
                   {
                        d[v]=G[u][v]+d[u];
                        pre[v].clear();
                        pre[v].push_back(u);
                   }
                    else if(G[u][v]+d[u]==d[v])
                   {
                       pre[v].push_back(u);
                   }
                }
            }
        }
}

void ResultWindow::DFS(int v)
{
    tempPath.push_back(v);
       if(v == citymp[QueryWindow::stcity]){
           path=tempPath;
       }

       else{
           for(int i = 0; i < pre[v].size(); i++){
               DFS(pre[v][i]);
       }
       tempPath.pop_back();
       }
}



void ResultWindow::on_listView_2_doubleClicked(const QModelIndex &index)
{
    QMessageBox::StandardButton reply;
       reply = QMessageBox::information(this, tr("机票购买"),tr("确定购买该航班次的票吗？"),
                                       QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
       //确定购买机票，则相应的机票数减1
       if (reply == QMessageBox::Yes)
       {
           QStandardItemModel * model = qobject_cast<QStandardItemModel *>(ui->listView_2->model());
           QModelIndexList modelIndexList = ui->listView_2->selectionModel()->selectedIndexes();
           QString str=model->data(index).toString();
           QString flightId = str.mid(0,5);//分隔字符串，获取航班号
           QSqlQuery sqlnum;
           QString sqlstr=QString("select 剩余票数 from tickets where 航班号='"+flightId+"'");
           sqlnum.exec(sqlstr);
           sqlnum.next();
           int ticketnum=sqlnum.value(0).toInt();
           int rowNum=0;
           if(ticketnum>0)
           {
               QSqlTableModel *newmodel = new QSqlTableModel(this);
               newmodel->setTable("tickets");
               ticketnum=ticketnum-1;

               ui->tableView->setModel(newmodel);
               //newmodel->setEditStrategy(QSqlTableModel::OnManualSubmit);
                newmodel->select();
               for(int i=0;i<newmodel->rowCount();i++)
               {
                    QAbstractItemModel *amodel=ui->tableView->model();
                    QModelIndex index=amodel->index(i,0);
                    QString id=amodel->data(index).toString();
                    if(QString::compare(id,flightId)==0)
                    {
                        rowNum=i;
                        break;
                    }

                 }
               qDebug()<<rowNum;
               newmodel->setData(newmodel->index(rowNum,7),ticketnum);
               newmodel->submitAll();
               QMessageBox::information(this,"机票购买","机票购买成功",QMessageBox::Ok,QMessageBox::Ok);
               LinkNode *p =L->next;
               while(p)
               {
                   if(QString::compare((p->ticket).no,flightId)==0)
                    {
                       (p->ticket).num= (p->ticket).num-1;
                       qDebug()<<(p->ticket).num;
                        break;
                  }
                   p=p->next;
               }
           }
           else if(ticketnum==0)
           {
               QMessageBox::information(this,"机票购买","剩余票数不足",QMessageBox::Ok,QMessageBox::Ok);
           }

       };
}
