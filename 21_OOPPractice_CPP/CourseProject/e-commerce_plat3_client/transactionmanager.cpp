#include "transactionmanager.h"

TransactionManagerWidget::TransactionManagerWidget(User *user, QVector<User *> &ulist, QVector<Product *> &plist, QVector<QPair<Product *, int> > &clist, QVector<Order> &olist, QWidget *parent)
    : QWidget(parent), user_list(ulist), product_list(plist), cart_list(clist), order_list(olist)
{
    now_user = user;
    cart_manager = nullptr;
    order_manager = nullptr;

    setFixedSize(280, 100);

    if (now_user->getUserType() == "Buyer")
    {
        cart_btn = new QPushButton("购物车", this);
        cart_btn->move(10, 0);
        connect(cart_btn, &QPushButton::clicked, this, &TransactionManagerWidget::openCart);
        order_btn = new QPushButton("我的订单", this);
        order_btn->move(95, 0);
        connect(order_btn, &QPushButton::clicked, this, &TransactionManagerWidget::openOrderManager);
    }
}

void TransactionManagerWidget::openCart()
{
    if (cart_manager == nullptr)
    {
        cart_manager = new CartManager(now_user, cart_list);
        connect(cart_manager, &CartManager::destroyed, this, &TransactionManagerWidget::closeCart);
        connect(cart_manager, &CartManager::makeOrder, this, &TransactionManagerWidget::recvMakeOrder);
    }
}

void TransactionManagerWidget::closeCart()
{
    cart_manager = nullptr;
}

void TransactionManagerWidget::recvMakeOrder()
{
    Order new_order;
    for (auto i = cart_list.begin(); i < cart_list.end(); i++)
        if (i->first->getAmount() - i->second < 0)
        {
            QErrorMessage *error = new QErrorMessage();
            error->showMessage("商品余量不足，订单生成失败");
            return;
        }
    for (auto i = cart_list.begin(); i < cart_list.end(); i++)
    {
        new_order.list.push_back(qMakePair(*(i->first), i->second));
        i->first->setAmount(i->first->getAmount() - i->second);     //冻结
    }
    new_order.buying_time = QDateTime::currentDateTime();
    new_order.is_purchased = false;
    new_order.is_canceled = false;
    order_list.push_front(new_order);
    cart_list.clear();
    emit refreshNow();
}

void TransactionManagerWidget::openOrderManager()
{
    if (order_manager == nullptr)
    {
        order_manager = new OrderManager(now_user, user_list, product_list, order_list);
        connect(order_manager, &OrderManager::destroyed, this, &TransactionManagerWidget::closeOrderManager);
        connect(order_manager, &OrderManager::refreshSignal, this, &TransactionManagerWidget::recvRefreshSignal);
    }
}

void TransactionManagerWidget::closeOrderManager()
{
    order_manager = nullptr;
}

void TransactionManagerWidget::recvAddToCart(Product *product, int amount)
{
    bool found = false;
    for (auto i = cart_list.begin(); i < cart_list.end(); i++)
        if (i->first == product)
        {
            found = true;
            i->second += amount;
        }
    if (!found)
        cart_list.push_back(qMakePair(product, amount));
}

void TransactionManagerWidget::recvRefreshSignal()
{
    emit refreshNow();
}

TransactionManagerWidget::~TransactionManagerWidget()
{

}

CartManager::CartManager(User *user, QVector<QPair<Product *, int> > &list, QWidget *parent)
    : QWidget(parent), my_list(list)
{
    now_user = user;

    setAttribute(Qt::WA_QuitOnClose,false);
    setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(370, 240);
    setWindowTitle("购物车");

    total_price_text = new QLabel(this);
    QFont ft;
    ft.setPointSize(12);
    ft.setBold(true);
    total_price_text->setFont(ft);
    total_price_text->move(20, 210);

    table_widget = new QTableWidget(my_list.size(), 4, this);
    table_widget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_widget->setSelectionMode(QAbstractItemView::SingleSelection);
    table_widget->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_widget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    table_widget->verticalHeader()->setHidden(true);
    table_widget->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
    table_widget->setColumnWidth(0, 180);
    table_widget->setColumnWidth(1, 70);
    table_widget->setColumnWidth(2, 75);
    table_widget->setColumnWidth(3, 20);
    table_widget->setFixedSize(369, 200);
    QStringList header;
    header << "商品名" << "价格" << "数量" << " ";
    table_widget->setHorizontalHeaderLabels(header);
    table_widget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:#dddddd;}");
    for (int i = 0; i < my_list.size(); i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem(my_list[i].first->getName());
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        table_widget->setItem(i, 0, item);
        item = new QTableWidgetItem(QString::number(my_list[i].first->getPrice()));
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        table_widget->setItem(i, 1, item);
        QuantityWidget *quantity_widget = new QuantityWidget(1, my_list[i].first->getAmount());
        quantity_widget->setValue(my_list[i].second);
        connect(quantity_widget, &QuantityWidget::valueChanged, this, &CartManager::refreshTotalPrice);
        connect(quantity_widget, &QuantityWidget::valueChanged, this, &CartManager::updateCartList);
        table_widget->setCellWidget(i, 2, quantity_widget);
        item = new QTableWidgetItem(" ×");
        table_widget->setItem(i, 3, item);
    }
    connect(table_widget, &QTableWidget::cellClicked, this, &CartManager::deleteProductInCart);
    connect(table_widget, &QTableWidget::clicked, this, &CartManager::refreshTotalPrice);

    submit_btn = new QPushButton("提交订单", this);
    submit_btn->move(280, 207);
    connect(submit_btn, &QPushButton::clicked, this, &CartManager::sendMakeOrderSignal);
    refreshTotalPrice();
    show();
}

