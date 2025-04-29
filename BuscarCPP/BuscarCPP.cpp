#include <iostream>
#include <Windows.h>
#include <fstream>
#include <cstring>
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;

bool fileTypeFilter(const char* filename, const char* ext) {
    int filenameSize = strlen(filename);
    int extSize = strlen(ext);

    //Verifica si el nombre completo con la extension es mayor que la misma extension
    if (extSize >= filenameSize) return false;

    for (int i = 0; i < extSize; ++i) {
        if (filename[filenameSize-extSize+i] != ext[i]) return false;
    }

    return true;
}

int main(int args, const char** argv) {
	if (args < 2) {
		cout << "Debe pasar la palabra que desea buscar y el tipo de archivo en que quiere buscar \n";
		return EXIT_FAILURE;
	}

    string word = argv[1];
    WIN32_FIND_DATAA datos; // <--- OJO: WIN32_FIND_DATAA (ASCII version)
    HANDLE handle = FindFirstFileA("./*", &datos); // <--- FindFirstFileA

    if (handle == INVALID_HANDLE_VALUE) {
        cout << "No se pudo abrir la carpeta." << endl;
        return EXIT_FAILURE;
    }

    char filesname[20][256];
    int filesCounter = 0; //Cuenta los archivos
    do {
        if (fileTypeFilter(datos.cFileName, argv[2])) {
            for (int i = 0; i < strlen(datos.cFileName); ++i) {
                filesname[filesCounter][i] = datos.cFileName[i];
            }
            filesname[filesCounter][strlen(datos.cFileName)] = '\0';
            ++filesCounter;
        }

    } while (FindNextFileA(handle, &datos)); // <--- FindNextFileA
    
    cout << "Archivos contados: " << filesCounter<<endl;

    int fileIterator = 0;
    int finderCounter = 0;
    while (fileIterator < filesCounter) {
        ifstream file;
        file.open(filesname[fileIterator]);
        if (!file.is_open()) {
            cout << "Error al abrir el archivo...";
            return EXIT_FAILURE;
        }
        
        string line;
        int lineNumber = 0;
        cout << "Archivo: " << filesname[fileIterator] <<endl;
        while (getline(file, line)) {
            ++lineNumber;
            string line2;
            line2.resize(line.size());
            transform(line.begin(), line.end(), line2.begin(), [](char x) {
                return tolower(x);
            });
            if (line2.find(word) != string::npos) {
                cout << "-\t\t Se encontro en la linea " << lineNumber << endl;
                finderCounter++;
            }
        }
        file.close();
        ++fileIterator;
    }

    cout << "Se encontro la palabra " << finderCounter <<" veces. " << endl;

    FindClose(handle);
    return EXIT_SUCCESS;

}
