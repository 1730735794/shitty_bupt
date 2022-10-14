#include <iostream>
#include <cmath>

class Shape
{
public:
    Shape();
    ~Shape();
    const double getArea() const;
};

class Rectangle : public Shape
{
private:
    double length, height;

public:
    Rectangle(double l, double h);
    ~Rectangle();
    const double getArea() const;
};

class Round : public Shape
{
private:
    double radius;

public:
    Round(double r);
    ~Round();
    const double getArea() const;
};

class Square : public Rectangle
{
public:
    Square(double a);
    ~Square();
};

Shape::Shape()
{
    std::cout << "Constructing a Shape." << std::endl;
}

Shape::~Shape()
{
    std::cout << "Deconstructing a Shape." << std::endl;
}

const double Shape::getArea() const
{
    return 0;
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
    std::cout << "This is a " << typeid(*shapes[0]).name() + 1 << ", its area is " << static_cast<Rectangle *>(shapes[0])->getArea() << std::endl;
    std::cout << "This is a " << typeid(*shapes[1]).name() + 1 << ", its area is " << static_cast<Round *>(shapes[1])->getArea() << std::endl;
    std::cout << "This is a " << typeid(*shapes[2]).name() + 1 << ", its area is " << static_cast<Square *>(shapes[2])->getArea() << std::endl;
    delete static_cast<Square *>(shapes[2]);
}