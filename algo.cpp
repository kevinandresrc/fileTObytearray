#include <iostream>
#include <fstream>
#include <dirent.h>
#include <vector>
#include <limits.h>
#include <math.h>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <sstream>
#include "Xor.h"
#include <bitset>

using namespace std;
using namespace cv;
int* getBytes(const char *name);
void generarParidad(int* array1,int* array2, int size1);
void getBits(int* array, int size);
void generateVid(int* array, int size);
void revisarInsertarRAID(int array[]);
vector<char> metadata;
typedef unsigned char byte;
int lenVid;
int lenBits;
int posDiscos = 0;
vector<string> elementos;
string dirr;
class bloque{
public:
    bool isLleno = false;
    bool isParidad = false;
    int* storage;
    int sizeblock = pow(2,16);;
};
class diskNode{
public:
    bool isLleno=false;
    bool tieneParidad = false;
    bloque espacioDisco[3];
    int espacioTotal=4000;
};
diskNode disco1;
diskNode disco2;
diskNode disco3;
diskNode RAID[3];


void insertar(const char *direccion){
    int* x=getBytes(direccion);
    generateVid(x, lenVid);
    if (lenVid/2 < RAID[0].espacioDisco[0].sizeblock){
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
        revisarInsertarRAID(primeraParte);
        revisarInsertarRAID(segundaParte);
        generarParidad(primeraParte,segundaParte,sizeof(primeraParte)/ sizeof(primeraParte[0]));
    }
    else {
        cout <<"La extrencion del video es demaciado extenza para la implementación del RAID" << endl;
    }
}
void iniciarRaid(){
    RAID[posDiscos]=disco1;
    posDiscos+=1;
    RAID[posDiscos]=disco2;
    posDiscos+=1;
}
void revisarInsertarRAID(int array[]){
    int contador = 0;
    int contador2 = 0;
    bool salir=false;
    while(contador2!=3){
        contador=0;
        while(contador!=3){
            if(RAID[contador].isLleno==false){
                if(RAID[contador].espacioDisco[contador2].isLleno== false){ 
                    RAID[contador].espacioDisco[contador2].storage = array;
                    RAID[contador].espacioDisco[contador2].isLleno=true;
                    salir=true;
                    break;
                }
                contador++;
            }
        }
        if(salir){
            break;
        }
        contador2++;
    }
}
void revisarInsertarRAIDParidad(int array[]){
    int contador = 0;
    int contador2 = 0;
    bool salir=false;
    while(contador2!=3){
        contador=0;
        while(contador!=3){
            if(RAID[contador].isLleno==false){
                if(RAID[contador].tieneParidad==true){
                    contador++;
                }else{
                    if(RAID[contador].espacioDisco[contador2].isLleno== false){
                        RAID[contador].espacioDisco[contador2].storage = array;
                        RAID[contador].espacioDisco[contador2].isLleno=true;
                        RAID[contador].tieneParidad = true;
                        salir=true;
                        break;
                    }
                    contador++;
                }
            }
            contador++;
        }
        if(salir){
            break;
        }
        contador2++;
    }
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
    int paridad[size1*8]={0};
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
                paridad[y]=0;

            }else{
                paridad[y]=1;
            }
            //cout<<paridad[y]<<endl;
            y++;

        }
    }
    revisarInsertarRAIDParidad(paridad);
    lenBits=y;
    cout<<"Bits: "<< y<<endl;
}
void agregarNodo(){
    diskNode disco;
    RAID[posDiscos] = disco;
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
    //cout<<y<<endl;

}


vector<int> getBitsXOR(int* array, int size){
    int x;
    int y =0;
    vector<int> bitsResultado;
    for(x=0; x<size; x++){
        unsigned char byte = array[x];
        unsigned char mask = 1;
        unsigned char bits[8];
        int i, j = CHAR_BIT-1;
        for ( i = 0; i < 8; i++,j--,mask = 1) {
            bits[i] =( byte & (mask<<=j))  != NULL;
            bitsResultado.push_back(( byte & (mask<<=j))  != NULL);
        }
        for (int i = 0; i < 8; i++) {
            //printf("%d", bits[i]);
        }
        y+=8;
        //puts("");
    }
    //cout<<y<<endl;
    //cout<<bitsResultado.size()<<endl;
    return bitsResultado;

}

void list_dir(string dir)
{
  DIR * directorio;
  struct dirent * elemento;
  string elem;
  if (directorio = opendir(dir.c_str()))
  { 
   while (elemento = readdir(directorio))
  {
    elem = elemento->d_name;
    if (elem != "." && elem != "..") elementos.push_back(elem);
   }            
  }
  closedir(directorio);
}

