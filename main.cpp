#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include "parser/xml_parser.h"
#include <sstream> 
#include <fstream>  // Para escribir el archivo DOT
#include <cstdlib>

#include <set>
#include <map>

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


// Función para dividir una cadena en un vector de cadenas usando un delimitador
std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}


// Definir estructuras para el AFN
struct Estado {
    int id;
    bool esFinal;
};

struct Transicion {
    int estadoActual;
    char simbolo;
    int estadoSiguiente;
};

void GenerarArchivoDOT(
    const std::vector<std::string>& alfabeto_tokens_vector,
    const std::vector<std::string>& estados_atributos_tokens_vector,
    const std::vector<std::string>& transiciones_tokens_vector,
    const std::vector<std::string>& finalesattributes_vector
) {
    std::ofstream archivoDOT("afn.dot");
    if (!archivoDOT) {
        std::cerr << "Error al abrir el archivo DOT." << std::endl;
        return;
    }

    archivoDOT << "digraph AFN {" << std::endl;
    archivoDOT << "rankdir=LR;" << std::endl;
    archivoDOT << "node [shape=ellipse];" << std::endl;

    // Agregar nodos
    
    for (const std::string& estado_str : estados_atributos_tokens_vector) {
        std::cout << estado_str << std::endl;
        int estado = std::stoi(estado_str);
        cout << estado << std::endl;
        std::string forma = "ellipse";
        bool esEstadoFinal = false;
        for (const std::string& estado_final : finalesattributes_vector) {
            if (estado_final == estado_str) {
                esEstadoFinal = true;
                break;
            }
        }
        if (esEstadoFinal) {
            forma = "doublecircle";
        }
        archivoDOT << estado << " [shape=" << forma << "];" << std::endl;
    }

    // Agregar arcos
    for (const std::string& transicion_str : transiciones_tokens_vector) {
        std::vector<std::string> partes = split(transicion_str, ',');
        if (partes.size() == 3) {
            cout << transicion_str << endl;
            int estadoActual = std::stoi(partes[0]);
            std::string simbolo = partes[1];
            int estadoSiguiente = std::stoi(partes[2]);
            archivoDOT << estadoActual << " -> " << estadoSiguiente << " [label=\"" << simbolo << "\"];" << std::endl;
        }
    }

    archivoDOT << "}" << std::endl;
    archivoDOT.close();

    // Generar el gráfico utilizando Graphviz
    system("dot -Tpng afn.dot -o afn.png");
}

void show_png(const std::string& rutaImagen) {
    std::ofstream archivoHTML("imagen.html");
    if (!archivoHTML) {
        std::cerr << "Error al abrir el archivo HTML." << std::endl;
        return;
    }

    archivoHTML << "<!DOCTYPE html>" << std::endl;
    archivoHTML << "<html>" << std::endl;
    archivoHTML << "<head>" << std::endl;
    archivoHTML << "<title>Imagen AFN</title>" << std::endl;
    archivoHTML << "</head>" << std::endl;
    archivoHTML << "<body>" << std::endl;
    archivoHTML << "<h1>Imagen AFN</h1>" << std::endl;
    archivoHTML << "<img src=\"" << rutaImagen << "\" alt=\"AFN\">" << std::endl;
    archivoHTML << "</body>" << std::endl;
    archivoHTML << "</html>" << std::endl;

    archivoHTML.close();
    std::cout << "Archivo HTML creado correctamente." << std::endl;

    
}

    //const char* archivo = "parser/vitacora_tokens.html";  // Abre el archivo en el navegador predeterminado
    // std::string comando = "start " + filename;
    // system(comando.c_str());



void show_filehtml(
    const std::string filename) {

    //const char* archivo = "parser/vitacora_tokens.html";

    // Abre el archivo en el navegador predeterminado
    std::string comando = "start " + filename;
    system(comando.c_str());
}

