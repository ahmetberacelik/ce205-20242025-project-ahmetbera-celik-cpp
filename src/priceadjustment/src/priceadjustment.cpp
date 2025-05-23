/**
 * @file priceadjustment.cpp
 * @brief This file contains functions to adjust and reset ingredient prices using different hash table search techniques.
 */
#include "../../ingredientmanagement/header/ingredientmanagement.h"
#include "../../userauthentication/header/userauthentication.h"
#include "../header/priceadjustment.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
 /**
  * @brief Converts a doubly linked list of ingredients to an array.
  * @param pathFileIngredients Path to the ingredient file.
  * @param baseIngredients Pointer to the array of ingredients to be populated.
  * @return Total number of ingredients or -1 if there was an error.
  */
int ConvertDoubleLinkToArray(const char* pathFileIngredients, Ingredient** baseIngredients) {
	Ingredient* ingredients = loadIngredientsFromFile(pathFileIngredients);
	if (ingredients == NULL) {
		printf("Ingredients could not be loaded\n");
		enterToContinue();
		return -1;
	}

	// Find the total number of ingredients
	int count = 0;
	Ingredient* current = ingredients;
	while (current != NULL) {
		count++;
		current = current->next;
	}

	// Allocate memory for the ingredients array
	*baseIngredients = (Ingredient*)malloc(count * sizeof(Ingredient));

	// Copy ingredients from linked list to array
	current = ingredients;
	for (int i = 0; i < count; i++) {
		(*baseIngredients)[i] = *current;
		current = current->next;
	}

	return count;
}
/**
 * @brief Prints the ingredients loaded from a file to the console.
 * @param pathFileIngredients Path to the ingredient file.
 * @return Total number of ingredients printed.
 */
 /**
  * @brief Prints the ingredients loaded from a file to the console in descending order of price.
  * @param pathFileIngredients Path to the ingredient file.
  * @return Total number of ingredients printed.
  */
int PrintIngredientsToConsole(const char* pathFileIngredients) {
	// Find the total number of ingredients
	Ingredient* ingredientsArray;
	int count = ConvertDoubleLinkToArray(pathFileIngredients, &ingredientsArray);

	if (count == -1) {
		return 0;
	}

	// Sort ingredients using Max Heap to display them in descending order of price
	sortIngredientsMaxHeap(ingredientsArray, count);

	// Write the ingredients to the console
	printf("Ingredients (sorted by price in descending order):\n");
	for (int i = 0; i < count; i++) {
		printf("+--------------------------------------+\n");
		printf("| Ingredient ID: %-5d                   \n", ingredientsArray[i].id);
		printf("| Name         : %-20s |\n", ingredientsArray[i].name);
		printf("| Price        : $%-7.2f           \n", ingredientsArray[i].price);
		printf("+--------------------------------------+\n");
	}

	free(ingredientsArray);
	return count;
}
/**
 * @brief Checks if a given number is a prime number.
 * @param number The number to check.
 * @return True if the number is prime, otherwise false.
 */
bool isPrime(int number) {
	if (number <= 1) return false;
	for (int i = 2; i * i <= number; i++) {
		if (number % i == 0) return false;
	}
	return true;
}
/**
 * @brief Finds the next prime number greater than or equal to a given number.
 * @param number The starting number.
 * @return The next prime number.
 */
int findNextPrime(int number) {
	while (!isPrime(number)) {
		number++;
	}
	return number;
}

/**
 * @brief Heapify function for Max Heap.
 * @param ingredients Array of ingredients.
 * @param n Size of the array.
 * @param i Index of the root element.
 */
void heapify(Ingredient* ingredients, int n, int i) {
	int largest = i; // Initialize largest as root
	int left = 2 * i + 1;
	int right = 2 * i + 2;

	// If left child is larger than root
	if (left < n && ingredients[left].price > ingredients[largest].price) {
		largest = left;
	}

	// If right child is larger than largest so far
	if (right < n && ingredients[right].price > ingredients[largest].price) {
		largest = right;
	}

	// If largest is not root
	if (largest != i) {
		Ingredient temp = ingredients[i];
		ingredients[i] = ingredients[largest];
		ingredients[largest] = temp;

		// Recursively heapify the affected sub-tree
		heapify(ingredients, n, largest);
	}
}

/**
 * @brief Sorts the ingredients in descending order using Max Heap.
 * @param ingredients Array of ingredients.
 * @param n Total number of ingredients.
 */
