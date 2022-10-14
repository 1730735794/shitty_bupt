#include <iostream>
#include <cmath>

class Shape
{
public:
    Shape();
    virtual ~Shape();
    virtual const double getArea() const = 0;
};

class Rectangle : public Shape
{
private:
    double length, height;

public:
    Rectangle(double l, double h);
    virtual ~Rectangle();
    virtual const double getArea() const;
};

class Round : public Shape
{
private:
    double radius;

public:
    Round(double r);
    virtual ~Round();
    virtual const double getArea() const;
};

class Square : public Rectangle
{
public:
    Square(double a);
    virtual ~Square();
};

Shape::Shape()
{
    std::cout << "Constructing a Shape." << std::endl;
}

Shape::~Shape()
{
    std::cout << "Deconstructing a Shape." << std::endl;
}

Rectangle::Rectangle(double l, double h)
{
    std::cout << "Constructing a Rectangle." << std::endl;
    length = l;
    height = h;
}

Rectangle::~Rectangle()
{
    std::cout << "Deconstructing a Rectangle." << std::endl;
}

const double Rectangle::getArea() const
{
    return length * height;
}

Round::Round(double r)
{
    std::cout << "Constructing a Round." << std::endl;
    radius = r;
}

Round::~Round()
{
    std::cout << "Deconstructing a Round." << std::endl;
}

const double Round::getArea() const
{
    return M_PI * radius * radius;
}

Square::Square(double a) : Rectangle(a, a)
{
    std::cout << "Constructing a Square." << std::endl;
}

Square::~Square()
{
    std::cout << "Deconstructing a Square." << std::endl;
}

int main()
{
    Shape *shapes[3];
    shapes[0] = new Rectangle(10, 5);
    shapes[1] = new Round(2);
    shapes[2] = new Square(4);
    for (int i = 0; i < 3; i++)
        std::cout << "This is a " << typeid(*shapes[i]).name() + 1 << ", its area is " << shapes[i]->getArea() << std::endl;
    for (int i = 0; i < 3; i++)
        delete shapes[i];
}