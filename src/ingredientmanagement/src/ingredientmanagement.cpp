/**
 * @file ingredientmanagement.cpp
 * @brief Implementation of ingredient management functions, including Huffman encoding for ingredient names,
 * linked list operations, and file handling for saving/loading ingredients.
 */
#include "../header/ingredientmanagement.h"
#include "../../userauthentication/header/userauthentication.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

 /**
  * @brief Creates a new Huffman tree node.
  *
  * @param character The character to store in the node.
  * @param frequency The frequency of the character.
  * @return A pointer to the newly created HuffmanTreeNode.
  */
HuffmanTreeNode* createHuffmanTreeNode(char character, int frequency) {
	HuffmanTreeNode* node = (HuffmanTreeNode*)malloc(sizeof(HuffmanTreeNode));
	node->character = character;
	node->frequency = frequency;
	node->left = node->right = NULL;
	return node;
}

/**
 * @brief Counts character frequencies in a given string.
 *
 * @param str The input string.
 * @param freq An array to store the frequency of each character.
 */
void countFrequencies(const char* str, int freq[256]) {
	for (int i = 0; i < 256; i++) {
		freq[i] = 0;
	}
	for (int i = 0; str[i] != '\0'; i++) {
		freq[(unsigned char)str[i]]++;
	}
}

/**
 * @brief Constructs a Huffman tree from character frequencies.
 *
 * @param freq An array containing the frequency of each character.
 * @return A pointer to the root of the constructed Huffman tree.
 */
HuffmanTreeNode* constructHuffmanTree(const int freq[256]) {
	HuffmanTreeNode* nodes[256];
	int count = 0;

	for (int i = 0; i < 256; i++) {
		if (freq[i] > 0) {
			nodes[count++] = createHuffmanTreeNode((char)i, freq[i]);
		}
	}

	while (count > 1) {
		int minIndex1 = -1, minIndex2 = -1;
		for (int i = 0; i < count; i++) {
			if (minIndex1 == -1 || nodes[i]->frequency < nodes[minIndex1]->frequency) {
				minIndex2 = minIndex1;
				minIndex1 = i;
			}
			else if (minIndex2 == -1 || nodes[i]->frequency < nodes[minIndex2]->frequency) {
				minIndex2 = i;
			}
		}

		HuffmanTreeNode* merged = createHuffmanTreeNode('\0', nodes[minIndex1]->frequency + nodes[minIndex2]->frequency);
		merged->left = nodes[minIndex1];
		merged->right = nodes[minIndex2];

		nodes[minIndex1] = merged;
		nodes[minIndex2] = nodes[--count];
	}

	return nodes[0];
}

/**
 * @brief Generates Huffman codes from the Huffman tree.
 *
 * @param root The root of the Huffman tree.
 * @param code The current code being generated.
 * @param depth The depth of the current node.
 * @param codes An array to store the generated codes for each character.
 */
void generateHuffmanCodes(HuffmanTreeNode* root, char* code, int depth, char codes[256][256]) {
	if (root->left == NULL && root->right == NULL) {
		code[depth] = '\0';
		strcpy(codes[(unsigned char)root->character], code);
		return;
	}

	if (root->left != NULL) {
		code[depth] = '0';
		generateHuffmanCodes(root->left, code, depth + 1, codes);
	}

	if (root->right != NULL) {
		code[depth] = '1';
		generateHuffmanCodes(root->right, code, depth + 1, codes);
	}
}

/**
 * @brief Encodes a given string using Huffman codes.
 *
 * @param str The input string to be encoded.
 * @param codes An array containing Huffman codes for each character.
 * @param encodedStr The encoded output string.
 */
void encodeString(const char* str, const char codes[256][256], char* encodedStr) {
	encodedStr[0] = '\0';
	for (int i = 0; str[i] != '\0'; i++) {
		strcat(encodedStr, codes[(unsigned char)str[i]]);
	}
}

/**
 * @brief Decodes an encoded string using the Huffman tree.
 *
 * @param root The root of the Huffman tree.
 * @param encodedStr The encoded string to be decoded.
 * @param decodedStr The decoded output string.
 */
void decodeString(HuffmanTreeNode* root, const char* encodedStr, char* decodedStr) {
	HuffmanTreeNode* current = root;
	int index = 0;
	for (int i = 0; encodedStr[i] != '\0'; i++) {
		if (encodedStr[i] == '0') {
			current = current->left;
		}
		else {
			current = current->right;
		}

		if (current->left == NULL && current->right == NULL) {
			decodedStr[index++] = current->character;
			current = root;
		}
	}
	decodedStr[index] = '\0';
}

/**
 * @brief Adds a new ingredient to the linked list.
 *
 * @param head The head of the linked list.
 * @param name The name of the ingredient.
 * @param price The price of the ingredient.
 * @param filePath The file path to save the ingredients.
 * @return The updated head of the linked list.
 */
Ingredient* addIngredient(Ingredient* head, const char* name, float price, const char* filePath) {
	int freq[256];
	countFrequencies(name, freq);

	HuffmanTreeNode* root = constructHuffmanTree(freq);

	char codes[256][256];
	char code[256];
	generateHuffmanCodes(root, code, 0, codes);

	char encodedName[1024];
	encodeString(name, codes, encodedName);

	char decodedName[100];
	decodeString(root, encodedName, decodedName);

	Ingredient* newIngredient = (Ingredient*)malloc(sizeof(Ingredient));
	int newId = 1;
	if (head != NULL) {
		Ingredient* temp = head;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		newId = temp->id + 1;
	}
	newIngredient->id = newId;
	strcpy(newIngredient->name, decodedName);
	newIngredient->price = price;
	newIngredient->prev = NULL;
	newIngredient->next = NULL;

	if (head == NULL) {
		head = newIngredient;
	}
	else {
		Ingredient* temp = head;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = newIngredient;
		newIngredient->prev = temp;
	}

	saveIngredientsToFile(head, filePath);
	return head;
}