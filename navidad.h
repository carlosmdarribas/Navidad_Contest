/*
**  navidad.h
**  ACM USAL Code Contest. "Navidad"
**
**  Copyright © 2018 Carlos Martin de Arribas. All rights reserved. (carlosmdarribas@usal.es)
**  Copyright © 2018 Gonzalo Martín González. All rights reserved.  (kiruhm@usal.es)
**
**  Universidad de Salamanca. USAL.
*/

/**
* Librerías del sistema necesarias
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <locale.h>

/*
* Macros
*/
#if _WIN32 || _WIN64
#	define CLEAR_COMMAND "cls"
#else
#	define CLEAR_COMMAND "clear"
#endif

/*
* Constantes del programa
*/
#define MAX_MENU_ELEMENTS_LENGHT 50
#define MAX_MENU_ELEMENTS 6
#define MENU_EXIT_KEY 5

#define MENU_HELP_KEY 6

#define MAX_TOY_NAME_LENGHT 25
#define EFFECTIVE_MAX_TOY_NAME_LENGHT 250
#define MAX_TOYS_ALLOWED 8
#define MIN_TOYS_ALLOWED 1

#define MAX_ERROR_REASON_LENGHT 200
#define MAX_MSG_LENGHT 100

/****************************************************************
*****************  PROTOTIPOS DE LAS FUNCIONES  *****************				
*****************************************************************/

// Funciones correspondientes a acciones globales.
void printMenuHeader();
void showMenu(const char (*options)[MAX_MENU_ELEMENTS_LENGHT], int numberOfOptions);															
int askForOption();																																
int makeDecisionByOption(int option_id, char (*toysList)[MAX_TOY_NAME_LENGHT], int *currentToysNum); 											
int isOptionCorrect(int min_number, int max_number, int userOption);																
void pressToClear();																															

// Funciones referentes a la 1ª opcion (Añadir juguetes).
int canAddToy(int currentToysNum, int max_toys);																								
int askForToyName(char *toyName, char (*toysList)[MAX_TOY_NAME_LENGHT]);																	
int addToy(char (*toysList)[MAX_TOY_NAME_LENGHT], int *toyPosition);																			
int askForAnotherToy();																															

//Funciones referentes a la 2ª opcion (Borrar juguetes).
int canDeleteToy(int currentToysNum);																									
int askForToyNameOrNum(char *toyName, char (*toysList)[MAX_TOY_NAME_LENGHT], int currentToysNum);										
int filter(char *toyName, char (*toysList)[MAX_TOY_NAME_LENGHT], int *currentToysNum);												
int searchAndDestroy(char *toyName, char (*toysList)[MAX_TOY_NAME_LENGHT], int *currentToysNum);										
int deleteToy(char (*toysList)[MAX_TOY_NAME_LENGHT], int *toyPosition);																	
int askForDeleteAnotherToy();

// Funcion correspondiente a la 3ª opcion (mostrar juguetes de la lista).
void printToys_Header();
int printToys(char (*toysList)[MAX_TOY_NAME_LENGHT], int currentToysNum);																

//Funciones correspondientes a la 4ª opcion (Cambiar juguetes).
int canChangeToy(int currentToysNum);																										
void askForToyNameOrNum_Change(char *toyName, char *toyNameChanged, char (*toysList)[MAX_TOY_NAME_LENGHT], int *currentToysNum, int option);
int change(char *toyName, char *toyNameChanged, char (*toysList)[MAX_TOY_NAME_LENGHT], int *currentToysNum);									
int changeToy(char (*toysList)[MAX_TOY_NAME_LENGHT], int toyPosition);																		
int askForChangeAnotherToy();

// Funciones correspondientes a la 5ª opcion (Enviar carta).
int canSendLetter(int currentToysNum, int min_toys);																							
void sendLetter(int currentToysNum, int *granted_toys, int *rejected_toys, int *granted_num, int *rejected_num);
void printFinalLetter(int option, char (*toysList)[MAX_TOY_NAME_LENGHT], int *toys_assigned, int num_toys_assigned);							

// Funciones correspondientes a la 6ª opcion (Ayuda).
void printHelp();

// Funciones complementarias
int get_randomNum(int min, int max);		
int checkIfStringAreEqual(char *str1, char *str2);																								
void getErrorReason(int errorNum, char *errorReason);																							
void printError(int error_num);			
void printGoodBye();																									

/****************************************************************
***************  IMPLEMENTACION DE LAS FUNCIONES  ***************				
*****************************************************************/

void printMenuHeader() {
	system(CLEAR_COMMAND); // Limpiamos la pantalla por si hay algo antes.

	printf("\n\n-------------------------------------------------------------------------\n");
	printf("                       Carta a los Reyes Magos                                \n");
	printf("-------------------------------------------------------------------------\n\n\n");
}


