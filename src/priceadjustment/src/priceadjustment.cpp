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
int PrintIngredientsToConsole(const char* pathFileIngredients) {
	// Find the total number of ingredients
	Ingredient* ingredientsArray;
	int count = ConvertDoubleLinkToArray(pathFileIngredients, &ingredientsArray);


	// Write the ingredients to the console
	printf("Ingredients:\n");
	for (int i = 0; i < count; i++) {
		printf("+--------------------------------------+\n");
		printf("| Ingredient ID: %-5d                   \n", ingredientsArray[i].id);
		printf("| Name         : %-20s |\n", ingredientsArray[i].name);
		printf("| Price        : $%-7.2f           \n", ingredientsArray[i].price);
		printf("+--------------------------------------+\n");

	}

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

int findNextPrime(int number) {
	while (!isPrime(number)) {
		number++;
	}
	return number;
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
	int hash2 = 1 + (ingredientId % (totalIngredient - 1));
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
	int hash2 = 1 + (ingredientId % (totalIngredient - 1));
	int bestIndex = hash1;
	for (int i = 0; i < totalIngredient; i++) {
		int index = (hash1 + i * hash2) % totalIngredient;
		if (ingredients[index].id == ingredientId) {
			return &ingredients[index];
		}
		// Brent's Method: swap if we find a shorter path
		if (i < (bestIndex - hash1) % totalIngredient) { bestIndex = index; }
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
Ingredient* bucketSearch(Bucket* buckets, int bucketSize, int ingredientId) {
	int bucketIndex = ingredientId % bucketSize;
	Ingredient* current = buckets[bucketIndex].head;
	while (current != NULL) {
		if (current->id == ingredientId) {
			return current;
		}
		current = current->next;
	}
	return NULL;
}
/**
 * @brief Searches for an ingredient using a hashtable approach.
 * @param ingredients Array of ingredients.
 * @param totalIngredient Total number of ingredients in the array.
 * @param ingredientId The ID of the ingredient to search for.
 * @return Pointer to the ingredient if found, otherwise NULL.
 */
Ingredient* hashtableSearch(Ingredient* ingredients, int totalIngredient, int ingredientId) {
	int index = ingredientId % totalIngredient;
	while (ingredients[index].id != -1) {
		if (ingredients[index].id == ingredientId) { return &ingredients[index]; }index = (index + 1) % totalIngredient;
	}return NULL;
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

	// Ensure totalIngredient is a prime number for better hash distribution
	totalIngredient = findNextPrime(totalIngredient);

	while (1) {
		clearScreen();
		PrintIngredientsToConsole(pathFileIngredients);

		// Get ingredient id from user
		int ingredientId;
		printf("Enter Ingredient Id: ");
		scanf("%d", &ingredientId);

		// Check if ingredient id is valid
		if (ingredientId < 0) {
			printf("Invalid Ingredient Id");
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
		printf("| 8. Hashtable                         |\n");
		printf("+--------------------------------------+\n");
		printf("Enter your choice (1-8): ");

		scanf("%d", &algorithmChoice);

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

			ingredient = bucketSearch(buckets, totalBuckets, ingredientId);

			// Update the original ingredients array if found in bucket
			if (ingredient != NULL) {
				for (int i = 0; i < totalIngredient; i++) {
					if (ingredients[i].id == ingredient->id) {
						ingredients[i] = *ingredient;break;
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
		case 8:
			ingredient = hashtableSearch(ingredients, totalIngredient, ingredientId);
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
		// Get new price from user
		printf("Enter new price: ");
		scanf("%f", &newPrice);
		getchar();

		// Update the price of the ingredient
		ingredient->price = newPrice;

		// Save the updated ingredients to file
		FILE* file = fopen(pathFileIngredients, "wb");
		if (!file) { printf("File could not be opened");getchar();enterToContinue();break; }
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