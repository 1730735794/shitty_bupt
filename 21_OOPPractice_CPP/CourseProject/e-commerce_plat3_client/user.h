#ifndef USER_H
#define USER_H
#include <QString>

class User      //用户基类
{
private:
    QString m_username;     //用户名
    QString m_password;     //密码
    double m_balance;       //余额

public:
    User(QString username, QString password, double balance);
    QString getUserName() const;    //返回用户名
    QString getPassword() const;    //返回密码
    double getBalance() const;      //返回余额
    void setPassword(const QString new_password);   //设置密码
    void setBalance(const double new_balance);      //设置余额
    bool isPasswordCorrect(const QString password) const;   //判断密码是否正确
    virtual QString getUserType() const = 0;    //返回用户类型
    virtual ~User(){};
};

class Seller : public User  //卖家
{
private:

public:
    Seller(QString username, QString password, double balance);
    virtual QString getUserType() const override;   //返回用户类型
    virtual ~Seller(){};
};

class Buyer : public User   //买家
{
private:

public:
    Buyer(QString username, QString password, double balance);
    virtual QString getUserType() const override;   //返回用户类型
    virtual ~Buyer(){};
};

#endif // USER_H
