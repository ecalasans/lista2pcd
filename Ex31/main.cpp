#include <iostream>
#include <math.h>
using namespace std;

double Trap(double base_1, double base_2, double h);
double Fx(double x);

int main(int argc, char* argv[]) {
    double a = strtod(argv[1], nullptr);
    double b = strtod(argv[2], nullptr);
    double n = strtod(argv[3], nullptr);

    //Especifica 2 h's:  um para a parte divisível e outra para o resto
    double hd, hr;

    cout << "Lendo a, b e n..." << endl;
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    cout << "n = " << n << endl;

    //Calcula hd e hr
    hd = (b-a)/n;
    hr = (int)(b-a) % (int)n;
    cout << hd << endl << hr << endl;
    return 0;
}

double Trap(double base_1, double base_2, double h){
    return (base_1 + base_2) * (h/2);
}

double Fx(double x){
    return pow(x, 3);
}
