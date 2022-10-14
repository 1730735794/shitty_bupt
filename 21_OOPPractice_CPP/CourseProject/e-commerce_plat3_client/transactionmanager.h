#ifndef TRANSACTIONMANAGER_H
#define TRANSACTIONMANAGER_H

#include <QWidget>
#include <QVector>
#include <QPushButton>
#include <QTableWidget>
#include <QLineEdit>
#include <QPair>
#include <QHeaderView>
#include <QLabel>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QErrorMessage>
#include "user.h"
#include "product.h"
#include "quantitywidget.h"

class CartManager : public QWidget
{
    Q_OBJECT
private:
    User *now_user;
    QVector<QPair<Product *, int> > &my_list;
    QTableWidget *table_widget;
    QLabel *total_price_text;
    QPushButton *submit_btn;
    void deleteProductInCart(int row, int col);
    void refreshTotalPrice();
    void updateCartList();
    void sendMakeOrderSignal();
public:
    explicit CartManager(User *user, QVector<QPair<Product *, int> > &list, QWidget *parent = nullptr);
signals:
    void makeOrder();
};

struct Order
{
    QVector<QPair<Product, int> > list;
    QDateTime buying_time;
    bool is_purchased;
    bool is_canceled;
};

class OrderDetail : public QWidget
{
    Q_OBJECT
private:
    User *now_user;
    QVector<User *> &user_list;  //从json文件读取的用户信息
    QVector<Product *> &product_list;
    Order &now_order;
    QTableWidget *table_widget;
    QLabel *time_text;
    QLabel *total_price_text;
    QPushButton *purchase_btn;
    QPushButton *cancel_btn;
    QLabel *error_text;
    void purchaseOrder();
    void cancelOrder();
public:
    explicit OrderDetail(User *user, QVector<User *> &ulist, QVector<Product *> &plist, Order &order, QWidget *parent = nullptr);
signals:
    void refreshSignal();
};

class OrderManager : public QWidget
{
    Q_OBJECT
private:
    User *now_user;
    QVector<User *> &user_list;  //从json文件读取的用户信息
    QVector<Product *> &product_list;
    QVector<Order> &order_list;
    QTableWidget *table_widget;
    OrderDetail *order_detail;
    void openOrderDetail(QTableWidgetItem *item);
    void closeOrderDetail();
    void refreshAndSendRefreshSignal();
public:
    explicit OrderManager(User *user, QVector<User *> &ulist, QVector<Product *> &plist, QVector<Order> &olist, QWidget *parent = nullptr);
signals:
    void refreshSignal();
};

class TransactionManagerWidget : public QWidget
{
    Q_OBJECT
private:
    User *now_user;
    QVector<User *> &user_list;  //从json文件读取的用户信息
    QVector<Product *> &product_list;
    QVector<QPair<Product *, int> > &cart_list;
    QVector<Order> &order_list;
    QPushButton *cart_btn;
    CartManager *cart_manager;
    QPushButton *order_btn;
    OrderManager *order_manager;
    void openCart();
    void closeCart();
    void recvMakeOrder();
    void openOrderManager();
    void closeOrderManager();
    void recvRefreshSignal();
public:
    explicit TransactionManagerWidget(User *user, QVector<User *> &ulist, QVector<Product *> &plist, QVector<QPair<Product *, int> > &clist, QVector<Order> &olist, QWidget *parent = nullptr);
    void recvAddToCart(Product *product, int amount);
    ~TransactionManagerWidget();
signals:
    void refreshNow();
};

#endif // TRANSACTIONMANAGER_H
