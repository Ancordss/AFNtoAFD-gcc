// AFN_MENU.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include "parser/xml_parser.h"

using namespace std;

bool fileExists(const string& filename) {
    ifstream file(filename);
    return file.good();
}

std::vector<std::string> charArrayToVector(char **charArray) {
    std::vector<std::string> result;
    for (int i = 0; charArray[i] != NULL; i++) {
        result.push_back(std::string(charArray[i]));
    }
    return result;
}

void GenerateHelloWorldHTML() {
    ofstream htmlFile("output.html");
    if (htmlFile.is_open()) {
        htmlFile << "<html><body><h1>Hola Mundo</h1></body></html>";
        htmlFile.close();
        cout << "Se ha generado el archivo HTML 'output.html' con 'Hola Mundo' en grande." << endl;
    }
    else {
        cout << "No se pudo abrir el archivo HTML para escritura." << endl;
    }
}

void LoadXMLFile() {
    cout << "¿Desea generar el archivo XML? (Sí/No): ";
    string choice;
    cin >> choice;

    if (choice == "Sí" || choice == "si") {
        // Generar el archivo XML aquí (código no proporcionado).
        cout << "Generando archivo XML..." << endl;
    }
    else if (choice == "No" || choice == "no") {
        // Buscar un archivo HTML en la carpeta del proyecto.
        if (fileExists("output.html")) {
            cout << "Se encontró un archivo HTML en la carpeta del proyecto." << endl;
        }
        else {
            cout << "No se encontró un archivo HTML en la carpeta del proyecto." << endl;
        }
    }
    else {
        cout << "Opción no válida." << endl;
    }
}

void ShowLoadedData() {
    std::string filePath;
    // Implementar la lógica para mostrar los datos cargados (código no proporcionado).  // Clear the input buffer.
    std::cin.ignore();
    cout << "Enter the path of the XML file: " << endl;
    std::getline(std::cin, filePath);  // Use getline to read the entire line, including spaces

    // Check if the input begins and ends with double quotes
    if (!filePath.empty() && filePath.front() == '"' && filePath.back() == '"') {
        // Remove the double quotes
        filePath = filePath.substr(1, filePath.size() - 2);
        // Replace backslashes with forward slashes
        // std::replace(filePath.begin(), filePath.end(), '\\', '/');
    }

    try {
        FILE *xml_file_ptr = fopen(filePath.c_str(), "r");
        if (!xml_file_ptr) {
            std::cerr << "Error opening temporary file" << std::endl;
        }

        // Inicializa el parser con el archivo XML
        init_parser(xml_file_ptr);
        int result = parse_xml();
        if (result == 0) {
            std::cout << "Análisis exitoso." << std::endl;
        }
        }
    catch (const std::exception& e) {
        cout << e.what() << endl;
    }
            
    cout << "Mostrando datos cargados..." << endl;

    char **alfabeto_tokens = get_tokens_linearlayout();
        std::vector<std::string> alfabeto_tokens_vector;
        if (alfabeto_tokens != NULL) {
            alfabeto_tokens_vector = charArrayToVector(alfabeto_tokens);
            if (!alfabeto_tokens_vector.empty()) {
                std::cout << "Tokens de alfabeto:" << std::endl;
                for (const std::string& token : alfabeto_tokens_vector) {
                    std::cout << token << std::endl;
                }
            } else {
                std::cout << "No se encontraron tokens de alfabeto." << std::endl;
            }
        }

    char **estados_atributos_tokens = get_tokens_estadosattributes();
        std::vector<std::string> estados_atributos_tokens_vector;
        if (estados_atributos_tokens != NULL) {
            estados_atributos_tokens_vector = charArrayToVector(estados_atributos_tokens);
            if (!estados_atributos_tokens_vector.empty()) {
                std::cout << "Tokens de estados atributos:" << std::endl;
                for (const std::string& token : estados_atributos_tokens_vector) {
                    std::cout << token << std::endl;
                }
            } else {
                std::cout << "No se encontraron tokens de estados atributos." << std::endl;
            }
        }
    
    char **transiconales_tokens = get_tokens_transcicionales();
        std::vector<std::string> transiconales_tokens_vector;
        if (transiconales_tokens != NULL) {
            transiconales_tokens_vector = charArrayToVector(transiconales_tokens);
            if (!transiconales_tokens_vector.empty()) {
                std::cout << "Tokens de transiciones:" << std::endl;
                for (const std::string& token : transiconales_tokens_vector) {
                    std::cout << token << std::endl;
                }
            } else {
                std::cout << "No se encontraron tokens de transiciones." << std::endl;
            }
        }
}

void ShowAFN() {
    // Implementar la lógica para mostrar el AFN (código no proporcionado).
    cout << "Mostrando AFN..." << endl;
}

void AFNToAFD() {
    // Implementar la lógica para convertir AFN a AFD (código no proporcionado).
    cout << "Convirtiendo AFN a AFD..." << endl;
}

void ShowAFD() {
    // Implementar la lógica para mostrar el AFD (código no proporcionado).
    cout << "Mostrando AFD..." << endl;
}

int main() {
    int choice;

    while (true) {
        cout << "----------Menu------------" << endl;
        cout << "1. Load XML File" << endl;
        cout << "2. Show Loaded Data" << endl;
        cout << "3. Show AFN" << endl;
        cout << "4. AFN To AFD" << endl;
        cout << "5. Show AFD" << endl;
        cout << "6. Generate File" << endl;
        cout << "7. Exit" << endl;
        cout << "---------------------------" << endl;
        cout << "Seleccione una opcion: ";
        cin >> choice;

        switch (choice) {
        case 1:
            LoadXMLFile();
            break;
        case 2:
            ShowLoadedData();
            break;
        case 3:
            ShowAFN();
            break;
        case 4:
            AFNToAFD();
            break;
        case 5:
            ShowAFD();
            break;
        case 6:
            GenerateHelloWorldHTML();
            break;
        case 7:
            cout << "Saliendo del programa." << endl;
            return 0;
        default:
            cout << "Opción no válida. Por favor, seleccione una opción válida." << endl;
            break;
        }
    }

    return 0;
}

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