/** void showMenu(const char (*options)[MAX_MENU_ELEMENTS_LENGHT], int numberOfOptions) **
* ====================================================================================== *
* Mostrara e imprimira un menu. Devolvera un entero que correspondera al numero de la opcion elegida.
*
* El array constante de char corresponde al array que almacena las opciones. Definido en main.c
* numberOfOptions es un entero que indica el numero de opciones disponibles. 
*/
void showMenu(const char (*options)[MAX_MENU_ELEMENTS_LENGHT], int numberOfOptions){
	printMenuHeader();                                                                                                                       
	
	int i;
	printf("===================\n");
	for  (i = 0; i < numberOfOptions; i++) {
		if (i+1 == MENU_HELP_KEY) puts("\n");
		printf("%d) %s\n", i+1, *(options+i));
	}
	printf("===================\n");
}


/** int askForOption() **
* ===================== *
* Preguntara al usuario la opcion deseada y lo guardara en la variable optionID.
*
* Devuelve un entero que corresponde al ID de la opcion elegida.
*/
int askForOption() {
	int optionID;
	printf("\nIntroduzca la opción deseada: ");
	scanf("%d", &optionID);
	return optionID;
}


/** int makeDecisionByOption(int option_id, char (*toysList)[MAX_TOY_NAME_LENGHT], int *currentToysNum) **
* ====================================================================================================== *
* Realizara una accion segun el identificador de opcion que reciba como parÃametro.
*
* option_id es un entero que corresponde a la opcion elegida por el usuario y sobre la que debera realizarse una accion.
* El array de char corresponde al array que almacena los juguetes de la lista. Definido en main.c
* currentToysNum es un puntero a un entero que indica el numero de juguetes que hay actualmente en la lista.
*
* Devuelve un entero como flag de errores. 0 => Sin errores. !0 => Ha habido algun error.
*/
int makeDecisionByOption(int option_id, char (*toysList)[MAX_TOY_NAME_LENGHT], int *currentToysNum) {
	switch (option_id) {
		case 1:	// Añadir Juguete. 		
			do {
				int errornum = addToy(toysList, currentToysNum);
				if (errornum) return errornum;
			} while ((askForAnotherToy() == 1));
			break;

		case 2: // Eliminar juguetes 	
			do {
				int errornum = deleteToy(toysList, currentToysNum);
				if (errornum) return errornum;
			} while ((askForDeleteAnotherToy() == 1));
			break;

		case 3: // Mostrar carta 
			printToys_Header();

			int errornum = printToys(toysList, *currentToysNum);
			if (errornum) return errornum;
			break;

		case 4: // Cambiar juguetes 	
			do {
				int errornum = changeToy(toysList, *currentToysNum);
				if (errornum) return errornum;		
			} while ((askForChangeAnotherToy() == 1));
			break;

		case 5: // Enviar carta 		
			if (!canSendLetter(*currentToysNum, MIN_TOYS_ALLOWED)) return 107;
			break;

		case MENU_HELP_KEY: // Ayuda
			printHelp();
			break;

		default:
			// Si la opcion seleccionada no es correcta, se lo indico al usuario.
			if (isOptionCorrect(0, MAX_MENU_ELEMENTS, option_id))
				return 102;
			break;
	}
	return 0;
}

/** int isOptionCorrect(int min_number, int max_number, int userOption) **
* ====================================================================== *
* Comprueba si la opcion que ha elegido el usuario esta comprendida entre las posibles.
*
* min_number es un entero que corresponde al numero MENOR que se puede usar.
* maX_number es un entero que corresponde al numero MAYOR que se puede usar.
* userOption es un entero que indica la opcion seleccionada por el usuario
*
* Devuelve un entero como flag de errores. 0 => Sin errores (opcion correcta). 1=> Ha habido algun error (opcion incorrecta).
*/
int isOptionCorrect(int min_number, int max_number, int userOption) {
	return ((userOption <= min_number||userOption > max_number));
}


/** void pressToClear() **
* ====================== *
* Muestra un mensaje y espera hasta que el usuario pulsa intro para limpiar la pantalla.
*
* Sin parámetros. Sin retornos.
*/
void pressToClear() {
	printf("Presione la tecla Intro para continuar.\n");
	int c;
	while ((c = getchar()) != '\n' && c != EOF);	//Limpiamos el buffer.
	getchar();
	system(CLEAR_COMMAND);
}


/****************************************************************
************************  CASE 1  *******************************					
*****************************************************************/


/** int canAddToy(int currentToysNum, int max_toys) **
* ================================================== *
* Comprueba si se puede añadir un juguete. Se podrá cuando el número actual sea menor al límite.
*
* currentToysNum es un entero que indica el numero de juguetes que hay actualmente en la lista.
* max_toys es un entero que indica el numero maximo de juguetes que puede haber en la lista.
*
* Devuelve un entero como flag de errores. 1 => Puede añadirse el juguete. 0 => No se puede añadir el juguete, la lista esta completa.
*/
int canAddToy(int currentToysNum, int max_toys) {
	return (currentToysNum < max_toys);
}

