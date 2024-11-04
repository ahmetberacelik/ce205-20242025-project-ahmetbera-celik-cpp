#include "gtest/gtest.h"
#include "../../priceadjustment/header/priceadjustment.h"  // Adjust this include path based on your project structure

class PriceadjustmentTest : public ::testing::Test {
protected:

	void SetUp() override {

	}

	void TearDown() override {

	}
};

/**
 * @brief The main function of the test program.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line argument strings.
 * @return int The exit status of the program.
 */
int main(int argc, char** argv) {
#ifdef ENABLE_PRICEADJUSTMENT_TEST
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
#else
	return 0;
#endif
}