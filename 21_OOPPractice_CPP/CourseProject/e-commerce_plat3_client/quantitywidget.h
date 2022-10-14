#ifndef QUANTITYWIDGET_H
#define QUANTITYWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QRegularExpression>
#include <QValidator>

class QuantityWidget : public QWidget
{
    Q_OBJECT
private:
    int upper_bound;
    int lower_bound;
    QPushButton *sub_btn;
    QPushButton *add_btn;
    QLineEdit *quantity_box;
    void subQuantity();
    void addQuantity();
    void sendSignal() const;
public:
    int value() const;
    void setValue(const int value);
    explicit QuantityWidget(int lower, int upper, QWidget *parent = nullptr);

signals:
    void valueChanged(int value) const;
};

#endif // QUANTITYWIDGET_H
