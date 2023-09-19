#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include "parser/xml_parser.h" // Asegúrate de incluir el encabezado adecuado

using namespace std;

std::vector<std::string> charArrayToVector(char **charArray) {
    std::vector<std::string> result;
    for (int i = 0; charArray[i] != NULL; i++) {
        result.push_back(std::string(charArray[i]));
    }
    return result;
}


int main() {
    // Abre el archivo XML para analizar (cambia el nombre del archivo según tu caso)
    // std::ifstream xml_file("CodeXML.xml");
    // if (!xml_file.is_open()) {
    //     std::cerr << "Error opening file" << std::endl;
    //     return 1;
    // }

    // // Crear un archivo temporal para copiar el contenido del std::ifstream
    // std::ofstream temp_file("temp.xml");
    // temp_file << xml_file.rdbuf();
    // temp_file.close();



    std::string filePath;
    // Implementar la lógica para mostrar los datos cargados (código no proporcionado).
  // Clear the input buffer
    cout << "Enter the path of the XML file: " << endl;
    std::getline(std::cin, filePath);  // Use getline to read the entire line, including spaces

    // Check if the input begins and ends with double quotes
    if (!filePath.empty() && filePath.front() == '"' && filePath.back() == '"') {
        // Remove the double quotes
        filePath = filePath.substr(1, filePath.size() - 2);
        // Replace backslashes with forward slashes
        // std::replace(filePath.begin(), filePath.end(), '\\', '/');
    }
    // Abre el archivo temporal con FILE*
    FILE *xml_file_ptr = fopen(filePath.c_str(), "r");
    if (!xml_file_ptr) {
        std::cerr << "Error opening temporary file" << std::endl;
        return 1;
    }

    // Inicializa el parser con el archivo XML
    init_parser(xml_file_ptr);

    // Realiza el análisis del XML
    int result = parse_xml();
    if (result == 0) {
        std::cout << "Análisis exitoso." << std::endl;

        // Obtiene los tokens de tokens_linearlayout
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

        // Obtener los tokens de estados_atributos
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

        // Obtener los tokens de tokens_iniciales
        char **tokens_iniciales = get_tokens_iniciales();
        std::vector<std::string> tokens_iniciales_vector;
        if (tokens_iniciales != NULL) {
            tokens_iniciales_vector = charArrayToVector(tokens_iniciales);
            if (!tokens_iniciales_vector.empty()) {
                std::cout << "Tokens iniciales:" << std::endl;
                for (const std::string& token : tokens_iniciales_vector) {
                    std::cout << token << std::endl;
                }
            } else {
                std::cout << "No se encontraron tokens iniciales." << std::endl;
            }
        }

        // Obtener los tokens de tokens_finales
        char **tokens_finales = get_tokens_finalesattributes();
        std::vector<std::string> tokens_finales_vector;
        if (tokens_finales != NULL) {
            tokens_finales_vector = charArrayToVector(tokens_finales);
            if (!tokens_finales_vector.empty()) {
                std::cout << "Tokens finales:" << std::endl;
                for (const std::string& token : tokens_finales_vector) {
                    std::cout << token << std::endl;
                }
            } else {
                std::cout << "No se encontraron tokens finales." << std::endl;
            }
        }

        // Obtener los tokens de transiconales_tokens
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

        // Continúa de manera similar para los otros conjuntos de tokens

    } else {
        std::cout << "Análisis fallido." << std::endl;
    }

    // Cierra el archivo automáticamente al salir del ámbito

    return 0;
}