/** int askForToyName(char *toyName, char (*toysList)[MAX_TOY_NAME_LENGHT]) **
* ========================================================================== *
* Pide al usuario por teclado una cadena de texto (el juguete a añadir).
*
* El array de char toysList corresponde al array que almacena los juguetes de la lista. Definido en main.c
* toyName es un puntero a un vector de char, donde se almacenara el string introducido por el usuario.
* 
* Devuelve un entero como flag de errores. 0 => Sin errores. !0 => Ha habido algun error.
*/
int askForToyName(char *toyName, char (*toysList)[MAX_TOY_NAME_LENGHT]) {	
	printf("\nIntroduzca el nombre del juguete: ");
	getchar();

	char localToyName[EFFECTIVE_MAX_TOY_NAME_LENGHT];
	fgets(localToyName, EFFECTIVE_MAX_TOY_NAME_LENGHT-1, stdin);
	
	//Compruebo si la longitud del string introducido es mayor que la maxima. Si lo es, devuelvo error 103
	if (strlen(localToyName) > MAX_TOY_NAME_LENGHT)
		return 103;

	//Conversion de la palabra introducida a letras minusculas con la primera letra mayuscula. Ej: meSA -> Mesa 
	int i;
	localToyName[0]= toupper(localToyName[0]);
	for(i=1; i<strlen(localToyName); i++)
		localToyName[i] = tolower(localToyName[i]);
		
	//Verifico si el nombre del juguete (toyName) ya aparecia en la lista. Devuelve error 106 si asi es.
	for(i = 0; i < strlen(localToyName); i++){
		if (!strcmp(localToyName,toysList[i]))
			return 106;
	}

	// Si ha cumplido todas las condiciones para que sea correcto, se "copia" al string original recibido como parámetro
	strcpy(toyName, localToyName);
	return 0;
}


/** int addToy(char (*toysList)[MAX_TOY_NAME_LENGHT], int *toysNumber) **
* ===================================================================== *
* Añadira un "juguete" que preguntara al usuario (mediante la funcion askForToyName), al array principal "toyList", si no ha habido errores.
*
* El array de char toysList corresponde al array que almacena los juguetes de la lista. Definido en main.c
* toysNumber es un puntero a un entero que indica el numero de juguetes que hay actualmente en la lista.
* toyName es un array de caracteres donde se almacenara el string del juguete que se quiere añadir.
*
* Devuelve un entero como flag de errores. 0 => Sin errores. !0 => Ha habido algun error.
*/
int addToy(char (*toysList)[MAX_TOY_NAME_LENGHT], int *toysNumber) { //CASE 1
	//Compruebo si el numero de juguetes que hay en la lista es el maximo. Si es así, devuelvo error 100.
	if (!canAddToy(*toysNumber, MAX_TOYS_ALLOWED))
		return 100;

	// Pido al usuario el nombre del juguete
	char toyName[MAX_TOY_NAME_LENGHT];
	int error = askForToyName(toyName,toysList);

	if (error)
		return error;
		
	// Asigno a toysList en el ultimo index disponible el nombre del juguete que se le pidio al usuario.
	strcpy(toysList[(*toysNumber)++], toyName); // Asigno a toysList en el último index disponible el nombre del juguete que se le pidió al usuario.

	return 0;
}


/** int askForAnotherToy() **
* ========================= *
* Pregunta al usuario si desea añadir otro juguete.
*
* Si el usuario introduce si (S/s) devolvera un 1. 
* Si introduce no (N/n) o cualquier otro valor, devolvera un 0.
*/
int askForAnotherToy() {
	char answer;
	printf("¿Desea añadir otro juguete? S/n: ");
	scanf("%c", &answer);

	return (answer == 'S' || answer == 's');
}



/****************************************************************
************************  CASE 2  *******************************					
*****************************************************************/

/** int canDeleteToy(int currentToysNum) **
* ======================================= *
*
* Comprueba si se puede eliminar un juguete.
*
* currentToysNum es un entero que indica el número de juguetes que hay actualmente en la lista.
*
* Devuelve un entero como flag de errores. 1 => Puede eliminarse el juguete. 0 => No se puede eliminar el juguete, la lista esta vacia.
*/
int canDeleteToy(int currentToysNum) {
	return (currentToysNum != 0);
}


/** int askForToyNameOrNum(char *toyName, char (*toysList)[MAX_TOY_NAME_LENGHT], int currentToysNum) **
* =================================================================================================== *
*
* Pide al usuario por teclado una cadena de texto o la posicion que ocupa un juguete en la lista.
*
* toyName es un puntero a un vector de char, donde se almacenara el string o el entero introducido por el usuario.
* El array de char toysList corresponde al array que almacena los juguetes de la lista. Definido en main.c
* currentToysNum es un entero que indica el número de juguetes que hay actualmente en la lista.
*
* Devuelve un entero como flag de errores. 0 => Sin errores. !0 => Ha habido algÃºn error.
*/
int askForToyNameOrNum(char *toyName, char (*toysList)[MAX_TOY_NAME_LENGHT], int currentToysNum) {	
	printToys(toysList, currentToysNum); //Imprimo primero la lista
	
	printf("\n\nIntroduzca el nombre del juguete o su número de posición en la lista: ");
	getchar();
	fgets(toyName, MAX_TOY_NAME_LENGHT-1, stdin);
	strtok(toyName, "\n"); // Elimino el último \n almacenado, si lo hubiera.

	return 0;
}


