#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "rapidxml.hpp"

class XMLLoader {
public:
    XMLLoader(const std::string& filePath) : filePath(filePath) {}

    bool loadXMLContent(std::string& xmlContent) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cout << "Error: Unable to open file." << std::endl;
            return false;
        }

        xmlContent.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return true;
    }

    bool parseXML(const std::string& xmlContent) {
        try {
            document.parse<0>(const_cast<char*>(xmlContent.c_str()));
            return true;
        }
        catch (rapidxml::parse_error& e) {
            std::cout << "XML parsing error: " << e.what() << std::endl;
            return false;
        }
    }

    void displayContent() {
        std::string xmlContent;
        if (loadXMLContent(xmlContent)) {
            std::cout << "XML Content:" << std::endl;
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
};


