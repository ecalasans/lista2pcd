#include <iostream>
#include <mpi.h>

using namespace std;

void Read_vector(double local_a[], int local_n, int n,
        char vec_name[], int my_rank, MPI_Comm comm){
    double * a = NULL;
    int i;

    MPI_Datatype meu_tipo;
    MPI_Type_contiguous(local_n, MPI_DOUBLE, &meu_tipo);
    MPI_Type_commit(&meu_tipo);

    if(my_rank == 0){
        a = (double *) malloc(n * sizeof(double));
        cout << "Enter the vector " <<  vec_name;
        for(i = 0; i < n; ++i)
            cin >> a[i];

        MPI_Scatter(a, 1, meu_tipo, local_a, 1, meu_tipo, 0, comm);
        free(a);
    } else {
        MPI_Scatter(a, 1, meu_tipo, local_a, 1, meu_tipo, 0, comm);
    }
    MPI_Type_free(&meu_tipo);
}

void Print_vector(double local_b[], int local_n, int n, char title[],
        int my_rank, MPI_Comm comm){
    double * b = NULL;

    MPI_Datatype meu_tipo;
    MPI_Type_contiguous(local_n, MPI_DOUBLE, &meu_tipo);
    MPI_Type_commit(&meu_tipo);

    if(my_rank == 0){
        b = (double *) malloc(n * sizeof(double));

        MPI_Gather(local_b, 1, meu_tipo, b, 1, meu_tipo, 0, comm);

        cout << "Print " << title << endl;
        for(int i = 0; i < n; ++i)
            cout << b[i] << ", ";

        free(b);
    } else {
        MPI_Gather(local_b, 1, meu_tipo, b, 1, meu_tipo, 0, comm);
    }
}

int main() {
    return 0;
}