void init()
{
  cout << "Ubicación de los videos: ";
  string dir;
  getline(cin, dir);
  list_dir(dir);
  dirr = dir + "/";

}
void generateVid(int* array, int size){
    FILE* file = fopen("videoMV.3gp","we");
    fwrite(array, sizeof(char), size,file);
    fclose(file);
}
void generateVidIncompleto(int* array, int size){
    FILE* file = fopen("videoIncompleto.3gp","we");
    fwrite(array, sizeof(char), size,file);
    fclose(file);
}
void eliminarDisco(int disc){
    diskNode tmp;
    
    int x = 0;
    tmp.espacioDisco[0].storage = &x;
    tmp.espacioDisco[1].storage = &x;
    tmp.espacioDisco[2].storage = &x;

    RAID[disc] = tmp;
    cout << "El disco " << disc << " ha sido eliminado satisfactoriamente" << endl;
}
int* doXor(vector<int> a, vector<int> b, int tamanoA, int tamanoB) {
        if (tamanoA == tamanoB) {
            int i = 0;
            int*  c=new int[tamanoA];                
            while (i + 1 <= tamanoA) {
                c[i] = a.at(i) ^ b.at(i);
                //cout<<c[i]<<endl;   
                i++;
            }
            return  c;
        } else
            cout << "Arreglos de diferentes tamanos, no se puede aplicar xor" << endl;
    }
int main() {
    iniciarRaid();
    agregarNodo();
    init();
    for (int i = 0; i <elementos.size(); i++)
    {
        string str = dirr + elementos.at(i);
        char *cstr = &str[0u];
        metadata.push_back(*cstr); 
        insertar(cstr);
    } 
    int dato;
    cout << "Ingrese el disco que desea eliminar(0, 1, 2): ";
    cin >> dato;
    cout << *(RAID[dato].espacioDisco[0].storage) << endl;
    eliminarDisco(dato);
    //cout<<getBitsXOR(RAID[1].espacioDisco[0].storage,lenVid/2).size()<<endl;
    //vector<int> a = getBitsXOR(RAID[1].espacioDisco[0].storage,lenVid/2);
    //vector<int> b = getBitsXOR(RAID[2].espacioDisco[0].storage,lenVid/2);
    vector<int> a;
    if(dato != 2){
        for (int i = 0; i < (lenVid/2)*8; i++)
        {
            a.push_back(*(RAID[2].espacioDisco[0].storage+i));
        }
        if(dato == 0){
            generateVidIncompleto(RAID[1].espacioDisco[0].storage,lenVid/2);
            RAID[dato].espacioDisco[0].storage = doXor(getBitsXOR(RAID[1].espacioDisco[0].storage,lenVid/2),a, getBitsXOR(RAID[1].espacioDisco[0].storage,lenVid/2).size(), a.size());
        }else{
            generateVidIncompleto(RAID[0].espacioDisco[0].storage,lenVid/2);
            RAID[dato].espacioDisco[0].storage = doXor(getBitsXOR(RAID[0].espacioDisco[0].storage,lenVid/2),a, getBitsXOR(RAID[0].espacioDisco[0].storage,lenVid/2).size(), a.size());
        }
            
    }else{
        RAID[dato].espacioDisco[0].storage = doXor(getBitsXOR(RAID[0].espacioDisco[0].storage,lenVid/2),getBitsXOR(RAID[1].espacioDisco[0].storage,lenVid/2), getBitsXOR(RAID[1].espacioDisco[0].storage,lenVid/2).size(), getBitsXOR(RAID[0].espacioDisco[0].storage,lenVid/2).size());
    }
    
    //cout << *(RAID[0].espacioDisco[0].storage) << endl;
    int size = lenVid/2;
    int x[size];
    for (int j = 0; j < size; j++)
    {
        string bit_string;
        for (int i = 0; i < 8; i++)
        {
            ostringstream ss;
            ss << *(RAID[dato].espacioDisco[0].storage+((j*8)+i));
            bit_string = bit_string + ss.str();
                
        }
        bitset<8> b(bit_string);       // [1,0,1,0,1,0,1,0]
        unsigned char c = ( b.to_ulong() & 0xFF);
        int byte = (int)c;
        x[j]=byte;
        //cout << byte<< endl;
        //cout << bit_string << endl;
    }
    RAID[dato].espacioDisco[0].storage = x;
    cout << *(RAID[dato].espacioDisco[0].storage) << endl;

    int video[lenVid];
    for (int i = 0; i < lenVid/2; i++)
     {
         video[i] = *RAID[0].espacioDisco[0].storage+i;
     }
     for (int i = 0; i < lenVid/2; i++)
     {
         video[lenVid/2+i] = *RAID[1].espacioDisco[0].storage+i;
     }  
    generateVid(video,lenVid);
    return 0;
}