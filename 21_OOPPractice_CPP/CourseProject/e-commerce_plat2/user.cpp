#include "user.h"

User::User(QString username, QString password, double balance)
{
    m_username = username;
    m_password = password;
    m_balance = balance;
}

QString User::getUserName() const
{
    return m_username;
}

bool User::isPasswordCorrect(const QString password) const
{
    if (password == getPassword())
        return true;
    return false;
}

QString User::getPassword() const
{
    return m_password;
}

void User::setPassword(const QString new_password)
{
    m_password = new_password;
}

double User::getBalance() const
{
    return m_balance;
}

void User::setBalance(const double new_balance)
{
    m_balance = new_balance;
}

Seller::Seller(QString username, QString password, double balance) : User(username, password, balance)
{

}

QString Seller::getUserType() const
{
    return "Seller";
}

Buyer::Buyer(QString username, QString password, double balance) : User(username, password, balance)
{

}

QString Buyer::getUserType() const
{
    return "Buyer";
}
