/******************************************************************************
test_ekf.cpp
A set of unit tests for ekf.cpp
*******************************************************************************
The MIT License (MIT)

  Copyright (c) 2015 Matthew A. Klein

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#include <limits.h>
#include <cmath>
#include <gtest/gtest.h>
#include <eigen-checks/gtest.h>
#include "snowmower_localization/ekf.h"

TEST(fSystemTest, PosVelZeroOmega) {
  Ekf ekf;
  Eigen::MatrixXd state(6,1);
  Eigen::MatrixXd stateNew(6,1);
  double dt;

  state << 0, 0, 0, 10, 0, 0;
  dt = 0.1;
  stateNew << 1, 0, 0, 10, 0, 0;
  EXPECT_TRUE(EIGEN_MATRIX_EQUAL_DOUBLE(stateNew,ekf.fSystem(state,dt)));

  dt = 10;
  stateNew << 100, 0, 0, 10, 0, 0;
  EXPECT_TRUE(EIGEN_MATRIX_EQUAL_DOUBLE(stateNew,ekf.fSystem(state,dt)));

  state << 0, 0, M_PI, 10, 0, 0;
  dt = 0.1;
  stateNew << -1, 0, -M_PI, 10, 0, 0;
  EXPECT_TRUE(EIGEN_MATRIX_EQUAL_DOUBLE(stateNew, ekf.fSystem(state,dt)));

  state << 0, 0, M_PI_4, 10, 0, 0;
  dt = 0.1;
  stateNew << sqrt(2)/2, sqrt(2)/2, M_PI_4, 10, 0, 0;
  EXPECT_TRUE(EIGEN_MATRIX_EQUAL_DOUBLE(stateNew, ekf.fSystem(state,dt)));

  state << 0, 0, 3*M_PI_4, 10, 0, 0;
  dt = 0.1;
  stateNew << -sqrt(2)/2, sqrt(2)/2, 3*M_PI_4, 10, 0, 0;
  EXPECT_TRUE(EIGEN_MATRIX_EQUAL_DOUBLE(stateNew, ekf.fSystem(state,dt)));

  state << 0, 0, 5*M_PI_4, 10, 0, 0;
  dt = 0.1;
  stateNew << -sqrt(2)/2, -sqrt(2)/2, -3*M_PI_4, 10, 0, 0;
  EXPECT_TRUE(EIGEN_MATRIX_EQUAL_DOUBLE(stateNew, ekf.fSystem(state,dt)));

  state << 0, 0, 7*M_PI_4, 10, 0, 0;
  dt = 0.1;
  stateNew << sqrt(2)/2, -sqrt(2)/2, -M_PI_4, 10, 0, 0;
  EXPECT_TRUE(EIGEN_MATRIX_EQUAL_DOUBLE(stateNew, ekf.fSystem(state,dt)));
}

TEST(fSystemTest, NegVelZeroOmega) {
  Ekf ekf;
  Eigen::MatrixXd state(6,1);
  Eigen::MatrixXd stateNew(6,1);
  double dt;

  state << 0, 0, 0, -10, 0, 0;
  dt = 0.1;
  stateNew << -1, 0, 0, -10, 0, 0;
  EXPECT_TRUE(EIGEN_MATRIX_EQUAL_DOUBLE(stateNew,ekf.fSystem(state,dt)));

  dt = 10;
  stateNew << -100, 0, 0, -10, 0, 0;
  EXPECT_TRUE(EIGEN_MATRIX_EQUAL_DOUBLE(stateNew,ekf.fSystem(state,dt)));

  state << 0, 0, M_PI, -10, 0, 0;
  dt = 0.1;
  stateNew << 1, 0, -M_PI, -10, 0, 0;
  EXPECT_TRUE(EIGEN_MATRIX_EQUAL_DOUBLE(stateNew, ekf.fSystem(state,dt)));

  state << 0, 0, M_PI_4, -10, 0, 0;
  dt = 0.1;
  stateNew << -sqrt(2)/2, -sqrt(2)/2, M_PI_4, -10, 0, 0;
  EXPECT_TRUE(EIGEN_MATRIX_EQUAL_DOUBLE(stateNew, ekf.fSystem(state,dt)));

  state << 0, 0, 3*M_PI_4, -10, 0, 0;
  dt = 0.1;
  stateNew << sqrt(2)/2, -sqrt(2)/2, 3*M_PI_4, -10, 0, 0;
  EXPECT_TRUE(EIGEN_MATRIX_EQUAL_DOUBLE(stateNew, ekf.fSystem(state,dt)));

  state << 0, 0, 5*M_PI_4, -10, 0, 0;
  dt = 0.1;
  stateNew << sqrt(2)/2, sqrt(2)/2, -3*M_PI_4, -10, 0, 0;
  EXPECT_TRUE(EIGEN_MATRIX_EQUAL_DOUBLE(stateNew, ekf.fSystem(state,dt)));

  state << 0, 0, 7*M_PI_4, -10, 0, 0;
  dt = 0.1;
  stateNew << -sqrt(2)/2, sqrt(2)/2, -M_PI_4, -10, 0, 0;
  EXPECT_TRUE(EIGEN_MATRIX_EQUAL_DOUBLE(stateNew, ekf.fSystem(state,dt)));
}

TEST(fSystemTest, ZeroVelNonZeroOmega) {
  Ekf ekf;
  Eigen::MatrixXd state(6,1);
  Eigen::MatrixXd stateNew(6,1);
  double dt;

  state << 0, 0, 0, 0, M_PI,  0;
  dt = 1;
  stateNew << 0, 0, -M_PI, 0, M_PI, 0;
  EXPECT_TRUE(EIGEN_MATRIX_EQUAL_DOUBLE(stateNew,ekf.fSystem(state,dt)));

  state << 0, 0, 0, 0, M_PI, 0;
  dt = 2;
  stateNew << 0, 0, 0, 0, M_PI, 0;
  EXPECT_TRUE(EIGEN_MATRIX_EQUAL_DOUBLE(stateNew,ekf.fSystem(state,dt)));

}

TEST(fSystemTest, drawCircles) {
  Ekf ekf;
  Eigen::MatrixXd state(6,1);
  Eigen::MatrixXd stateNew(6,1);

  double omega = 1;
  double v = 2.5;
  double r = v/omega;
  double c = 2*M_PI*r;
  double dt = c/v;

  state << 0, 0, 0, v, omega, 0;
  stateNew << 0, 0, 0, v, omega, 0;
  EXPECT_TRUE(EIGEN_MATRIX_EQUAL_DOUBLE(stateNew,ekf.fSystem(state,dt)));

  state << 0, 0, 0, v, -omega, 0;
  stateNew << 0, 0, 0, v, -omega, 0;
  EXPECT_TRUE(EIGEN_MATRIX_EQUAL_DOUBLE(stateNew,ekf.fSystem(state,dt)));

}

TEST(hDecaWaveTest, 3_4_5_Grid) {
  Ekf ekf;
  Eigen::MatrixXd state(6,1);

  MatrixXd DecaWaveBeaconLoc(4,2);
  Vector2d DecaWaveOffset;

  // Set up a grid of 4 3-4-5 triangles.
  // When beacon is in the middle, all distances will be 5. 
  DecaWaveBeaconLoc << 0.0, 0.0,
                       6.0, 0.0,
                       6.0, 8.0,
                       0.0, 8.0;
  Vector4d h;
  h << 5.0, 5.0, 5.0, 5.0;

  // Start with zero offset and robot in middle
  DecaWaveOffset << 0.0, 0.0;
  state << 3.0, 4.0, 0.0, 0.0, 0.0, 0.0;
  EXPECT_TRUE(EIGEN_MATRIX_EQUAL_DOUBLE(h,ekf.hDecaWave(state, DecaWaveBeaconLoc, DecaWaveOffset)));

  // Offset the decawave and offset the robot
  DecaWaveOffset << -1.0, -2.0;
  state << 4.0, 6.0, 0.0, 0.0, 0.0, 0.0;
  EXPECT_TRUE(EIGEN_MATRIX_EQUAL_DOUBLE(h,ekf.hDecaWave(state, DecaWaveBeaconLoc, DecaWaveOffset)));

  // Now also offset the angle
  DecaWaveOffset << -1.0, -2.0;
  state << 1.0, 5.0, M_PI_2, 0.0, 0.0, 0.0;
  EXPECT_TRUE(EIGEN_MATRIX_EQUAL_DOUBLE(h,ekf.hDecaWave(state, DecaWaveBeaconLoc, DecaWaveOffset)));
}

TEST(hEncTest, straightLine) {
  Ekf ekf;
  Eigen::MatrixXd state(6,1);
  double b;
  double tpmRight, tpmLeft;
  int ticksPreRight, ticksPreLeft;
  double dt;
  Eigen::MatrixXd h(2,1);

  // Stand still at 0 ticks each.
  state << 0.0, 0.0, 0.0, // x, y, theta
           0.0, 0.0, 0.0; // v, omega, bias
  b = 1.0;
  tpmRight = 10.0;
  tpmLeft = 10.0;
  ticksPreRight = 0;
  ticksPreLeft = 0;
  dt = 1.0;
  h << 0.0, 0.0;
  EXPECT_TRUE(EIGEN_MATRIX_EQUAL_DOUBLE(h,ekf.hEnc(state, b, tpmRight, tpmLeft,
						   ticksPreRight, ticksPreLeft,
						   dt)));

  // Move forward 10 ticks from -10, 10.
  state << 0.0, 0.0, 0.0, // x, y, theta
           1.0, 0.0, 0.0; // v, omega, bias
  b = 1.0;
  tpmRight = 10.0;
  tpmLeft = 10.0;
  ticksPreRight = -10.0;
  ticksPreLeft = 10.0;
  dt = 1.0;
  h << 0.0, 20.0;
  EXPECT_TRUE(EIGEN_MATRIX_EQUAL_DOUBLE(h,ekf.hEnc(state, b, tpmRight, tpmLeft,
						   ticksPreRight, ticksPreLeft,
						   dt)));

  // Move forward 10 ticks from -10, 10.
  state << 0.0, 0.0, 0.0, // x, y, theta
           1.0, 0.0, 0.0; // v, omega, bias
  b = 1.0;
  tpmRight = 10.0;
  tpmLeft = 10.0;
  ticksPreRight = 1.0;
  ticksPreLeft = 0.0;
  dt = 1.0;
  h << 11.0, 10.0;
  EXPECT_TRUE(EIGEN_MATRIX_EQUAL_DOUBLE(h,ekf.hEnc(state, b, tpmRight, tpmLeft,
						   ticksPreRight, ticksPreLeft,
						   dt)));

}

TEST(hEncTest, spinInPlace) {
  Ekf ekf;
  Eigen::MatrixXd state(6,1);
  double b;
  double tpmRight, tpmLeft;
  int ticksPreRight, ticksPreLeft;
  double dt;
  Eigen::MatrixXd h(2,1);
  double omega, v, vRight, vLeft, r;

  // Spin in place for a second.
  omega = 1;
  v = 0;
  b = 1.0;
  if (omega != 0) {
    r = v/omega;
    vRight = (r + b/2) * omega;
    vLeft =  (r - b/2) * omega;
  }
  else {
    vRight = v;
    vLeft = v;
  }
  dt = 1.0;

  state << 0.0, 0.0, 0.0, // x, y, theta
           v, omega, 0.0; // v, omega, bias
  tpmRight = 10.0;
  tpmLeft = 10.0;
  ticksPreRight = 0.0;
  ticksPreLeft = 0.0;

  h << ticksPreRight + vRight*dt*tpmRight,
    ticksPreLeft + vLeft*dt*tpmLeft;
  EXPECT_TRUE(EIGEN_MATRIX_EQUAL_DOUBLE(h,ekf.hEnc(state, b, tpmRight, tpmLeft,
						   ticksPreRight, ticksPreLeft,
						   dt)));

  // Spin the other way for a second.
  omega = 3;
  v = 0;
  b = 0.6;
  if (omega != 0) {
    r = v/omega;
    vRight = (r + b/2) * omega;
    vLeft =  (r - b/2) * omega;
  }
  else {
    vRight = v;
    vLeft = v;
  }
  dt = 1.0;

  state << 1.0, 0.2, M_PI, // x, y, theta
           v, omega, 0.3; // v, omega, bias
  tpmRight = 25000.0;
  tpmLeft =  24000.0;
  ticksPreRight = 290234;
  ticksPreLeft = -289082;

  h << ticksPreRight + vRight*dt*tpmRight,
    ticksPreLeft + vLeft*dt*tpmLeft;
  EXPECT_TRUE(EIGEN_MATRIX_EQUAL_DOUBLE(h,ekf.hEnc(state, b, tpmRight, tpmLeft,
						   ticksPreRight, ticksPreLeft,
						   dt)));

}

TEST(hEncTest, moveAndSpin) {
  Ekf ekf;
  Eigen::MatrixXd state(6,1);
  double b;
  double tpmRight, tpmLeft;
  int ticksPreRight, ticksPreLeft;
  double dt;
  Eigen::MatrixXd h(2,1);
  double omega, v, vRight, vLeft, r;

  // Forward and CCW turn.
  omega = 1;
  v = 1;
  b = 1.0;
  if (omega != 0) {
    r = v/omega;
    vRight = (r + b/2) * omega;
    vLeft =  (r - b/2) * omega;
  }
  else {
    vRight = v;
    vLeft = v;
  }
  dt = 1.0;

  state << 0.0, 0.0, 0.0, // x, y, theta
           v, omega, 0.0; // v, omega, bias
  tpmRight = 10.0;
  tpmLeft = 10.0;
  ticksPreRight = 0.0;
  ticksPreLeft = 0.0;

  h << ticksPreRight + vRight*dt*tpmRight,
    ticksPreLeft + vLeft*dt*tpmLeft;
  EXPECT_TRUE(EIGEN_MATRIX_EQUAL_DOUBLE(h,ekf.hEnc(state, b, tpmRight, tpmLeft,
						   ticksPreRight, ticksPreLeft,
						   dt)));

  // Forward and CW TURN.
  omega = -3;
  v = 4;
  b = 0.6;
  if (omega != 0) {
    r = v/omega;
    vRight = (r + b/2) * omega;
    vLeft =  (r - b/2) * omega;
  }
  else {
    vRight = v;
    vLeft = v;
  }
  dt = 1.0;

  state << 1.0, 0.2, M_PI, // x, y, theta
           v, omega, 0.3; // v, omega, bias
  tpmRight = 25000.0;
  tpmLeft =  24000.0;
  ticksPreRight = 290234;
  ticksPreLeft = -289082;

  h << ticksPreRight + vRight*dt*tpmRight,
    ticksPreLeft + vLeft*dt*tpmLeft;
  EXPECT_TRUE(EIGEN_MATRIX_EQUAL_DOUBLE(h,ekf.hEnc(state, b, tpmRight, tpmLeft,
						   ticksPreRight, ticksPreLeft,
						   dt)));

  // Reverse and CCW turn.
  omega = 1;
  v = -1;
  b = 1.0;
  if (omega != 0) {
    r = v/omega;
    vRight = (r + b/2) * omega;
    vLeft =  (r - b/2) * omega;
  }
  else {
    vRight = v;
    vLeft = v;
  }
  dt = 1.0;

  state << 0.0, 0.0, 0.0, // x, y, theta
           v, omega, 0.0; // v, omega, bias
  tpmRight = 10.0;
  tpmLeft = 10.0;
  ticksPreRight = 0.0;
  ticksPreLeft = 0.0;

  h << ticksPreRight + vRight*dt*tpmRight,
    ticksPreLeft + vLeft*dt*tpmLeft;
  EXPECT_TRUE(EIGEN_MATRIX_EQUAL_DOUBLE(h,ekf.hEnc(state, b, tpmRight, tpmLeft,
						   ticksPreRight, ticksPreLeft,
						   dt)));

  // Reverse and CW TURN.
  omega = -3;
  v = -4;
  b = 0.6;
  if (omega != 0) {
    r = v/omega;
    vRight = (r + b/2) * omega;
    vLeft =  (r - b/2) * omega;
  }
  else {
    vRight = v;
    vLeft = v;
  }
  dt = 1.0;

  state << 1.0, 0.2, M_PI, // x, y, theta
           v, omega, 0.3; // v, omega, bias
  tpmRight = 25000.0;
  tpmLeft =  24000.0;
  ticksPreRight = 290234;
  ticksPreLeft = -289082;

  h << ticksPreRight + vRight*dt*tpmRight,
    ticksPreLeft + vLeft*dt*tpmLeft;
  EXPECT_TRUE(EIGEN_MATRIX_EQUAL_DOUBLE(h,ekf.hEnc(state, b, tpmRight, tpmLeft,
						   ticksPreRight, ticksPreLeft,
						   dt)));

}

GTEST_API_ int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
