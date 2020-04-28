#include "checkwindow.h"
#include "ui_checkwindow.h"

#include<QSqlDatabase>
#include<QMessageBox>
#include<QSqlQuery>
#include<QVariantList>
#include<QSqlError>
#include<QDebug>
#include<QSqlTableModel>
#include<QString>
#include<QSqlQueryModel>

CheckWindow::CheckWindow(QWidget *parent) :
    QMainWindow(parent),
ui(new Ui::CheckWindow)
{
    ui->setupUi(this);
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("1124533854");
    db.setDatabaseName("airlines");
    db.open();
    static QSqlQueryModel *model = new QSqlQueryModel(ui->tableView);
    model->setQuery(QString("select * from tickets"));
    model->setHeaderData(1, Qt::Horizontal, tr("航班号"));
    model->setHeaderData(2, Qt::Horizontal, tr("票价"));
    ui->tableView->setModel(model);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

   QSqlQuery query(db);
    query.exec(QString("select * from tickets"));
    if(query.next()){
        QString text = query.value(1).toString();
        ui->label->setText(text);
    }

}

CheckWindow::~CheckWindow()
{
    delete ui;
}