void sortIngredientsMaxHeap(Ingredient* ingredients, int n) {
	// Build heap (rearrange array)
	for (int i = n / 2 - 1; i >= 0; i--) {
		heapify(ingredients, n, i);
	}

	// Extract elements from heap one by one
	for (int i = n - 1; i > 0; i--) {
		// Move current root to end
		Ingredient temp = ingredients[0];
		ingredients[0] = ingredients[i];
		ingredients[i] = temp;

		// Call max heapify on the reduced heap
		heapify(ingredients, i, 0);
	}
}

/**
 * @brief Searches for an ingredient using linear probing.
 * @param ingredients Array of ingredients.
 * @param totalIngredient Total number of ingredients in the array.
 * @param ingredientId The ID of the ingredient to search for.
 * @return Pointer to the ingredient if found, otherwise NULL.
 */
Ingredient* linearProbingSearch(Ingredient* ingredients, int totalIngredient, int ingredientId) {
	for (int i = 0; i < totalIngredient; i++) {
		if (ingredients[i].id == ingredientId) {
			return &ingredients[i];
		}
	}
	return NULL;
}

/**
 * @brief Searches for an ingredient using quadratic probing.
 * @param ingredients Array of ingredients.
 * @param totalIngredient Total number of ingredients in the array.
 * @param ingredientId The ID of the ingredient to search for.
 * @return Pointer to the ingredient if found, otherwise NULL.
 */
Ingredient* quadraticProbingSearch(Ingredient* ingredients, int totalIngredient, int ingredientId) {
	int startIdx = ingredientId % totalIngredient;
	for (int i = 0; i < totalIngredient; i++) {
		int index = (startIdx + i * i) % totalIngredient;
		if (ingredients[index].id == ingredientId) {
			return &ingredients[index];
		}
	}
	return NULL;
}

/**
 * @brief Searches for an ingredient using double hashing.
 * @param ingredients Array of ingredients.
 * @param totalIngredient Total number of ingredients in the array.
 * @param ingredientId The ID of the ingredient to search for.
 * @return Pointer to the ingredient if found, otherwise NULL.
 */
Ingredient* doubleHashingSearch(Ingredient* ingredients, int totalIngredient, int ingredientId) {
	int hash1 = ingredientId % totalIngredient;
	int hash2 = 1;
	if (totalIngredient > 1) {
		hash2 += ingredientId % (totalIngredient - 1);
	}

	for (int i = 0; i < totalIngredient; i++) {
		int index = (hash1 + i * hash2) % totalIngredient;
		if (ingredients[index].id == ingredientId) {
			return &ingredients[index];
		}
	}
	return NULL;
}

/**
 * @brief Searches for an ingredient using progressive overflow.
 * @param ingredients Array of ingredients.
 * @param totalIngredient Total number of ingredients in the array.
 * @param ingredientId The ID of the ingredient to search for.
 * @return Pointer to the ingredient if found, otherwise NULL.
 */
Ingredient* progressiveOverflowSearch(Ingredient* ingredients, int totalIngredient, int ingredientId) {
	int startIdx = ingredientId % totalIngredient;
	for (int i = 0; i < totalIngredient; i++) {
		int index = (startIdx + i) % totalIngredient;
		if (ingredients[index].id == ingredientId) {
			return &ingredients[index];
		}
	}
	return NULL;
}

/**
 * @brief Searches for an ingredient using linear quotient.
 * @param ingredients Array of ingredients.
 * @param totalIngredient Total number of ingredients in the array.
 * @param ingredientId The ID of the ingredient to search for.
 * @param c The increment value for linear quotient.
 * @return Pointer to the ingredient if found, otherwise NULL.
 */
Ingredient* linearQuotientSearch(Ingredient* ingredients, int totalIngredient, int ingredientId, int c) {
	int startIdx = ingredientId % totalIngredient;
	for (int i = 0; i < totalIngredient; i++) {
		int index = (startIdx + i * c) % totalIngredient;
		if (ingredients[index].id == ingredientId) {
			return &ingredients[index];
		}
	}
	return NULL;
}

/**
 * @brief Searches for an ingredient using Brent's method.
 * @param ingredients Array of ingredients.
 * @param totalIngredient Total number of ingredients in the array.
 * @param ingredientId The ID of the ingredient to search for.
 * @return Pointer to the ingredient if found, otherwise NULL.
 */
