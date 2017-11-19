#include <iostream>
using namespace std;
#include "Xor.h"


int main() {
    Xor Xorprueba;
    int a[] = {0,0,1,1,1};
    int b[] = {1,0,1,0,1};
    int tamanoA = sizeof(a) / sizeof(*a);
    int tamanoB = sizeof(b) / sizeof(*b);
    Xorprueba.doXor(a,b, tamanoA, tamanoB);

}
