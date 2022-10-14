#ifndef PRODUCTMANAGER_H
#define PRODUCTMANAGER_H

#include <QWidget>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTableWidget>
#include <QPushButton>
#include <QHeaderView>
#include <QErrorMessage>
#include <QKeyEvent>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QLabel>
#include <QCheckBox>
#include <QRegularExpression>
#include "product.h"
#include "user.h"

class ProductDisplayer : public QWidget //商品展示窗口
{
    Q_OBJECT
private:
    QVector<Product *> &product_list;   //商品列表
    const int now_index;                //当前展示的商品序号
    QLabel *product_name_text;          //商品名文本
    QLabel *product_type_text;          //商品类型文本
    QLabel *product_describe_text;      //商品描述文本
    QLabel *product_price_text;         //商品价格文本
    QLabel *product_amount_text;        //商品数量文本
    QLabel *product_seller_text;        //商家文本
public:
    explicit ProductDisplayer(QVector<Product *> &list, int index, QWidget *parent = nullptr);
    virtual ~ProductDisplayer(){};
signals:

};

class ProductDiscounter : public QWidget    //商品打折窗口
{
    Q_OBJECT
private:
    User *now_user;                     //当前用户指针
    QVector<Product *> &product_list;   //商品列表
    QLabel *product_type_text;          //商品类型提示文本
    QComboBox *product_type_box;        //商品类型选择框
    QLabel *product_discount_text;      //商品打折提示文本
    QLineEdit *product_discount_box;    //商品打折输入框
    QPushButton *submit_btn;            //提交按钮
    void displaySliderValue(int value); //根据滑动条内容显示数值
    void discountProduct();             //商品打折，由滑动条的改变触发
public:
    explicit ProductDiscounter(User *user, QVector<Product *> &list, QWidget *parent = nullptr);
    virtual ~ProductDiscounter(){};
signals:

};

class ProductManager : public QWidget   //商品管理窗口基类
{
    Q_OBJECT
protected:
    User *now_user;                     //当前用户指针
    QVector<Product *> &product_list;   //商品列表
    QLabel *product_name_text;          //商品名称提示文本
    QLineEdit *product_name_box;        //商品名称输入框
    QLabel *product_type_text;          //商品类型提示文本
    QComboBox *product_type_box;        //商品类型选择框
    QLabel *product_describe_text;      //商品描述提示文本
    QTextEdit *product_describe_box;    //商品描述输入框
    QLabel *product_price_text;         //商品价格提示文本
    QLineEdit *product_price_box;       //商品价格输入框
    QLabel *product_discount_text;      //商品折扣提示文本
    QLineEdit *product_discount_box;    //商品折扣输入框
    QLabel *product_amount_text;        //商品数量提示文本
    QLineEdit *product_amount_box;      //商品数量输入框
    QLabel *product_seller_text;        //商家提示文本
    QPushButton *submit_btn;            //提交按钮
    virtual void keyPressEvent(QKeyEvent *ev);  //重载键盘事件
public:
    explicit ProductManager(User *user, QVector<Product *> &list, QWidget *parent = nullptr);
    virtual ~ProductManager(){};
signals:

};

class ProductAdder : public ProductManager  //商品添加窗口
{
    Q_OBJECT
private:
    void addProduct();  //添加商品，由提交按钮触发
public:
    explicit ProductAdder(User *user, QVector<Product *> &list, QWidget *parent = nullptr);
    virtual ~ProductAdder(){};
signals:

};

class ProductModifier : public ProductManager   //商品修改窗口
{
    Q_OBJECT
private:
    const int now_index;        //当前修改的商品序号
    QPushButton *stop_sale_btn; //下架按钮
    void modifyProduct();       //修改商品信息，由提交按钮触发
    void deleteProduct();       //下架商品，由下架按钮触发
public:
    explicit ProductModifier(User *user, QVector<Product *> &list, int index, QWidget *parent = nullptr);
    virtual ~ProductModifier(){};
signals:

};

class ProductScreenerWidget : public QWidget    //商品筛选部件
{
    Q_OBJECT
private:
    QTableWidget *table_widget;     //商品列表指针
    QLabel *title_text;             //部件名称提示文本
    QLabel *name_text;              //商品名称提示文本
    QLineEdit *name_box;            //商品名称筛选输入框
    QLabel *type_text;              //商品类型提示文本
    QComboBox *type_box;            //商品类型选择框
    QLabel *price_text;             //商品价格提示文本
    QLineEdit *lower_price;         //商品价格下限输入框
    QLabel *mid_text;               //商品价格上下限之间提示文本
    QLineEdit *upper_price;         //商品价格上线输入框
    QLabel *seller_text;            //商家提示文本
    QLineEdit *seller_box;          //商家筛选输入框
    QCheckBox *on_stock;            //商品是否有货选择框
    void changeScreen();            //改变筛选条件，由此部件中筛选条件的改变触发
public:
    explicit ProductScreenerWidget(QTableWidget *table, QWidget *parent = nullptr);
    virtual ~ProductScreenerWidget(){};
signals:

};

class ProductManagerWidget : public QWidget //商品管理部件
{
    Q_OBJECT
private:
    User *now_user;                         //当前用户指针
    QVector <Product *> product_list;       //商品列表
    QTableWidget *table_widget;             //商品展示列表
    QPushButton *add_product_btn;           //添加商品按钮
    QPushButton *discount_product_btn;      //商品打折按钮
    ProductAdder *product_adder;            //添加商品窗口
    ProductModifier *product_modifier;      //修改商品窗口
    ProductDisplayer *product_displayer;    //展示商品窗口
    ProductDiscounter *product_discounter;  //商品打折窗口
    ProductScreenerWidget *screener;        //商品筛选部件
    void refreshTable();                            //刷新商品展示列表
    void addProduct();                              //创建添加商品窗口，由添加商品按钮触发
    void addProductDone();                          //删除添加商品窗口
    void checkPermission(QTableWidgetItem *item);                         //权限检查，由双击展示列表中的商品触发
    void modifyProduct(QTableWidgetItem *item);     //创建修改商品窗口，由双击展示列表中的商品触发
    void modifyProductDone();                       //删除修改商品窗口
    void displayProduct(QTableWidgetItem *item);    //创建商品展示窗口，由双击展示列表中的商品触发
    void displayProductDone();                      //删除商品展示窗口
    void discountProduct();                         //创建商品打折窗口，由商品打折按钮触发
    void discountProductDone();                     //删除商品打折窗口
public:
    explicit ProductManagerWidget(User *user, QWidget *parent = nullptr);
    ~ProductManagerWidget();
signals:

};

#endif // PRODUCTMANAGER_H
