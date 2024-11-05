#include "gtest/gtest.h"
#include "../../priceadjustment/header/priceadjustment.h"  // Adjust this include path based on your project structure
extern User loggedUser;

class PriceadjustmentTest : public ::testing::Test {
protected:
	const char* testPathFileIngrednients = "test_ingredients.bin";
	const char* inputTest = "inputTest.txt";
	const char* outputTest = "outputTest.txt";

	void SetUp() override {
		loggedUser.id = 1;
		strcpy(loggedUser.name, "TestName");
		strcpy(loggedUser.surname, "TestSurname");
		strcpy(loggedUser.email, "test@example.com");
		strcpy(loggedUser.password, "password");
	}

	void TearDown() override {
		// Remove the ingredients file if it exists
		std::remove(testPathFileIngrednients);
		std::remove(inputTest);
		std::remove(outputTest);
	}

	void simulateUserInput(const char* userInput) {
		FILE* fileinput = fopen(inputTest, "wb");
		fputs(userInput, fileinput);
		fclose(fileinput);
		freopen(inputTest, "rb", stdin);
		freopen(outputTest, "wb", stdout);
	}
	void readOutput(const char* outputFilePath, char* buffer, size_t bufferSize) {
		FILE* fileoutput = fopen(outputFilePath, "rb");
		size_t charsRead = fread(buffer, sizeof(char), bufferSize - 1, fileoutput);
		fclose(fileoutput);
		buffer[charsRead] = '\0';
		removeClearScreenCharsFromOutputFile(buffer);
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

	void removeClearScreenCharsFromOutputFile(char* str) {
		char* src = str;
		char* dst = str;
		while (*src) {
			if (*src != '\f') {
				*dst++ = *src;
			}
			src++;
		}
		*dst = '\0';
	}

	void createIngredients() {
		//Ingredients are double linked list. Create 5 ingredients and link them.
		Ingredient* head = NULL;
		head = addIngredient(head, "Ingredient1", 1.0, testPathFileIngrednients);
		head = addIngredient(head, "Ingredient2", 2.0, testPathFileIngrednients);
		head = addIngredient(head, "Ingredient3", 3.0, testPathFileIngrednients);
		head = addIngredient(head, "Ingredient4", 4.0, testPathFileIngrednients);
		head = addIngredient(head, "Ingredient5", 5.0, testPathFileIngrednients);
		saveIngredientsToFile(head, testPathFileIngrednients);
	}
};

TEST_F(PriceadjustmentTest, HandlesAdjustIngredientPrice) {
	createIngredients();
	const char* inputSequence = "-1\n\n1\n1\n9\n1\n1\n1\n\n1\n1\n2\n2\n\n1\n1\n3\n3\n\n1\n1\n4\n4\n\n1\n1\n5\n5\n\n1\n1\n6\n6\n\n1\n1\n7\n7\n\n1\n1\n8\n8\n\n3\n";
	//1\n1\n1\n\n1\n2\n2\n\n1\n3\n3\n\n1\n4\n4\n\n1\n5\n5\n\n1\n6\n6\n\n1\n7\n7\n\n1\n8\n8\n\n
	simulateUserInput(inputSequence);

	int result = AdjustIngredientPriceMenu(testPathFileIngrednients);
	resetStdinStdout();

	EXPECT_EQ(result, 1);
}
TEST_F(PriceadjustmentTest, InvalidInput) {
	createIngredients();
	const char* inputSequence = "-1\n\n3\n";
	//1\n1\n9\n1\n1\n1\n\n1\n1\n2\n2\n\n1\n1\n3\n3\n\n1\n1\n4\n4\n\n1\n1\n5\n5\n\n1\n1\n6\n6\n\n1\n1\n7\n7\n\n1\n1\n8\n8\n\n3\n
	simulateUserInput(inputSequence);

	int result = AdjustIngredientPriceMenu(testPathFileIngrednients);
	resetStdinStdout();

	EXPECT_EQ(result, 1);
}
TEST_F(PriceadjustmentTest, PriceAdjustmentMenuInvalýdInput) {
	const char* inputSequence = "-1\n\n3\n";
	simulateUserInput(inputSequence);

	int result = AdjustIngredientPriceMenu(testPathFileIngrednients);
	resetStdinStdout();

	EXPECT_EQ(result, 1);
}
TEST_F(PriceadjustmentTest, PriceAdjustmentNoIngredients) {
	const char* inputSequence = "1\n\n3\n";
	simulateUserInput(inputSequence);

	int result = AdjustIngredientPriceMenu(testPathFileIngrednients);
	resetStdinStdout();

	EXPECT_EQ(result, 1);
}
TEST_F(PriceadjustmentTest, PriceAdjustmentInvalýdIngredientId) {
	createIngredients();
	const char* inputSequence = "1\n-1\n3\n";
	simulateUserInput(inputSequence);

	int result = AdjustIngredientPriceMenu(testPathFileIngrednients);
	resetStdinStdout();

	EXPECT_EQ(result, 1);
}
TEST_F(PriceadjustmentTest, PriceAdjustmentInvalýdAndFýrstAlgorithm) {
	createIngredients();
	const char* inputSequence = "1\n1\n9\n9\n1\n\n1\n1\n1\n1\n\n3\n";
	//1\n1\n2\n2\n\n1\n1\n3\n3\n\n1\n1\n4\n4\n\n1\n1\n5\n5\n\n1\n1\n6\n6\n\n1\n1\n7\n7\n\n1\n1\n8\n8\n\n3\n
	simulateUserInput(inputSequence);

	int result = AdjustIngredientPriceMenu(testPathFileIngrednients);
	resetStdinStdout();

	EXPECT_EQ(result, 1);
}
TEST_F(PriceadjustmentTest, PriceAdjustmentSecondAlgorithm) {
	createIngredients();
	const char* inputSequence = "1\n9\n2\n\n1\n1\n2\n2\n\n3\n";
	//1\n1\n3\n3\n\n1\n1\n4\n4\n\n1\n1\n5\n5\n\n1\n1\n6\n6\n\n1\n1\n7\n7\n\n1\n1\n8\n8\n\n3\n
	simulateUserInput(inputSequence);

	int result = AdjustIngredientPriceMenu(testPathFileIngrednients);
	resetStdinStdout();

	EXPECT_EQ(result, 1);
}
TEST_F(PriceadjustmentTest, PriceAdjustmentThirdAlgorithm) {
	createIngredients();
	const char* inputSequence = "1\n9\n3\n\n1\n1\n3\n3\n\n3\n";
	//1\n1\n4\n4\n\n1\n1\n5\n5\n\n1\n1\n6\n6\n\n1\n1\n7\n7\n\n1\n1\n8\n8\n\n3\n
	simulateUserInput(inputSequence);

	int result = AdjustIngredientPriceMenu(testPathFileIngrednients);
	resetStdinStdout();

	EXPECT_EQ(result, 1);
}
TEST_F(PriceadjustmentTest, PriceAdjustmentFourthAlgorithm) {
	createIngredients();
	const char* inputSequence = "1\n9\n4\n\n1\n1\n4\n4\n\n3\n";
	//1\n1\n5\n5\n\n1\n1\n6\n6\n\n1\n1\n7\n7\n\n1\n1\n8\n8\n\n3\n
	simulateUserInput(inputSequence);

	int result = AdjustIngredientPriceMenu(testPathFileIngrednients);
	resetStdinStdout();

	EXPECT_EQ(result, 1);
}
TEST_F(PriceadjustmentTest, PriceAdjustmentFifthAlgorithm) {
	createIngredients();
	const char* inputSequence = "1\n9\n5\n\n1\n1\n5\n5\n\n3\n";
	//1\n1\n6\n6\n\n1\n1\n7\n7\n\n1\n1\n8\n8\n\n3\n
	simulateUserInput(inputSequence);

	int result = AdjustIngredientPriceMenu(testPathFileIngrednients);
	resetStdinStdout();

	EXPECT_EQ(result, 1);
}
TEST_F(PriceadjustmentTest, PriceAdjustmentSixthAlgorithm) {
	createIngredients();
	const char* inputSequence = "1\n9\n6\n\n1\n1\n6\n6\n\n3\n";
	//1\n1\n7\n7\n\n1\n1\n8\n8\n\n3\n
	simulateUserInput(inputSequence);

	int result = AdjustIngredientPriceMenu(testPathFileIngrednients);
	resetStdinStdout();

	EXPECT_EQ(result, 1);
}
TEST_F(PriceadjustmentTest, PriceAdjustmentSeventhAlgorithm) {
	createIngredients();
	const char* inputSequence = "1\n9\n7\n\n1\n1\n7\n7\n\n3\n";
	//1\n1\n8\n8\n\n3\n
	simulateUserInput(inputSequence);

	int result = AdjustIngredientPriceMenu(testPathFileIngrednients);
	resetStdinStdout();

	EXPECT_EQ(result, 1);
}
TEST_F(PriceadjustmentTest, PriceAdjustmentEighthAlgorithm) {
	createIngredients();
	const char* inputSequence = "1\n1\n8\n8\n\n3\n";
	//9\n8\n\n1\n
	simulateUserInput(inputSequence);

	int result = AdjustIngredientPriceMenu(testPathFileIngrednients);
	resetStdinStdout();

	EXPECT_EQ(result, 1);
}
TEST_F(PriceadjustmentTest, ResetPriceNoIngredients) {
	const char* inputSequence = "2\n\n3\n";
	simulateUserInput(inputSequence);

	int result = AdjustIngredientPriceMenu(testPathFileIngrednients);
	resetStdinStdout();

	EXPECT_EQ(result, 1);
}
TEST_F(PriceadjustmentTest, ResetPriceSuccess) {
	createIngredients();
	const char* inputSequence = "2\n1\n\n3\n";
	simulateUserInput(inputSequence);

	int result = AdjustIngredientPriceMenu(testPathFileIngrednients);
	resetStdinStdout();

	EXPECT_EQ(result, 1);
}
TEST_F(PriceadjustmentTest, ConvertDoubleLinkToArray_NoIngredients) {
	// Arrange
	Ingredient* ingredients;
	// Act
	simulateUserInput("\n");
	int ingredientsCount = ConvertDoubleLinkToArray(testPathFileIngrednients, &ingredients);
	resetStdinStdout();
	// Assert
	EXPECT_EQ(ingredientsCount, -1);
}
TEST_F(PriceadjustmentTest, HandlesNonPrimeNumbers) {
	EXPECT_FALSE(isPrime(1));
}
TEST_F(PriceadjustmentTest, HandlesPrimeNumbers) {
	EXPECT_TRUE(isPrime(2));
}
TEST_F(PriceadjustmentTest, HandlesNextPrime) {
	EXPECT_EQ(findNextPrime(0), 2);
}
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