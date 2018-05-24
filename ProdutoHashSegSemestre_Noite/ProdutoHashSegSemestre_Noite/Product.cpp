// Stock Control and product managemant
// FATEC - MC - JVZ - 09/21/2017 - Version 0.0

#include "Programa.h"

//
// program entry point
//

void main(void)
{
	bool flagStatus;													// Status of program construction
	char cOperOption,													// To receive operator option
		cWork[150];														// For the sprintf_S (sting edition)
	PRODUTO stProduct;													// Struture to contains a product data
	SYSTEMTIME stTime;													// To receive system time
	BRASIL;																// To accept portuguese language

	//
	// Instanciate the product class and create an object
	clProduto objProduct(&flagStatus);
	if (!flagStatus)														// Construction error?
	{
		cout << "Problema na construção do Programa!!!!" << endl;
		PAUSE;
		return;															// Go back to operating system
	}
	// Succes!!!
	// Infinite Loop
	while (true)
	{
		CLEAR_VIDEO;
		GetLocalTime(&stTime);											// Get system time
		sprintf_s(cWork, sizeof(cWork),
			"FATEC - MC - Controle de Estoque %02d/%02d/%04d %02d:%02d",
			stTime.wDay, stTime.wMonth, stTime.wYear, stTime.wHour, 
			stTime.wMinute);
		cout << "\n\t" << cWork << endl;
		
		// Operators Menu
		cout << INSERT_NEW_PRODUCT << " - Inserir um novo produto" << endl;
		cout << DELETE_PRODUCT << " - Excluir produto existente" << endl;
		cout << SHOW_PRODUCTS_DATA << " - Mostrar os dados de um produto" << endl;
		cout << PRODUCT_ADD << " - Dar entrada em um produto existente" << endl;
		cout << PRODUCT_SUBTRACT << " - Dar baixa em um produto existente" << endl;
		cout << PROGRAM_EXIT << " - Sair do Programa" << endl;
		cout << "\n\tSelecione: ";
		cin >> cOperOption;												// Get operator selection
		cOperOption = toupper(cOperOption);								// Convert to Upper case
		switch (cOperOption)											// Evaluate operator selection
		{
			case INSERT_NEW_PRODUCT:
				break;													// Go back to operator's menu
			case DELETE_PRODUCT:
				break;													// Go back to operator's menu
			case SHOW_PRODUCTS_DATA:
				break;													// Go back to operator's menu
			case PRODUCT_ADD:
				break;													// Go back to operator's menu
			case PRODUCT_SUBTRACT:
				break;													// Go back to operator's menu
			case PROGRAM_EXIT:
				break;													// Go back to operator's menu
			default:
				cout << "Digite uma opção válida!!!" << endl;
					PAUSE;
		} // Switch
	} // While True
} // Main