/** int filter(char *toyName, char (*toysList)[MAX_TOY_NAME_LENGHT], int *currentToysNum) **
* ======================================================================================== *
*
* Comprueba que el elemento introducido sea correcto y aplica cambios en base al tipo de dato introducido. 
*  +Si el dato introducido es la posicion del juguete (int), toyName tomara el valor de dicho entero.
*  +Si el dato introducido es el nombre del juguete (string), primero se le asiganara a la matriz palabra los 
*   elementos de la cadena de caracteres para despues compararla con la cadena toysList y dar a toyName un entero en base a la posicion de la palabra en toysList.
* 
* toyName es un puntero a un vector de char, donde se almacenara el string o el entero introducido por el usuario.
* El array toyList de char corresponde al array que almacena los juguetes de la lista. Definido en main.c
* currentToysNum es un puntero a un entero que indica el numero de juguetes que hay actualmente en la lista.
* El entero control se utiliza en el bucle for que comprueba que la palabra introducida pertenece a la lista. Si la palabra pertenece, control tomara el valor 1 para no devolver el error 104.
* palabra es una matriz de una unica fila que almacenara los caracteres de la palabra introducida en un solo string, para despues comparar los strings de toysList con palabra.
*
* Devuelve un entero como flag de errores. 0 => Sin errores. !0 => Ha habido algun error.
*/
int filter(char *toyName, char (*toysList)[MAX_TOY_NAME_LENGHT], int *currentToysNum) {
	int i, control=0;
	char palabra [MAX_TOY_NAME_LENGHT]={'\0'};	
	
	// Compruebo que la longitud de toyName es 1 y que no es un numero entre 0 y 9. Si no lo es, devuelvo error 104.
	if (strlen(toyName) == 1 && ((*toyName) <= (int)'/' || (*toyName) >= (int)':')) {
		return 104; 
	}
		
	// Transforma el valor en ASCII de la posicion del juguete en un numero entero, siempre que sea una posicion comprendida entre los enteros 0 y 9.	
	for(i=0;i<11;i++){	
		if (*toyName == i + 48)
			*toyName = i;	
	}
		
	/* Compruebo que:
		1. el entero introducido es mayor que el numero de elementos de la lista.
	    2. es menor que el numero minimo de elementos de la lista.
	    3. o la posicion introducida es 0 o 9.
	 */
	if ((((*toyName) > (*currentToysNum) || (*toyName) < MIN_TOYS_ALLOWED)) && (strlen(toyName) == 1 || (*toyName==0 || *toyName==9)))
		return 105; 
	
	// If que se activa cuando la longitud de toyName es mayor de uno, es decir, una cadena (o un numero mayor que 8, pero esa posibilidad daria error 105, en el if de arriba).
	if (strlen(toyName) > 1){
		
		//Bucle for que asigna a palabra los caracteres de toyName.
		for(i = 0; i<strlen(toyName); i++)
			palabra[i]=toyName[i];
		
		//Conversion de la palabra introducida a letras minusculas con la primera letra mayuscula. Ej: meSA -> Mesa 
		for(i = 1; i < strlen(toyName); i++)
			palabra[i] = tolower(palabra[i]);
		palabra[0]= toupper(palabra[0]);		
		
		//Bucle for que comprueba que palabra coincida con alguno de los juguetes de toysList
		for(i = 0; i < *currentToysNum; i++){
			if (checkIfStringAreEqual(palabra,toysList[i])){	// Si las palabras SON iguales. 
				*toyName = i+1;
				control = 1;
				break;
			}
		}
		if (control==0)
			return 104; 
	}
	return 0;
}

/** int searchAndDestroy(char *toyName, char (*toysList)[MAX_TOY_NAME_LENGHT], int *currentToysNum) **
* ================================================================================================== *
*
* Elimina el elemento de la lista que se corresponde con la posición dada o con la cadena de caracteres dada y coloca el resto de elementos.
* 
* toyName es un puntero a un vector de char, donde se almacenará el string o el entero introducido por el usuario.
* El array toyList de char corresponde al array que almacena los juguetes de la lista. Definido en main.c
* currentToysNum es un puntero a un entero que indica el nÃºmero de juguetes que hay actualmente en la lista.
*
* Devuelve un entero como flag de errores. 0 => Sin errores. !0 => Ha habido algun error.
*/
int searchAndDestroy(char *toyName, char (*toysList)[MAX_TOY_NAME_LENGHT], int *currentToysNum) {
	//Llamada a la funcion filter
	int error = filter(toyName, toysList, currentToysNum);
	
	if (error)
		return error;
	
	//Bucle for que mueve los elementos de la lista un lugar hacia atras desde la posicion introducida hasta el final y vacia el ultimo elemento.
	for(;*toyName<=*currentToysNum;(*toyName)++){
		*(toysList[(*toyName)-1]) = '\0';
		if (*toyName != *currentToysNum)	
			strcpy(toysList[(*toyName)-1], toysList[*toyName]);
		}

	return 0;
}

