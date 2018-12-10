#include "crs/matrix.h"

int main(int , char **)
{
    CRS<int, 0> c1;
    c1.add(1, 7, 2);
    c1.print();
    c1.add(1, 2, 0);
    c1.print();
    c1.add(3, 3, 0);
    c1.print();
    c1.add(7, 5, 2);
    c1.print();
    c1.add(5, 7, 0);
    c1.print();
    c1.add(6, 7, 10);
    c1.print();
    c1.add(0, 7, 10);
    c1.print();
    c1.add(0, 6, 10);
    c1.print();
    c1.add(2, 6, 10);
    c1.print();
    c1.add(0, 6, 10);
    c1.print();
    std::cout << c1[0][0] << std::endl;
    std::cout << c1[0][2] << std::endl;
    ((c1[0][2] = 12) = 21) = 121;
    std::cout << c1[0][2] << std::endl;
    std::cout << c1[0][3] << std::endl;
    std::cout << c1[0][7] << std::endl;
    std::cout << c1[2][5] << std::endl;
    std::cout << c1[2][7] << std::endl;
    c1.print();
    return 0;
}
