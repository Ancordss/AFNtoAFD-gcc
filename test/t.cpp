#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <string>

using namespace std;

struct AFNTransition {
    string fromState;
    string inputSymbol;
    string toState;
};

map<set<string>, map<string, set<string>>> convertToAFD(
    vector<AFNTransition>& afnTransitions,
    set<string>& afnAcceptStates,
    vector<string>& alphabet,
    string startState
) {
    map<set<string>, map<string, set<string>>> afdTransitions;
    queue<set<string>> statesQueue;
    map<set<string>, bool> visited;

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
    vector<string> transitions = {
        "0,&,1",
        "1,&,2",
        "2,a,3",
        "3,&,4",
        "1,&,5",
        "5,b,6",
        "6,&,4",
        "4,&,7",
        "7,a,8",
        "8,b,9",
        "9,b,10",
        "4,&,1",
        "0,&,7"
    };

    vector<AFNTransition> afnTransitions;
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

    set<string> afnAcceptStates = {"10"};
    vector<string> alphabet = {"a", "b"};
    string startState = "0";

    map<set<string>, map<string, set<string>>> afdTransitions = convertToAFD(afnTransitions, afnAcceptStates, alphabet, startState);

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
