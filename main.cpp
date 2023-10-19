#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include "parser/xml_parser.h"
#include <sstream> 
#include <fstream>  // Para escribir el archivo DOT
#include <cstdlib>
#include <unordered_set>
#include <unordered_map>
#include <algorithm> 

#include <set>
#include <map>
#include <queue>

using namespace std;



char **alfabeto_tokens;
char **finalesattributes_tokens;
char **estados_atributos_tokens;
char **transiconales_tokens;
char **iniciales_tokens;

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


std::set<std::string> EpsilonClosure(const std::set<std::string>& estados, const std::map<std::string, std::set<std::string>>& transiciones_epsilon) {
    std::set<std::string> cierre_epsilon = estados;
    std::set<std::string> por_procesar = estados;

    while (!por_procesar.empty()) {
        std::string estado = *por_procesar.begin();
        por_procesar.erase(estado);

        if (transiciones_epsilon.find(estado) != transiciones_epsilon.end()) {
            const std::set<std::string>& transiciones = transiciones_epsilon.at(estado);

            for (const std::string& siguiente_estado : transiciones) {
                if (cierre_epsilon.find(siguiente_estado) == cierre_epsilon.end()) {
                    cierre_epsilon.insert(siguiente_estado);
                    por_procesar.insert(siguiente_estado);
                }
            }
        }
    }

    return cierre_epsilon;
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

class EstadoAFN {
public:
    int estado;
    char simbolo;
};

// class TransicionAFN {
// public:
//     EstadoAFN origen;
//     EstadoAFN destino;
// };
struct TransicionAFN {
    int origen;
    char simbolo;
    int destino;
};

struct Transition {
    std::string from;
    std::string symbol;
    std::string to;
};


// Definición de una transición en el AFN
struct AFNTransition {
    string fromState;
    string inputSymbol;
    string toState;
};

// Función para convertir un AFN en un AFD
map<set<string>, map<string, set<string>>> convertToAFD(
    vector<AFNTransition>& afnTransitions,
    set<string>& afnAcceptStates,
    vector<string>& alphabet,
    string startState
) {
    map<set<string>, map<string, set<string>>> afdTransitions;
    queue<set<string>> statesQueue;
    map<set<string>, bool> visited;

    // Calcula la clausura-épsilon del estado inicial
    set<string> initialState;
    initialState.insert(startState);
    statesQueue.push(initialState);
    visited[initialState] = true;

    while (!statesQueue.empty()) {
        set<string> currentState = statesQueue.front();
        statesQueue.pop();

        for (const string& symbol : alphabet) {
            set<string> nextState;
            for (const string& state : currentState) {
                for (const AFNTransition& transition : afnTransitions) {
                    if (transition.fromState == state && (transition.inputSymbol == symbol || transition.inputSymbol == "&")) {
                        nextState.insert(transition.toState);
                    }
                }
            }

            if (!nextState.empty() && !visited[nextState]) {
                statesQueue.push(nextState);
                visited[nextState] = true;
            }

            afdTransitions[currentState][symbol] = nextState;
        }
    }

    return afdTransitions;
}







void GenerarArchivoDOT_AFD(
    const std::vector<std::string>& alfabeto_tokens_vector,
    const std::vector<std::string>& estados_atributos_tokens_vector,
    const std::vector<std::string>& transiciones_tokens_vector,
    const std::string& estado_inicial,
    const std::vector<std::string>& estados_finales
) {
    std::ofstream archivoDOT("afd.dot");
    if (!archivoDOT) {
        std::cerr << "Error al abrir el archivo DOT." << std::endl;
        return;
    }

    archivoDOT << "digraph AFD {" << std::endl;
    archivoDOT << "rankdir=LR;" << std::endl;
    archivoDOT << "node [shape=ellipse];" << std::endl;

    // Agregar nodos
    for (const std::string& estado_str : estados_atributos_tokens_vector) {
        std::string forma = "ellipse";
        bool esEstadoFinal = (std::find(estados_finales.begin(), estados_finales.end(), estado_str) != estados_finales.end());

        if (estado_str == estado_inicial) {
            archivoDOT << estado_str << " [shape=point];" << std::endl;
        }

        if (esEstadoFinal) {
            forma = "doublecircle";
        }
        archivoDOT << estado_str << " [shape=" << forma << "];" << std::endl;
    }

    // Agregar arcos
    for (const std::string& transicion_str : transiciones_tokens_vector) {
        std::vector<std::string> partes = split(transicion_str, ',');
        if (partes.size() == 3) {
            std::string estadoActual = partes[0];
            std::string simbolo = partes[1];
            std::string estadoSiguiente = partes[2];
            archivoDOT << estadoActual << " -> " << estadoSiguiente << " [label=\"" << simbolo << "\"];" << std::endl;
        }
    }

    archivoDOT << "}" << std::endl;
    archivoDOT.close();

    // Generar el gráfico utilizando Graphviz
    system("dot -Tpng afd.dot -o afd.png");
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

void CrearArchivoHTMLafd(
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
    archivoHTML << "<title>AFD</title>\n";
    archivoHTML << "</head>\n";
    archivoHTML << "<body>\n";

    // Escribir la información del AFD en el archivo HTML
    archivoHTML << "<h1>Información del AFD</h1>\n";

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
std::tuple<std::vector<std::string>, std::vector<std::string>, std::vector<std::string>, std::vector<std::string>> ProcesarAFN(
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

    // std::cout << "Conjunto de estados:" << std::endl;
    // for (int estado : estados) {
    //     std::cout << estado << " ";
    // }
    // std::cout << std::endl;

    // std::cout << "Alfabeto:" << std::endl;
    // for (char simbolo : alfabeto) {
    //     std::cout << simbolo << " ";
    // }
    // std::cout << std::endl;

    // std::cout << "Estados finales:" << std::endl;
    // for (int estado : estadosFinales) {
    //     std::cout << estado << " ";
    // }
    // std::cout << std::endl;

    // std::cout << "Tabla de transiciones:" << std::endl;
    // for (const Transicion& transicion : tablaTransiciones) {
    //     std::cout << transicion.estadoActual << " -> " << transicion.simbolo << " -> " << transicion.estadoSiguiente << std::endl;
    // }

    return std::make_tuple(alfabeto_tokens_vector, estados_atributos_tokens_vector, transiciones_tokens_vector, finalesattributes_vector );
    // CrearArchivoHTML(alfabeto_tokens_vector, estados_atributos_tokens_vector, transiciones_tokens_vector,finalesattributes_vector,"afn.html");
    // show_filehtml("afn.html");
    // GenerarArchivoDOT(alfabeto_tokens_vector, estados_atributos_tokens_vector, transiciones_tokens_vector,finalesattributes_vector);
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
    std::string filePath;
    std::cout << "Enter the path of the XML file: " << std::endl;
    std::cin.ignore();  // Limpia el búfer de entrada
    std::getline(std::cin, filePath);  // Usa getline para leer toda la línea, incluyendo espacios

    // Verifica si la entrada comienza y termina con comillas dobles
    if (!filePath.empty() && filePath.front() == '"' && filePath.back() == '"') {
        // Elimina las comillas dobles
        filePath = filePath.substr(1, filePath.size() - 2);
        // Reemplaza las barras invertidas por barras diagonales
        // std::replace(filePath.begin(), filePath.end(), '\\', '/');
    }

    try {
        FILE *xml_file_ptr = fopen(filePath.c_str(), "r");
        if (!xml_file_ptr) {
            std::cerr << "Error opening temporary file" << std::endl;
        }

        if (filePath.length() < 4 || filePath.substr(filePath.length() - 4) != ".xml") {
            std::cerr<<"Error: Filepath must end with '.xml'."<< std::endl;
            throw std::runtime_error("Error.");
        }

        // Inicializa el parser con el archivo XML
        init_parser(xml_file_ptr);
        int result = parse_xml();
        if (result == 0) {
            std::cout << "Cargando archivo XML desde '" << filePath << "'..." << std::endl;
            std::cout << "Analisis exitoso." << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    // Implementa la lógica para cargar el archivo XML aquí (código no proporcionado).
    
}


std::vector<std::string> alfabeto_tokens_vector;
std::vector<std::string> finalesattributes_vector;
std::vector<std::string> estados_atributos_tokens_vector;
std::vector<std::string> transiconales_tokens_vector;
std::vector<std::string> iniciales_tokens_vector;

std::tuple<std::vector<std::string>, std::vector<std::string>, std::vector<std::string>, std::vector<std::string>,std::vector<std::string>> ShowLoadedData() {
    std::cout << "Mostrando datos cargados..." << std::endl;



    alfabeto_tokens = get_tokens_linearlayout();
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

    finalesattributes_tokens = get_tokens_finalesattributes();
    if (finalesattributes_tokens != NULL) {
        finalesattributes_vector = charArrayToVector(finalesattributes_tokens);
        if (!finalesattributes_vector.empty()) {
            std::cout << "Tokens de finalesattributes:" << std::endl;
            for (const std::string& token : finalesattributes_vector) {
                std::cout << token << std::endl;
            }
        } else {
            std::cout << "No se encontraron tokens finalesattributes." << std::endl;
        }
    }

    estados_atributos_tokens = get_tokens_estadosattributes();
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

    transiconales_tokens = get_tokens_transcicionales();
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



    iniciales_tokens = get_tokens_iniciales();
    if (iniciales_tokens != NULL) {
        iniciales_tokens_vector = charArrayToVector(iniciales_tokens);
        if (!iniciales_tokens_vector.empty()) {
            std::cout << "Tokens de inicio:" << std::endl;
            for (const std::string& token : iniciales_tokens_vector) {
                std::cout << token << std::endl;
            }
        } else {
            std::cout << "No se encontraron tokens de inicio." << std::endl;
        }
    }

    return std::make_tuple(alfabeto_tokens_vector, estados_atributos_tokens_vector, transiconales_tokens_vector, finalesattributes_vector,iniciales_tokens_vector);
}

// void ShowLoadedData() {            
//     cout << "Mostrando datos cargados..." << endl;

//     alfabeto_tokens = get_tokens_linearlayout();
//         //std::vector<std::string> alfabeto_tokens_vector;
//         if (alfabeto_tokens != NULL) {
//             alfabeto_tokens_vector = charArrayToVector(alfabeto_tokens);
//             if (!alfabeto_tokens_vector.empty()) {
//                 std::cout << "Tokens de alfabeto:" << std::endl;
//                 for (const std::string& token : alfabeto_tokens_vector) {
//                     std::cout << token << std::endl;
//                 }
//             } else {
//                 std::cout << "No se encontraron tokens de alfabeto." << std::endl;
//             }
//         }

//     finalesattributes_tokens = get_tokens_finalesattributes();
        
//         if (finalesattributes_tokens != NULL) {
//             finalesattributes_vector = charArrayToVector(finalesattributes_tokens);
//             if (!alfabeto_tokens_vector.empty()) {
//                 std::cout << "Tokens de finalesattributes:" << std::endl;
//                 for (const std::string& token : finalesattributes_vector) {
//                     std::cout << token << std::endl;
//                 }
//             } else {
//                 std::cout << "No se encontraron tokens finalesattributes." << std::endl;
//             }
//         }

//     estados_atributos_tokens = get_tokens_estadosattributes();
        
//         if (estados_atributos_tokens != NULL) {
//             estados_atributos_tokens_vector = charArrayToVector(estados_atributos_tokens);
//             if (!estados_atributos_tokens_vector.empty()) {
//                 std::cout << "Tokens de estados atributos:" << std::endl;
//                 for (const std::string& token : estados_atributos_tokens_vector) {
//                     std::cout << token << std::endl;
//                 }
//             } else {
//                 std::cout << "No se encontraron tokens de estados atributos." << std::endl;
//             }
//         }
    
//     transiconales_tokens = get_tokens_transcicionales();
        
//         if (transiconales_tokens != NULL) {
//             transiconales_tokens_vector = charArrayToVector(transiconales_tokens);
//             if (!transiconales_tokens_vector.empty()) {
//                 std::cout << "Tokens de transiciones:" << std::endl;
//                 for (const std::string& token : transiconales_tokens_vector) {
//                     std::cout << token << std::endl;
//                 }
//             } else {
//                 std::cout << "No se encontraron tokens de transiciones." << std::endl;
//             }
//         }
    
//     ProcesarAFN(alfabeto_tokens_vector, estados_atributos_tokens_vector, transiconales_tokens_vector,finalesattributes_vector);
// }

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

void Cleanpath() {
        const char* archivos[] = {
        "vitacora_tokens.html",
        "vitacora_errores.html",
        "afn.html",
        "afn.png",
        "afn.dot",
        "imagen.html"
    };

    free(alfabeto_tokens);
    alfabeto_tokens = NULL;

    free(finalesattributes_tokens);
    finalesattributes_tokens = NULL;

    free(estados_atributos_tokens);
    estados_atributos_tokens = NULL;

    free(transiconales_tokens);
    transiconales_tokens = NULL;


    alfabeto_tokens_vector.clear();
    finalesattributes_vector.clear();
    estados_atributos_tokens_vector.clear();
    transiconales_tokens_vector.clear();


     for (const char* archivo : archivos) {
        if (std::remove(archivo) == 0) {
            std::cout << "cleaning Path..." << std::endl;
        } else {
            // std::cerr << "Error al eliminar el archivo '" << archivo << "'" << std::endl;
        }
    }

    cleanup_parser();
}


void transform(std::vector<std::string> transiconales_tokens_vector,
                    std::vector<std::string> alfabeto_tokens_vector,// Define e inicializa tus vectores aquí
                std::vector<std::string> estados_atributos_tokens_vector,
                std::vector<std::string> estados_finales_vector,
                std::vector<std::string> iniciales_tokens_vector) {
    vector<string> transitions = transiconales_tokens_vector;

    // Vector de AFNTransition donde se almacenarán las transiciones
    vector<AFNTransition> afnTransitions;

    // Iterar sobre el vector de strings y convertir los datos a AFNTransition
    for (const string& transitionString : transitions) {
        size_t pos1 = transitionString.find(',');
        size_t pos2 = transitionString.rfind(',');

        if (pos1 != string::npos && pos2 != string::npos && pos1 != pos2) {
            string fromState = transitionString.substr(0, pos1);
            string inputSymbol = transitionString.substr(pos1 + 1, pos2 - pos1 - 1);
            string toState = transitionString.substr(pos2 + 1);

            AFNTransition transition;
            transition.fromState = fromState;
            transition.inputSymbol = inputSymbol;
            transition.toState = toState;

            afnTransitions.push_back(transition);
        }
    }

    // Imprimir las transiciones convertidas
    for (const AFNTransition& transition : afnTransitions) {
        cout << "From: " << transition.fromState << ", Symbol: " << transition.inputSymbol << ", To: " << transition.toState << endl;
    }

    set<string> afnAcceptStates;
    for (const string& token : iniciales_tokens_vector) {
        afnAcceptStates.insert(token);
    }
    //set<string> afnAcceptStates = {"3"};
    vector<string> alphabet = alfabeto_tokens_vector;
    string startState = iniciales_tokens_vector[0];
    cout << startState<< endl;

    // Convertir el AFN en un AFD
    map<set<string>, map<string, set<string>>> afdTransitions = convertToAFD(afnTransitions, afnAcceptStates, alphabet, startState);

    // Imprimir las transiciones del AFD resultante
    for (const auto& [currentState, transitionMap] : afdTransitions) {
        cout << "Estado actual: {";
        for (const string& state : currentState) {
            cout << state << " ";
        }
        cout << "}\n";

        for (const auto& [symbol, nextState] : transitionMap) {
            cout << "  Con símbolo '" << symbol << "' va a {";
            for (const string& state : nextState) {
                cout << state << " ";
            }
            cout << "}\n";
        }
    }



}

int main() {
    int choice;
    bool exitProgram = false; // Variable para controlar la ejecución del programa

    do {
        cout << "----------Menu Inicial------------" << endl;
        cout << "1. Cargar Archivo XML" << endl;
        cout << "2. Crear Archivo XML Existente" << endl;
        cout << "3. Salir" << endl;
        cout << "-----------------------------------" << endl;
        cout << "Seleccione una opcion: ";
        cin >> choice;

        switch (choice) {
        case 1:
            // Llamar a LoadXMLFile();
            
            LoadXMLFile();
            break;
        case 2:
            // Llamar a GenerateHelloWorldXML();
            GenerateHelloWorldXML();
            break;
        case 3:
            cout << "Saliendo del programa." << endl;
            return 0;
        default:
            cout << "Opción no válida. Por favor, seleccione una opción válida." << endl;
            break;
        }

        if (choice != 3) {
            bool returnToMainMenu = false;

            do {
                std::vector<std::string> alfabeto_tokens_vector; // Define e inicializa tus vectores aquí
                std::vector<std::string> estados_atributos_tokens_vector;
                std::vector<std::string> transiconales_tokens_vector;
                std::string estado_inicial;
                std::vector<std::string> estados_finales_vector;

                cout << "----------Menu Principal------------" << endl;
                cout << "1. Mostrar Vitacora" << endl;
                cout << "2. Mostrar AFN" << endl;
                cout << "3. Convertir AFN a AFD" << endl;
                cout << "4. Mostrar AFD" << endl;
                cout << "5. Salir al Menu Inicial" << endl; // Cambié 6 a 5 para volver al menú inicial
                cout << "6. Capturar arrays" << endl;
                cout << "7. Mostrar dibujito" << endl;
                cout << "--------------------------------------" << endl;
                cout << "Seleccione una opcion: ";
                cin >> choice;
                //AFD afd = CreateAFD();

                switch (choice) {
                case 1:
                    // Llamar a show_filehtml("vitacora_tokens.html");
                    show_filehtml("vitacora_tokens.html");
                    break;
                case 2:
                    // Llamar a ShowLoadedData();
                    //ShowLoadedData();
                    std::tie(alfabeto_tokens_vector, estados_atributos_tokens_vector, transiconales_tokens_vector, finalesattributes_vector,iniciales_tokens_vector) = ShowLoadedData();

                    // Ahora puedes usar las variables retornadas como desees

                   // ProcesarAFN(alfabeto_tokens_vector, estados_atributos_tokens_vector, transiconales_tokens_vector, finalesattributes_vector);

                    std::tie(alfabeto_tokens_vector, estados_atributos_tokens_vector, transiconales_tokens_vector, finalesattributes_vector) = ProcesarAFN(alfabeto_tokens_vector, estados_atributos_tokens_vector, transiconales_tokens_vector, finalesattributes_vector);

                    CrearArchivoHTML(alfabeto_tokens_vector, estados_atributos_tokens_vector, transiconales_tokens_vector,finalesattributes_vector,"afn.html");

                    show_filehtml("afn.html");
                    GenerarArchivoDOT(alfabeto_tokens_vector, estados_atributos_tokens_vector, transiconales_tokens_vector,finalesattributes_vector);

                    

                    break;
                case 3:
                    // Llamar a AFNToAFD();
                    std::tie(alfabeto_tokens_vector, estados_atributos_tokens_vector, transiconales_tokens_vector, finalesattributes_vector,iniciales_tokens_vector) = ShowLoadedData();
                    std::tie(alfabeto_tokens_vector, estados_atributos_tokens_vector, transiconales_tokens_vector, finalesattributes_vector) = ProcesarAFN(alfabeto_tokens_vector, estados_atributos_tokens_vector, transiconales_tokens_vector, finalesattributes_vector);
                       // Datos del AFN almacenados en vectores de string
                    transform(transiconales_tokens_vector,alfabeto_tokens_vector,estados_atributos_tokens_vector,estados_finales_vector,iniciales_tokens_vector);

                    
                    break;
                case 4:
                    // Llamar a ShowAFD();
                    ShowAFD();
                    break;
                case 5:
                    Cleanpath();
                    returnToMainMenu = true;
                    break;
                case 6:
                    // Llamar a GenerateHTMLWithData(alfabeto_tokens_vector, estados_atributos_tokens_vector, transiconales_tokens_vector);
                    GenerateHTMLWithData(alfabeto_tokens_vector, estados_atributos_tokens_vector, transiconales_tokens_vector);
                    break;
                case 7:
                    // Llamar a show_png("afn.png");
                    // Llamar a show_filehtml("imagen.html")
                    if (fileExists("afn.png")) {
                    show_png("afn.png");
                    show_filehtml("imagen.html");
                    } else {
                        cout << "Primero debes seleccionar la opción 2 para generar el archivo afn.png." << endl;
                    }
                    break;
                default:
                    cout << "Opcion no válida. Por favor, seleccione una opcion valida." << endl;
                    break;
                }
            } while (!returnToMainMenu); // Continuar el segundo menú hasta que se elija regresar al menú inicial
        }
    } while (!exitProgram);

    return 0;
}