void CartManager::deleteProductInCart(int row, int col)
{
    if (col == 3)
    {
        table_widget->removeRow(row);
        my_list.erase(my_list.begin() + row);
    }
}

void CartManager::refreshTotalPrice()
{
    double total_price = 0;
    for (int i = 0; i < table_widget->rowCount(); i++)
        total_price += table_widget->item(i, 1)->text().toDouble() * ((QuantityWidget *)table_widget->cellWidget(i, 2))->value();
    total_price_text->setText("金额总计:" + QString::number(total_price) + "元");
    total_price_text->adjustSize();
    if (table_widget->rowCount() == 0)
        submit_btn->setDisabled(true);
    else
        submit_btn->setEnabled(true);
}

void CartManager::updateCartList()
{
    for (int i = 0; i < table_widget->rowCount(); i++)
    {
        my_list[i].second = ((QuantityWidget *)table_widget->cellWidget(i, 2))->value();
    }
}

void CartManager::sendMakeOrderSignal()
{
    emit makeOrder();
    close();
}

OrderDetail::OrderDetail(User *user, QVector<User *> &ulist, QVector<Product *> &plist, Order &order, QWidget *parent)
    : QWidget(parent), user_list(ulist), product_list(plist), now_order(order)
{
    now_user = user;
    setAttribute(Qt::WA_QuitOnClose,false);
    setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(329, 240);
    setWindowTitle("订单详情");

    QFont ft;
    ft.setPointSize(10);
    ft.setBold(true);
    time_text = new QLabel(order.buying_time.toString(), this);
    time_text->setFont(ft);
    time_text->move(10, 7);

    double total_price = 0;
    for (auto j = now_order.list.begin(); j < now_order.list.end(); j++)
        total_price += j->first.getPrice() * j->second;
    total_price_text = new QLabel("金额总计:" + QString::number(total_price) + "元", this);
    total_price_text->setFont(ft);
    total_price_text->move(200, 7);

    error_text = new QLabel(this);
    error_text->setStyleSheet("color:red;");
    error_text->move(120, 210);
    error_text->hide();

    table_widget = new QTableWidget(order.list.size(), 3, this);
    table_widget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_widget->setSelectionMode(QAbstractItemView::SingleSelection);
    table_widget->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_widget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    table_widget->verticalHeader()->setHidden(true);
    table_widget->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
    table_widget->setColumnWidth(0, 180);
    table_widget->setColumnWidth(1, 70);
    table_widget->setColumnWidth(2, 60);
    table_widget->setFixedSize(329, 170);
    table_widget->move(0, 30);
    QStringList header;
    header << "商品名" << "价格" << "数量";
    table_widget->setHorizontalHeaderLabels(header);
    table_widget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:#dddddd;}");
    for (int i = 0; i < now_order.list.size(); i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem(now_order.list[i].first.getName());
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        table_widget->setItem(i, 0, item);
        item = new QTableWidgetItem(QString::number(now_order.list[i].first.getPrice()));
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        table_widget->setItem(i, 1, item);
        item = new QTableWidgetItem(QString::number(now_order.list[i].second));
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        table_widget->setItem(i, 2, item);
    }

    if (!now_order.is_purchased && !now_order.is_canceled)
    {
        cancel_btn = new QPushButton("取消订单", this);
        cancel_btn->move(15, 207);
        connect(cancel_btn, &QPushButton::clicked, this, &OrderDetail::cancelOrder);

        purchase_btn = new QPushButton("付款", this);
        purchase_btn->move(240, 207);
        connect(purchase_btn, &QPushButton::clicked, this, &OrderDetail::purchaseOrder);

        error_text->setText("请在30分钟内付款");
        error_text->show();
    }

    if (now_order.is_canceled)
    {
        error_text->setText("订单已取消");
        error_text->show();
    }

    show();
}

