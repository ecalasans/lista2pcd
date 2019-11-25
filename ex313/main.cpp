#include <iostream>
#include <mpi.h>
#include <cmath>
#include <cstdlib>
#include <string>
#include <sstream>
#include <ctime>

using namespace std;

const int MAX_DIM = 1382487;
const int DISTANCIA = 3;

double * GeraVetor();

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

    int comm_sz, myrank, quociente, resto;

    int * deslocamentos, * quants;

    //Inicializacao do MPI
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    //Aloca memoria para os buffers de recepcao e outras variaveis
    b1 = (double *) malloc(MAX_DIM * DISTANCIA * sizeof(double));
    b2 = (double *) malloc(MAX_DIM * DISTANCIA * sizeof(double));
    c1 = (double *) malloc(MAX_DIM * DISTANCIA * sizeof(double));
    c2 = (double *) malloc(MAX_DIM * DISTANCIA * sizeof(double));
    deslocamentos = new int[comm_sz];
    quants = new int[comm_sz];
    rec_escalar = (double *) malloc(sizeof(double));

    quociente = MAX_DIM/(comm_sz - 1);
    resto = MAX_DIM % (comm_sz - 1);

    //Preenche os vetores necessarios para as funcoes Scatterv e Gatherv
    for(int i = 0; i < comm_sz; ++i){
        deslocamentos[i] = i * DISTANCIA;

        //Para o ultimo processo separa apenas o que restou da divisao do vetor em comm_sz pedacos
        if(i == comm_sz - 1){
            quants[i] = resto;
        } else {
            quants[i] = quociente;
        }
    }

    //Espalhar os valores para todos os processos - usar a funcao MPI_Scatter
    if (myrank == 0){
        //Geram os vetores e o escalar para serem compartilhados pelos processos
        vx = GeraVetor();
        vy = GeraVetor();
        escalar = 2.0;

        //Imprime os 5 primeiros valores de vx e vy
        cout << "Trabalhando com " << comm_sz << " processos..." << endl;
        cout << "Trabalhando com vetores de tamanho " << MAX_DIM << endl;

        cout << endl;
        //Distribui os dados para os processos
        MPI_Scatterv(vx, quants, deslocamentos, MPI_DOUBLE, b1, quants[myrank], MPI_DOUBLE,
                0, MPI_COMM_WORLD);
        MPI_Scatterv(vy, quants, deslocamentos, MPI_DOUBLE, b2, quants[myrank], MPI_DOUBLE,
                0, MPI_COMM_WORLD);
        MPI_Bcast(&escalar, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        cout << "Sou o processo " << myrank << " e recebi " << quants[myrank] <<
            " elementos para trabalhar" << endl << endl;

         //Evetua o produto escalar e o produto por escalar localmente
        for (int j=0; j < quants[myrank]; ++j){
            local_prod_escalar += b1[j] * b2[j];
            b1[j] = b1[j] * escalar;
            b2[j] = b2[j] * escalar;
        }

        //Manda os elementos para serem juntados no rank 0 apos o produto por escalar
        MPI_Gatherv(b1, quants[myrank], MPI_DOUBLE, c1, quants, deslocamentos, MPI_DOUBLE,
                0, MPI_COMM_WORLD);
        MPI_Gatherv(b2, quants[myrank], MPI_DOUBLE, c2, quants, deslocamentos, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

        //Faz a reducao
        MPI_Reduce(&local_prod_escalar, &total_prod_escalar, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        cout << "Produto escalar dos vetores gerados = " << total_prod_escalar << endl;
        cout << "Cinco primeiros elementos apos multiplicacao por " << escalar << endl;

        cout << "vx = ";
        for(int i = 0; i < 5; ++i)
            cout << *(c1+i) << ", ";
        cout << endl << "vy = ";
        for(int j=0; j < 5; ++j)
            cout << *(c2+j) << ", ";
        cout << endl;

    } else {
        //Recebe os dados provenientes do rank 0
        MPI_Scatterv(vx, quants, deslocamentos, MPI_DOUBLE, b1, quants[myrank], MPI_DOUBLE,
                     0, MPI_COMM_WORLD);
        MPI_Scatterv(vy, quants, deslocamentos, MPI_DOUBLE, b2, quants[myrank], MPI_DOUBLE,
                     0, MPI_COMM_WORLD);
        MPI_Bcast(rec_escalar, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        cout << "Sou o processo " << myrank << " e recebi " << quants[myrank] <<
             " elementos para trabalhar" << endl << endl;

        //Evetua o produto escalar e o produto por escalar localmente
        for (int j=0; j < quants[myrank]; ++j){
            local_prod_escalar += b1[j] * b2[j];
            b1[j] = b1[j] * (*rec_escalar);
            b2[j] = b2[j] * (*rec_escalar);
        }

        //Manda os elementos para serem juntados no rank 0 apos o produto por escalar
        MPI_Gatherv(b1, quants[myrank], MPI_DOUBLE, c1, quants, deslocamentos, MPI_DOUBLE,
                    0, MPI_COMM_WORLD);
        MPI_Gatherv(b2, quants[myrank], MPI_DOUBLE, c2, quants, deslocamentos, MPI_DOUBLE,
                    0, MPI_COMM_WORLD);

        //Manda os produtos escalares locais para reducao
        MPI_Reduce(&local_prod_escalar, &total_prod_escalar, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}

double * GeraVetor(){
    double * v = new double[MAX_DIM];
    for(int i=0; i < MAX_DIM; ++i){
        v[i] = (double) i*rand();
    }
    return v;
}
