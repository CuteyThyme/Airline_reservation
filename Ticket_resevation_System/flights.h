#ifndef FLIGHTS_H
#define FLIGHTS_H
#include<iostream>
#include<vector>
#include<map>
#include<set>

#include<QDateTime>
#include<QMetaType>
#include<QMessageBox>
#include<QString>

using namespace std;
struct Tickets
{
    QString no;// 航班号
    QString beg;// 起点
    QString ed;// 终点
    int tk;// 总票数
    int num;// 剩余票数
    int money;// 价格
    QDate date; //起飞日期
    QTime beg_tim;//起飞时间
    QTime end_tim;//到达时间
    int dis;  //航程
};
//QT中定义结构体要向QT声明
Q_DECLARE_METATYPE(Tickets);

typedef struct Node
{
    Tickets ticket;
    struct Node *next;
}LinkNode,*LinkList;
Q_DECLARE_METATYPE(Node);
//extern LinkList L;

class Flight{
public:
    Flight(QWidget *parent = nullptr);
   ~Flight();
   const int size=1e3+5;
   const int inf=0x3f3f3f3f;
   typedef pair<int,int> pii;
   vector<string> roar;//记录路线

   static LinkList L;

   vector<Tickets> total;//存储所有航线和车次
   set<string> point;//存储城市（具有自动去重的能力）
   map<string,int> pointno;//将城市映射为数字

   double Distance(LinkNode a,LinkNode b);
   LinkList Init();
   void Insert(LinkList *L,QString flightId);
   void Delete(LinkList *L,QString flightId);
   void Clear(LinkList *L);
   int min_money(string s,string t);//最小费用
int min_trans(string s,string t);//最少中转次数;
};


#endif // FLIGHTS_H
