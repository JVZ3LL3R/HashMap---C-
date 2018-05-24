#pragma once

#include<iostream>
#include<Windows.h>

using namespace std;

// Defines of program

#define CLEAR_VIDEO										system("cls")
#define PAUSE											system("pause")
#define WEIGH_INITIAL									50							// Initial weigh for hash code calculates
#define HASH_CODE_QTY									200							// Quantity of Hash Code
#define DESCRIPTION_SIZE								40
#define FILE_PATH										"..\\Cadastro.cad"			// Path and file name
#define BRASIL											setlocale(LC_ALL,"portuguese_brazil")

//
// Operators Options
//

#define INSERT_NEW_PRODUCT								'I'							// Insert a new product
#define DELETE_PRODUCT									'D'							// Delete an existing product
#define	SHOW_PRODUCTS_DATA								'S'							// Show de data of the existing product
#define PRODUCT_ADD										'P'							// Add quantity to stock of product
#define PRODUCT_SUBTRACT								'M'							// Subtract quantity to stock of product
#define PROGRAM_EXIT									'X'							// Program exit

// 
// Product Struct - Template of Product Record
//

typedef struct tagPROduto
{
	char cEAN13[14],																// Product Code
		cActive,																	// Product Active = 'A', 'E' = Excluded
		cDescription[DESCRIPTION_SIZE + 1];											// Product Description
	double bUnitPrice,																// Unit Price
		dStockQty;																	// Quantity of Stock
}PRODUTO;

//
// Search Key or Linked List Structure
//

typedef struct tagCHAVE_BUSCA														// Search Key
{
	int nPositional;																// Record Positional in file
	char cEAN13[14],																// Product Code
		cActive;																	// Product Active = 'A', 'E' = Excluded
	tagCHAVE_BUSCA *ptrNext;														// Pointer of next Key
}CHAVE_BUSCA;

//
//Hash table item
//

typedef struct tagHASH_TABLE
{
	CHAVE_BUSCA *ptrFirst,															// First linked list pointer of one hash code
		*ptrLast;																	// Last linked list pointer of last item
}HASH_TABLE;

//
// Class clProduto
//

class clProduto
{
private:
	// Private class atributes and methods
	int nNextPosProduct;															// Positional of next product
	HASH_TABLE vetHashTable[HASH_CODE_QTY];											// Hash table
	PRODUTO stProduct;																// To contains data of one product
	FILE *fdProduct;																// File descriptor of product file

	// TO DO -- ???????
	// Gravar posicional
	// Ler posicional
	// ???????????

	// Method for hash code calculation
	//		Parameter:
	//			Input:		char *ptrEAN13 - pointer of an string that contains 
	//										 the key
	//
	//			Output:		int nSum - hash code calculated
	int HashCodeCalculation(char *ptrEAN13)
	{
		int i;																		// Index and generic counter
		int	nSum = 0,																// Sum of ASCII digit * weight
			nWeight = WEIGH_INITIAL;												// Weigth of each digit
		for (i = 0; i < strnlen_s(ptrEAN13,sizeof(ptrEAN13)); i++)
			nSum += nWeight-- * *(ptrEAN13 + 1);									// Sum of ASCII digit * weigth
		return nSum % HASH_CODE_QTY;												// Return the hash Code
	}

	// Method for insertion of the Key in the Linked List
	// Parameter:
	//		Input: int nHash	  - Hash Code value
	//			   char *ptrEAN13 - String of Product Code
	//
	//		Output: False - Memory Out
	//				True  - Indicates OK
	bool InsertKeyHashTable(int nHash, char *ptrEAN13)
	{
		if (vetHashTable[nHash].ptrFirst == NULL)									// First of linked lkist?
		{	// It's the first of Linked List
			vetHashTable[nHash].ptrLast =
				vetHashTable[nHash].ptrFirst = new CHAVE_BUSCA;
		}
		// This product is not the first of the Linked List and will be the last
		else
			vetHashTable[nHash].ptrLast =
				vetHashTable[nHash].ptrLast->ptrNext = new CHAVE_BUSCA;
		// Have we memory for the New?
		if (vetHashTable[nHash].ptrLast == NULL)									// Is memory out?
			return false;															// Indicates memory out
		// Fill the data in the pointer last
		strcpy_s(vetHashTable[nHash].ptrLast->cEAN13, 14, ptrEAN13);
		vetHashTable[nHash].ptrLast->nPositional = nNextPosProduct;
		vetHashTable[nHash].ptrLast->cActive = 'A';									// Active product
		vetHashTable[nHash].ptrLast->ptrNext = NULL;								
		return true;																// Indicates OK
	}

	// Private Method for positional read 
	// Parameters: 
	//		Input: int nPositional - Product record positional
	//			   PRODUTO *ptrStProduto - Pointer of a structure to contais 
	//									   the product data
	//
	//		Output: Bool - True  - Read OK
	//					   False - fseek or read error
	bool ReadPositional(int nPositional, PRODUTO *ptrStProduto)
	{
		if (fseek(fdProduct, nPositional * sizeof(PRODUTO), SEEK_SET) !=0)			// Error?
			return false;															// Advise the error
		if (fread_s(ptrStProduto, sizeof(PRODUTO), sizeof(PRODUTO), 1, fdProduct) == 0)
			return false;
		return true;																// Read is OK
	}

