//#define ENABLE_COSTCALCULATOR_TEST  // Uncomment this line to enable the Costcalculator tests

#include "gtest/gtest.h"
#include "../../costcalculator/header/costcalculator.h"  // Adjust this include path based on your project structure

using namespace Coruh::Costcalculator;

class CostcalculatorTest : public ::testing::Test {
protected:
	void SetUp() override {
		// Setup test data
	}

	void TearDown() override {
		// Clean up test data
	}
};

TEST_F(CostcalculatorTest, TestAdd) {
	double result = Costcalculator::add(5.0, 3.0);
	EXPECT_DOUBLE_EQ(result, 8.0);
}

TEST_F(CostcalculatorTest, TestSubtract) {
	double result = Costcalculator::subtract(5.0, 3.0);
	EXPECT_DOUBLE_EQ(result, 2.0);
}

TEST_F(CostcalculatorTest, TestMultiply) {
	double result = Costcalculator::multiply(5.0, 3.0);
	EXPECT_DOUBLE_EQ(result, 15.0);
}

TEST_F(CostcalculatorTest, TestDivide) {
	double result = Costcalculator::divide(6.0, 3.0);
	EXPECT_DOUBLE_EQ(result, 2.0);
}

TEST_F(CostcalculatorTest, TestDivideByZero) {
	EXPECT_THROW(Costcalculator::divide(5.0, 0.0), std::invalid_argument);
}

/**
 * @brief The main function of the test program.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line argument strings.
 * @return int The exit status of the program.
 */
int main(int argc, char** argv) {
#ifdef ENABLE_COSTCALCULATOR_TEST
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
#else
	return 0;
#endif
}