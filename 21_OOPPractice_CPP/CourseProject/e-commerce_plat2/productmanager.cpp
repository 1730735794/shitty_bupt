#include "productmanager.h"

ProductDisplayer::ProductDisplayer(User *user, QVector<Product *> &list, int index, QWidget *parent) : QWidget(parent), product_list(list), now_index(index)
{
    now_user = user;

    setAttribute(Qt::WA_QuitOnClose,false);
    setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(220, 290);
    setWindowTitle("商品信息");

    product_name_text = new QLabel("商品名称: " + product_list[now_index]->getName(), this);
    product_name_text->setWordWrap(true);
    product_name_text->setFixedSize(180, 25);
    product_name_text->move(20, 12);

    product_type_text = new QLabel("商品类型: " + product_list[now_index]->getType(), this);
    product_type_text->setWordWrap(true);
    product_type_text->setFixedSize(180, 25);
    product_type_text->move(20, 42);

    product_describe_text = new QLabel("商品描述: " + product_list[now_index]->getDescribe(), this);
    product_describe_text->setWordWrap(true);
    product_describe_text->setFixedSize(180, 50);
    product_describe_text->move(20, 62);

    product_price_text = new QLabel("商品价格: " + QString::number(product_list[now_index]->getPrice(), 'f', 2), this);
    product_price_text->setWordWrap(true);
    product_price_text->setFixedSize(180, 25);
    product_price_text->move(20, 132);

    product_amount_text = new QLabel("商品数量: " + QString::number(product_list[now_index]->getAmount()), this);
    product_amount_text->setWordWrap(true);
    product_amount_text->setFixedSize(180, 25);
    product_amount_text->move(20, 162);

    product_seller_text = new QLabel("商家: " + product_list[now_index]->getSellerName(), this);
    product_seller_text->setWordWrap(true);
    product_seller_text->setFixedSize(180, 25);
    product_seller_text->move(20, 192);

    if (now_user != nullptr && now_user->getUserType() == "Buyer")
    {
        quantity_widget = new QuantityWidget(0, product_list[now_index]->getAmount(), this);
        quantity_widget->move(72, 215);
        add_cart_btn = new QPushButton("添加到购物车", this);
        add_cart_btn->move(70, 250);
        connect(add_cart_btn, &QPushButton::clicked, this, &ProductDisplayer::sendAddToCartSignal);
    }
    show();
}

void ProductDisplayer::sendAddToCartSignal()
{
    Product *product = product_list[now_index];
    int amount = quantity_widget->value();
    if (amount > 0)
        emit addToCart(product, amount);
    close();
}

ProductDiscounter::ProductDiscounter(User *user, QVector<Product *> &list, QWidget *parent) : QWidget(parent), product_list(list)
{
    now_user = user;

    setAttribute(Qt::WA_QuitOnClose,false);
    setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(230, 120);
    setWindowTitle("商品打折");

    product_type_text = new QLabel("请选择打折品类:", this);
    product_type_text->move(20, 12);
    product_type_box = new QComboBox(this);
    product_type_box->addItem("All");
    product_type_box->addItem("Book");
    product_type_box->addItem("Electronic");
    product_type_box->addItem("Clothing");
    product_type_box->setEditable(false);
    product_type_box->move(115, 10);

    product_discount_text = new QLabel("请输入打折幅度:", this);
    product_discount_text->move(20, 42);
    product_discount_box = new QLineEdit(this);
    QRegularExpression rx("^(100|[0-9]?\\d)$");
    product_discount_box->setValidator(new QRegularExpressionValidator(rx, this));
    product_discount_box->setMaxLength(3);
    product_discount_box->setFixedSize(50, 20);
    product_discount_box->move(115, 40);

    submit_btn = new QPushButton("提交", this);
    submit_btn->move(80, 80);
    connect(submit_btn, &QPushButton::clicked, this, &ProductDiscounter::discountProduct);

    show();
}

void ProductDiscounter::discountProduct()
{
    for (auto i = product_list.begin(); i < product_list.end(); i++)
    {
        if ((*i)->getSellerName() == now_user->getUserName() && ((*i)->getType() == product_type_box->currentText() || product_type_box->currentText() == "All"))
            (*i)->setDiscount(product_discount_box->text().toInt());
    }
    close();
}

