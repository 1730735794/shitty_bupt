#include "product.h"

Product::Product(QString name, QString describe, double price, int discount, int amount, QString sellername, QString type)
{
    m_name = name;
    m_type = type;
    m_describe= describe;
    m_price = price;
    m_discount = discount;
    m_amount = amount;
    m_sellername = sellername;
}

void Product::setName(const QString new_name)
{
    m_name = new_name;
}

void Product::setDescribe(const QString new_describe)
{
    m_describe = new_describe;
}

void Product::setOriginalPrice(const double new_price)
{
    m_price = new_price;
}

void Product::setAmount(const int new_amount)
{
    m_amount = new_amount;
}

QString Product::getName() const
{
    return m_name;
}

QString Product::getDescribe() const
{
    return m_describe;
}

QString Product::getType() const
{
    return m_type;
}

double Product::getOriginalPrice() const
{
    return m_price;
}

double Product::getPrice() const
{
    return m_price * getDiscount() / 100;
}

int Product::getAmount() const
{
    return m_amount;
}

int Product::getDiscount() const
{
    return m_discount;
}

void Product::setDiscount(const int new_discount)
{
    m_discount = new_discount;
}

QString Product::getSellerName() const
{
    return m_sellername;
}

void Product::setSellerName(const QString seller_name)
{
    m_sellername = seller_name;
}

BookProduct::BookProduct(QString name, QString describe, double price, int discount, int amount, QString sellername)
    : Product(name, describe, price, discount, amount, sellername, "Book")
{

}

ElectronicProduct::ElectronicProduct(QString name, QString describe, double price, int discount, int amount, QString sellername)
    : Product(name, describe, price, discount, amount, sellername, "Electronic")
{

}

ClothingProduct::ClothingProduct(QString name, QString describe, double price, int discount, int amount, QString sellername)
    : Product(name, describe, price, discount, amount, sellername, "Clothing")
{

}
