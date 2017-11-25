#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <thread>

using namespace std;
typedef class memoria{
public:
    char key[6]="vacio";
    void* memoria_real;
    bool activo;
    bool en_uso;
    memoria *memoriaSiguiente =NULL;
    int size;
    int espacioMemoria;
    int operacion =1000;
    bool operator==(memoria mem){
        if(memcmp(this->key,mem.key,6)==0){
            return true;
        }
        else{
            return false;
        }
    }
    bool operator!=(memoria mem){
        if(memcmp(this->key,mem.key,6)!=0){
            return true;
        }
        else{
            return false;
        }
    }
}*ptrmemoria;
ptrmemoria puntero;
ptrmemoria punteroCache;
void ordenarCache(ptrmemoria memoriaMover,ptrmemoria memoriaPasada);
void inicializarMemoria();
void verLista();
void insertarEnMemoria(ptrmemoria memoria);
void incializarCache();
void actualizarCache(ptrmemoria memoria);
void rm_delete(ptrmemoria mem);
ptrmemoria rm_get(char* key);
void timer();
void verMemoria();

int main(){
    timer();
    inicializarMemoria();
    incializarCache();
    verMemoria();
    int client, server;
    int portNum = 1500;
    bool isExit = false;
    bool endServer = false;
    int bufsize = 1024;
    char buffer[bufsize];
    ptrmemoria recivo = new(memoria);
    struct sockaddr_in server_aadr;
    socklen_t size;
    client = socket(AF_INET, SOCK_STREAM,0);
    if(client<0){
        cout <<"Error de coneccion."<< endl;
        exit(1);
    }
    cout <<"Socket creado coneccion establecida."<< endl;
    server_aadr.sin_family = AF_INET;
    server_aadr.sin_addr.s_addr = htons(INADDR_ANY);
    server_aadr.sin_port = htons(portNum);
    if(bind(client,(struct sockaddr*)&server_aadr,sizeof(server_aadr))<0){
        cout << "Error coneccion de socket"<<endl;
        exit(1);
    }
    size = sizeof(server_aadr);
    cout<<"Buscando usuario"<<endl;
    while(endServer==false){
        listen(client,1);
        //std::thread threadEscuchar accept(client,(struct sockaddr*)&server_aadr,&size);
        //std::thread threadEjecutar accionesMemoria(accept(client,(struct sockaddr*)&server_aadr,&size));
        server = accept(client,(struct sockaddr*)&server_aadr,&size);
        if(server<0){
            cout << "Error"<<endl;
            exit(1);
        }
        while(server>0){
            strcpy(buffer,"Server conectado\n");
            //send(server,buffer,bufsize,0);
            cout<<"Connectado a usuario"<<endl;
            cout << "Presion # para terminar coneccion"<<endl;
            cout <<"Usuario:" ;
            isExit=false;
            do{
                int salir=recv(server,recivo,48,0);
                cout<<recivo->operacion<<endl;
                if(salir==0){
                    isExit = true;
                    server=-1;
                    break;
                }

                if(recivo->operacion==1){
                    ptrmemoria punteroEnviar = rm_get(recivo->key);
                    if(punteroEnviar==NULL){
                        punteroEnviar=new(memoria);
                        strcpy(punteroEnviar->key,"vacio");
                        send(server,punteroEnviar,48,0);
                    }
                    else{
                        cout<<punteroEnviar->size;
                        uint8_t bytes[punteroEnviar->size]={0};
                        memcpy(bytes,punteroEnviar->memoria_real,punteroEnviar->size);
                        punteroEnviar->activo = true;
                        send(server,punteroEnviar,48,0);
                        send(server,bytes,punteroEnviar->size,0);
                    }
                    break;
                }
                if(recivo->operacion==2){
                    rm_delete(recivo);
                    verMemoria();
                    break;
                }
                if(recivo->operacion==3){
                    uint8_t bytes[recivo->size];
                    recv(server,bytes,recivo->size,0);
                    ptrmemoria punteroCambiar = rm_get(recivo->key);
                    memcpy(punteroCambiar->memoria_real,bytes,recivo->size);
                    cout<<*static_cast<int*>(punteroCambiar->memoria_real);
                    break;
                }
                else{
                    recivo->memoria_real = operator new(150);
                    uint8_t bytes[recivo->size];
                    recv(server,bytes,recivo->size,0);
                    memcpy(recivo->memoria_real,bytes,recivo->size);
                    insertarEnMemoria(recivo);
                    actualizarCache(recivo);
                    verMemoria();
                    cout<<recivo->key;
                    recivo->activo = true;
                    cout<<*static_cast<int*>(recivo->memoria_real);
                    isExit =true;
                }


            }while(isExit == false);

        }
    }

    /*
    while(*buffer != '*');

       do{
           cout<<"\nServer: ";
           do{
               cin>> buffer;
               send(server,buffer,bufsize,0);
               if(*buffer == '#'){
                   send(server,buffer,bufsize,0);
                   *buffer = '*';
                   isExit = true;
               }
           }while(*buffer != '*');
           cout << "Usuario: ";
           do{
               recv(server,recivo,40,0);
               insertarEnMemoria(recivo);
               cout<<recivo->size<<" ";
               if(*buffer == '#'){
                   *buffer == '*';
                   isExit = true;
               }
           }while(*buffer != '*');
       }while(!isExit);
       cout<<"Coneccion terminada"<<endl;
       isExit = false;
       exit(1);
       */
    //}
    //close(client);
    return 0;

}
void inicializarMemoria(){
    int y=40;
    int flag=1;
    puntero = new(memoria);
    puntero->memoria_real = operator new(y);
    puntero->espacioMemoria = y;
    int x = 1;
    ptrmemoria memActual = puntero;
    while(x!=33){
        if(flag==0){
            y=2;
            while(y%5!=0){
                y=rand() % 20+30;
            }
            ptrmemoria punteroNuevo = new(memoria);
            punteroNuevo->memoria_real = operator new(y);
            punteroNuevo->espacioMemoria = y;
            memActual->memoriaSiguiente = punteroNuevo;
            memActual = punteroNuevo;
            x = x+1;
            flag=1;

        }else{
            ptrmemoria punteroNuevo = new(memoria);
            punteroNuevo->memoria_real = operator new(125-y);
            punteroNuevo->espacioMemoria = 125-y;
            memActual->memoriaSiguiente = punteroNuevo;
            memActual = punteroNuevo;
            x = x+1;
            flag=0;
        }
    }
}
void incializarCache(){
    punteroCache = new(memoria);
    punteroCache->memoria_real = operator new(150);
    punteroCache->espacioMemoria = 150;
    int x = 1;
    ptrmemoria memActual = punteroCache;
    while(x!=5){
        ptrmemoria punteroNuevo = new(memoria);
        punteroNuevo->memoria_real = operator new(150);
        punteroNuevo->espacioMemoria = 150;
        memActual->memoriaSiguiente = punteroNuevo;
        memActual = punteroNuevo;
        x = x+1;
    }
}
void actualizarCache(ptrmemoria memoria){
    char comp[6] = "vacio";
    ptrmemoria memActual = punteroCache;
    while(memActual!=NULL){
        if(memcmp(memActual->key,memoria->key,6)==0){
            cout<<"No se peude insertar la llave ya que el nombre esta repetido";
            break;
        }
        if(memcmp(memActual->key,comp,6)==0){
            strcpy(memActual->key,memoria->key);
            memActual->size = memoria->size;
            memActual->memoria_real = memoria->memoria_real;
            memActual->activo = memoria->activo;
            memActual->en_uso = true;
            break;
        }
        else{
            memActual = memActual->memoriaSiguiente;
        }
    }
    if(memActual==NULL){
        int x = 5;
        int recorrer = 2;
        ptrmemoria memActual2 = punteroCache;
        ptrmemoria memPrevio = punteroCache;
        while(recorrer<=x){
            memPrevio = punteroCache;
            memActual2 = memActual2->memoriaSiguiente;
            recorrer+=1;
        }
        strcpy(memActual2->key,memoria->key);
        memActual2->size = memoria->size;
        memActual2->memoria_real = memoria->memoria_real;
        memActual2->activo = memoria->activo;
        memActual2->en_uso = true;
        ordenarCache(memActual2,memPrevio);
    }

}
ptrmemoria rm_get(char* key){
    ptrmemoria memCache = punteroCache;
    ptrmemoria memCachepasada = punteroCache;
    while(memCache!=NULL){
        char* comp = memCache->key;
        if (strcmp(comp,key)==0){
            if(memcmp(memCache->key,memCachepasada,6)==0){
                return memCache;
            }else{
                ordenarCache(memCache,memCachepasada);
                return memCache;
            }
        }
        else{
            memCachepasada = memCache;
            memCache = memCache->memoriaSiguiente;
        }
    }
    ptrmemoria memActual = puntero;
    while(memActual!=NULL){
        char* comp = memActual->key;
        if (strcmp(comp,key)==0){
            return memActual;
        }
        else{
            memActual = memActual->memoriaSiguiente;
        }
    }
    return NULL;
}

