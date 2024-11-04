#include "gtest/gtest.h"
#include "../../userauthentication/header/userauthentication.h"  // Adjust this include path based on your project structure
#include "stdio.h"

class UserauthenticationTest : public ::testing::Test {
protected:


	void SetUp() override {
		// Setup test data
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
#ifdef ENABLE_USERAUTHENTICATION_TEST
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
#else
	return 0;
#endif
}