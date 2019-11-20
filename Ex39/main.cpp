#include <iostream>
#include <mpi.h>
#include <cmath>
#include <cstdlib>
#include <string>
#include <sstream>

using namespace std;

void GeraVetor(int dim, double* v);
void EspalhaVetores();

int main() {
    string x, y;
    double escalar;

    double* vx = nullptr;
    double* vy = nullptr;
    double* z = nullptr;
    double* b1 = nullptr;
    double* b2 = nullptr;

    int comm_sz, myrank, send_count = 0;

    int* dim;


    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    //Espalhar os valores para todos os processos - usar a funcao MPI_Scatter
    if (myrank == 0){
        int temp = 0;
        cout << "Digite a dimensão do vetor:  ";
        cin >> temp;
        dim = &temp;

        GeraVetor(*dim, vx);
        free(vx);
    } else {
        cout << myrank << "Printando algo" << endl;
    }

    //Efetuar os calculos nos processo
    //Recolher os resultados - usar a funcao MPI_Gather
    //Imprimir o resultado no processo 0
    MPI_Finalize();
    return 0;
}

void GeraVetor(int dim, double* v){
    v = (double*) malloc(dim * sizeof(double));
    for(int i=0; i < dim; ++i){
        v[i] = rand()*10.0;
        cout << v[i] << endl;
    }

}
