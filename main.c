/*******************
 * Actividad 4: Procesos
 * Luis Revilla - A01022320
 * Trabaje con Antonio Junco y Daniel Roa
 ******************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int promedio(int a, int b);
int maximoArchivos(int *vector, int n);
void imprimirHistograma(int *vector, int *vector2, int n);

int main(int argc, char * argv[]) {

  char *cvalue = NULL;
  int c;

  int n=0;

  while ((c = getopt (argc, argv, "n:")) != -1)
        switch (c)
    {
        case 'n':
            cvalue = optarg;
            break;

        case '?':
            if (optopt == 'n')
                fprintf (stderr, "Opción -%c requiere un argumento.\n", optopt);
            else if (isprint (optopt))
                fprintf (stderr, "Opción desconocida '-%c'.\n", optopt);
            else
                fprintf (stderr,
                         "Opción desconocida '\\x%x'.\n",
                         optopt);
            return 1;
        default:
            abort ();
    }




  n=atoi(cvalue);

  int * vector = (int *) malloc(n * sizeof(int));
  int * vector2 = (int *) malloc(n * sizeof(int));

    int * final = vector + n;

    int *aux2=vector2;
    for (int *aux = vector; aux < final; ++aux){
      pid_t pid;
      int estado;


      int tubo[2];
      int tubo2[2];

      pipe(tubo);
      pipe(tubo2);
      pid = fork();

      if (pid == -1)
      {
          printf("Error al crear el proceso hijo \n");
          return -1;
      }
      else if (pid == 0)
      {
        int prom=promedio(getpid(),getppid());
        printf("Soy el proceso hijo con PID = %d y mi promedio es = %d\n",getpid(),prom);
                
        close (tubo[0]);
        write(tubo[1],&prom,sizeof(int));

        int id=getpid();
        close (tubo2[0]);
        write(tubo2[1],&id,sizeof(int));
        exit(0);
      }
      else {
          if (waitpid(pid, &estado, 0) != -1)
          {
            int a =WEXITSTATUS(estado);
              if (WIFEXITED(estado)){

                close(tubo[1]);
                int promedioexit=0;
                read(tubo[0],&promedioexit,sizeof(int));
                *aux=promedioexit;
                close(tubo2[1]);
                read (tubo2[0],aux2,sizeof(int));
              }

          }


      }


    aux2++;
    }


    aux2=vector2;

    imprimirHistograma(vector, vector2,n);
    free(vector);
    free(vector2);
    return 0;
}




int promedio(int a, int b){
  return((a+b)/2);
}


void imprimirHistograma(int *vector, int *vector2, int n){
  int max=maximoArchivos(vector, n);
  double asteriscos=40.0;


  int * final = vector + n;
  int *aux2=vector2;


  printf("%8s %8s %10s\n","PID hijo","Promedio","Histograma");
  for (int *aux = vector; aux < final; ++aux) {

    printf("%8d: %8d ",*aux2,*aux);
    for (int i=0; i<(*aux*asteriscos)/max;++i){
      printf("*");
    }
    printf("\n");

    aux2++;
  }



}


int maximoArchivos(int *vector,int n){
  int max=0;
  int *aux=vector;
  int *final=vector +n;

  for (aux=vector; aux < final; ++aux) {
      if(max<*aux){
        max=*aux;
      }
	}
  return max;
}