void OrderDetail::purchaseOrder()
{
    double total_price = 0;
    for (auto i = now_order.list.begin(); i < now_order.list.end(); i++)
        total_price += i->first.getPrice() * i->second;
    if (now_user->getBalance() > total_price)
    {
        now_user->setBalance(now_user->getBalance() - total_price);
        for (auto i = now_order.list.begin(); i < now_order.list.end(); i++)
        {
            for (auto j = user_list.begin(); j < user_list.end(); j++)
            {
                if ((*j)->getUserName() == i->first.getSellerName())
                    (*j)->setBalance((*j)->getBalance() + i->first.getPrice() * i->second);
            }
        }
        now_order.is_purchased = true;
        close();
    }
    else
    {
        error_text->setText("余额不足!");
        error_text->show();
    }
    emit refreshSignal();
}

void OrderDetail::cancelOrder()
{
    for (auto i = now_order.list.begin(); i < now_order.list.end(); i++)    //恢复冻结
    {
        for (auto j = product_list.begin(); j < product_list.end(); j++)
            if (i->first.getName() == (*j)->getName() && i->first.getSellerName() == (*j)->getSellerName())
            {
                (*j)->setAmount((*j)->getAmount() + i->second);
                break;
            }
    }
    now_order.is_canceled = true;
    emit refreshSignal();
    close();
}

OrderManager::OrderManager(User *user, QVector<User *> &ulist, QVector<Product *> &plist, QVector<Order> &olist, QWidget *parent)
    : QWidget(parent), user_list(ulist),product_list(plist), order_list(olist)
{
    now_user = user;
    order_detail = nullptr;
    setAttribute(Qt::WA_QuitOnClose,false);
    setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(344, 220);
    setWindowTitle("我的订单");

    table_widget = new QTableWidget(order_list.size(), 3, this);
    table_widget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_widget->setSelectionMode(QAbstractItemView::SingleSelection);
    table_widget->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_widget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    table_widget->verticalHeader()->setHidden(true);
    table_widget->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
    table_widget->setColumnWidth(0, 180);
    table_widget->setColumnWidth(1, 70);
    table_widget->setColumnWidth(2, 75);
    table_widget->setFixedSize(344, 200);
    QStringList header;
    header << "商品名" << "价格" << "状态";
    table_widget->setHorizontalHeaderLabels(header);
    table_widget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:#dddddd;}");
    refreshAndSendRefreshSignal();
    connect(table_widget, &QTableWidget::itemDoubleClicked, this, &OrderManager::openOrderDetail);

    for (auto i = order_list.begin(); i < order_list.end(); i++)    //过期取消
        if (QDateTime::currentDateTime() > i->buying_time.addSecs(1800))
            i->is_canceled = true;
    show();
}

void OrderManager::openOrderDetail(QTableWidgetItem *item)
{
    if (order_detail == nullptr)
    {
        order_detail = new OrderDetail(now_user, user_list, product_list, order_list[item->row()]);
        connect(order_detail, &OrderDetail::destroyed, this, &OrderManager::closeOrderDetail);
        connect(order_detail, &OrderDetail::refreshSignal, this, &OrderManager::refreshAndSendRefreshSignal);
    }
}

void OrderManager::closeOrderDetail()
{
    order_detail = nullptr;
}

void OrderManager::refreshAndSendRefreshSignal()
{
    table_widget->setRowCount(order_list.size());
    table_widget->clearContents();
    for (int i = 0; i < order_list.size(); i++)
    {
        int product_count = 0;
        double total_price = 0;
        for (auto j = order_list[i].list.begin(); j < order_list[i].list.end(); j++)
        {
            product_count += j->second;
            total_price += j->first.getPrice() * j->second;
        }

        QTableWidgetItem *item;
        if (product_count == 1)
            item = new QTableWidgetItem(order_list[i].list.front().first.getName());
        else
            item = new QTableWidgetItem(order_list[i].list.front().first.getName() + "等共" + QString::number(product_count) + "件商品");
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        table_widget->setItem(i, 0, item);
        item = new QTableWidgetItem(QString::number(total_price));
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        table_widget->setItem(i, 1, item);
        if (order_list[i].is_canceled)
            item = new QTableWidgetItem("已取消");
        else if (order_list[i].is_purchased)
            item = new QTableWidgetItem("已付款");
        else
            item = new QTableWidgetItem("未付款");
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        table_widget->setItem(i, 2, item);
    }
    emit refreshSignal();
}
