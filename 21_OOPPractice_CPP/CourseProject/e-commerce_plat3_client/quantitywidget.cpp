#include "quantitywidget.h"

QuantityWidget::QuantityWidget(int lower, int upper, QWidget *parent) : QWidget(parent)
{
    setFixedSize(74, 25);
    upper_bound = upper;
    lower_bound = lower;
    sub_btn = new QPushButton("-", this);
    sub_btn->setFixedSize(20, 20);
    sub_btn->move(0, 5);

    quantity_box = new QLineEdit(this);
    QRegularExpression rx("^(100|[0-9]?\\d)$");
    quantity_box->setValidator(new QRegularExpressionValidator(rx, this));
    quantity_box->setAlignment(Qt::AlignHCenter);
    quantity_box->setText("1");
    if (upper_bound == 0)
        quantity_box->setText("0");
    quantity_box->setFixedSize(30, 20);
    quantity_box->move(22, 5);

    add_btn = new QPushButton("+", this);
    add_btn->setFixedSize(20, 20);
    add_btn->move(54, 5);

    connect(sub_btn, &QPushButton::clicked, this, &QuantityWidget::subQuantity);
    connect(add_btn, &QPushButton::clicked, this, &QuantityWidget::addQuantity);
    connect(quantity_box, &QLineEdit::textChanged, this, &QuantityWidget::sendSignal);
}

void QuantityWidget::subQuantity()
{
    if (quantity_box->text().toInt() <= 0 || quantity_box->text().toInt() <= lower_bound)
        return;
    quantity_box->setText(QString::number(quantity_box->text().toInt() - 1));
    sendSignal();
}

void QuantityWidget::addQuantity()
{
    if (quantity_box->text().toInt() >= 100 || quantity_box->text().toInt() >= upper_bound)
        return;
    quantity_box->setText(QString::number(quantity_box->text().toInt() + 1));
    sendSignal();
}

void QuantityWidget::sendSignal() const
{
    if (quantity_box->text().toInt() >= upper_bound)
        quantity_box->setText(QString::number(upper_bound));
    emit valueChanged(quantity_box->text().toInt());
}

int QuantityWidget::value() const
{
    return quantity_box->text().toInt();
}

void QuantityWidget::setValue(const int value)
{
    quantity_box->setText(QString::number(value));
}
