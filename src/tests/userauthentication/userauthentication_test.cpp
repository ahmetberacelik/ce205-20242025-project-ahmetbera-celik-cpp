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

TEST_F(UserauthenticationTest, getInputTest_EmptyInput) {
	simulateUserInput("");
	int result = getInput();
	EXPECT_EQ(-1, result);
}

TEST_F(UserauthenticationTest, loginUserTest_FailedFileOpen) {

	User testUser;
	strcpy(testUser.email, "test@example.com");
	strcpy(testUser.password, "test123");

	simulateUserInput("asd_input");

	int result = loginUser(testUser, pathFileUsers);

	resetStdinStdout();

	EXPECT_EQ(result, 0);
	EXPECT_FALSE(result);
}

TEST_F(UserauthenticationTest, loginUserTest_UsersRegistered) {
	simulateUserInput("\n\n\n\n\n");

	User testUser = {};
	int result = loginUser(testUser, pathFileUsers);

	resetStdinStdout();

	EXPECT_EQ(result, 0);
}

TEST_F(UserauthenticationTest, loginUserTest_NoUsersRegistered) {
	FILE* emptyFile = fopen(pathFileUsers, "wb");

	simulateUserInput("\n\n\n");

	fclose(emptyFile);

	User testUser = {};

	int result = loginUser(testUser, pathFileUsers);

	resetStdinStdout();

	EXPECT_EQ(result, 0);
}
TEST_F(UserauthenticationTest, loginUserTest_InvalidUser) {
	User testUser;
	strcpy(testUser.email, "qwe");
	strcpy(testUser.password, "qwe");
	strcpy(testUser.name, "qwe");
	strcpy(testUser.surname, "qwe");
	User testUser2;
	strcpy(testUser2.email, "qwe123");
	strcpy(testUser2.password, "qwe123");
	strcpy(testUser2.name, "qwe123");
	strcpy(testUser2.surname, "qwe123");

	simulateUserInput("\n\n\n");
	registerUser(testUser, pathFileUsers);


	int result = loginUser(testUser2, pathFileUsers);

	resetStdinStdout();

	EXPECT_EQ(result, 0);
}
TEST_F(UserauthenticationTest, printMainMenuTest) {
	int result = printMainMenu();
	EXPECT_EQ(result, 1);
}

TEST_F(UserauthenticationTest, printUserMenuTest) {
	int result = printUserMenu();
	EXPECT_EQ(result, 1);
}

TEST_F(UserauthenticationTest, printGuestMenuTest) {
	int result = printGuestMenu();
	EXPECT_EQ(result, 1);
}

TEST_F(UserauthenticationTest, getNewUserIdTest_EmptyUsers) {
	User users[1];
	int result = getNewUserId(users, 0);
	EXPECT_EQ(result, 1);
}

TEST_F(UserauthenticationTest, getNewUserIdTest_WithUsers) {
	User users[1];
	users[0].id = 5;
	int result = getNewUserId(users, 1);
	EXPECT_EQ(result, 6);
}

TEST_F(UserauthenticationTest, guestOperationsTest_ExitChoice) {
	simulateUserInput("3\n");

	int result = guestOperations(pathFileIngredients, "testRecipes.bin");

	resetStdinStdout();

	EXPECT_EQ(result, 0);
}

TEST_F(UserauthenticationTest, userOperationsTest_ExitChoice) {
	simulateUserInput("5\n");

	int result = userOperations(pathFileIngredients, "testRecipes.bin");

	resetStdinStdout();

	EXPECT_EQ(result, 0);
}

TEST_F(UserauthenticationTest, RegisterUser_Success) {
	User testUser;
	strcpy(testUser.email, "newuser@example.com");
	strcpy(testUser.password, "password123");

	// Ensure test file is reset
	FILE* file = fopen(pathFileUsers, "wb");
	int userCount = 0;
	fwrite(&userCount, sizeof(int), 1, file);
	fclose(file);

	// Simulate pressing enter
	simulateUserInput("\n");

	int result = registerUser(testUser, pathFileUsers);

	resetStdinStdout();

	EXPECT_EQ(result, 1);
}

TEST_F(UserauthenticationTest, RegisterUser_UserAlreadyExists) {
	User existingUser;
	strcpy(existingUser.email, "existinguser@example.com");
	strcpy(existingUser.password, "password123");

	// Write existing user to test file
	FILE* file = fopen(pathFileUsers, "wb");
	int userCount = 1;
	fwrite(&userCount, sizeof(int), 1, file);
	fwrite(&existingUser, sizeof(User), 1, file);
	fclose(file);

	User testUser;
	strcpy(testUser.email, "existinguser@example.com");
	strcpy(testUser.password, "newpassword456");

	// Simulate pressing enter
	simulateUserInput("\n");

	int result = registerUser(testUser, pathFileUsers);

	resetStdinStdout();

	EXPECT_EQ(result, 0);
}
TEST_F(UserauthenticationTest, mainMenuShouldEnterEveryCase) {
	simulateUserInput("qwe\n\n\n6\n\n2\n\n\n\n\n\n1\n\n\n\n5\n3\n3\n4\n");

	int result = mainMenu(pathFileUsers, pathFileIngredients, pathFileRecieps);

	resetStdinStdout();

	EXPECT_EQ(result, 0);
}
TEST_F(UserauthenticationTest, userOperationsShouldEnterEveryCase) {
	simulateUserInput("qwe\n\n\n6\n\n1\n5\n\n2\n6\n3\n3\n4\n123\n3\n5\n");

	int result = userOperations(pathFileIngredients, pathFileRecieps);

	resetStdinStdout();

	EXPECT_EQ(result, 0);
}
TEST_F(UserauthenticationTest, guestOperationsShouldEnterEveryCase) {
	simulateUserInput("qwe\n\n\n6\n\n1\n\n\n2\n\n3\n");

	int result = guestOperations(pathFileIngredients, pathFileRecieps);

	resetStdinStdout();

	EXPECT_EQ(result, 0);
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
	return 0;
#endif
}