ProductManager::ProductManager(User *user, QVector<Product *> &list, QWidget *parent) : QWidget(parent), product_list(list)
{
    now_user = user;

    setAttribute(Qt::WA_QuitOnClose,false);
    setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(250, 280);

    product_name_text = new QLabel("商品名称:", this);
    product_name_text->move(20, 12);
    product_name_box = new QLineEdit(this);
    product_name_box->move(80, 10);

    product_type_text = new QLabel("商品类型:", this);
    product_type_text->move(20, 42);
    product_type_box = new QComboBox(this);
    product_type_box->addItem("Book");
    product_type_box->addItem("Electronic");
    product_type_box->addItem("Clothing");
    product_type_box->addItem("None");
    product_type_box->setEditable(false);
    product_type_box->move(80, 42);

    product_describe_text = new QLabel("商品描述:", this);
    product_describe_text->move(20, 72);
    product_describe_box = new QTextEdit(this);
    product_describe_box->setFixedSize(150, 50);
    product_describe_box->move(80, 72);

    product_price_text = new QLabel("商品价格:", this);
    product_price_text->move(20, 132);
    product_price_box = new QLineEdit(this);
    product_price_box->setValidator(new QDoubleValidator(0, 20000, 2, this));
    product_price_box->setMaxLength(6);
    product_price_box->setFixedSize(80, 20);
    product_price_box->move(80, 132);

    product_discount_text = new QLabel("商品折扣:", this);
    product_discount_text->move(20, 162);
    product_discount_box = new QLineEdit(this);
    QRegularExpression rx("^(100|[0-9]?\\d)$");
    product_discount_box->setValidator(new QRegularExpressionValidator(rx, this));
    product_discount_box->setMaxLength(3);
    product_discount_box->setFixedSize(50, 20);
    product_discount_box->move(80, 162);

    product_amount_text = new QLabel("商品数量:", this);
    product_amount_text->move(20, 192);
    product_amount_box = new QLineEdit(this);
    product_amount_box->setValidator(new QIntValidator(0, 10000, this));
    product_amount_box->setMaxLength(10);
    product_amount_box->setFixedSize(80, 20);
    product_amount_box->move(80, 192);

    submit_btn = new QPushButton("提交", this);
    submit_btn->move(90, 230);
}

void ProductManager::keyPressEvent(QKeyEvent *ev)
{
    if(ev->key() == Qt::Key_Return || ev->key() == Qt::Key_Enter)
        submit_btn->click();
    QWidget::keyPressEvent(ev);
}

ProductAdder::ProductAdder(User *user, QVector<Product *> &list, QWidget *parent) : ProductManager(user, list , parent)
{
    setWindowTitle("添加商品");
    connect(submit_btn, &QPushButton::clicked, this, &ProductAdder::addProduct);
    show();
}

void ProductAdder::addProduct()
{
    QString name = product_name_box->text();
    QString type = product_type_box->currentText();
    QString describe = product_describe_box->toPlainText();
    double price = product_price_box->text().toDouble();
    int discount = product_discount_box->text().toInt();
    int amount = product_amount_box->text().toInt();

    Product *new_product;
    if (type == "Book")
        new_product = new BookProduct(name, describe, price, discount, amount, now_user->getUserName());
    else if (type == "Electronic")
        new_product = new ElectronicProduct(name, describe, price, discount, amount, now_user->getUserName());
    else if (type == "Clothing")
        new_product = new ClothingProduct(name, describe, price, discount, amount, now_user->getUserName());
    else
        new_product = new Product(name, describe, price, discount, amount, now_user->getUserName());
    product_list.push_back(new_product);
    close();
}

ProductModifier::ProductModifier(User *user, QVector<Product *> &list, int index, QWidget *parent) : ProductManager(user, list, parent), now_index(index)
{
    setWindowTitle("修改商品");
    product_name_box->setText(product_list[now_index]->getName());
    product_type_box->setCurrentText(product_list[now_index]->getType());
    product_type_box->setDisabled(true);
    product_describe_box->setText(product_list[now_index]->getDescribe());
    product_price_box->setText(QString::number(product_list[now_index]->getOriginalPrice()));
    product_discount_box->setText(QString::number(product_list[now_index]->getDiscount()));
    product_amount_box->setText(QString::number(product_list[now_index]->getAmount()));
    submit_btn->move(45, 230);
    connect(submit_btn, &QPushButton::clicked, this, &ProductModifier::modifyProduct);
    stop_sale_btn = new QPushButton("下架", this);
    stop_sale_btn->move(135, 230);
    connect(stop_sale_btn, &QPushButton::clicked, this, &ProductModifier::deleteProduct);
    show();
}

void ProductModifier::modifyProduct()
{
    QString name = product_name_box->text();
    QString describe = product_describe_box->toPlainText();
    double price = product_price_box->text().toDouble();
    int discount = product_discount_box->text().toInt();
    int amount = product_amount_box->text().toInt();
    product_list[now_index]->setName(name);
    product_list[now_index]->setDescribe(describe);
    product_list[now_index]->setOriginalPrice(price);
    product_list[now_index]->setDiscount(discount);
    product_list[now_index]->setAmount(amount);
    close();
}

