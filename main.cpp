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

std::string join(const std::vector<std::string>& elements, const std::string& separator) {
    std::string result;
    for (size_t i = 0; i < elements.size(); ++i) {
        result += elements[i];
        if (i < elements.size() - 1) {
            result += separator;
        }
    }
    return result;
}



void convertirAFNtoAFD(const std::vector<std::string>& alfabeto_tokens_vector,
                       const std::vector<std::string>& finalesattributes_vector,
                       const std::vector<std::string>& estados_atributos_tokens_vector,
                       const std::vector<std::string>& transicionales_tokens_vector) {
    
    //transicionales_tokens_vector.clear();
     std::vector<Transition> AFNtransitions;
    std::cout << "Tamaño del vector: " << transicionales_tokens_vector.size() << std::endl;

    // Recorrer las cadenas en transicionales_tokens_vector
    for (size_t i = 0; i < 6 && i < transicionales_tokens_vector.size(); ++i) {
    const std::string& transicion = transicionales_tokens_vector[i];
    // Usar un stringstream para dividir la cadena en sus componentes
    std::stringstream ss(transicion);
    std::string from, symbol, to;

    // Leer los componentes separados por comas
    if (std::getline(ss, from, ',') && std::getline(ss, symbol, ',') && std::getline(ss, to, ',')) {
        // Verificar si la transición ya existe en AFNtransitions antes de agregarla
        bool transicionDuplicada = false;
        for (const Transition& existingTransition : AFNtransitions) {
            if (existingTransition.from == from && existingTransition.symbol == symbol && existingTransition.to == to) {
                transicionDuplicada = true;
                break;
            }
        }
        
        // Si no es una transición duplicada, agrégala a AFNtransitions
        if (!transicionDuplicada) {
            AFNtransitions.push_back({from, symbol, to});
        } else {
            // Manejar transiciones duplicadas aquí si es necesario
            std::cerr << "Transición duplicada: " << transicion << std::endl;
        }
    } else {
        // Manejar errores o registros mal formateados aquí si es necesario
        std::cerr << "Error en el formato de transición: " << transicion << std::endl;
        }
    }

    std::vector<std::string> AFNstates = estados_atributos_tokens_vector;
    std::string AFNinitialState = estados_atributos_tokens_vector[0];
    std::vector<std::string> AFNfinalStates = finalesattributes_vector;

    // Obtener símbolos AFD de las transiciones de AFN
    std::vector<std::string> AFDsymbols;
    for (const Transition& afnTransition : AFNtransitions) {
        if (afnTransition.symbol != "&") {
            AFDsymbols.push_back(afnTransition.symbol);
        }
    }

    // Datos de AFD
    std::vector<Transition> AFDtransitions;
    std::vector<std::vector<std::string>> AFDstates;
    std::string AFDinitialState = AFNinitialState;
    std::vector<std::vector<std::string>> AFDfinalStates;

    // Combinación de estados sin repetición
    for (const std::string& state : AFNstates) {
        AFDstates.push_back({state});
    }

    for (int i = 0; i < AFDstates.size(); i++) {
        for (const std::string& symbol : AFDsymbols) {
            std::vector<std::string> combinedStates;
            for (const Transition& afnTransition : AFNtransitions) {
                if (afnTransition.from == AFDstates[i][0] && afnTransition.symbol == symbol) {
                    combinedStates.push_back(afnTransition.to);
                }
            }
            if (!combinedStates.empty()) {
                std::sort(combinedStates.begin(), combinedStates.end());
                combinedStates.erase(std::unique(combinedStates.begin(), combinedStates.end()), combinedStates.end());
                AFDtransitions.push_back({AFDstates[i][0], symbol, combinedStates[0]});

                // Agregar nuevos estados combinados al conjunto de estados AFD
                if (std::find(AFDstates.begin(), AFDstates.end(), combinedStates) == AFDstates.end()) {
                    AFDstates.push_back(combinedStates);
                }
            }
        }
    }

    // Encontrar estados finales de AFD
    for (const std::vector<std::string>& combinedStates : AFDstates) {
        for (const std::string& state : combinedStates) {
            if (std::find(AFNfinalStates.begin(), AFNfinalStates.end(), state) != AFNfinalStates.end()) {
                AFDfinalStates.push_back(combinedStates);
                break;
            }
        }
    }

    // Impresión de datos de AFD (puedes adaptar esto según tus necesidades)
    std::cout << "=============AFD=============" << std::endl;
    std::cout << "AFD - Transitions:" << std::endl;
    for (const Transition& transition : AFDtransitions) {
        std::cout << "from: " << transition.from << ", symbol: " << transition.symbol << ", to: " << transition.to << std::endl;
    }
    std::cout << "AFD - Symbols:" << std::endl;
    for (const std::string& symbol : AFDsymbols) {
        std::cout << symbol << std::endl;
    }
    std::cout << "AFD - States:" << std::endl;
    for (const std::vector<std::string>& state : AFDstates) {
        for (const std::string& substate : state) {
            std::cout << substate << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "AFD - InitialState: " << AFDinitialState << std::endl;
    std::cout << "AFD - FinalStates:" << std::endl;
    for (const std::vector<std::string>& finalState : AFDfinalStates) {
        for (const std::string& substate : finalState) {
            std::cout << substate << " ";
        }
        std::cout << std::endl;
    }
    std::map<std::pair<std::string, std::string>, std::string> AFDtransitionsMap;

    for (int i = 0; i < AFDstates.size(); i++) {
        for (const std::string& symbol : AFDsymbols) {
            std::vector<std::string> combinedStates;
            for (const Transition& afnTransition : AFNtransitions) {
                if (afnTransition.from == AFDstates[i][0] && afnTransition.symbol == symbol) {
                    combinedStates.push_back(afnTransition.to);
                }
            }
            if (!combinedStates.empty()) {
                std::sort(combinedStates.begin(), combinedStates.end());
                combinedStates.erase(std::unique(combinedStates.begin(), combinedStates.end()), combinedStates.end());

                std::string combinedState = join(combinedStates, ",");
                AFDtransitionsMap[{AFDstates[i][0], symbol}] = combinedState;

                // Agregar nuevos estados combinados al conjunto de estados AFD
                if (std::find(AFDstates.begin(), AFDstates.end(), combinedStates) == AFDstates.end()) {
                    AFDstates.push_back(combinedStates);
                }
            }
        }
    }
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

        // Inicializa el parser con el archivo XML
        init_parser(xml_file_ptr);
        int result = parse_xml();
        if (result == 0) {
            std::cout << "Analisis exitoso." << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    // Implementa la lógica para cargar el archivo XML aquí (código no proporcionado).
    std::cout << "Cargando archivo XML desde '" << filePath << "'..." << std::endl;
}


std::vector<std::string> alfabeto_tokens_vector;
std::vector<std::string> finalesattributes_vector;
std::vector<std::string> estados_atributos_tokens_vector;
std::vector<std::string> transiconales_tokens_vector;
std::tuple<std::vector<std::string>, std::vector<std::string>, std::vector<std::string>, std::vector<std::string>> ShowLoadedData() {
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

    return std::make_tuple(alfabeto_tokens_vector, estados_atributos_tokens_vector, transiconales_tokens_vector, finalesattributes_vector);
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
                    std::tie(alfabeto_tokens_vector, estados_atributos_tokens_vector, transiconales_tokens_vector, finalesattributes_vector) = ShowLoadedData();

                    // Ahora puedes usar las variables retornadas como desees

                   // ProcesarAFN(alfabeto_tokens_vector, estados_atributos_tokens_vector, transiconales_tokens_vector, finalesattributes_vector);

                    std::tie(alfabeto_tokens_vector, estados_atributos_tokens_vector, transiconales_tokens_vector, finalesattributes_vector) = ProcesarAFN(alfabeto_tokens_vector, estados_atributos_tokens_vector, transiconales_tokens_vector, finalesattributes_vector);

                    CrearArchivoHTML(alfabeto_tokens_vector, estados_atributos_tokens_vector, transiconales_tokens_vector,finalesattributes_vector,"afn.html");

                    show_filehtml("afn.html");
                    GenerarArchivoDOT(alfabeto_tokens_vector, estados_atributos_tokens_vector, transiconales_tokens_vector,finalesattributes_vector);

                    

                    break;
                case 3:
                    // Llamar a AFNToAFD();
                    std::tie(alfabeto_tokens_vector, estados_atributos_tokens_vector, transiconales_tokens_vector, finalesattributes_vector) = ShowLoadedData();
                    std::tie(alfabeto_tokens_vector, estados_atributos_tokens_vector, transiconales_tokens_vector, finalesattributes_vector) = ProcesarAFN(alfabeto_tokens_vector, estados_atributos_tokens_vector, transiconales_tokens_vector, finalesattributes_vector);
                    convertirAFNtoAFD(alfabeto_tokens_vector, finalesattributes_vector, estados_atributos_tokens_vector, transiconales_tokens_vector);
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

