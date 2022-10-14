#ifndef PRODUCT_H
#define PRODUCT_H
#include <QString>
#include <QDateTime>

class Product   //商品基类
{
private:
    QString m_name;     //商品名称
    QString m_type;     //商品类型
    QString m_describe; //商品描述
    double m_price;     //商品价格
    int m_discount;     //商品打折幅度
    int m_amount;       //商品数量
    QString m_sellername;  //商家名称
protected:
public:
    Product(QString name, QString describe, double price, int discount, int amount, QString sellername, QString type = "None");
    void setName(const QString new_name);   //设置商品名称
    void setDescribe(const QString new_describe);   //设置商品描述
    void setOriginalPrice(const double new_price);  //设置商品原价
    void setAmount(const int new_amount);           //设置商品数量
    QString getName() const;                //返回商品名称
    QString getType() const;                //返回商品类型
    QString getDescribe() const;            //返回商品描述
    double getOriginalPrice() const;        //返回商品原价
    virtual double getPrice() const;        //返回商品价格
    int getAmount() const;                  //返回商品数量
    int getDiscount() const;               //返回打折幅度
    void setDiscount(const int new_discount);    //设置打折幅度
    QString getSellerName() const;
    void setSellerName(const QString seller_name);
    virtual ~Product(){};
};

class BookProduct : public Product  //图书商品类
{
private:
public:
    BookProduct(QString name, QString describe, double price, int discount, int amount, QString sellername);
    virtual ~BookProduct(){};
};

class ElectronicProduct : public Product    //电子商品类
{
private:
public:
    ElectronicProduct(QString name, QString describe, double price, int discount, int amount, QString sellername);
    virtual ~ElectronicProduct(){};
};

class ClothingProduct : public Product      //服装商品类
{
private:
public:
    ClothingProduct(QString name, QString describe, double price, int discount, int amount, QString sellername);
    virtual ~ClothingProduct(){};
};

struct Order
{
    QVector<QPair<Product, int> > list;
    QDateTime buying_time;
    bool is_purchased;
    bool is_canceled;
};
#endif // PRODUCT_H