// Función para crear un archivo HTML con los datos del AFN
void CrearArchivoHTML(
    const std::vector<std::string>& alfabeto_tokens_vector,
    const std::vector<std::string>& estados_atributos_tokens_vector,
    const std::vector<std::string>& transiciones_tokens_vector,
    const std::vector<std::string>& finalesattributes_vector,
    const std::string filename
) {
    std::ofstream archivoHTML(filename); // Nombre del archivo HTML

    // Verificar si se pudo abrir el archivo
    if (!archivoHTML.is_open()) {
        std::cerr << "No se pudo abrir el archivo HTML." << std::endl;
        return;
    }

    // Escribir el contenido HTML en el archivo
    archivoHTML << "<!DOCTYPE html>\n";
    archivoHTML << "<html>\n";
    archivoHTML << "<head>\n";
    archivoHTML << "<title>AFN</title>\n";
    archivoHTML << "</head>\n";
    archivoHTML << "<body>\n";

    // Escribir la información del AFN en el archivo HTML
    archivoHTML << "<h1>Información del AFN</h1>\n";

    archivoHTML << "<h2>Conjunto de estados:</h2>\n";
    archivoHTML << "<ul>\n";
    for (const std::string& estado_str : estados_atributos_tokens_vector) {
        int estado = std::stoi(estado_str);
        archivoHTML << "<li>Estado " << estado << "</li>\n";
    }
    archivoHTML << "</ul>\n";

    archivoHTML << "<h2>Alfabeto:</h2>\n";
    archivoHTML << "<ul>\n";
    for (const std::string& simbolo_str : alfabeto_tokens_vector) {
        char simbolo = simbolo_str[0];
        archivoHTML << "<li>Símbolo " << simbolo << "</li>\n";
    }
    archivoHTML << "</ul>\n";

    archivoHTML << "<h2>Estados finales:</h2>\n";
    archivoHTML << "<ul>\n";
    for (const std::string& token : finalesattributes_vector) {
        int estadofinal = std::stoi(token);
        archivoHTML << "<li>Estado final " << estadofinal << "</li>\n";
    }
    archivoHTML << "</ul>\n";

    archivoHTML << "<h2>Tabla de transiciones:</h2>\n";
    archivoHTML << "<table border='1'>\n";
    archivoHTML << "<tr><th>Estado Actual</th><th>Símbolo</th><th>Estado Siguiente</th></tr>\n";
    for (const std::string& transicion_str : transiciones_tokens_vector) {
        std::vector<std::string> partes = split(transicion_str, ',');
        if (partes.size() == 3) {
            archivoHTML << "<tr><td>" << partes[0] << "</td><td>" << partes[1] << "</td><td>" << partes[2] << "</td></tr>\n";
        }
    }
    archivoHTML << "</table>\n";

    archivoHTML << "</body>\n";
    archivoHTML << "</html>\n";

    // Cerrar el archivo
    archivoHTML.close();

}

// Función para procesar los tokens y construir el AFN
void ProcesarAFN(
    const std::vector<std::string>& alfabeto_tokens_vector,
    const std::vector<std::string>& estados_atributos_tokens_vector,
    const std::vector<std::string>& transiciones_tokens_vector,
    const std::vector<std::string>& finalesattributes_vector

) {
    // Construir el conjunto de estados
    std::set<int> estados;
    for (const std::string& estado_str : estados_atributos_tokens_vector) {
        int estado = std::stoi(estado_str);
        estados.insert(estado);
    }

    // Construir el alfabeto
    std::set<char> alfabeto;
    for (const std::string& simbolo_str : alfabeto_tokens_vector) {
        char simbolo = simbolo_str[0];
        alfabeto.insert(simbolo);
    }

    // Construir el conjunto de estados finales
    std::set<int> estadosFinales;
    for (const std::string& token : finalesattributes_vector) {
        int estadofinal = std::stoi(token);
        estadosFinales.insert(estadofinal);
    }

    // Construir la tabla de transiciones
    std::vector<Transicion> tablaTransiciones;
    for (const std::string& transicion_str : transiciones_tokens_vector) {
        std::vector<std::string> partes = split(transicion_str, ',');
        if (partes.size() == 3) {
            Transicion transicion;
            transicion.estadoActual = std::stoi(partes[0]);
            transicion.simbolo = partes[1][0];
            transicion.estadoSiguiente = std::stoi(partes[2]);
            tablaTransiciones.push_back(transicion);
        }
    }

    // Mostrar la información del AFN

    std::cout << "Conjunto de estados:" << std::endl;
    for (int estado : estados) {
        std::cout << estado << " ";
    }
    std::cout << std::endl;

    std::cout << "Alfabeto:" << std::endl;
    for (char simbolo : alfabeto) {
        std::cout << simbolo << " ";
    }
    std::cout << std::endl;

    std::cout << "Estados finales:" << std::endl;
    for (int estado : estadosFinales) {
        std::cout << estado << " ";
    }
    std::cout << std::endl;

    std::cout << "Tabla de transiciones:" << std::endl;
    for (const Transicion& transicion : tablaTransiciones) {
        std::cout << transicion.estadoActual << " -> " << transicion.simbolo << " -> " << transicion.estadoSiguiente << std::endl;
    }


    CrearArchivoHTML(alfabeto_tokens_vector, estados_atributos_tokens_vector, transiciones_tokens_vector,finalesattributes_vector,"afn.html");
    show_filehtml("afn.html");
    GenerarArchivoDOT(alfabeto_tokens_vector, estados_atributos_tokens_vector, transiciones_tokens_vector,finalesattributes_vector);
}