/** int deleteToy(char (*toysList)[MAX_TOY_NAME_LENGHT], int *toyPosition) **
* ========================================================================= *
*
* Eliminara un "juguete" que preguntará al usuario (mediante la función askForToyNameOrNum), del array principal "toyList", si no ha habido errores.
*
* El array de char toysList corresponde al array que almacena los juguetes de la lista. Definido en main.c
* toyPosition es un puntero a un entero que indica el numero de juguetes que hay actualmente en la lista.
* toyName es un vector de char, donde se almacenará el string o el entero introducido por el usuario.
*
* Devuelve un entero como flag de errores. 0 => Sin errores. !0 => Ha habido algun error.
*/
int deleteToy(char (*toysList)[MAX_TOY_NAME_LENGHT], int *toyPosition) { 
	char toyName[MAX_TOY_NAME_LENGHT]={'\0'};

	if (!canDeleteToy(*toyPosition))
		return 101;
	
	// Pido al usuario el nombre o la posicion del juguete
	askForToyNameOrNum(toyName, toysList, *toyPosition);
	
	int error = searchAndDestroy(toyName, toysList, toyPosition);
	if (error)
		return error;

	// Si no ha habido ningun error, reduzco en 1 el número de posiciones.
	(*toyPosition)--;
	return 0;
}

/** int askForDeleteAnotherToy() ** 
* =============================== *
*
* Pregunta al usuario si desea eliminar otro juguete.
*
* Si el usuario introduce si (S/s) devolverá un 1. Si introduce no (N/n), devolverá un 0
*/
int askForDeleteAnotherToy() {
	char answer;
	printf("¿Desea eliminar otro juguete? S/n: ");
	scanf("%c", &answer);

	return (answer == 'S'||answer == 's') ? 1 : 0;
}



/****************************************************************
************************  CASE 3  *******************************					
*****************************************************************/	

void printToys_Header() {
	system(CLEAR_COMMAND); // Primero se limpia la pantalla. Luego se ve el código.
	printf("\n  __^__                                      __^__	");
	printf("\n ( ___ )------------------------------------( ___ )	");
	printf("\n  |   |                                      |   |	");
	printf("\n  |   |       Carta a los Reyes Magos	     |   |	    ");
	printf("\n  |___|                                      |___|	");
	printf("\n (_____)------------------------------------(_____)   \n\n");
}

/** int printToys(char (*toysList)[MAX_TOY_NAME_LENGHT], int currentToysNum) **
* =========================================================================== *
*
* Imprimira la lista de juguetes (el array toysList)
*
* El array de char toysList corresponde al array que almacena los juguetes de la lista. Definido en main.c
* currentToysNum es un puntero a un entero que indica el nÃºmero de juguetes que hay actualmente en la lista.
*
* Devuelve un entero como flag de errores. 0 => Sin errores. !0 => Ha habido algun error.
*/	
int printToys(char (*toysList)[MAX_TOY_NAME_LENGHT], int currentToysNum) { 	
	//If que provoca que si la lista esta vacia, acabe mandando el error 101.
	if (currentToysNum == 0)
		return 101;

	printf("Carta a los Reyes Magos: \n");
	int i;
	for (i = 0; i < currentToysNum; i++) {
		printf("  %d) - %s",i+1, *(toysList+i));
	}
	printf("\n\n");

	return 0;
}



/****************************************************************
************************  CASE 4  *******************************					
*****************************************************************/

/* int canChangeToy(int currentToysNum) **
* ====================================== *
*
* Comprueba si se puede cambiar un juguete por otro.
*
* currentToysNum es un entero que indica el numero de juguetes que hay actualmente en la lista.
*
* Devuelve un entero como flag de errores. 1 => Puede cambiarse el juguete. 0 => No se puede cambiar el juguete, la lista está vacía.
*/
int canChangeToy(int currentToysNum) {
	return (currentToysNum != 0);
}

/** void askForToyNameOrNum_Change(char *toyName, char *toyNameChanged, char (*toysList)[MAX_TOY_NAME_LENGHT], int *currentToysNum, int option) **
* ============================================================================================================================================== *
* 
* Pide al usuario por teclado la cadena de texto o la posicion que ocupa el juguete que desea cambiar (option = 0) 
* y despues pide la cadena de texto o la posicion que ocupa el juguete por el que desea cambiar el primero (option = 1).
*
* toyName es un puntero a un vector de char definido en la funcion changeToy, donde se almacenara el string o el entero que hace referencia al juguete que se quiere cambiar.
* toyNameChanged es un puntero a un vector de char definido en la funcion changeToy, donde se almacenara el nombre del juguete por el que se quiere cambiar el primero.
* El array de char toysList corresponde al array que almacena los juguetes de la lista. Definido en main.c
* currentToysNum es un entero que indica el numero de juguetes que hay actualmente en la lista.
* option es un puntero a un entero definido en la funcion changeToy que, dependiendo de si toma valor 1 o 0, realizara el if o else if.
* 
*/
void askForToyNameOrNum_Change(char *toyName, char *toyNameChanged, char (*toysList)[MAX_TOY_NAME_LENGHT], int *currentToysNum, int option) {
	if (option == 0) {
		printToys(toysList, *currentToysNum);
		printf("\n\nIntroduzca el nombre del juguete que desea cambiar o su número de posición en la lista: ");
		getchar();
		fgets(toyName, MAX_TOY_NAME_LENGHT-1, stdin);
	} else if (option == 1) {
		printf("\nIntroduzca el nombre del juguete por el que desea cambiar el anterior: ");
		fgets(toyNameChanged, MAX_TOY_NAME_LENGHT-1, stdin);
	}
	strtok(toyName, "\n"); // Elimino el último \n almacenado, si lo hubiera.
} 