void rm_delete(ptrmemoria mem){
    ptrmemoria memCache = punteroCache;
    ptrmemoria memCachepasada = punteroCache;
    while(memCache != NULL){
        if(memcmp(memCache->key,mem->key,6)==0){
            if(memcmp(memCache->key,memCachepasada->key,6)==0){
                ::operator delete(memCache->memoria_real);
                memCache->memoria_real = operator new(memCache->espacioMemoria);
                strcpy(memCache->key,"vacio");
                memCache->size = 0;
                break;
            }else{
                ordenarCache(memCache,memCachepasada);
                ::operator delete(memCache->memoria_real);
                memCache->memoria_real = operator new(memCache->espacioMemoria);
                strcpy(memCache->key,"vacio");
                memCache->size = 0;
                break;
            }
        }
        else{
            memCache = memCache->memoriaSiguiente;
        }
    }
    ptrmemoria memActual = puntero;
    while(memActual!=NULL){
        if(memcmp(memActual->key,mem->key,6)==0){
            ::operator delete(memActual->memoria_real);
            memActual->memoria_real = operator new(memActual->espacioMemoria);
            strcpy(memActual->key,"vacio");
            memActual->size = 0;
            break;
        }
        else{
            memActual = memActual->memoriaSiguiente;
        }
    }
}
void ordenarCache(ptrmemoria memoriaMover,ptrmemoria memoriaPasada){
    memoriaPasada->memoriaSiguiente = memoriaMover->memoriaSiguiente;
    memoriaMover->memoriaSiguiente = punteroCache;
    punteroCache = memoriaMover;
}
void insertarEnMemoria(ptrmemoria memoria){
    char comp[6] = "vacio";
    ptrmemoria memActual = puntero;
    ptrmemoria mejorAjuste = puntero;
    while(memActual!=NULL){
        if(memcmp(memActual->key,memoria->key,6)==0){
            cout<<"No se peude insertar la llave ya que el nombre esta repetido";
            break;
        }
        else{
            if(memActual->espacioMemoria-memoria->size<mejorAjuste->espacioMemoria-memoria->size){
                if(memcmp(memActual->key,comp,6)==0){
                    mejorAjuste = memActual;
                }
            }

            memActual = memActual->memoriaSiguiente;
        }
    }
    if (memcmp(mejorAjuste->key,comp,6)==0){
        strcpy(mejorAjuste->key,memoria->key);
        mejorAjuste->size = memoria->size;
        mejorAjuste->memoria_real = memoria->memoria_real;
        mejorAjuste->activo = memoria->activo;
        mejorAjuste->en_uso = true;
    }
    else{
        cout<<"La memoria esta llena";
    }
}
void limpiarMemoria(){
    ptrmemoria memCache = punteroCache;
    ptrmemoria memCachepasada = punteroCache;
    while(memCache != NULL){
        if(memCache->activo==false){
            if(memcmp(memCache->key,memCachepasada->key,6)==0){
                ::operator delete(memCache->memoria_real);
                memCache->memoria_real = operator new(memCache->espacioMemoria);
                strcpy(memCache->key,"vacio");
                memCache->size = 0;
                memCache=memCache->memoriaSiguiente;
            }else{
                ordenarCache(memCache,memCachepasada);
                ::operator delete(memCache->memoria_real);
                memCache->memoria_real = operator new(memCache->espacioMemoria);
                strcpy(memCache->key,"vacio");
                memCache->size = 0;
                memCache=memCache->memoriaSiguiente;
            }
        }
        else{
            memCache->activo = false;
            memCache = memCache->memoriaSiguiente;
        }
    }

    ptrmemoria memActual = puntero;
    while(memActual!=NULL){
        if(memActual->activo==false){
            ::operator delete(memActual->memoria_real);
            memActual->memoria_real = operator new(memActual->espacioMemoria);
            strcpy(memActual->key,"vacio");
            memActual->size = 0;
            memActual = memActual->memoriaSiguiente;
        }
        else{
            memActual->activo=false;
            memActual = memActual->memoriaSiguiente;
        }
    }
    timer();
    cout<<"Limpia";
}
void alarm_handler(int signo){
    if(signo==SIGALRM);{
        limpiarMemoria();
    }
}
void timer(){
    signal(SIGALRM,alarm_handler);
    alarm(180);
}

