#include <iostream>
#include <mpi.h>

using namespace std;

const int DIMENSAO = 15;

int * GeraBlocos(int dim, int * tam);
int * GeraDeslocamentos(int dim);
int * GeraMatriz(int dim);

int main() {
     int comm_sz, my_rank, tamanho;
     int * blocos = NULL;
     int * deslocs = NULL;
     int * matriz = NULL;
     int * rec_buf = NULL;

     MPI_Init(NULL, NULL);
     MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
     MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    matriz = GeraMatriz(DIMENSAO);
    blocos = GeraBlocos(DIMENSAO, &tamanho);
    deslocs = GeraDeslocamentos(DIMENSAO);

    MPI_Datatype triang;
    MPI_Type_indexed(DIMENSAO, blocos, deslocs, MPI_INT, &triang);
    MPI_Type_commit(&triang);

     if(my_rank == 0){
         cout << "Gerando a matriz quadrada de tamanho " << DIMENSAO << "..." << endl;
         for(int i = 0; i < DIMENSAO * DIMENSAO; i++){
             if(i % DIMENSAO != 0) {
                 cout << matriz[i] << " ";
             } else {
                 cout << endl << matriz[i] << " ";
             }
         }
         cout << endl;
         MPI_Send(matriz, 1, triang, 1, 0, MPI_COMM_WORLD);
     } else {
         rec_buf = new int[DIMENSAO * DIMENSAO];
        MPI_Recv(rec_buf, 1, triang, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        cout << rec_buf[3] << endl;
     }

     MPI_Finalize();
    return 0;
}

int * GeraBlocos(int dim, int * tam){
    int * blocos = new int[dim];
    for(int i = 0; i < dim; ++i) {
        blocos[i] = dim - i;
        *(tam) += blocos[i];
    }
    return blocos;
}
int * GeraDeslocamentos(int dim){
    int * d = (int *) malloc(dim * sizeof(int));
    for(int j = 0; j < dim; ++j){
        (j == 0) ? d[j] = 0 : d[j] = (dim * j) + j;
    }
    return d;
}
int * GeraMatriz(int dim){
    int * m = new int[dim * dim];
    for(int i = 0; i < dim*dim; i++){
        m[i] =  i ;
    }
    return m;
}