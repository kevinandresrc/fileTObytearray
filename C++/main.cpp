#include <iostream>
#include <dirent.h>
#include <vector>

using namespace std;

vector<string> elementos;

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
    //if (elem != "." && elem != "..") cout << elem << endl;  
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
}

int main(int argc, char *argv[])
{
 init();
 for (int i = 0; i < elementos.size(); i++)
 {
 	cout << elementos.at(i) << endl;
 }
 return 0;
}
