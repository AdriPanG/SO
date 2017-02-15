#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/io.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>


void juegos(int fd);

/*Variables globales*/
static char msg[3];

#define _REENTRANT

int main()
{
	srand(time(NULL));
	int fichero = open("/dev/leds", O_WRONLY);
	if(fichero < 0)
	{
		printf("No se ha podido abrir /dev/leds \n");
		exit(-1); 
	}

	juegos(fichero);
				

	close(fichero);
	return 0;
}

void juegos(int fd)
{
	int opJuego;
	int opBinario;
	int opNumero=-1;
	int opAleatorio=0;
	int numeroAleatorio;
	printf("1: Mostrar número, 1: Adivinar numero, 0: Salir.\n");
	scanf("%d",&opJuego);
	if(opJuego == 1) {
		while (opNumero != 0 ) {
			printf("Introduce un número: ");
			scanf("%d",&opNumero);
			switch(opNumero) {
				case 0:
					msg[0]='4';
					write(fd,msg,0);
					break;
				case 1: 
					msg[0]='3';
					write(fd,msg,1);
					break;
				case 2: 
					msg[0]='2';
					write(fd,msg,1);
					break;
				case 3: 
					msg[0]='2';
					msg[1]='3';
					write(fd,msg,2);
					break;
				case 4: 
					msg[0]='1';
					write(fd,msg,1);
					break;
				case 5: 
					msg[0]='1';
					msg[1]='3';
					write(fd,msg,2);
					break;
				case 6: 
					msg[0]='1';
					msg[1]='2';
					write(fd,msg,2);
					break;
				case 7: 
					msg[0]='1';
					msg[1]='2';
					msg[2]='3';
					write(fd,msg,3);
					break;
				default:
					printf("Opcion incorrecta\n");
					break;
			}
		}
	} 
	else if(opJuego == 1) {
		srand(time(NULL)); 
		int contador = 1;
		int i;
		bool ok = false;
		numeroAleatorio = rand()%8;
		while (opAleatorio >= 0 && opAleatorio <= 7 && !ok) {
			printf("Introduce un número: ");
			scanf("%d",&opAleatorio);
			if (opAleatorio == numeroAleatorio) {
				ok = true;
				printf("Has acertado despues de: %d veces!!!!! \n", contador);
				for (i = 0; i < 4; i++) {
					msg[0]='1';
					msg[1]='2';
					msg[2]='3';
					write(fd,msg,3);
					usleep(200000);
					msg[0]='4';
					write(fd,msg,0);
					usleep(200000);
				}
				
			} else if (opAleatorio > numeroAleatorio) {
				printf("Pista: el numero es menor que el introducido \n");
				for (i = 0; i < 2; i++) {
					msg[0]='1';
					write(fd,msg,1);
					usleep(1000000);
					msg[0]='4';
					write(fd,msg,0);
					usleep(200000);
				}
				contador++;
			} else if (opAleatorio < numeroAleatorio) {
				printf("Pista: el numero es mayor que el introducido \n");
				for (i = 0; i < 2; i++) {
					msg[0]='3';
					write(fd,msg,1);
					usleep(1000000);
					msg[0]='4';
					write(fd,msg,0);
					usleep(200000);
				}
				contador++;
			}
		}
	}
}