void GenerateHelloWorldXML() {
    ofstream xmlFile("output.xml");
    if (xmlFile.is_open()) {
        xmlFile << "<AUTOMATA_AFN>\n";
        xmlFile << "<ALFABETO>\n";
        xmlFile << "</ALFABETO>\n";
        xmlFile << "<ESTADO>\n";
        xmlFile << "</ESTADO>\n";
        xmlFile << "<INICIAL>\n";
        xmlFile << "</INICIAL>\n";
        xmlFile << "<FINAL>\n";
        xmlFile << "</FINAL>\n";
        xmlFile << "<TRANSICIONES>\n";
        xmlFile << "</TRANSICIONES>\n";
        xmlFile << "</AUTOMATA_AFN>\n";
        xmlFile.close();
        cout << "Se ha generado el archivo XML 'output.xml'" << endl;
    } else {
        cout << "No se pudo abrir el archivo XML para escritura." << endl;
    }
}


void LoadXMLFile() {
    cout << "¿Desea cargar un archivo existente? (Cargar): ";
    string choice;
    cin >> choice;

     if (choice == "Cargar" || choice == "cargar") {
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
        // Implementa la lógica para cargar el archivo XML aquí (código no proporcionado).
        cout << "Cargando archivo XML desde '" << filePath << "'..." << endl;
    }
    else {
        cout << "Opción no válida." << endl;
    }
}



