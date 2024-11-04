#include "gtest/gtest.h"
#include "../../userauthentication/header/userauthentication.h"  // Adjust this include path based on your project structure
#include "stdio.h"

class UserauthenticationTest : public ::testing::Test {
protected:
	const char* pathFileUsers = "test_users.bin";
	const char* pathFileIngredients = "test_ingredients.bin";
	const char* pathFileRecieps = "test_recieps.bin";
	const char* inputTest = "inputTest.txt";
	const char* outputTest = "outputTest.txt";

	void SetUp() override {
		// Setup test data
	}

	void TearDown() override {
		// Clean up test data
		remove(pathFileUsers);
		remove(pathFileIngredients);
		remove(pathFileRecieps);
		remove(inputTest);
		remove(outputTest);
	}

	void simulateUserInput(const char* userInput) {
		FILE* fileinput = fopen(inputTest, "wb");
		fputs(userInput, fileinput);
		fclose(fileinput);
		freopen(inputTest, "rb", stdin);
		freopen(outputTest, "wb", stdout);
	}

	void resetStdinStdout() {
		fclose(stdin);
		fflush(stdout);
#ifdef _WIN32
		freopen("CON", "a", stdout);
		freopen("CON", "r", stdin);
#else
		freopen("/dev/tty", "a", stdout);
		freopen("/dev/tty", "r", stdin);
#endif // _WIN32
	}

};


TEST_F(UserauthenticationTest, enterToContinueTest) {
	simulateUserInput("\n");

	bool result = enterToContinue();

	resetStdinStdout();

	EXPECT_TRUE(result);
}

TEST_F(UserauthenticationTest, handleInputErrorTest) {
	simulateUserInput("asd\n");

	bool result = handleInputError();

	resetStdinStdout();

	EXPECT_FALSE(result);
}

TEST_F(UserauthenticationTest, getInputTest) {
	simulateUserInput("5\n");
	int result = getInput();
	EXPECT_EQ(5, result);
}

TEST_F(UserauthenticationTest, getInputTest_WrongInput) {
	simulateUserInput("asd\n");
	int result = getInput();
	EXPECT_EQ(-2, result);
}




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