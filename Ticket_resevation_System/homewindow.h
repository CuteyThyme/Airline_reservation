#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QMainWindow>
#include<QPushButton>
#include "querywindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class HomeWindow; }
QT_END_NAMESPACE

class HomeWindow : public QMainWindow
{
    Q_OBJECT

public:
    HomeWindow(QWidget *parent = nullptr);
    ~HomeWindow();

private slots:
    void on_pushButton_clicked();
    void reshow();

    void on_pushButton_2_clicked();

private:
    Ui::HomeWindow *ui;
    QPushButton *pushButton;
};
#endif // HOMEWINDOW_H