void ShowLoadedData() {            
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

    char **finalesattributes_tokens = get_tokens_finalesattributes();
        std::vector<std::string> finalesattributes_vector;
        if (finalesattributes_tokens != NULL) {
            finalesattributes_vector = charArrayToVector(finalesattributes_tokens);
            if (!alfabeto_tokens_vector.empty()) {
                std::cout << "Tokens de finalesattributes:" << std::endl;
                for (const std::string& token : finalesattributes_vector) {
                    std::cout << token << std::endl;
                }
            } else {
                std::cout << "No se encontraron tokens finalesattributes." << std::endl;
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
    
    ProcesarAFN(alfabeto_tokens_vector, estados_atributos_tokens_vector, transiconales_tokens_vector,finalesattributes_vector);
}

void GenerateHTMLWithData(const std::vector<std::string>& data1, const std::vector<std::string>& data2, const std::vector<std::string>& data3) {
    ofstream htmlFile("output.html");
    if (htmlFile.is_open()) {
        htmlFile << "<html><body>" << endl;
        
        // Mostrar datos del primer array en h1
        htmlFile << "<h1>Datos del primer array:</h1>" << endl;
        for (const std::string& item : data1) {
            htmlFile << "<p>" << item << "</p>" << endl;
        }
        
        // Mostrar datos del segundo array en h2
        htmlFile << "<h2>Datos del segundo array:</h2>" << endl;
        for (const std::string& item : data2) {
            htmlFile << "<p>" << item << "</p>" << endl;
        }
        
        // Mostrar datos del tercer array en texto normal
        htmlFile << "<p>Datos del tercer array:</p>" << endl;
        for (const std::string& item : data3) {
            htmlFile << "<p>" << item << "</p>" << endl;
        }
        
        htmlFile << "</body></html>" << endl;
        htmlFile.close();
        cout << "Se ha generado el archivo HTML 'output.html'" << endl;
    } else {
        cout << "No se pudo abrir el archivo HTML para escritura." << endl;
    }
}

void ShowAFN() {
    // Implementa la lógica para mostrar el AFN (código no proporcionado).
    cout << "Mostrando AFN..." << endl;
}

void AFNToAFD() {
    // Implementa la lógica para convertir AFN a AFD (código no proporcionado).
    cout << "Convirtiendo AFN a AFD..." << endl;
}

void ShowAFD() {
    // Implementa la lógica para mostrar el AFD (código no proporcionado).
    cout << "Mostrando AFD..." << endl;
}

int main() {
    int choice;

    cout << "----------Menu Inicial------------" << endl;
    cout << "1. Cargar Archivo XML" << endl;
    cout << "2. Crear Archivo XML Existente" << endl;
    cout << "3. Salir" << endl;
    cout << "-----------------------------------" << endl;
    cout << "Seleccione una opcion: ";
    cin >> choice;

    switch (choice) {
    case 1:
        LoadXMLFile();
        break;
    case 2:
        GenerateHelloWorldXML();
        break;
    case 3:
        cout << "Saliendo del programa." << endl;
        return 0;
    default:
        cout << "Opción no válida. Por favor, seleccione una opción valida." << endl;
        break;
    }

    while (true) {
        std::vector<std::string> alfabeto_tokens_vector = charArrayToVector(get_tokens_linearlayout());
        std::vector<std::string> estados_atributos_tokens_vector = charArrayToVector(get_tokens_estadosattributes());
        std::vector<std::string> transiconales_tokens_vector = charArrayToVector(get_tokens_transcicionales());
        cout << "----------Menu Principal------------" << endl;
        cout << "1. Mostrar Vitacora" << endl;
        cout << "2. Mostrar AFN" << endl;
        cout << "3. Convertir AFN a AFD" << endl;
        cout << "4. Mostrar AFD" << endl;
        cout << "6. Salir" << endl;
        cout << "7. Capturar arrays" << endl;
        cout << "8. Mostrar dibujito" << endl;
        cout << "--------------------------------------" << endl;
        cout << "Seleccione una opcion: ";
        cin >> choice;

        switch (choice) {
        case 1:
            //LoadXMLFile();
            show_filehtml("parser/vitacora_tokens.html");
            break;
        case 2:
            ShowLoadedData();
            break;
        case 3:
            AFNToAFD();
            break;
        case 4:
            ShowAFD();
            break;
        case 5:
            GenerateHelloWorldXML();
            break;
        case 6:
            cout << "Saliendo del programa." << endl;
            return 0;
        case 7:
            GenerateHTMLWithData(alfabeto_tokens_vector, estados_atributos_tokens_vector, transiconales_tokens_vector);
            break;
        case 8:
            
            show_png("afn.png");
            show_filehtml("imagen.html");
        break;
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
/*#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include "parser/xml_parser.h"

using namespace std;

// Define una función para extraer valores de un char** y guardarlos en un vector de strings
std::vector<std::string> extractValues(char **charArray) {
    std::vector<std::string> result;
    if (charArray != NULL) {
        for (int i = 0; charArray[i] != NULL; i++) {
            result.push_back(std::string(charArray[i]));
        }
    }
    return result;
}

// Define una función para generar un archivo HTML con los datos
void GenerateHTMLWithData(const std::vector<std::string>& data1, const std::vector<std::string>& data2, const std::vector<std::string>& data3) {
    ofstream htmlFile("output.html");
    if (htmlFile.is_open()) {
        htmlFile << "<html><body>" << endl;
        
        // Mostrar datos del primer array en h1
        htmlFile << "<h1>Datos del primer array:</h1>" << endl;
        for (const std::string& item : data1) {
            htmlFile << "<p>" << item << "</p>" << endl;
        }
        
        // Mostrar datos del segundo array en h2
        htmlFile << "<h2>Datos del segundo array:</h2>" << endl;
        for (const std::string& item : data2) {
            htmlFile << "<p>" << item << "</p>" << endl;
        }
        
        // Mostrar datos del tercer array en texto normal
        htmlFile << "<p>Datos del tercer array:</p>" << endl;
        for (const std::string& item : data3) {
            htmlFile << "<p>" << item << "</p>" << endl;
        }
        
        htmlFile << "</body></html>" << endl;
        htmlFile.close();
        cout << "Se ha generado el archivo HTML 'output.html'" << endl;
    } else {
        cout << "No se pudo abrir el archivo HTML para escritura." << endl;
    }
}

int main() {
    // ... Código previo ...

    // Llama a las funciones para extraer valores y generar el archivo HTML
    std::vector<std::string> alfabeto_tokens_vector = extractValues(get_tokens_linearlayout());
    std::vector<std::string> estados_atributos_tokens_vector = extractValues(get_tokens_estadosattributes());
    std::vector<std::string> transiconales_tokens_vector = extractValues(get_tokens_transcicionales());

    GenerateHTMLWithData(alfabeto_tokens_vector, estados_atributos_tokens_vector, transiconales_tokens_vector);

    // ... Resto del código ...

    return 0;
}
*/