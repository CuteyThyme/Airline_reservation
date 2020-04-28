#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
namespace Ui {
class ResultWindow;
}

class ResultWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ResultWindow(QWidget *parent = nullptr);
    ~ResultWindow();
    QSqlDatabase linkDataBase();
    void Purchase(const QModelIndex &index);
    void Result();
    void Dijsktra(int s);
    void DFS(int v);

private slots:
    void on_pushButton_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_tableView_2_activated(const QModelIndex &index);

    void on_tableView_2_clicked(const QModelIndex &index);

    void on_listView_2_clicked(const QModelIndex &index);

    void on_listView_2_doubleClicked(const QModelIndex &index);

private:
    Ui::ResultWindow *ui;

signals:
    void sendsignal();

};
#endif // RESULTWINDOW_H
