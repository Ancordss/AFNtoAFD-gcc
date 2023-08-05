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



void displayOptions() {
    std::cout << "----- Menu -----" << std::endl;
    std::cout << "1. ShowLoadedData" << std::endl;
    std::cout << "2. ShowAFN" << std::endl;
    std::cout << "3. AFNToAFD" << std::endl;
    std::cout << "4. ShowAFD" << std::endl;
    std::cout << "5. GenFile" << std::endl;
    std::cout << "6. Exit" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Usage: app --path path_of_the_file_xml" << std::endl;
        return 0;
    }

    if (std::string(argv[1]) != "--path") {
        std::cout << "Invalid argument. Usage: app --path path_of_the_file_xml" << std::endl;
        return 0;
    }

    std::string filePath = argv[2];
    XMLLoader xmlLoader(filePath);
    std::string xmlContent;
    int choice;
    do {
        displayOptions();
        std::cout << "Enter an option: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            xmlLoader.displayContent();
            break;
        case 2:
            if (xmlLoader.loadXMLContent(xmlContent)) {
                if (xmlLoader.parseXML(xmlContent)) {
                    xmlLoader.displayParsedData();
                }
            }
            break;
        case 3:
            //AFNToAFD();
            std::cout << "converting....";
            break;
        case 4:
            //ShowAFD();
            std::cout << "show new diagram";
            break;
        case 5:
            //GenFile()
            std::cout << "generating file";
            break;
        case 6:
            std::cout << "Exiting the program" << std::endl;
            return 0;
        default:
            std::cout << "Invalid choice. Please select a valid option." << std::endl;
            break;
        }
    } while (choice != 6);
};


   