void ProductModifier::deleteProduct()
{
    product_list.erase(product_list.begin() + now_index);
    close();
}

ProductScreenerWidget::ProductScreenerWidget(QTableWidget *table, QWidget *parent) : QWidget(parent)
{
    setFixedSize(180, 180);

    table_widget = table;

    title_text = new QLabel("———商品筛选———", this);
    title_text->move(32, 10);

    name_text = new QLabel("名称:", this);
    name_text->move(10, 40);
    name_box = new QLineEdit(this);
    name_box->setFixedSize(120, 20);
    name_box->move(40, 38);
    connect(name_box, &QLineEdit::textChanged, this, &ProductScreenerWidget::changeScreen);

    type_text = new QLabel("类型:", this);
    type_text->move(10, 70);
    type_box = new QComboBox(this);
    type_box->addItem("All");
    type_box->addItem("Book");
    type_box->addItem("Electronic");
    type_box->addItem("Clothing");
    type_box->setEditable(false);
    type_box->move(40, 68);
    connect(type_box, &QComboBox::currentTextChanged, this, &ProductScreenerWidget::changeScreen);

    price_text = new QLabel("价格:", this);
    price_text->move(10, 100);

    lower_price = new QLineEdit(this);
    lower_price->setValidator(new QDoubleValidator(0, 10000, 2, this));
    lower_price->setMaxLength(10);
    lower_price->setFixedSize(50, 20);
    lower_price->move(40, 100);
    connect(lower_price, &QLineEdit::textChanged, this, &ProductScreenerWidget::changeScreen);

    mid_text = new QLabel("~", this);
    mid_text->move(95, 100);

    upper_price = new QLineEdit(this);
    upper_price->setValidator(new QDoubleValidator(0, 10000, 2, this));
    upper_price->setMaxLength(10);
    upper_price->setFixedSize(50, 20);
    upper_price->move(110, 100);
    connect(upper_price, &QLineEdit::textChanged, this, &ProductScreenerWidget::changeScreen);

    seller_text = new QLabel("商家:", this);
    seller_text->move(10, 130);
    seller_box = new QLineEdit(this);
    seller_box->setFixedSize(120, 20);
    seller_box->move(40, 130);
    connect(seller_box, &QLineEdit::textChanged, this, &ProductScreenerWidget::changeScreen);

    on_stock = new QCheckBox("只看有货", this);
    on_stock->move(40, 160);
    connect(on_stock, &QCheckBox::stateChanged, this, &ProductScreenerWidget::changeScreen);
}

void ProductScreenerWidget::changeScreen()
{
    for (int i = 0; i < table_widget->rowCount(); i++)
    {
        table_widget->showRow(i);
        if (table_widget->item(i, 0)->text().contains(name_box->text(), Qt::CaseInsensitive))
        {
            if (type_box->currentText() == "All"  || type_box->currentText() ==table_widget->item(i, 1)->text())
            {
                if (table_widget->item(i, 2)->data(Qt::DisplayRole).toDouble() >= lower_price->text().toDouble())
                {
                    if (upper_price->text() == "" || table_widget->item(i, 2)->data(Qt::DisplayRole).toDouble() <= upper_price->text().toDouble())
                    {
                        if (!on_stock->isChecked() || table_widget->item(i, 3)->data(Qt::DisplayRole).toInt() > 0)
                        {
                            if (table_widget->item(i, 4)->text().contains(seller_box->text(), Qt::CaseInsensitive))
                                continue;
                        }
                    }
                }
            }
        }
        table_widget->hideRow(i);
    }
}

