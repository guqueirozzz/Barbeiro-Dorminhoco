#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <Windows.h>

//MAX representa o número de cadeiras da sala de espera
#define MAX 1

//Utiliza-se MAX+1, pois deve-se levar em conta a cadeira do barbeiro
int comeco = 0, fim = 0, tam = 0, fila[MAX+1];
pthread_mutex_t travaFila;


int voidd()
{
  return !tam;
}

int desenfilera()
{
  if (!voidd())
  {
    srand(time(NULL));
    Sleep(rand() % 1 + 25); // O barbeiro atende os clientes em media a cada 25 minutos.
    printf("O barbeiro termina de cortar o cabelo do cliente %d, que vai embora.\n", fila[comeco]);

    int cliente = fila[comeco];
    comeco = (comeco+1) % (MAX+1);
    tam--;

    if (voidd())
      puts("Não há clientes para serem atendidos, o barbeiro vai dormir.");
    else
      printf("O cliente %d senta na cadeira do barbeiro.\n", fila[comeco]);

    return cliente;
  }
 else
    //O barbeiro está dormindo, portanto nada ocorre e a função retorna -1 como código de erro
    return -1;
}

void enfilera(int cliente)
{
  if (tam < MAX+1)
  {
    if (voidd())
      printf("O cliente %d acorda o barbeiro e senta em sua cadeira.\n", cliente);
    else
      printf("O cliente %d senta em uma das cadeiras vagas da sala de espera.\n", cliente);

    fila[fim] = cliente;
    fim = (fim+1) % (MAX+1);
    tam++;
  }
 else
    printf("Todas as cadeiras estavam ocupadas, o cliente %d foi embora.\n", cliente);
}


void* barbeiro(void* arg)
{
  for ( ; ; )
  {
    pthread_mutex_trylock(&travaFila);
    desenfilera();
    pthread_mutex_unlock(&travaFila);
  }
}

void* cliente(void* arg)
{
  unsigned long cd_cliente = *((unsigned long*) arg);

  pthread_mutex_trylock(&travaFila);
  enfilera(cd_cliente);
  pthread_mutex_unlock(&travaFila);
}


int main(){
  pthread_mutex_init(&travaFila, NULL); //mutex para o travaFila

  int i;
  pthread_t threadbarb, threadcli; // paramtrização das threads para o barbeiro e para o cliente 

  pthread_create(&threadbarb, NULL, barbeiro, NULL); // criação de thread para o barbeiro
  srand(time(NULL));
  for (i = 0 ; ; ++i)
  {
    Sleep(rand() % 1 + 30); //Intervalo de tempo para a chegada de clientes
    pthread_create(&threadcli, NULL, cliente, &i); // criação de thread para o cliente 
  }

  return 0;
}
