//
// Created by Varo on 11/18/2017.
//
#include <iostream>
using namespace std;


class Xor {

public:

    void doXor(int a[] , int b[], int tamanoA, int tamanoB) {
        if (tamanoA == tamanoB) {
            int i = 0;
            int c[tamanoA];                   /////ESTE ES EL ARREGLO QUE RESULTANTE DEL XOR
            while (i + 1 <= tamanoA) {
                c[i] = a[i] ^ b[i];
                cout << c[i] << endl;
                i++;
            }
        } else
            cout << "Arreglos de diferentes tamanos, no se puede aplicar xor" << endl;
    }

};
