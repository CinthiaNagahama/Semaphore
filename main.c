#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define INITIAL_STATE 0
#define TURNED_YELLOW_STATE 1
#define TURNED_RED_STATE 2

volatile char SemaphoreControler;

pthread_t thread;
volatile bool pedestre = false;

void *checaPedestre(void *arg) 
{ 
	(void)arg;
	char aux;

	for(; ; ){
		aux = getchar();
		if(aux == 'S' || aux == 's'){
			pedestre = true;
		}
		if(aux == 'N' || aux == 'n'){
			printf("Então tá.\n");
		}
	}
} 

void Semaphore(){
  switch (SemaphoreControler){
  	case INITIAL_STATE:{
			printf("Farol verde\n");
			printf("Gostaria de atravessar? [S/N]\n");
			pthread_create(&thread, NULL, checaPedestre, NULL);

			time_t start, end;
			time(&start);
			
			do{
				if(pedestre == true){
					SemaphoreControler = TURNED_YELLOW_STATE;

					pedestre = false;
					pthread_cancel(thread);
					break;
				}
				time(&end);
			}while(difftime(end, start) < 64);
			
			SemaphoreControler = TURNED_YELLOW_STATE;
			break;
		}

		case TURNED_YELLOW_STATE:{
			printf("Farol amarelo\n");
			sleep(8);
			SemaphoreControler = TURNED_RED_STATE;
			break;
		}

		case TURNED_RED_STATE:{
			printf("Farol vermelho\n");
			sleep(64);
			SemaphoreControler = INITIAL_STATE;
			break;
		}

		default:
			printf("Algo não está certo\n");
			break;
	}
}

int main(void){
  SemaphoreControler = INITIAL_STATE;  
	
  while(1){
    Semaphore();
  }
	
	pthread_exit(NULL);
  system("PAUSE");
  return 0;
}