	// Private Method for positional Write 
	// Parameters: 
	//		Input: int nPositional - Product record positional
	//			   PRODUTO *ptrStProduto - Pointer of a structure that contains 
	//									   the product data
	//
	//		Output: Bool - True  - Write OK
	//					   False - fseek or read error
	bool WritePositional(int nPositional, PRODUTO *ptrStProduto)
	{
		if (fseek(fdProduct, nPositional * sizeof(PRODUTO), SEEK_SET) != 0)			// Error?
			return false;															// Advise the error
		if (fwrite(ptrStProduto, sizeof(PRODUTO), 1, fdProduct) == 0)
			return false;
		return true;																// Write is OK
	}

public:
	// Public atributes and publc methods
	// Clalss Constructor

	// Parameters:
	//		Input: Bool *ptrStatud - Addres of a boolean on the object of class instanciator
	//								 to receive the status of the object creation
	//						
	//								True  - Indicates sucess creation
	//								False - Indicate probelm in the object of class creation
	clProduto(bool *ptrStatus)
	{
		int i;																		// Index and generic counter
		nNextPosProduct = 0;														// Positional of first product in the file
		
		// Make all the Hash with an empty Linked List
		for (i = 0; i < HASH_CODE_QTY; i++)
			vetHashTable[i].ptrFirst = NULL;										// Empty list of each Hash Code
		
		// Open file with mode "r+b" - Must Exist
		if (fopen_s(&fdProduct, FILE_PATH, "r+b") != 0)								// File not exist?
		{	// File not exist - lets open the file in "w+B" because is the
			// first time of program execution
			if (fopen_s(&fdProduct, FILE_PATH, "w+b") != 0)							// Fatal error?
			 
			{
				cout << "Open file Fatal Error!!!" << endl;
				ptrStatus = false;													// Indicates Fatal Error
				PAUSE;
				return;																// Out of Constructor
			}

			// Go out of constructor because is OK
			*ptrStatus = true;														// Indicates everything OK
			return;																	// Out of constructor
		} // File dont exist

		else
		{	// File exist
			// Let's read all the file and constructor the Linked List of each
			// Hash Code
			while (true)															// Infinite Loop
			{
				if (fread_s(&stProduct, sizeof(PRODUTO),
					sizeof(PRODUTO), 1, fdProduct) == 0)							// End of File?
				{
					*ptrStatus = true;												// Indicates OK	
					return;															// Go out of constructor
				}
				// Verify if the product is active
				if (stProduct.cActive != 'A')										// Not Active
				{
					nNextPosProduct++;												// Increase it's posição
					continue;														// Read next product Record
				}
				// Calculates the Hash and insert the search key int Hash Table
				if (!InsertKeyHashTable(HashCodeCalculation(stProduct.cEAN13),
					stProduct.cEAN13))												// Out of memory?
				{
					*ptrStatus = false;												// Indicates the error
					fclose(fdProduct);												//	File Closed
					return;															// Go out of constructor
				}
				// Success!!!
				nNextPosProduct++;													// Positional is incremented
			} // While True
		}
	} //  Constructor

	//Public Method that verify a product existance
	//	Parameters:
	//		Input: char *ptrEAN13  - Pointer to a string that contains a product code
	//			   PRODUTO *ptrStProduct - Pointer of a struct sended by the caller
	//									   that willl recive the product data if
	//									   exists.
	//
	//		Output: Bool - True  - Product exists and it's data is in a struct of
	//							   the method caller
	//					   False - Product does not exist
	bool VerifyExistanceOfProduct(char *ptrEAN13, PRODUTO *ptrStProduct)
	{
		int nHash = HashCodeCalculation(ptrEAN13);									
		CHAVE_BUSCA *ptrWork;														// Ponteiro de Trabalho
		
		// Loop of search
		for (ptrWork = vetHashTable[nHash].ptrFirst;								// Start with the first search key
			ptrWork;																// Currently search key or zero = false
			ptrWork = ptrWork->ptrNext)												// ptrWork is the next Key
		{	// We have a key pointed by ptrWork
			if (ptrWork->cActive != 'A')											// This key is excluded?
				continue;															// Go to the next key
			if (strcmp(ptrEAN13, ptrWork->cEAN13) == 0)								// Equals?
			{	// Is the same product code - Let's read to the caller pointer
				if (!ReadPositional(ptrWork->nPositional, ptrStProduct))			// Read error?
					return false;
				return true;														// Advise OK
			}
			return false;															// Advise not found
		} // For
		
	}

	// Public method for product insert
	//	Parameters:
	//		Input: PRODUTO *ptrStProduct - pointer to the struct that contains
	//									   all the product data
	//
	//		Output: Bool - True  - Insertion OK
	//					   False - Insertion error
	bool ProductInsertion(PRODUTO *ptrStProduct)
	{
		if (!InsertKeyHashTable(HashCodeCalculation(ptrStProduct->cEAN13),
			ptrStProduct->cEAN13))													// Out of memory
			return false;															// Advise the error
		if (!WritePositional(nNextPosProduct, ptrStProduct))						// Write error?
			return false;															// Advise the error?
		nNextPosProduct++;															// Forn the next Product
		return true;																// Advise everything OK
	}
};