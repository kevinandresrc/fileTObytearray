//
// Created by kevin on 25/11/17.
//

#include <iostream>
#include <string>
#include <bitset>

int main() {

    std::string bit_string = "10101010";
    bit_string+=__STRING(1); //Manera para agregar un int al string
    std::cout<<bit_string<<std::endl;
    std::bitset<8> b(bit_string);       // [1,0,1,0,1,0,1,0]
    unsigned char c = ( b.to_ulong() & 0xFF);
    int byte = (int)c;
    std::cout << byte<<std::endl; // prints 170

    return 0;
}