/** int change(char *toyName, char *toyNameChanged, char (*toysList)[MAX_TOY_NAME_LENGHT], int *currentToysNum) ** 
* ============================================================================================================== *
* 
* Cambia el elemento seleccionado en primer lugar por el segundo.
* 
* toyName es un puntero a un vector de char definido en la funcion changeToy, donde se almacenara el string o el entero que hace referencia al juguete que se quiere cambiar.
* toyNameChanged es un puntero a un vector de char definido en la funcion changeToy, donde se almacenara el nombre del juguete por el que se quiere cambiar el primero.
* El array toysList de char corresponde al array que almacena los juguetes de la lista. Definido en main.c
* currentToysNum es un puntero a un entero que indica el nÃºmero de juguetes que hay actualmente en la lista.
* El array word de caracteres es un array de una unica fila que almacenara los caracteres de toyNameChanged para despues comparar en toysList y sustituir el elemento indicado 
*
* Devuelve un entero como flag de errores. 0 => Sin errores. !0 => Ha habido algun error.
*/
int change(char *toyName, char *toyNameChanged, char (*toysList)[MAX_TOY_NAME_LENGHT], int *currentToysNum){
	int i;
	char word[1][MAX_TOY_NAME_LENGHT]={'\0'};
	
	//Bucle for que asigna a word los caracteres de toyNameChanged.
	for(i=0;i<strlen(toyNameChanged);i++)
		word[0][i]=toyNameChanged[i];
		
	//Conversion de la palabra introducida a letras minusculas con la primera letra mayuscula. Ej: mesa -> Mesa 
	for(i=1;i<strlen(toyNameChanged);i++)
		word[0][i] = tolower(word[0][i]);
	word[0][0]= toupper(word[0][0]);
	
	//Bucle for que verifica si toyNameChanged ya aparecia en la lista y manda error 106 si asi era.
	for(i=0;i<strlen(toyNameChanged);i++){
		if (!strcmp(word[0],toysList[i]))
			return 106;
	}
	
	//Copia la cadena almacenada en word (el juguete por el que se quiere cambiar al previamente introducido) en el elemento de toysList que corresponde al juguete que se quiere cambiar.	
	strcpy(toysList[(*toyName)-1], word[0]);
	
	return 0;
}

/** int changeToy(char (*toysList)[MAX_TOY_NAME_LENGHT], int toyPosition) **
* ======================================================================== *
*
* Cambiara un "juguete" que preguntará al usuario (mediante la función askForToyNameOrNum_Change), del array principal "toyList", si no ha habido errores.
*
* El array de char toysList corresponde al array que almacena los juguetes de la lista. Definido en main.c
* toyPosition es un puntero a un entero que indica el numero de juguetes que hay actualmente en la lista.
* toyName es un vector de char donde se almacenara el string o el entero que hace referencia al juguete que se quiere cambiar.
* toyNameChanged un vector de char donde se almacenara el nombre del juguete por el que se quiere cambiar el primero.
* option es una variable de control que afecta dentro de la funcion askForToyNameOrNum_Change.
*
* Devuelve un entero como flag de errores. 0 => Sin errores. !0 => Ha habido algún error.
*/	
int changeToy(char (*toysList)[MAX_TOY_NAME_LENGHT], int toyPosition) {	
	char toyName[MAX_TOY_NAME_LENGHT]={'\0'}, toyNameChanged[MAX_TOY_NAME_LENGHT]={'\0'};;	
	if (!canDeleteToy(toyPosition))
		return 101;	
	
	// Pido al usuario el nombre o la posición de los juguetes
	askForToyNameOrNum_Change(toyName, toyNameChanged, toysList , &toyPosition, 0);

	// Compruebo que el nombre introducido es correcto.
	int error;
	error = filter(toyName, toysList, &toyPosition);
	if (error)
		return error;

	askForToyNameOrNum_Change(toyName, toyNameChanged, toysList , &toyPosition, 1);
	
	error = change(toyName,toyNameChanged, toysList, &toyPosition);
	if (error)
		return error;

	return 0;	
}

/** int askForChangeAnotherToy() **
* =============================== *
*
* Pregunta al usuario si desea cambiar otro juguete.
*
* Si el usuario introduce si (S/s) devolverá un 1. Si introduce no (N/n), devolvera un 0
*/
int askForChangeAnotherToy() {
	char answer;
	printf("¿Desea cambiar otro juguete? S/n: ");
	scanf("%c", &answer);
	system(CLEAR_COMMAND);

	return (answer == 'S'||answer == 's') ? 1 : 0;
}



