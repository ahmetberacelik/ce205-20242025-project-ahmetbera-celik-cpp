/**
 * @file priceadjustment.h
 * @brief Header file for price adjustment functions using various search algorithms.
 */

#ifndef PRICEADJUSTMENT_H
#define PRICEADJUSTMENT_H

#include "../../ingredientmanagement/header/ingredientmanagement.h"
#include "../../userauthentication/header/userauthentication.h"
#include <stdbool.h>

/**
  * @brief Bucket structure to store ingredients in hash table
  * @struct Bucket
  * @var Bucket::head Pointer to the first ingredient in the bucket
  */
typedef struct Bucket {
    Ingredient* head;
} Bucket;

/**
 * @brief Ensures the max-heap property for a given node in the ingredient array.
 *
 * This function checks and adjusts the max-heap property for the specified node
 * and its subtrees within the ingredient array. It assumes that the subtrees
 * of the node are already max-heaps.
 *
 * @param ingredients Pointer to the array of Ingredient objects.
 * @param n Total number of elements in the ingredient array.
 * @param i Index of the node to be checked and adjusted.
 */
void heapify(Ingredient* ingredients, int n, int i);

/**
 * @brief Sorts an array of ingredients in descending order using the max-heap algorithm.
 *
 * This function builds a max-heap from the given ingredient array and performs heap sort
 * to arrange the ingredients in descending order based on a specific criterion (e.g., price, quantity).
 *
 * @param ingredients Pointer to the array of Ingredient objects to be sorted.
 * @param n Total number of elements in the ingredient array.
 */
void sortIngredientsMaxHeap(Ingredient* ingredients, int n);

/**
 * @brief Converts a doubly linked list of ingredients to an array.
 * @param pathFileIngredients Path to the ingredient file.
 * @param baseIngredients Pointer to the array of ingredients to be populated.
 * @return Total number of ingredients or -1 if there was an error.
 */
int ConvertDoubleLinkToArray(const char* pathFileIngredients, Ingredient** baseIngredients);

/**
 * @brief Prints the ingredients loaded from a file to the console.
 * @param pathFileIngredients Path to the ingredient file.
 * @return Total number of ingredients printed.
 */
int PrintIngredientsToConsole(const char* pathFileIngredients);

/**
 * @brief Checks if a given number is a prime number.
 * @param number The number to check.
 * @return True if the number is prime, otherwise false.
 */
bool isPrime(int number);

/**
 * @brief Finds the next prime number greater than or equal to a given number.
 * @param number The starting number.
 * @return The next prime number.
 */
int findNextPrime(int number);

/**
 * @brief Searches for an ingredient using linear probing.
 * @param ingredients Array of ingredients.
 * @param totalIngredient Total number of ingredients in the array.
 * @param ingredientId The ID of the ingredient to search for.
 * @return Pointer to the ingredient if found, otherwise NULL.
 */
Ingredient* linearProbingSearch(Ingredient* ingredients, int totalIngredient, int ingredientId);

/**
 * @brief Searches for an ingredient using quadratic probing.
 * @param ingredients Array of ingredients.
 * @param totalIngredient Total number of ingredients in the array.
 * @param ingredientId The ID of the ingredient to search for.
 * @return Pointer to the ingredient if found, otherwise NULL.
 */
Ingredient* quadraticProbingSearch(Ingredient* ingredients, int totalIngredient, int ingredientId);

/**
 * @brief Searches for an ingredient using double hashing.
 * @param ingredients Array of ingredients.
 * @param totalIngredient Total number of ingredients in the array.
 * @param ingredientId The ID of the ingredient to search for.
 * @return Pointer to the ingredient if found, otherwise NULL.
 */
Ingredient* doubleHashingSearch(Ingredient* ingredients, int totalIngredient, int ingredientId);

/**
 * @brief Searches for an ingredient using progressive overflow.
 * @param ingredients Array of ingredients.
 * @param totalIngredient Total number of ingredients in the array.
 * @param ingredientId The ID of the ingredient to search for.
 * @return Pointer to the ingredient if found, otherwise NULL.
 */
Ingredient* progressiveOverflowSearch(Ingredient* ingredients, int totalIngredient, int ingredientId);

/**
 * @brief Searches for an ingredient using linear quotient.
 * @param ingredients Array of ingredients.
 * @param totalIngredient Total number of ingredients in the array.
 * @param ingredientId The ID of the ingredient to search for.
 * @param c The increment value for linear quotient.
 * @return Pointer to the ingredient if found, otherwise NULL.
 */
Ingredient* linearQuotientSearch(Ingredient* ingredients, int totalIngredient, int ingredientId, int c);

/**
 * @brief Searches for an ingredient using Brent's method.
 * @param ingredients Array of ingredients.
 * @param totalIngredient Total number of ingredients in the array.
 * @param ingredientId The ID of the ingredient to search for.
 * @return Pointer to the ingredient if found, otherwise NULL.
 */
Ingredient* brentMethodSearch(Ingredient* ingredients, int totalIngredient, int ingredientId);

/**
 * @brief Searches for an ingredient within a bucket.
 * @param buckets Array of buckets.
 * @param bucketSize Total number of buckets.
 * @param ingredientId The ID of the ingredient to search for.
 * @return Pointer to the ingredient if found, otherwise NULL.
 */
Ingredient* bucketSearch(Bucket* buckets, int bucketSize, int ingredientId);

/**
 * @brief Searches for an ingredient using a hashtable approach.
 * @param ingredients Array of ingredients.
 * @param totalIngredient Total number of ingredients in the array.
 * @param ingredientId The ID of the ingredient to search for.
 * @return Pointer to the ingredient if found, otherwise NULL.
 */
Ingredient* hashtableSearch(Ingredient* ingredients, int totalIngredient, int ingredientId);

/**
 * @brief Adjusts the price of an ingredient.
 * @param pathFileIngredients Path to the ingredient file.
 * @return 1 if the ingredient price was successfully updated, otherwise 0.
 */
int adjustIngredientPrice(const char* pathFileIngredients);

/**
 * @brief Resets the prices of all ingredients.
 * @param pathFileIngredients Path to the ingredient file.
 * @return 1 if successful.
 */
int resetIngredientPrice(const char* pathFileIngredients);

/**
 * @brief Displays a menu for adjusting ingredient prices.
 * @param pathFileIngredients Path to the ingredient file.
 * @return 1 on exit.
 */
int AdjustIngredientPriceMenu(const char* pathFileIngredients);

#endif // PRICEADJUSTMENT_H
