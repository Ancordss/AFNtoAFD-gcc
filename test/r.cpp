#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <queue>

using namespace std;

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

int main() {
    // Datos del AFN almacenados en vectores de string
    vector<AFNTransition> afnTransitions = {
        {"0", "a", "0"},
        {"0", "&", "1"},
        {"0", "a", "1"},
        {"0", "r", "0"},
        {"1", "r", "2"},
        {"2", "r", "3"}
    };

    set<string> afnAcceptStates = {"4"};
    vector<string> alphabet = {"a", "b"};
    string startState = "0";

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

    return 0;
}
