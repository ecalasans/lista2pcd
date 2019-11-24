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
    double* z = NULL;
    double* b1 = NULL;
    double* b2 = NULL;

    int comm_sz, myrank;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    //Espalhar os valores para todos os processos - usar a funcao MPI_Scatter
    if (myrank == 0){
        vx = GeraVetor(comm_sz);
        vy = GeraVetor(comm_sz);
        escalar = 2.0;

        b1 = (double *) malloc(MAX_DIM * sizeof(double));
        b2 = (double *) malloc(MAX_DIM * sizeof(double));

        MPI_Scatter(vx, MAX_DIM, MPI_DOUBLE, b1, MAX_DIM, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Scatter(vy, MAX_DIM, MPI_DOUBLE, b2, MAX_DIM, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(&escalar, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    } else {
        b1 = (double *) malloc(MAX_DIM * sizeof(double));
        b2 = (double *) malloc(MAX_DIM * sizeof(double));
        double * rec_escalar = (double *) malloc(sizeof(double));

        MPI_Scatter(vx, MAX_DIM, MPI_DOUBLE, b1, MAX_DIM, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Scatter(vy, MAX_DIM, MPI_DOUBLE, b2, MAX_DIM, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(rec_escalar, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        for(int i=0; i < 3; ++i)
            cout << "Printado de " << myrank << " - " << *(b1+i) << endl;

        for(int j=0; j < 2; ++j)
            cout << "Printado de " << myrank << " - " << *(b2+j) << endl;
        cout << "Escalar = " << *(rec_escalar) << endl;
    }

    //Efetuar os calculos nos processo
    //Recolher os resultados - usar a funcao MPI_Gather
    //Imprimir o resultado no processo 0
    MPI_Finalize();
    return 0;
}

double * GeraVetor(int fator){
    int tam = fator * MAX_DIM;
    double * v = new double[tam];
    for(int i=0; i < tam; ++i){
        v[i] = (double) i*1.65;
    }
    return v;
}
