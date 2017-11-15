#include <iostream>
#include <fstream>
#include <vector>


char* readFileBytes(const char *name);
typedef unsigned char byte;

int main() {
    std::cout << "Hello, World!" << std::endl;
    char* abc = readFileBytes("/home/kevin/Mamut.mp4");
    std::cout<<abc<<std::endl;
    return 0;
}

//C
/*
char* readFileBytes(const char *name){
    FILE *fl = fopen(name,"r");
    fseek(fl, 0, SEEK_END);
    long len = ftell(fl);
    char *ret = static_cast<char *>(malloc(len));
    fseek(fl,0,SEEK_END);
    fread(ret,1,len,fl);
    fclose(fl);
    return ret;

}
*/

//C++

char* readFileBytes(const char *name){
    /*
    std::ifstream fl(name);
    fl.seekg(0,std::ios::end);
    size_t len = fl.tellg();
    char * ret = new char[len];
    fl.seekg(0,std::ios::beg);
    fl.read(ret,len);
    fl.close();
    std::cout<<sizeof(ret)<<std::endl;
    return ret;
    */

    // open the file
    std::ifstream file;
    file.open(name);
    if(!file.is_open())
        return NULL;

    // get the length of the file
    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // create a vector to hold all the bytes in the file
    std::vector<char> data(fileSize, 0);
    printf(&data[0]);


    // read the file
    file.read(&data[0], fileSize);

}
