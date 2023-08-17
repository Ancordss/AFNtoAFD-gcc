#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "rapidxml.hpp"
#include <cstdlib>
#include "spdlog/spdlog.h"


class XMLLoader {
public:
    XMLLoader() : isOpen(false) {}  // Initialize isOpen to false

    bool loadXMLContent(const std::string& filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            spdlog::error("Error: Unable to open file.");
            return false;
        }

        if (filePath.length() < 4 || filePath.substr(filePath.length() - 4) != ".xml") {
            spdlog::error("Error: Filepath must end with '.xml'.");
            return false;
        }

        xmlContent.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        isOpen = true;  // Set isOpen to true when the file is successfully opened
        return true;
    }

    bool isFileOpen() const {
        return isOpen;
    }

    std::string getXMLContent() const {
        return xmlContent;
    }

    bool parseXML(const std::string& xmlContent) {
        try {
            document.parse<0>(const_cast<char*>(xmlContent.c_str()));
            return true;
        }
        catch (rapidxml::parse_error& e) {
            spdlog::error("XML parsing error: {} ", e.what());
  
            return false;
        }
    }

    void displayContent() {
        std::string xmlContent;
        if (loadXMLContent(xmlContent)) {
            spdlog::info("XML Content:");
            std::cout << xmlContent << std::endl;
        }
    }

     void displayParsedData() {
        rapidxml::xml_node<>* rootNode = document.first_node();
        displayNodeContent(rootNode, 0);
    }

private:
    void displayNodeContent(rapidxml::xml_node<>* node, int depth) {
        for (int i = 0; i < depth; ++i) {
            std::cout << "  ";
        }

        std::cout << "<" << node->name() << ">" << std::endl;

        for (rapidxml::xml_node<>* child = node->first_node(); child; child = child->next_sibling()) {
            displayNodeContent(child, depth + 1);
        }

        for (int i = 0; i < depth; ++i) {
            std::cout << "  ";
        }

        std::cout << "</" << node->name() << ">" << std::endl;
    }

private:
    std::string filePath;
    rapidxml::xml_document<> document;

private:
    bool isOpen;
    std::string xmlContent;
};



