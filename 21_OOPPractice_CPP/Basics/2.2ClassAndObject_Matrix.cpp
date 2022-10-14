#include <iostream>
#include <iomanip>

class Matrix
{
    int rows;
    int cols;
    int **mat_ptr;

public:
    Matrix(int x, int y);
    Matrix(const Matrix &mx);
    ~Matrix();
    const Matrix &operator=(const Matrix &mx);
    const Matrix operator+(const Matrix &mx) const;
    const Matrix operator-(const Matrix &mx) const;
    int *const operator[](int index) const;
    friend std::istream &operator>>(std::istream &is, Matrix &mx);
    friend std::ostream &operator<<(std::ostream &os, const Matrix &mx);
};

Matrix::Matrix(int x, int y)
{
    rows = x;
    cols = y;
    mat_ptr = new int *[rows];
    for (int i = 0; i < cols; i++)
        mat_ptr[i] = new int[cols];
}

Matrix::Matrix(const Matrix &mx)
{
    rows = mx.rows;
    cols = mx.cols;
    mat_ptr = new int *[rows];
    for (int i = 0; i < cols; i++)
        mat_ptr[i] = new int[cols];
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            mat_ptr[i][j] = mx[i][j];
}

Matrix::~Matrix()
{
    for (int i = 0; i < rows; i++)
        delete[] mat_ptr[i];
    delete[] mat_ptr;
}

const Matrix &Matrix::operator=(const Matrix &mx)
{
    if (this == &mx)
        return *this;
    for (int i = 0; i < rows; i++)
        delete[] mat_ptr[i];
    delete[] mat_ptr;
    rows = mx.rows;
    cols = mx.cols;
    mat_ptr = new int *[rows];
    for (int i = 0; i < cols; i++)
        mat_ptr[i] = new int[cols];
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            mat_ptr[i][j] = mx[i][j];
    return *this;
}

const Matrix Matrix::operator+(const Matrix &mx) const
{
    if (rows != mx.rows || cols != mx.cols)
    {
        std::cout << "Operation not defined, the result will be the first matrix." << std::endl;
        return *this;
    }
    Matrix tmp(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            tmp[i][j] = mat_ptr[i][j] + mx[i][j];
    return tmp;
}

const Matrix Matrix::operator-(const Matrix &mx) const
{
    if (rows != mx.rows || cols != mx.cols)
    {
        std::cout << "Operation not defined, the result will be the first matrix." << std::endl;
        return *this;
    }
    Matrix tmp(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            tmp[i][j] = mat_ptr[i][j] - mx[i][j];
    return tmp;
}

int *const Matrix::operator[](int index) const
{
    return mat_ptr[index];
}

std::istream &operator>>(std::istream &is, Matrix &mx)
{
    std::cout << "Input a matrix:" << std::endl;
    for (int i = 0; i < mx.rows; i++)
        for (int j = 0; j < mx.cols; j++)
            std::cin >> mx[i][j];
    return is;
}

std::ostream &operator<<(std::ostream &os, const Matrix &mx)
{
    os << "Output a matrix:" << std::endl;
    os.setf(std::ios::left);
    for (int i = 0; i < mx.rows; i++)
    {
        for (int j = 0; j < mx.cols; j++)
            os << std::setw(5) << mx[i][j];
        os << std::endl;
    }
    return os;
}

int main()
{
    int x, y;
    std::cout << "Input the size of the matrix A1" << std::endl;
    std::cin >> x >> y;
    Matrix A1(x, y);
    std::cout << "Input the size of the matrix A2" << std::endl;
    std::cin >> x >> y;
    Matrix A2(x, y);
    std::cin >> A1;
    std::cin >> A2;
    Matrix A3 = A1 + A2;
    std::cout << A3;
    A3 = A1 - A2;
    std::cout << A3;
    std::cout << "Input the size of the matrix pA1" << std::endl;
    std::cin >> x >> y;
    Matrix *pA1 = new Matrix(x, y);
    std::cout << "Input the size of the matrix pA2" << std::endl;
    std::cin >> x >> y;
    Matrix *pA2 = new Matrix(x, y);
    std::cin >> *pA1;
    std::cin >> *pA2;
    Matrix *pA3 = new Matrix(*pA1 + *pA2);
    std::cout << *pA3;
    *pA3 = *pA1 - *pA2;
    std::cout << *pA3;
    delete pA1;
    delete pA2;
    delete pA3;
}