/****************************************************************
************************  CASE 5  *******************************					
*****************************************************************/

/** int canSendLetter(int currentToysNum, int min_toys) **
* ====================================================== *
* 
* Comprueba si puede enviarse la carta a los Reyes Magos. 
*
* currentToysNum es un entero que indica el numero de juguetes que hay actualmente en la lista.
*
* Devuelve un entero como flag de errores. 1 => Puede enviarse la carta. 0 => No se puede enviar la carta, la lista no tiene juguetes.
*/
int canSendLetter(int currentToysNum, int min_toys) {
	return (currentToysNum >= min_toys);
}


/** void sendLetter(int currentToysNum, int *granted_toys, int *rejected_toys, int *granted_num, int *rejected_num) **
* ================================================================================================================== *
*
* Enviara la carta a los Reyes Magos, decidiendo aleatoriamente los juguetes de toysList que seran concedidos y los que no.
*
* currentToysNum es un entero que indica el numero de juguetes que hay actualmente en la lista.
*
* Devuelve un entero como flag de errores. 0 => Sin errores. !0 => Ha habido algÃºn error.
*/
void sendLetter(int currentToysNum, int *granted_toys, int *rejected_toys, int *granted_num, int *rejected_num) {
	int i;
	for (i = 0; i < currentToysNum; i++) {
		if (get_randomNum(0, 2)) // 0: Denegado. 1: Concedido.
			granted_toys[(*granted_num)++] = i; // Concedido
		else
			rejected_toys[(*rejected_num)++] = i; // Denegado
	}
}

/** void printFinalLetter(int option, char (*toysList)[MAX_TOY_NAME_LENGHT], int *toys_assigned, int num_toys_assigned) **
* ====================================================================================================================== *
*
* Imprimira la decision final de los Reyes Magos (juguetes concedidos y denegados en sendLetter)
*
* option corresponde a un entero que indica si se tratan de juguetes concedidos (0) o juguetes denegados (1)
* El array de char corresponde al array que almacena los juguetes de la lista. Definido en main.c
* toys_asigned corresponde a un vector de enteros que almacena el ID de los juguetes concedidos o denegados con respecto a toysList. Definido en main.c
* num_toys_assigned corresponde a un entero que indica el numero de juguetes (concedidos o denegados), segun option.
*
*/
void printFinalLetter(int option, char (*toysList)[MAX_TOY_NAME_LENGHT], int *toys_assigned, int num_toys_assigned) {
	char message[MAX_MSG_LENGHT];
	switch (option) {
		case 0: strcpy(message, ((num_toys_assigned != 0) ? "En Envío" : "Los Reyes Magos no van a enviar juguetes ¯\\_(ツ)_/¯")); break;
		case 1: strcpy(message, ((num_toys_assigned != 0) ? "Denegados" : "¡Los Reyes Magos conceden todos los regalos!")); break;
	}
	printf("%s\n", message);

	int i;
	for(i = 0; i < num_toys_assigned; i++) {
		printf("    %s\n", toysList[*(toys_assigned+i)]);
	}

	printf("\n\n");
}


/****************************************************************
******************  CASE 6 - MENU_HELP_OPT  *************************					
*****************************************************************/
void printHelp() {
	system(CLEAR_COMMAND);
	printf("####################################################\n");
	printf("###############  Ayuda del programa  ###############\n");
	printf("####################################################\n\n\n");

	printf("El programa se encarga de guardar los regalos que se les va a enviar a los reyes magos\n");
	printf("El programa deberá mostrar un menú al usuario que permita hacer la carta a los reyes magos con el siguiente aspecto:\n");

	printf("\t1) Añadir juguete\n");
	printf("\t2) Eliminar juguetes\n");
	printf("\t3) Mostrar carta\n");
	printf("\t4) Cambiar juguetes\n");
	printf("\t5) Enviar carta\n\n");

	
	printf("Como máximo se podrán pedir 8 juguetes, el nombre de los juguetes no puede exceder los 25 caracteres.\n");
	printf("No se pueden añadir juguetes si ya hay 8 juguetes en la carta.\n");
	printf("No se pueden eliminar juguetes si no hay juguetes en la carta.\n");
	printf("Al mostrar la carta se mostraran los juguetes, por ejemplo:\n");

	printf("\tYoyó \n");
	printf("\tPlay Station 4\n");
	printf("\tScalextric\n");
	printf("\tDomino\n");

	printf("Cuando se deseen cambiar los juguetes, se preguntará que juguete quiere cambiar de su carta, si no existe, se volverá a mostrar la carta se volverá a preguntar.\n");
	printf("Mo se harán distinciones entre mayúsculas y minúsculas.\n");

	printf("Tras introducir un juguete que está en la carta se pedirá que escriba un nuevo juguete y se pondrá en el lugar que le correspondía al antiguo.\n");


	printf("Enviar la carta implica enviársela a los Reyes Magos, que decidirán si conceden el regalo o no, haciendo una lista de los regalos que conceden y los que no.\n");
	printf("En caso de que no haya regalos no se mostraran estas listas, siguiendo el ejemplo anterior:\n\n");

	printf("·En envio:\n");
	printf("\tDomino\n");
	printf("\tScalextric\n");

	printf("·Denegados:\n");
	printf("\tYoyó\n");
	printf("\tPlay Station 4\n\n");

	printf("Tras mostrar las listas el programa mostrara un mensaje deseando felices fiestas y acabara.\n");
	printf("Se estará solicitando una opción hasta que el usuario introduzca la opción de enviar carta.\n");
	printf("\n\n");
}

