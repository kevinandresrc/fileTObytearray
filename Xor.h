//
// Created by Varo on 11/18/2017.
//
#include <iostream>
using namespace std;


int Xor() {
    int a[] = {0,0,1,1,0};
    int b[] = {1,0,1,0,1};
    //Xor aXor;
    //aXor.prueba(a,b);
    int tamanoA = sizeof(a)/ sizeof(*a);
    int tamanoB = sizeof(b)/ sizeof(*b);
    int i = 0;
    if ( tamanoA == tamanoB){
        int c[ tamanoA];
        while (i+1 <= tamanoA){
            c[i] = a[i] ^ b[i];
            cout << c[i] <<endl;
            i++;
        }
    } else
        cout << "Arreglos de diferentes tamanos, no se puede aplicar xor" <<endl;
    return 0;
}
