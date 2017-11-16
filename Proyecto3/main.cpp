#include <iostream>
#include <iostream>
#include <fstream>
#include <vector>
#include <limits.h>
using namespace std;
int* getBytes(const char *name);
void generarParidad(int* array1,int* array2, int size1);
void getBits(int* array, int size);
typedef unsigned char byte;
int lenVid;
int lenBits;
class bloque{
public:
    int storage[];
};
class diskNode{
public:
    bloque espacioDisco[10];
    int espacioTotal=4000;
};
diskNode disco1;

int main() {
    int* x=getBytes("/home/gabriel/Mamut_volador.3gp");
    int primeraParte[lenVid/2];
    int segundaParte[lenVid/2];
    int contador=0;
    int contador2=lenVid/2;
    while(contador<(lenVid/2)-1){
        primeraParte[contador]=*(x+contador);
        contador++;
    }
    while(contador2<=lenVid-1){
        segundaParte[contador2]=*(x+contador2);
        contador2++;
    }
    generarParidad(primeraParte,segundaParte,sizeof(primeraParte)/ sizeof(primeraParte[0]));
    return 0;

}


/*
 * @brief Genera el arreglo de bytes
 * @param name es el path del archivo a trabajar
 */
int* getBytes(const char *name){
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
    int*  array=new int[x];
    int array2[x];
    lenVid=x;
    while(y<lsize){
        int first = fgetc(k);
        y++;
        array[aux] = first;
        array2[aux] = first;
        aux++;

        //Imprime los bytes
        //std::cout<<(first)<<std::endl;
    }

    fclose(f);
    fclose(k);
    std::cout<<"El tamano de: " << name << " es de " << sizeof(array2)/ sizeof(array2[0]) << " bytes" <<std::endl;
    //getBits(array2, sizeof(array2)/ sizeof(array2[0]));
    return array;

}


/*
 * @brief imprime los bits de un arreglo de bytes
 * @param array el arreglo de bytes
 * @param size el tamano del arreglo de bytes
 */
void generarParidad(int* array1,int* array2, int size1){
    int x=0;
    int y=0;
    int comp = size1;
    int paridad[size1*8];
    for(x=0; x<comp; x++){
        unsigned char byte = array1[x];
        unsigned char byte2=array2[x];
        unsigned char mask = 1;
        unsigned char bits[8];
        unsigned char bits2[8];
        int i, j = CHAR_BIT-1;
        for ( i = 0; i < 8; i++,j--,mask = 1) {
            bits[i] =( byte & (mask<<=j))  != NULL;
            bits2[i] = ( byte2 & (mask<<=j))  != NULL;
            if(bits[i]+bits2[i]==2||bits[i]+bits2[i]==0){
                paridad[y]=1;
            }
            y++;

        }
    }
    lenBits=y;
    cout<<y<<endl;
}
void getBits(int* array, int size){
    int x;
    int y =0;
    for(x=0; x<size; x++){
        unsigned char byte = array[x];
        unsigned char mask = 1;
        unsigned char bits[8];
        int i, j = CHAR_BIT-1;
        for ( i = 0; i < 8; i++,j--,mask = 1) {
            bits[i] =( byte & (mask<<=j))  != NULL;
        }
        for (int i = 0; i < 8; i++) {
            //printf("%d", bits[i]);
        }
        y+=8;
        //puts("");
    }
    cout<<y<<endl;

}