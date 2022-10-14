#include <iostream>
#include <iomanip>

const int ROW_NR = 4, COL_NR = 5;

void initMatrix(int **const mat);
void outputMatrix(int **const mat);
void addMatrix(int **const mat1, int **const mat2, int **const result);
void subtractMatrix(int **const mat1, int **const mat2, int **const result);

int main()
{
    int **A1 = new int *[ROW_NR];
    int **A2 = new int *[ROW_NR];
    int **A3 = new int *[ROW_NR];
    for (int i = 0; i < ROW_NR; i++)
    {
        A1[i] = new int[COL_NR];
        A2[i] = new int[COL_NR];
        A3[i] = new int[COL_NR];
    }
    initMatrix(A1);
    initMatrix(A2);
    addMatrix(A1, A2, A3);
    outputMatrix(A3);
    subtractMatrix(A1, A2, A3);
    outputMatrix(A3);
    for (int i = 0; i < ROW_NR; i++)
    {
        delete[] A1[i];
        delete[] A2[i];
        delete[] A3[i];
    }
    delete[] A1;
    delete[] A2;
    delete[] A3;
}

void initMatrix(int **const mat)
{
    std::cout << "Input a matrix:" << std::endl;
    for (int i = 0; i < ROW_NR; i++)
        for (int j = 0; j < COL_NR; j++)
            std::cin >> mat[i][j];
}

void outputMatrix(int **const mat)
{
    std::cout << "Output a matrix:" << std::endl;
    std::cout.setf(std::ios::left);
    for (int i = 0; i < ROW_NR; i++)
    {
        for (int j = 0; j < COL_NR; j++)
            std::cout << std::setw(5) << mat[i][j];
        std::cout << std::endl;
    }
}

void addMatrix(int **const mat1, int **const mat2, int **const result)
{
    for (int i = 0; i < ROW_NR; i++)
        for (int j = 0; j < COL_NR; j++)
            result[i][j] = mat1[i][j] + mat2[i][j];
}

void subtractMatrix(int **const mat1, int **const mat2, int **const result)
{
    for (int i = 0; i < ROW_NR; i++)
        for (int j = 0; j < COL_NR; j++)
            result[i][j] = mat1[i][j] - mat2[i][j];
}