ProductManagerWidget::ProductManagerWidget(User *user, QVector <Product *> &list, QWidget *parent) : QWidget(parent), product_list(list)
{
    setFixedSize(700, 575);

    now_user = user;
    product_adder = nullptr;
    product_modifier = nullptr;
    product_displayer = nullptr;
    product_discounter = nullptr;

    //界面
    table_widget = new QTableWidget(product_list.size(), 5, this);
    table_widget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_widget->setSelectionMode(QAbstractItemView::SingleSelection);
    table_widget->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_widget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    table_widget->horizontalHeader()->setSortIndicatorShown(true);
    table_widget->verticalHeader()->setHidden(true);
    table_widget->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
    connect(table_widget->horizontalHeader(), &QHeaderView::sortIndicatorChanged, table_widget, &QTableWidget::sortItems);
    table_widget->setColumnWidth(0, 190);
    table_widget->setColumnWidth(1, 100);
    table_widget->setColumnWidth(2, 90);
    table_widget->setColumnWidth(3, 60);
    table_widget->setColumnWidth(4, 60);
    table_widget->setFixedSize(519, 476);
    QStringList header;
    header << "商品名" << "类别" << "价格" << "数量" << "商家";
    table_widget->setHorizontalHeaderLabels(header);
    table_widget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:#dddddd;}");
    refresh();

    if (now_user != nullptr && now_user->getUserType() == "Seller")
    {
        add_product_btn = new QPushButton("添加商品", this);
        add_product_btn->move(530, 200);
        connect(add_product_btn, &QPushButton::clicked, this, &ProductManagerWidget::addProduct);

        discount_product_btn = new QPushButton("商品打折", this);
        discount_product_btn->move(615, 200);
        connect(discount_product_btn, &QPushButton::clicked, this, &ProductManagerWidget::discountProduct);
    }
    connect(table_widget, &QTableWidget::itemDoubleClicked, this, &ProductManagerWidget::checkPermission);

    screener = new ProductScreenerWidget(table_widget, this);
    screener->move(520, 10);
}

void ProductManagerWidget::refresh()
{
    table_widget->setRowCount(product_list.size());
    table_widget->clearContents();
    for (int i = 0; i < product_list.size(); i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem(product_list[i]->getName());
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        table_widget->setItem(i, 0, item);
        item = new QTableWidgetItem(product_list[i]->getType());
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        table_widget->setItem(i, 1, item);
        item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, product_list[i]->getPrice());
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        table_widget->setItem(i, 2, item);
        item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, product_list[i]->getAmount());
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        table_widget->setItem(i, 3, item);
        item = new QTableWidgetItem(product_list[i]->getSellerName());
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        table_widget->setItem(i, 4, item);
    }
    table_widget->horizontalHeader()->setSortIndicator(0,Qt::SortOrder::AscendingOrder);
}

void ProductManagerWidget::addProduct()
{
    if (product_adder == nullptr)
    {
        product_adder = new ProductAdder(now_user, product_list);
        connect(product_adder, &ProductAdder::destroyed, this, &ProductManagerWidget::addProductDone);
    }
}

void ProductManagerWidget::addProductDone()
{
    product_adder = nullptr;
    refresh();
}

void ProductManagerWidget::checkPermission(QTableWidgetItem *item)
{
    if (now_user != nullptr && now_user->getUserType() == "Seller" && now_user->getUserName() == table_widget->item(item->row(), 4)->text())
        modifyProduct(item);
    else
        displayProduct(item);
}

void ProductManagerWidget::modifyProduct(QTableWidgetItem *item)
{
    if (product_modifier == nullptr)
    {
        for (int i = 0; i < product_list.size(); i++)
        {
            if (product_list[i]->getName() == table_widget->item(item->row(), 0)->text() && product_list[i]->getSellerName() == table_widget->item(item->row(), 4)->text())
            {
                product_modifier = new ProductModifier(now_user, product_list, i);
                connect(product_modifier, &ProductModifier::destroyed, this, &ProductManagerWidget::modifyProductDone);
                break;
            }
        }
    }
}

void ProductManagerWidget::modifyProductDone()
{
    product_modifier = nullptr;
    refresh();
}

void ProductManagerWidget::displayProduct(QTableWidgetItem *item)
{
    if (product_displayer == nullptr)
    {
        for (int i = 0; i < product_list.size(); i++)
        {
            if (product_list[i]->getName() == table_widget->item(item->row(), 0)->text() && product_list[i]->getSellerName() == table_widget->item(item->row(), 4)->text())
            {
                product_displayer = new ProductDisplayer(now_user, product_list, i);
                connect(product_displayer, &ProductDisplayer::addToCart, this, &ProductManagerWidget::recvAddToCart);
                connect(product_displayer, &ProductDisplayer::destroyed, this, &ProductManagerWidget::displayProductDone);
                break;
            }
        }
    }
}

void ProductManagerWidget::displayProductDone()
{
    product_displayer = nullptr;
}

void ProductManagerWidget::discountProduct()
{
    if (product_discounter == nullptr)
    {
        product_discounter = new ProductDiscounter(now_user, product_list);
        connect(product_discounter, &ProductDiscounter::destroyed, this, &ProductManagerWidget::discountProductDone);
    }
}

void ProductManagerWidget::discountProductDone()
{
    product_discounter = nullptr;
    refresh();
}

void ProductManagerWidget::recvAddToCart(Product * product, int amount)
{
    emit addToCart(product, amount);
}

ProductManagerWidget::~ProductManagerWidget()
{

}
