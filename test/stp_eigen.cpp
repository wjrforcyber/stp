#include <catch.hpp>

#include <stp/stp_eigen.hpp>

using namespace stp;
using Eigen::MatrixXi;

TEST_CASE( "stp calculation using eigen", "[eigen]" )
{
    MatrixXi A(2, 4);
    A << 1, 0, 0, 0,
         0, 1, 1, 1;

    MatrixXi B(2, 4);
    B << 1, 1, 0, 1,
         0, 0, 1, 0;

    MatrixXi C = semi_tensor_product( A, B );

    CHECK( C.rows() == 2u );
    CHECK( C.cols() == 8u );
}

TEST_CASE( "swap matrix for column vector", "[eigen]" )
{
  /* A stp B = W (swap matrix) stp B stp A */
  MatrixXi A( 2, 1 );
  A << 1, 0;

  MatrixXi B( 2, 1 );
  B << 0, 1;

  MatrixXi C = generate_swap_matrix( 2, 2 );

  MatrixXi stp1 = semi_tensor_product( A, B );
  MatrixXi stp2 = semi_tensor_product( semi_tensor_product( C, B ), A );

  CHECK( stp1.rows() == stp2.rows() );
  CHECK( stp1.cols() == stp2.cols() );
  CHECK( stp1 == stp2 );
  
  MatrixXi stp3 = semi_tensor_product( B, A );
  MatrixXi stp4 = semi_tensor_product( semi_tensor_product( C, A ), B );

  CHECK( stp3.rows() == stp4.rows() );
  CHECK( stp3.cols() == stp4.cols() );
  CHECK( stp3 == stp4 );
}

TEST_CASE( "power reducing matrix", "[eigen]" )
{
  /* A stp A = PR_2 stp A, note that A is a Boolean value, cannot be a Matrix */
  MatrixXi A( 2, 1 );
  A << 1, 0;

  MatrixXi PR2( 4, 2 );
  PR2 << 1, 0,
         0, 0,
         0, 0,
         0, 1;

  MatrixXi stp1 = semi_tensor_product( A, A );
  MatrixXi stp2 = semi_tensor_product( PR2, A );

  CHECK( stp1 == stp2 );
}

TEST_CASE( "STP calculation by two methods", "[eigen]" )
{
  //n % p = 0 type 
  int m = 200;
  int n1 = 4 << 5;
  int p1 = 4;
  int q = 100;
  matrix A1 = stp::matrix_random_generation( m, n1 );
  matrix B1 = stp::matrix_random_generation( p1, q );
  CHECK( stp::semi_tensor_product( A1, B1, false, stp::stp_method::copy_method   ) ==
      stp::semi_tensor_product( A1, B1, false, stp::stp_method::native_method ) ); 
  
  //p % n = 0 type 
  int n2 = 4;
  int p2 = 4 << 5;
  matrix A2 = stp::matrix_random_generation( m, n2 );
  matrix B2 = stp::matrix_random_generation( p2, q );
  CHECK( stp::semi_tensor_product( A2, B2, false, stp::stp_method::copy_method   ) ==
      stp::semi_tensor_product( A2, B2, false, stp::stp_method::native_method ) ); 
}
