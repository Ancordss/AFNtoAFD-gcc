// proyecto1.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln

#include <iostream>
#include "src/XmlLoader.h"
#include "spdlog/spdlog.h"




void displayOptions() {
    spdlog::info("----- Menu -----");
    spdlog::info("1. Load XML File");
    spdlog::info("2. Show Loaded Data");
    spdlog::info("3. Show AFN");
    spdlog::info("4. AFN To AFD");
    spdlog::info("5. Show AFD");
    spdlog::info("6. Generate File");
    spdlog::info("7. Exit");
}

int main() {
    spdlog::set_pattern("AFNtoAFD-[%^%l%$] %v");

    XMLLoader xmlLoader;
    std::string filePath;  // Moved filePath outside the loop
    std::string xmlContent;

    int choice;
    do {
        displayOptions();
        spdlog::info("Enter an option: ");
        std::cin >> choice;

        switch (choice) {
        case 1:
            std::cin.ignore();
            spdlog::info("Enter the path of the XML file: ");
            /*std::cin >> filePath;*/
            std::getline(std::cin, filePath);

            // Check if the input begins and ends with double quotes
            if (!filePath.empty() && filePath.front() == '"' && filePath.back() == '"') {
                // Remove the double quotes
                filePath = filePath.substr(1, filePath.size() - 2);
                // Replace backslashes with forward slashes
                std::replace(filePath.begin(), filePath.end(), '\\', '/');
            }

            if (xmlLoader.loadXMLContent(filePath)) {
                xmlContent = xmlLoader.getXMLContent();  // Retrieve the XML content from XMLLoader
                spdlog::info("XML file loaded successfully.");
                break;
            }
            break;
        case 2:
            if (xmlLoader.isFileOpen()) {
                spdlog::info("XML Content:");
                std::cout << xmlContent << std::endl;
            }
            else {
                spdlog::error("No file is currently open.");
            }
            break;
        case 3:
            if (xmlLoader.isFileOpen()) {
                if (xmlLoader.parseXML(xmlContent)) {
                    xmlLoader.displayParsedData();
                }
            }
            else {
                spdlog::error("No file is currently open.");
            }
            break;
        case 4:
            // AFNToAFD();
            std::cout << "converting....";
            break;
        case 5:
            // ShowAFD();
            std::cout << "show new diagram";
            break;
        case 6:
            // GenFile()
            std::cout << "generating file";
            break;
        case 7:
            std::cout << "Exiting the program" << std::endl;
            return 0;
        default:
            std::cout << "Invalid choice. Please select a valid option." << std::endl;
            break;
        }
    } while (choice != 7);

    return 0;
}
   
