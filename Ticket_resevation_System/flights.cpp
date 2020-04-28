#include "flights.h"

Flight::Flight(QWidget *parent)
{

}
LinkList Flight::Init()
{
    LinkList L=(LinkList)malloc(sizeof(LinkNode));
    L->next=NULL;
    return L;
}

void Flight::Insert(LinkList *L,QString flightId)
{
     LinkNode *newNode=new LinkNode;
     (newNode->ticket).no=flightId;
     newNode->next=(*L)->next;
     (*L)->next=newNode;
}

void Flight::Delete(LinkList *L, QString flightId)
{
    if(*L==NULL||(*L)->next==NULL)
       {
           return;
       }
       LinkNode *q=(*L)->next;
       LinkNode *p=(*L);
       while(q)
       {
           if(QString::compare((q->ticket).no,flightId)==0)
           {
               p->next=q->next;
               free(q);
               return;
           }
           p=q;
           q=q->next;
       }

}

void Flight::Clear(LinkList *L)
{
    LinkList p,q;
       p=(*L)->next;
       while(p)
       {
         q=p->next;
         free(p);
         p=q;
       }
       (*L)->next=NULL; /* 头结点指针域为空 */
}