/****************************************************************
*******************  FUNCIONES COMPLEMENTARIAS  *****************				
*****************************************************************/

/** int get_randomNum(int min, int max) ** 
* ====================================== *
*
* Obtendra un numero aleatorio entre min y max.
*
* min es un entero que corresponde al numero mas pequeño que puede escoger aleatoriamente.
* max es un entero que corresponde al numero mayor que puede escoger aleatoriamente.
*
* Devuelve un numero aleatorio entre los 2 limites dados.
*/
int get_randomNum(int min, int max) {
	return (rand() % max) + min;
}

/** checkIfStringAreEqual(char *str1, char *str2) **
* ================================================ *
*
* Comprueba si 2 strings son iguales.
*
* str1 es el primer string
* str2 es el segundo string
*
* Devuelve un entero como flag: 0: distinto. 1: iguales.
*/
int checkIfStringAreEqual(char *str1, char *str2) {
	int i;
	for (i = 0; i < strlen(str1); i++) {
		if (str1[i] != str2[i])
			return 0;
	}
	return 1;
}


/** void getErrorReason(int errorNum, char *errorReason) **
* ======================================================= *
* 
* Asigna al string pasado como parámetro el error correspodiente al código de error.
*
* error_num es el código de error.
* errorReason es un puntero a char que contendrá el mensaje de error.
*
* ================================================================================================
*  Los códigos de error están también descritos en la documentación | Documentacion/Codigos_Error
* ================================================================================================
*
*
* Codigo de error 		Causa
*      100				No se pueden aÃ±adir mas juguetes, se ha superado el limite.
* 	   101 				No hay juguetes en la carta.
* 	   102 				Opcion incorrecta seleccionada.
* 	   103				Longitud del juguete > permitida.
* 	   104				Juguete	no perteneciente a la lista.
* 	   105				Posicion no encontrada.
* 	   106				Juguete ya perteneciente a la lista.
*/
void getErrorReason(int errorNum, char *errorReason) {
	switch (errorNum) {
		case 100:
			strcpy(errorReason, "Se ha alcanzado el límite de juguetes."); break;

		case 101:
			strcpy(errorReason, "No hay juguetes en la carta a los Reyes Magos."); break;

		case 102:
			strcpy(errorReason, "La opción seleccionada no es correcta. Por favor, inténtelo de nuevo."); break;

		case 103:
			sprintf(errorReason, "El juguete introducido tiene más de %d caracteres.", MAX_TOY_NAME_LENGHT); break;
			
		case 104:
			strcpy(errorReason, "El juguete introducido no pertenece a la lista. Por favor, inténtelo de nuevo."); break;
		
		case 105:
			strcpy(errorReason, "La posición introducida no aparece en la lista. Por favor, inténtelo de nuevo."); break;
		
		case 106:
			strcpy(errorReason, "El juguete ya se encontraba en la lista. Por favor, inténtelo de nuevo."); break;	
		
		case 107:
			strcpy(errorReason, "La carta no ha podido ser enviada ya que esta vacía."); break;	
	}
}


/** void printError(int error_num) ** 
* ================================= *
* 
* Muestra en pantalla el error correspondiente al código introducido.
*
* error_num es un entero que indica el código de error.
*/
void printError(int error_num) {
	char errorName[MAX_ERROR_REASON_LENGHT];
	// Llamo a la funcion que almacena en errorName el string que define el error.
	getErrorReason(error_num, errorName); 

	// Imprimo el codigo de error y el error antes obtenido.
	printf("\nError %d: %s\n\n\n", error_num, errorName);
}

/** void printGoodBye ** 
* ================================= *
* 
* Imprime el mensaje de despedida.
*/
void printGoodBye() {
	printf("  \n\n\n  #######                                                 #######                                            ###  ### ");
 	printf("  \n  #        ######  #       #   ####   ######   ####       #        #  ######   ####   #####    ##     ####   ###  ### ");
 	printf("  \n  #        #       #       #  #    #  #       #           #        #  #       #         #     #  #   #       ###  ### ");
 	printf("  \n  #####    #####   #       #  #       #####    ####       #####    #  #####    ####     #    #    #   ####    #    #  ");
 	printf("  \n  #        #       #       #  #       #            #      #        #  #            #    #    ######       #           ");
 	printf("  \n  #        #       #       #  #    #  #       #    #      #        #  #       #    #    #    #    #  #    #  ###  ### ");
	printf("  \n  #        ######  ######  #   ####   ######   ####       #        #  ######   ####     #    #    #   ####   ###  ### ");
	printf("\n");
}