Ingredient* brentMethodSearch(Ingredient* ingredients, int totalIngredient, int ingredientId) {
	int hash1 = ingredientId % totalIngredient;
	int hash2 = 1;
	if (totalIngredient > 1) {
		hash2 += ingredientId % (totalIngredient - 1);
	}
	int bestIndex = hash1;
	for (int i = 0; i < totalIngredient; i++) {
		int index = (hash1 + i * hash2) % totalIngredient;
		if (ingredients[index].id == ingredientId) {
			return &ingredients[index];
		}
		// Brent's Method: swap if we find a shorter path
		if (i < (bestIndex - hash1) % totalIngredient) {
			bestIndex = index;
		}
	}
	return (ingredients[bestIndex].id == ingredientId) ? &ingredients[bestIndex] : NULL;
}

/**
 * @brief Searches for an ingredient within a bucket.
 * @param buckets Array of buckets.
 * @param bucketSize Total number of buckets.
 * @param ingredientId The ID of the ingredient to search for.
 * @return Pointer to the ingredient if found, otherwise NULL.
 */
Ingredient* bucketSearch(Bucket* buckets, int bucketSize, int ingredientId, float newPrice) {
	int bucketIndex = ingredientId % bucketSize;
	Ingredient* current = buckets[bucketIndex].head;
	while (current != NULL) {
		if (current->id == ingredientId) {
			current->price = newPrice; // Update the price of the ingredient
			return current;
		}
		current = current->next;
	}
	return NULL;
}

/**
 * @brief Adjusts the price of an ingredient.
 * @param pathFileIngredients Path to the ingredient file.
 * @return 1 if the ingredient price was successfully updated, otherwise 0.
 */
int adjustIngredientPrice(const char* pathFileIngredients) {
	clearScreen();
	Ingredient* ingredients;
	int totalIngredient = ConvertDoubleLinkToArray(pathFileIngredients, &ingredients);

	if (totalIngredient == -1) {
		return 0;
	}

	while (1) {
		clearScreen();
		PrintIngredientsToConsole(pathFileIngredients);

		// Get ingredient id from user
		int ingredientId;
		printf("Enter Ingredient Id: ");
		if (scanf("%d", &ingredientId) != 1) {
			printf("Invalid input. Please enter a valid number.\n");
			while (getchar() != '\n'); // Clear input buffer
			enterToContinue();
			continue;
		}

		// Check if ingredient id is valid
		if (ingredientId < 0) {
			printf("Invalid Ingredient Id\n");
			enterToContinue();
			break;
		}

		// Select algorithm for searching the ingredient
		int algorithmChoice;
		clearScreen();
		printf("\n+--------------------------------------+\n");
		printf("|     SELECT SEARCH ALGORITHM TO USE   |\n");
		printf("+--------------------------------------+\n");
		printf("| 1. Linear Probing                    |\n");
		printf("| 2. Quadratic Probing                 |\n");
		printf("| 3. Double Hashing                    |\n");
		printf("| 4. Progressive Overflow              |\n");
		printf("| 5. Use of Buckets                    |\n");
		printf("| 6. Linear Quotient                   |\n");
		printf("| 7. Brent's Method                    |\n");
		printf("+--------------------------------------+\n");
		printf("Enter your choice (1-8): ");

		if (scanf("%d", &algorithmChoice) != 1) {
			printf("Invalid input. Please enter a valid number.\n");
			while (getchar() != '\n'); // Clear input buffer
			enterToContinue();
			continue;
		}

		Ingredient* ingredient = NULL;

		// Search for the ingredient using the selected algorithm
		switch (algorithmChoice) {
		case 1:
			ingredient = linearProbingSearch(ingredients, totalIngredient, ingredientId);
			break;
		case 2:
			ingredient = quadraticProbingSearch(ingredients, totalIngredient, ingredientId);
			break;
		case 3:
			ingredient = doubleHashingSearch(ingredients, totalIngredient, ingredientId);
			break;
		case 4:
			ingredient = progressiveOverflowSearch(ingredients, totalIngredient, ingredientId);
			break;
		case 5: {
			// Bucket setup
			int totalBuckets = findNextPrime(totalIngredient);
			Bucket* buckets = (Bucket*)malloc(totalBuckets * sizeof(Bucket));
			for (int i = 0; i < totalBuckets; i++) {
				buckets[i].head = NULL;
			}

			// Fill buckets
			for (int i = 0; i < totalIngredient; i++) {
				int bucketIndex = ingredients[i].id % totalBuckets;
				Ingredient* newIngredient = (Ingredient*)malloc(sizeof(Ingredient));
				*newIngredient = ingredients[i];
				newIngredient->next = buckets[bucketIndex].head;
				buckets[bucketIndex].head = newIngredient;
			}

			float newPrice;
			printf("Enter new price: ");
			scanf("%f", &newPrice);
			ingredient = bucketSearch(buckets, totalBuckets, ingredientId, newPrice);

			// Update the original ingredients array if found in bucket
			if (ingredient != NULL) {
				for (int i = 0; i < totalIngredient; i++) {
					if (ingredients[i].id == ingredient->id) {
						ingredients[i] = *ingredient;
						break;
					}
				}
			}

			// Free bucket memory
			for (int i = 0; i < totalBuckets; i++) {
				Ingredient* current = buckets[i].head;
				while (current != NULL) {
					Ingredient* temp = current;
					current = current->next;
					free(temp);
				}
			}
			free(buckets);
			break;
		}
		case 6:
			ingredient = linearQuotientSearch(ingredients, totalIngredient, ingredientId, 1); // Using increment value of 1
			break;
		case 7:
			ingredient = brentMethodSearch(ingredients, totalIngredient, ingredientId);
			break;
		default:
			printf("Invalid choice");
			enterToContinue();
			continue;
		}

		// Check if ingredient was found
		if (ingredient == NULL) {
			printf("Ingredient not found");
			getchar();
			enterToContinue();
			break;
		}

		float newPrice;

		if (algorithmChoice == 5)
		{
			goto jump;
		}
		printf("Enter new price: ");
		if (scanf("%f", &newPrice) != 1 || newPrice < 0) {
			printf("Invalid price. Please enter a valid positive number.\n");
			while (getchar() != '\n'); // Clear input buffer
			enterToContinue();
			continue;
		}



		getchar();

		// Update the price of the ingredient
		ingredient->price = newPrice;

	jump:

		// Save the updated ingredients to file
		FILE* file = fopen(pathFileIngredients, "wb");
		if (!file) {
			printf("File could not be opened");
			getchar();
			enterToContinue();
			break;
		}
		fwrite(ingredients, sizeof(Ingredient), totalIngredient, file);
		fclose(file);

		printf("The ingredient was successfully updated\n");
		enterToContinue();
		return 1;
	}

	// Free allocated memory
	free(ingredients);
	return 0;
}


