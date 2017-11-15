#include <iostream>
#include <fstream>
#include <limits.h>


void getBytes(const char *name);
void getBits(int* array, int size);

int main() {

    getBytes("/home/kevin/enemy.png");
    return 0;

}


/*
 * @brief Genera el arreglo de bytes
 * @param name es el path del archivo a trabajar
 */
void getBytes(const char *name){
    FILE* f = fopen(name, "rb");
    fseek(f,0,SEEK_END);
    long lsize=0,i=0,y=0;
    lsize = ftell(f);
    rewind(f);
    int x = 0;
    int aux = 0;
    while(i<lsize){
        int first = fgetc(f);
        i++;
        x++;
    }
    FILE* k = fopen(name, "rb");
    fseek(k,0,SEEK_END);
    lsize = ftell(k);
    rewind(k);

    int array[x];
    while(y<lsize){
        int first = fgetc(k);
        y++;
        array[aux] = first;
        aux++;

        //Imprime los bytes
        //std::cout<<(first)<<std::endl;
    }

    fclose(f);
    fclose(k);
    std::cout<<"El tamano de: " << name << " es de " << sizeof(array)/ sizeof(array[0]) << " bytes" <<std::endl;
    getBits(array, sizeof(array)/ sizeof(array[0]));
}


/*
 * @brief imprime los bits de un arreglo de bytes
 * @param array el arreglo de bytes
 * @param size el tamano del arreglo de bytes
 */
void getBits(int* array, int size){
    int x;
    for(x=0; x<size; x++){
        unsigned char byte = array[x];
        unsigned char mask = 1;
        unsigned char bits[8];
        int i, j = CHAR_BIT-1;
        for ( i = 0; i < 8; i++,j--,mask = 1) {
            bits[i] =( byte & (mask<<=j))  != NULL;
        }
        for (int i = 0; i < 8; i++) {
            printf("%d", bits[i]);
        }
        puts("");
    }

}