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