#ifndef MANAGEWINDOW_H
#define MANAGEWINDOW_H

#include "flights.h"
#include <QMainWindow>
#include <QTableView>
namespace Ui {
class ManageWindow;
}

class ManageWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ManageWindow(QWidget *parent = nullptr);
    ~ManageWindow();

private:
    Ui::ManageWindow *ui;


signals:
    void sendsignal();
private slots:
    void on_pushButton_4_clicked();
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_clicked(bool checked);
    void on_pushButton_7_clicked();
};

#endif // MANAGEWINDOW_H