/**
 * @brief Resets the prices of all ingredients.
 * @param pathFileIngredients Path to the ingredient file.
 * @return 1 if successful.
 */
int resetIngredientPrice(const char* pathFileIngredients) {
	clearScreen();
	Ingredient* ingredients;
	int totalIngredient = ConvertDoubleLinkToArray(pathFileIngredients, &ingredients);

	if (totalIngredient == -1) {
		return 0;
	}

	// Show the current prices of all ingredients
	PrintIngredientsToConsole(pathFileIngredients);

	//Choose one of them to reset the price
	int ingredientId;
	printf("Enter Ingredient Id: ");
	scanf("%d", &ingredientId);
	getchar();

	//Find the ingredient and reset the price
	for (int i = 0; i < totalIngredient; i++) { if (ingredients[i].id == ingredientId) { ingredients[i].price = 0.0; break; } }

	// Save the updated ingredients to file
	FILE* file = fopen(pathFileIngredients, "wb");
	if (!file) { printf("File could not be opened"); getchar(); enterToContinue(); return 0; }
	fwrite(ingredients, sizeof(Ingredient), totalIngredient, file);
	fclose(file);

	printf("The ingredient price was successfully reset\n");
	enterToContinue();

	// Free allocated memory
	free(ingredients);
	return 1;
}

/**
 * @brief Displays a menu for adjusting ingredient prices.
 * @param pathFileIngredients Path to the ingredient file.
 * @return 1 on exit.
 */
int AdjustIngredientPriceMenu(const char* pathFileIngredients) {
	int choice;
	while (1) {
		clearScreen();
		printf("\n+--------------------------------------+\n");
		printf("|           PRICE ADJUSTMENT MENU      |\n");
		printf("+--------------------------------------+\n");
		printf("| 1. Adjust Ingredient Price           |\n");
		printf("| 2. Reset Price                       |\n");
		printf("| 3. Exit                              |\n");
		printf("+--------------------------------------+\n");
		printf("Enter your choice: ");


		choice = getInput();
		if (choice == -2) { handleInputError(); enterToContinue(); continue; }
		switch (choice) {
		case 1:
			adjustIngredientPrice(pathFileIngredients);
			break;
		case 2:
			resetIngredientPrice(pathFileIngredients);
			break;
		case 3:
			return 1;
		default:
			clearScreen();
			printf("Invalid choice. Please try again.\n");
			enterToContinue();
			break;

		}
	}

}