#include <filesystem>
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <cctype>
#include <cstring>

using namespace std;

namespace fs = filesystem;

string convertToUpper(const string& str);
vector<string> filterTypes(const string& str);

int main(int args, char** argv){
    fs::path currentPath = "./";

    if (!fs::exists(currentPath)) {
        std::cerr << "El directorio no existe.\n";
        return EXIT_FAILURE;
    }
    set<string> exception_files;

    if(args > 1){
        if(!strcmp(argv[1], "/help")){
            cout<<"Puedes usar el siguiente formato para poder obviar tipos de archivos\n\nFileOrganizer .txt/.pdf/.docx/.etc/ \n";
            return EXIT_SUCCESS;
        }
        vector<string> types = filterTypes(string(argv[1]));
        for(auto& type : types){
            exception_files.insert(type);
        }
    }


    set<string> extensions;
    for(const auto& entrie : fs::directory_iterator(currentPath)){ //Itera entre las rutas de los archivos
        if(fs::is_regular_file(entrie) && exception_files.count(entrie.path().extension().string()) == 0){
            string ext = entrie.path().extension().string();
            string directory = convertToUpper(ext.substr(1, ext.size()-1));
            if(extensions.count(ext) == 0){
                fs::create_directory(directory); //crea un directorio con la extension del archivo
                extensions.insert(ext);
            }

            string newPath = "./"  + directory + '/' + entrie.path().filename().string();
            fs::rename(entrie.path(), currentPath.string() + newPath);
        }
    }
    

    return EXIT_SUCCESS;
}

string convertToUpper(const string& str){
    string newstr;
    newstr.resize(str.size());
    transform(str.begin(), str.end(), newstr.begin(), [](const char& ch) {
        return toupper(ch);
    });

    return newstr;
}

vector<string> filterTypes(const string& str){
    vector<string> types;
    string current;
    for(auto& ch : str){
        current.push_back(ch);
        if(ch == '/'){
            current.pop_back();
            types.push_back(current);
            current.clear();
        }
    }

    return types;
}