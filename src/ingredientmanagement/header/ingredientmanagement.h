/**
 * @file ingredientmanagement.h
 * @brief Header file for ingredient management, including struct definitions and function declarations
 * for managing ingredients and Huffman coding.
 */
#ifndef INGREDIENTMANAGEMENT_H
#define INGREDIENTMANAGEMENT_H

#include <stdbool.h>

/**
  * @brief Ingredient struct definition.
  */
typedef struct Ingredient {
    int id; /**< Unique ingredient ID. */
    char name[100];  /**< Ingredient name. */
    float price; /**< Ingredient price. */
    struct Ingredient* prev; /**< Pointer to the previous ingredient in the list. */
    struct Ingredient* next; /**< Pointer to the next ingredient in the list. */
    struct Ingredient* npx; /**< Pointer to the npx ingredient in the list. */
} Ingredient;

/**
 * @brief Huffman Tree Node struct definition.
 */
typedef struct HuffmanTreeNode {
    char character; /**< Character represented by the node. */
    int frequency; /**< Frequency of the character. */
    struct HuffmanTreeNode* left; /**< Pointer to the left child. */
    struct HuffmanTreeNode* right; /**< Pointer to the right child. */
} HuffmanTreeNode;

// Functions for ingredient management
Ingredient* addIngredient(Ingredient* head, const char* name, float price, const char* filePath);
bool saveIngredientsToFile(Ingredient* head, const char* filePath);
bool listIngredientsDLL(Ingredient* head);
bool listIngredientsXLL(Ingredient* head);
bool listIngredients(Ingredient* head);
Ingredient* loadIngredientsFromFile(const char* filePath);
Ingredient* removeIngredient(Ingredient* head, int id, const char* filePath);
Ingredient* editIngredient(Ingredient* head, const char* filePath);
int ingredientManagementMenu(const char* filePath);
int printIngredientManagementMenu();
int printIngredientViewMenu();

// Huffman coding functions
void freeHuffmanTree(HuffmanTreeNode* root);
bool saveHuffmanEncodedIngredientsToFile(Ingredient* head);
Ingredient* loadHuffmanEncodedIngredientsFromFile();
HuffmanTreeNode* createHuffmanTreeNode(char character, int frequency);
void countFrequencies(const char* str, int freq[256]);
HuffmanTreeNode* constructHuffmanTree(const int freq[256]);
void generateHuffmanCodes(HuffmanTreeNode* root, char* code, int depth, char codes[256][256]);
void encodeString(const char* str, const char codes[256][256], char* encodedStr);
void decodeString(HuffmanTreeNode* root, const char* encodedStr, char* decodedStr);

void computeLPSArray(const char* pattern, int* lps, int m);
bool KMPSearch(const char* text, const char* pattern);
void searchIngredientByKMP(Ingredient* head);

#endif // INGREDIENTMANAGEMENT_H