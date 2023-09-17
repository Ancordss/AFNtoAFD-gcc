#include <stdio.h>
#include <stdlib.h>
#include "xml_parser.h"

int main() {
    // Abre el archivo XML para analizar (cambia el nombre del archivo según tu caso)
    FILE *xml_file = fopen("CodeXML.xml", "r");
    if (xml_file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Inicializa el parser con el archivo XML
    init_parser(xml_file);

    // Realiza el análisis del XML
    int result = parse_xml();
    if (result == 0) {
        printf("Análisis exitoso.\n");

        // Obtiene los tokens de tokens_linearlayout
        char **alfabeto_tokens = get_tokens_linearlayout();
        if (alfabeto_tokens != NULL) {
            printf("Tokens de alfabeto:\n");
            for (int i = 0; alfabeto_tokens[i] != NULL; i++) {
                printf("%s\n", alfabeto_tokens[i]);
            }
        } else {
            printf("No se encontraron tokens de alfabeto.\n");
        }

        char **estados_atributos_tokens = get_tokens_estadosattributes();
        if (estados_atributos_tokens != NULL) {
            printf("Tokens de estados:\n");
            for (int i = 0; estados_atributos_tokens[i] != NULL; i++) {
                printf("%s\n", estados_atributos_tokens[i]);
            }
        } else {
            printf("No se encontraron tokens de estados.\n");
        }

        char **tokens_iniciales = get_tokens_iniciales();
        if (tokens_iniciales != NULL) {
            printf("Tokens iniciales:\n");
            for (int i = 0; tokens_iniciales[i] != NULL; i++) {
                printf("%s\n", tokens_iniciales[i]);
            }
        } else {
            printf("No se encontraron tokens de inicial.\n");
        }

        char **tokens_finales = get_tokens_finalesattributes();
        if (tokens_finales != NULL) {
            printf("Tokens de final:\n");
            for (int i = 0; tokens_finales[i] != NULL; i++) {
                printf("%s\n", tokens_finales[i]);
            }
        } else {
            printf("No se encontraron tokens de final.\n");
        }

        char **transiconales_tokens = get_tokens_transcicionales();
        if (transiconales_tokens != NULL) {
            printf("Tokens de transciciones:\n");
            for (int i = 0; transiconales_tokens[i] != NULL; i++) {
                printf("%s\n", transiconales_tokens[i]);
            }
        } else {
            printf("No se encontraron tokens de transciciones.\n");
        }
        // Libera la memoria asignada para los tokens, si es necesario

    } else {
        printf("Análisis fallido.\n");
    }

    // Cierra el archivo y libera recursos
    fclose(xml_file);

    return 0;
}
