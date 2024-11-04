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


#endif // PRICEADJUSTMENT_H
