#define BOOST_TEST_MODULE matrix_test_module
#include "../crs/matrix.h"
#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE(matrix_test_suite)

BOOST_AUTO_TEST_CASE(matrix_test_creation)
{
    Matrix<int, -7> matrix;
    BOOST_CHECK_EQUAL(matrix.size(), 0);
    BOOST_CHECK_EQUAL(matrix[11][12], -7);
    BOOST_CHECK_EQUAL(matrix.size(), 0);
}

BOOST_AUTO_TEST_CASE(matrix_test_count)
{
    Matrix<int, -7> matrix;
    matrix[12][11]=1;
    matrix[10][12]=-7;
    BOOST_CHECK_EQUAL(matrix.size(), 1);
}

BOOST_AUTO_TEST_CASE(matrix_test_cell)
{
    Matrix<int, -7> matrix;
    matrix[12][11]=1;
    matrix[10][12]=-7;
    auto cell = *(matrix.begin());
    BOOST_CHECK_EQUAL(matrix.size(), 1);
    size_t row, col;
    int val;
    std::tie(row, col, val) = cell;
    BOOST_CHECK_EQUAL(matrix.size(), 1);
    BOOST_CHECK_EQUAL(row, 12);
    BOOST_CHECK_EQUAL(col, 11);
    BOOST_CHECK_EQUAL(val, 1);
}

BOOST_AUTO_TEST_CASE(matrix_test_canonical_eq)
{
    Matrix<int, -7> matrix;
    BOOST_CHECK_EQUAL(matrix.size(), 0);
    matrix[12][11]=-7;
    //BOOST_CHECK_EQUAL(matrix.size(), 0);
    //(matrix[12][11]=-7)=0;
    //BOOST_CHECK_EQUAL(matrix.size(), 1);
    //((matrix[12][11]=-7)=0)=-7;
    //BOOST_CHECK_EQUAL(matrix.size(), 0);
    //(((matrix[12][11]=-7)=0)=-7)=1231;
    //BOOST_CHECK_EQUAL(matrix[12][11], 1231);
}

BOOST_AUTO_TEST_SUITE_END()
