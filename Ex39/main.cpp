#include <iostream>
#include "/Users/ericcalasans/mpi/openmpi-4.0.2/ompi/include/mpi.h"
#include <math.h>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

void DotProduct(double* local_x, double* local_y, double* local_z, int local_n);
vector<double> LeVetor(string s);
void ImprimeResultados();
void EspalhaDados(vector<double> v1, vector<double> v2,
        double e, MPI_Comm comm, int qt_proc);

const int MAX_STRING = 100;

int main() {
    string x, y;
    double escalar;
    vector<double> vx, vy, z;
    int comm_sz, myrank;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    //Ler os vetores e escalar
    if(myrank == 0){
        cout << "Exercicio 3.9" << endl;
        cout << "Digite o primeiro vetor com elementos separados por espaco: ";
        getline(cin, x);
        vx = LeVetor(x);

        cout << "Digite o segundo vetor com elementos separados por espaco: ";
        getline(cin, y);
        vy = LeVetor(y);

        cout << "Digite o escalar: ";
        cin >> escalar;

        EspalhaDados(vx, vy, escalar, MPI_COMM_WORLD, comm_sz);
    }


    //Espalhar os valores para todos os processos - usar a funcao MPI_Scatter

    //Efetuar os calculos nos processo
    //Recolher os resultados - usar a funcao MPI_Gather
    //Imprimir o resultado no processo 0
    MPI_Finalize();
    return 0;
}

vector<double> LeVetor(string s){
    stringstream ss;
    ss << s;

    vector<double> v;

    double numero;

    while (ss >> numero){
        v.push_back(numero);
    }
    return v;
}

void EspalhaDados(vector<double> v1, vector<double> v2, double e, MPI_Comm comm, int qt_proc){
    int send_count = v1.size()/qt_proc;
    double* rec_buffer = (double* ) malloc(sizeof(double) * send_count);

    MPI_Scatter(&v1, send_count, MPI_DOUBLE, rec_buffer, send_count, MPI_DOUBLE, 0, comm);
    MPI_Scatter(&v2, send_count, MPI_DOUBLE, rec_buffer, send_count, MPI_DOUBLE, 0, comm);
    MPI_Bcast(&e, 1, MPI_DOUBLE, 0, comm);
}