/*
**  main.c
**  ACM USAL Code Contest. "Navidad"
**
**  Copyright © 2018 Carlos Martin de Arribas. All rights reserved. (carlosmdarribas@usal.es)
**  Copyright © 2018 Gonzalo Martín González. All rights reserved.  (kiruhm@usal.es)
**
**  Universidad de Salamanca. USAL.
*/

#include "navidad.h"

int main(void) {
	if (!setlocale(LC_ALL, "spanish"))
		setlocale(LC_ALL, "es_ES.UTF-8");
	// Las constantes estan definidas en el header.
	// Array constante que contiene las opciones del menu.
	const char menu_options[MAX_MENU_ELEMENTS][MAX_MENU_ELEMENTS_LENGHT] = {"Añadir Juguete", "Eliminar Juguete", "Mostrar Carta", "Cambiar Juguetes", "Enviar Carta", "== Ayuda =="};

	// Array que almacenara la lista de juguetes. Servira como DB a efectos practicos.
	char toysList[MAX_TOYS_ALLOWED][MAX_TOY_NAME_LENGHT];

	// Arrays que contendran el index en toyLists de los juguetes concedidos y los denegados.
	int toys_granted[MAX_TOYS_ALLOWED] = {0};
	int toys_rejected[MAX_TOYS_ALLOWED] = {0};

	// Entero que indica el numero de juguetes introducidos por el usuario.
	int userInpToys = 0;
	
	// Entero al que se le asigna el valor del error.
	int error_num=0;

	// Entero que indica la opcion que se seleccionara. Se comprobara el bucle en base a el.
	int selected_option = 0;
	do {
		// Muestro el menu de opciones.
		showMenu(menu_options, MAX_MENU_ELEMENTS);

		// Pregunto al usuario la opcion que desea y guardo el ID en selected_option
		selected_option = askForOption();

		// Actuo segun el ID recibido anteriormente en selected_option. 
		// Como la funcion devuelve 0 si no ha habido error y !0 si lo ha habido, guardo en error el numero devuelto y en error_num.
		int error = makeDecisionByOption(selected_option, toysList, &userInpToys);
		error_num = error;
		if (error)  // Si el error es diferente a 0
			printError(error);

		// Si la opcion es la correspondiente a la última que se pulsará antes de que termine el programa y no ha habido errores, procedemos a enviar la carta.
		if (selected_option == MENU_EXIT_KEY && error == 0) {
			srand(time(NULL));
			// numero de veces que ocurre cada suceso (que han sido concedidos y rechazados)
			int granted_num=0, rejected_num=0;
			// llamo a la funcion
			sendLetter(userInpToys, toys_granted, toys_rejected, &granted_num, &rejected_num);

			// Imprimo el numero de juguetes concedidos y denegados.
			// Bucle de 2 iteraciones. Llama 2 veces a la funcion printFinalLetter, para imprimir las 2 posibilidades (juguetes concedidos y denegados).
			int option;
			for(option = 0; option < 2; option++) 
				printFinalLetter(option, toysList, ((option == 0) ? toys_granted : toys_rejected) ,((option == 0) ? granted_num : rejected_num));
		}

		// Mostramos al usuario el mensaje "pulse cualquier tecla para continuar"
		pressToClear();

	} while (isOptionCorrect(0, MAX_MENU_ELEMENTS, selected_option) || (selected_option != MENU_EXIT_KEY || error_num == 107));

	// Mensaje de despedida y fin del programa.
	printGoodBye();
	return 0;
}
