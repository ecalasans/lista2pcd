#include <iostream>
#include <mpi.h>
#include <cmath>
#include <cstdlib>
#include <string>
#include <sstream>
#include <ctime>

using namespace std;

const int MAX_DIM = 1024;

double * GeraVetor(int fator);
void EspalhaVetores();

int main() {
    double  escalar = 0.0;

    double* vx = NULL;
    double* vy = NULL;
    double* b1 = NULL;
    double* b2 = NULL;
    double* c1 = NULL;
    double* c2 = NULL;
    double* rec_escalar = NULL;

    double local_prod_escalar = 0, total_prod_escalar = 0;

    int comm_sz, myrank;

    //Inicializacao do MPI
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    //Aloca memoria para os buffers de recepcao
    b1 = (double *) malloc(MAX_DIM * sizeof(double));
    b2 = (double *) malloc(MAX_DIM * sizeof(double));
    c1 = (double *) malloc(MAX_DIM * sizeof(double));
    c2 = (double *) malloc(MAX_DIM * sizeof(double));
    rec_escalar = (double *) malloc(sizeof(double));

    //Espalhar os valores para todos os processos - usar a funcao MPI_Scatter
    if (myrank == 0){
        //Geram os vetores e o escalar para serem compartilhados pelos processos
        vx = GeraVetor(comm_sz);
        vy = GeraVetor(comm_sz);
        escalar = 2.0;

        //Imprime os 5 primeiros valores de vx e vy
        cout << "Imprimindo 5 primeiros valores de vx e vy..." << endl;
        cout << "vx = ";
        for(int i = 0; i < 5; ++i)
            cout << *(vx+i) << ", ";
        cout << endl << "vy = ";
        for(int j=0; j < 5; ++j)
            cout << *(vy+j) << ", ";

        cout << endl;
        //Distribui os dados para os processos
        MPI_Scatter(vx, MAX_DIM, MPI_DOUBLE, b1, MAX_DIM, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Scatter(vy, MAX_DIM, MPI_DOUBLE, b2, MAX_DIM, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(&escalar, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

         //Evetua o produto escalar e o produto por escalar localmente
        for (int j=0; j < MAX_DIM; ++j){
            local_prod_escalar += b1[j] * b2[j];
            b1[j] = b1[j] * escalar;
            b2[j] = b2[j] * escalar;
        }

        //Manda os elementos para serem juntados no rank 0 apos o produto por escalar
        MPI_Gather(b1, MAX_DIM, MPI_DOUBLE, c1, MAX_DIM, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Gather(b2, MAX_DIM, MPI_DOUBLE, c2, MAX_DIM, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        //Faz a reducao
        MPI_Reduce(&local_prod_escalar, &total_prod_escalar, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        cout << "Produto escalar dos vetores gerados = " << total_prod_escalar << endl;
        cout << "Imprimindo 5 primeiros valores de vx e vx apos multiplicacao por " << escalar << endl;

        cout << "vx = ";
        for(int i = 0; i < 5; ++i)
            cout << *(c1+i) << ", ";
        cout << endl << "vy = ";
        for(int j=0; j < 5; ++j)
            cout << *(c2+j) << ", ";
        cout << endl;

    } else {
        //Recebe os dados provenientes do rank 0
        MPI_Scatter(vx, MAX_DIM, MPI_DOUBLE, b1, MAX_DIM, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Scatter(vy, MAX_DIM, MPI_DOUBLE, b2, MAX_DIM, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(rec_escalar, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        //Evetua o produto escalar e o produto por escalar localmente
        for (int j=0; j < MAX_DIM; ++j){
            local_prod_escalar += b1[j] * b2[j];
            b1[j] = b1[j] * (*rec_escalar);
            b2[j] = b2[j] * (*rec_escalar);
        }

        //Manda os elementos para serem juntados no rank 0 apos o produto por escalar
        MPI_Gather(b1, MAX_DIM, MPI_DOUBLE, c1, MAX_DIM, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Gather(b2, MAX_DIM, MPI_DOUBLE, c2, MAX_DIM, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        //Manda os produtos escalares locais para reducao
        MPI_Reduce(&local_prod_escalar, &total_prod_escalar, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}

double * GeraVetor(int fator){
    int tam = fator * MAX_DIM;
    double * v = new double[tam];
    for(int i=0; i < tam; ++i){
        v[i] = (double) i*rand();
    }
    return v;
}
