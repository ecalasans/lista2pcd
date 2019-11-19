#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <vector>

using namespace std;
void ImprimeVetor(vector<int> v);

int main() {
    int a = 100;   //Variavel
    int* pa = &a; // Ponteiro pra a
    cout << a << endl; // Conteúdo de a diretamente
    cout << pa << endl; // Endereço de a via ponteiro;
    cout << &a << endl; //Endereço de a via operador &
    cout << *pa << endl;  //Conteudo de a via ponteiro

    int** ppa = &pa;

    cout << ppa << endl;  // Endereço de ppa
    cout << *ppa << endl;  //Conteúdo de ppa, que é o endereço de pa
    cout << **ppa << endl;  //Conteúdo de a, que está em pa, que é acessado via ponteiro pra ponteiro


    cout << "Estudando matrizes com ponteiros" << endl;
    vector<int> m;

    vector<int>* p = &m;

    for (int i = 0; i < 4; ++i){
        m.push_back(i+2);
    }

    ImprimeVetor(*p);
    ImprimeVetor(m);
    return 0;
}

void ImprimeVetor(vector<int> v){
    for(int i=0; i < 4; ++i){
        cout << v[i] << endl;
    }
    cout << "Tamanho do vetor:  " << v.size() << endl;
}