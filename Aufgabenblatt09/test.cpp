#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;


void write_int_to_file() {
    fstream file;
    file.open("boggadbv2", ios::out);
    int i = 123;
    file.write((char *)&i, sizeof(i));
    file.close();
}

void read_int_from_file() {
    int i;
    ifstream file("boggadbv2", ios::in);
    if (file.is_open())
    {
        file.read((char *)&i, sizeof(i));
        cout <<"file is " <<  i << endl;
    }

    file.close();
}


int main() {
    read_int_from_file();
    write_int_to_file();

    return 0;
}