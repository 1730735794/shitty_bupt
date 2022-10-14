#include "mainwindow.h"
#include "login.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *w = new MainWindow(nullptr);
    Login *l = new Login(w);
    l->hide();
    w->show();
    int ret_value = a.exec();
    l->~Login();
    return ret_value;
}
