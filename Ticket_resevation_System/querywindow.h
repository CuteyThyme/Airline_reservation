#ifndef QUERYWINDOW_H
#define QUERYWINDOW_H
#include<QString>
#include <QMainWindow>

namespace Ui {
class QueryWindow;
}

class QueryWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit QueryWindow(QWidget *parent = nullptr);
    ~QueryWindow();
    static QString method,stcity,edcity;
    static QDate date;


private:
    Ui::QueryWindow *ui;


private slots:
    void on_pushButton_2_clicked();
    void reshow();
    void on_pushButton_clicked();

signals:
    void sendsignal();

};
#endif // QUERYWINDOW_H