void verMemoria(){
    char comp[6] = "vacio";
    cout<<"Estado de la memoria actualmente 1:Utilizada 0:Disponible, en los () se encuentra la cantidad de memoria de cada bloque"<<endl;
    ptrmemoria memCache = punteroCache;
    while(memCache!=NULL){
        if(memcmp(memCache->key,comp,6)==0){
            if(memCache->memoriaSiguiente==NULL){
                cout<<"0(" <<memCache->espacioMemoria<<") "<<endl;
                memCache=memCache->memoriaSiguiente;
            }else{
                cout<<"0(" <<memCache->espacioMemoria<<") ";
                memCache=memCache->memoriaSiguiente;
            }
        }
        else{
            if(memCache->memoriaSiguiente==NULL){
                cout<<"1(" <<memCache->espacioMemoria<<") "<<endl;
                memCache=memCache->memoriaSiguiente;
            }else{
                cout<<"1(" <<memCache->espacioMemoria<<") ";
                memCache=memCache->memoriaSiguiente;
            }
        }
    }

    ptrmemoria memActual = puntero;
    while(memActual!=NULL){
        if(memcmp(memActual->key,comp,6)==0){
            if(memActual->memoriaSiguiente==NULL){
                cout<<"0(" <<memActual->espacioMemoria<<")"<<endl;
                memActual=memActual->memoriaSiguiente;
            }else{
                cout<<"0(" <<memActual->espacioMemoria<<")";
                memActual=memActual->memoriaSiguiente;
            }
        }
        else{
            if(memActual->memoriaSiguiente==NULL){
                cout<<"1(" <<memActual->espacioMemoria<<")"<<endl;
                memActual=memActual->memoriaSiguiente;
            }else{
                cout<<"1(" <<memActual->espacioMemoria<<")";
                memActual=memActual->memoriaSiguiente;
            }
        }
    }

}
/**
void accionesMemoria(int server){
    int client;
    int portNum = 1500;
    bool isExit = false;
    bool endServer = false;
    int bufsize = 1024;
    char buffer[bufsize];
    ptrmemoria recivo = new(memoria);
    struct sockaddr_in server_aadr;
    socklen_t size;
    client = socket(AF_INET, SOCK_STREAM,0);
    server = accept(client,(struct sockaddr*)&server_aadr,&size);
    if(server<0){
        cout << "Error"<<endl;
        exit(1);
    }
    while(server>0){
        strcpy(buffer,"Server conectado\n");
        //send(server,buffer,bufsize,0);
        cout<<"Connectado a usuario"<<endl;
        cout << "Presion # para terminar coneccion"<<endl;
        cout <<"Usuario:" ;
        isExit=false;
        do{
            int salir=recv(server,recivo,48,0);
            cout<<recivo->operacion<<endl;
            if(salir==0){
                isExit = true;
                server=-1;
                break;
            }

            if(recivo->operacion==1){
                ptrmemoria punteroEnviar = rm_get(recivo->key);
                if(punteroEnviar==NULL){
                    punteroEnviar=new(memoria);
                    strcpy(punteroEnviar->key,"vacio");
                    send(server,punteroEnviar,48,0);
                }
                else{
                    cout<<punteroEnviar->size;
                    uint8_t bytes[punteroEnviar->size]={0};
                    memcpy(bytes,punteroEnviar->memoria_real,punteroEnviar->size);
                    punteroEnviar->activo = true;
                    send(server,punteroEnviar,48,0);
                    send(server,bytes,punteroEnviar->size,0);
                }
                break;
            }
            if(recivo->operacion==2){
                rm_delete(recivo);
                verMemoria();
                break;
            }
            if(recivo->operacion==3){
                uint8_t bytes[recivo->size];
                recv(server,bytes,recivo->size,0);
                ptrmemoria punteroCambiar = rm_get(recivo->key);
                memcpy(punteroCambiar->memoria_real,bytes,recivo->size);
                cout<<*static_cast<int*>(punteroCambiar->memoria_real);
                break;
            }
            else{
                recivo->memoria_real = operator new(150);
                uint8_t bytes[recivo->size];
                recv(server,bytes,recivo->size,0);
                memcpy(recivo->memoria_real,bytes,recivo->size);
                insertarEnMemoria(recivo);
                actualizarCache(recivo);
                verMemoria();
                cout<<recivo->key;
                recivo->activo = true;
                cout<<*static_cast<int*>(recivo->memoria_real);
                isExit =true;
            }


        }while(isExit == false);

        }
}
 */
