#include <iostream>
#include "/Users/ericcalasans/mpi/openmpi-4.0.2/ompi/include/mpi.h"
#include <math.h>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

const int MAX_STRING = 100;
vector<double> LeVetor(string s);

int main() {
    string x, y;
    double escalar;
    vector<double> vx, vy, z;
    int comm_sz, myrank, send_count;
    int rec_buffer_v1, rec_buffer_v2;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    //Para o processo 0
    if(myrank == 0){
        //Lê os dados e transforma em vetor
        cout << "Exercicio 3.9" << endl;
        cout << "Digite o primeiro vetor com elementos separados por espaco: ";
        getline(cin, x);
        vx = LeVetor(x);

        cout << "Digite o segundo vetor com elementos separados por espaco: ";
        getline(cin, y);
        vy = LeVetor(y);

        cout << "Digite o escalar: ";
        cin >> escalar;

        //Calcula a quantidade da particao dos vetores
        send_count = vx.size()/comm_sz;

        //Espalha os dados para os processos
        MPI_Scatter(&vx, send_count, MPI_DOUBLE, &rec_buffer_v1, send_count, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Scatter(&vy, send_count, MPI_DOUBLE, &rec_buffer_v2, send_count, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(&escalar, 1, MPI_DOUBLE, myrank, MPI_COMM_WORLD);

        for(int i = 1; i < comm_sz; ++i){
            cout << "Distribuindo " << rec_buffer_v1 << endl;
        }
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
