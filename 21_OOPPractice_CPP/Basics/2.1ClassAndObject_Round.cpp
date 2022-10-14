#include <iostream>
#include <cmath>
#include <algorithm>
class Point
{
private:
    double m_pos_x, m_pos_y;

public:
    Point(double x = 0, double y = 0);
    const double pos_x() const;
    const double pos_y() const;
};

class Round
{
private:
    Point m_center;
    double m_radius;

public:
    Round(double x, double y, double r);
    bool isIntersectWith(const Round &rx) const;
};

Point::Point(double x, double y)
{
    std::cout << "Constructing a Point." << std::endl;
    m_pos_x = x;
    m_pos_y = y;
}

const double Point::pos_x() const
{
    return m_pos_x;
}

const double Point::pos_y() const
{
    return m_pos_y;
}

Round::Round(double pos_x, double pos_y, double radius) : m_center(pos_x, pos_y)
{
    std::cout << "Constructing a Round." << std::endl;
    m_radius = radius;
}

bool Round::isIntersectWith(const Round &rx) const
{
    double length = sqrt((m_center.pos_x() - rx.m_center.pos_x()) * (m_center.pos_x() - rx.m_center.pos_x()) + (m_center.pos_y() - rx.m_center.pos_y()) * (m_center.pos_y() - rx.m_center.pos_y()));
    return m_radius + rx.m_radius >= length && std::min(m_radius, rx.m_radius) + length >= std::max(m_radius, rx.m_radius);
}

int main()
{
    double x, y, r;
    std::cout << "Input center position of the first round." << std::endl;
    std::cin >> x >> y;
    std::cout << "Input radius of the first round." << std::endl;
    std::cin >> r;
    Round round1(x, y, r);
    std::cout << "Input center position of the second round." << std::endl;
    std::cin >> x >> y;
    std::cout << "Input radius of the second round." << std::endl;
    std::cin >> r;
    Round round2(x, y, r);
    if (round1.isIntersectWith(round2))
        std::cout << "Round1 is intersect with round2." << std::endl;
    else
        std::cout << "Round1 is not intersect with round2." << std::endl;
}