#include "mainwindow.h"

MainWindow::MainWindow(User *user, QVector<User *> &ulist, QJsonObject &pjson, QJsonObject &cjson, QJsonObject &ojson, QWidget *parent)
    : QMainWindow(parent), user_list(ulist), product_json(pjson),cart_json(cjson), order_json(ojson)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(700, 500);

    now_user = user;

    //读取json并存入product_list
    QJsonArray product_array = product_json.value(QStringLiteral("product_info")).toArray();
    for (int i = 0; i < product_array.size(); i++)
    {
        Product *new_product;
        QString name = product_array[i].toObject()["name"].toString();
        QString type = product_array[i].toObject()["type"].toString();
        QString describe = product_array[i].toObject()["describe"].toString();
        double price = product_array[i].toObject()["price"].toDouble();
        int discount = product_array[i].toObject()["discount"].toInt();
        int amount = product_array[i].toObject()["amount"].toInt();
        QString sellername = product_array[i].toObject()["sellername"].toString();
        if (type == "Book")
            new_product = new BookProduct(name, describe, price, discount, amount, sellername);
        else if (type == "Electronic")
            new_product = new ElectronicProduct(name, describe, price, discount, amount, sellername);
        else if (type == "Clothing")
            new_product = new ClothingProduct(name, describe, price, discount, amount, sellername);
        else
            new_product = new Product(name, describe, price, discount, amount, sellername);
        product_list.push_back(new_product);
    }

    //读取购物车信息
    if (now_user != nullptr)
    {
        QJsonArray cart_array = cart_json.value(now_user->getUserName()).toArray();
        for (int i = 0; i < cart_array.size(); i++)
        {
            QString name = cart_array[i].toObject()["name"].toString();
            QString type = cart_array[i].toObject()["type"].toString();
            QString describe = cart_array[i].toObject()["describe"].toString();
            double price = cart_array[i].toObject()["price"].toDouble();
            int discount = cart_array[i].toObject()["discount"].toInt();
            int amount = cart_array[i].toObject()["amount"].toInt();
            QString sellername = cart_array[i].toObject()["sellername"].toString();
            int buying_amount = cart_array[i].toObject()["buying_amount"].toInt();
            for (auto i = product_list.begin(); i < product_list.end(); i++)
            {
                if ((*i)->getName() == name && (*i)->getType() == type && (*i)->getDescribe() == describe && (*i)->getOriginalPrice() == price && (*i)->getDiscount() == discount && (*i)->getAmount() == amount && (*i)->getSellerName() == sellername)
                    cart_list.push_back(qMakePair(*i, buying_amount));
            }
        }
    }

    //读取订单信息
    if (now_user != nullptr)
    {
        QJsonArray order_array = order_json.value(now_user->getUserName()).toArray();
        for (int i = 0; i < order_array.size(); i++)
        {
            Order new_order;
            QJsonArray cart_array = order_array[i].toObject()["list"].toArray();
            for (int j = 0; j < cart_array.size(); j++)
            {
                QString name = cart_array[j].toObject()["name"].toString();
                QString type = cart_array[j].toObject()["type"].toString();
                QString describe = cart_array[j].toObject()["describe"].toString();
                double price = cart_array[j].toObject()["price"].toDouble();
                int discount = cart_array[j].toObject()["discount"].toInt();
                int amount = cart_array[j].toObject()["amount"].toInt();
                QString sellername = cart_array[j].toObject()["sellername"].toString();
                int buying_amount = cart_array[j].toObject()["buying_amount"].toInt();
                Product product(name, describe, price, discount, amount, sellername, type);
                new_order.list.push_back(qMakePair(product, buying_amount));
            }
            new_order.buying_time = QDateTime::fromString(order_array[i].toObject()["buying_time"].toString());
            new_order.is_purchased = order_array[i].toObject()["is_purchased"].toBool();
            new_order.is_canceled = order_array[i].toObject()["is_canceled"].toBool();
            order_list.push_back(new_order);
        }
    }

    account_manager_bar = new AccountManagerBar(now_user, this);
    connect(account_manager_bar, &AccountManagerBar::toLogin, this, &MainWindow::recvToLogin);

    product_manager = new ProductManagerWidget(now_user, product_list, this);
    product_manager->move(0, 24);

    if (now_user != nullptr)
    {
        transaction_manager = new TransactionManagerWidget(now_user, user_list, product_list, cart_list, order_list, this);
        connect(product_manager, &ProductManagerWidget::addToCart, transaction_manager, &TransactionManagerWidget::recvAddToCart);
        connect(transaction_manager, &TransactionManagerWidget::refreshNow, product_manager, &ProductManagerWidget::refresh);
        connect(transaction_manager, &TransactionManagerWidget::refreshNow, account_manager_bar, &AccountManagerBar::refresh);
        transaction_manager->move(520, 260);
    }
}

void MainWindow::recvToLogin()
{
    emit toLogin();
    close();
}

MainWindow::~MainWindow()
{
    //保存订单信息
    if (now_user != nullptr)
    {
        QJsonArray order_array;
        for (auto i = order_list.begin(); i < order_list.end(); i++)
        {
            QJsonObject object;
            QJsonArray cart_array;
            for (auto j = i->list.begin(); j < i->list.end(); j++)
            {
                QJsonObject object;
                object["name"] = j->first.getName();
                object["type"] = j->first.getType();
                object["describe"] = j->first.getDescribe();
                object["price"] = j->first.getOriginalPrice();
                object["discount"] = j->first.getDiscount();
                object["amount"] = j->first.getAmount();
                object["sellername"] = j->first.getSellerName();
                object["buying_amount"] = j->second;
                cart_array.push_back(object);
            }
            object["list"] = cart_array;
            object["buying_time"] = i->buying_time.toString();
            object["is_purchased"] = i->is_purchased;
            object["is_canceled"] = i->is_canceled;
            order_array.push_back(object);
        }
        order_json[now_user->getUserName()] = order_array;
    }

    //保存购物车信息
    if (now_user != nullptr)
    {
        QJsonArray cart_array;
        for (auto i = cart_list.begin(); i < cart_list.end(); i++)
        {
            QJsonObject object;
            object["name"] = i->first->getName();
            object["type"] = i->first->getType();
            object["describe"] = i->first->getDescribe();
            object["price"] = i->first->getOriginalPrice();
            object["discount"] = i->first->getDiscount();
            object["amount"] = i->first->getAmount();
            object["sellername"] = i->first->getSellerName();
            object["buying_amount"] = i->second;
            cart_array.push_back(object);
        }
        cart_json[now_user->getUserName()] = cart_array;
    }

    //保存商品信息
    QJsonArray array;
    for (auto i = product_list.begin(); i < product_list.end(); i++)
    {
        QJsonObject object;
        object["name"] = (*i)->getName();
        object["type"] = (*i)->getType();
        object["describe"] = (*i)->getDescribe();
        object["price"] = (*i)->getOriginalPrice();
        object["discount"] = (*i)->getDiscount();
        object["amount"] = (*i)->getAmount();
        object["sellername"] = (*i)->getSellerName();
        array.push_back(object);
    }
    product_json["product_info"] = array;

    //释放内存
    for (auto i = product_list.begin(); i < product_list.end(); i++)
        delete *i;
}

