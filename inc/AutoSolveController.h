#ifndef AUTO_SOLVE_CONTROLLER_H_INCLUDED
#define AUTO_SOLVE_CONTROLLER_H_INCLUDED

#include "../cppAutoSolve.h"

#include <set>

#define DEBUG

#ifdef DEBUG
    #include <iostream>
    using namespace std;
#endif

template <typename T>
class AutoSolveController {
private:
    std::set<ParameterNode<T>*>
        _UncalculatedParameters,
        _ToBeCalculatedParameters, ///@todo function group might be enough
        _CalculatedParameters;

    std::set<FunctionNode<T>*>
        _UncalculatedFunctions,
        _ToBeCalculatedFunctions,
        _CalculatedFunctions;

public:
    void add(ParameterNode<T>* pNode) {
        if(pNode->_Calculated)
            _CalculatedParameters.insert(pNode);
        else
            _UncalculatedParameters.insert(pNode);
    }

    void add(FunctionNode<T>* fNode) {
        _UncalculatedFunctions.insert(fNode);
    }

    void connect_function_with_input(FunctionNode<T>*fNode, ParameterNode<T>* pNode) {
        fNode->connect_with_input(pNode);
    }

    void connect_function_with_output(FunctionNode<T>*fNode, ParameterNode<T>* pNode) {
        fNode->connect_with_output(pNode);
    }

    bool solve() {
        ///@todo proper error cases
        ///@todo check all nodes for required connections != nullptr

        #ifdef DEBUG
            cout << "SOLVING" << endl;
        #endif


        for(auto calculatedP : _CalculatedParameters) {
            #ifdef DEBUG
                cout << "CALCULATED PARA" << endl;
            #endif
            for(auto func : calculatedP->_NodesFuncOutput) {
                #ifdef DEBUG
                    cout << "FUNCTION OF CALCULATED PARA" << endl;
                #endif
                ///@todo only if function not in calculated
                bool allCalculated(true);
                for(auto input : func.second->_NodesParaInput) {
                    if(!input.second->_Calculated) {
                        allCalculated = false;
                        break;
                    }
                }
                if(allCalculated)
                    _ToBeCalculatedFunctions.insert(func.second);
            }
        }
        //INITIALLY or when todo is empty:
        //for each calculated parameter go to its functions
        //if all children of it are defined, move it from uncalculated to todo
        //if todo is empty, break

        #ifdef DEBUG
            cout << "TO BE CALCULATED: " << _ToBeCalculatedFunctions.size() << endl;
        #endif

        while(_ToBeCalculatedFunctions.size() > 0) { ///@todo warning, possible endless loop
            auto todo = _ToBeCalculatedFunctions.begin();
            auto todoF = *todo;
            _ToBeCalculatedFunctions.erase(todo);

            todoF->solve();
            ///@todo below only if above was successful
            _CalculatedParameters.insert(todoF->_NodeParaResult);
            auto rem = _UncalculatedParameters.find(todoF->_NodeParaResult);
            ///@todo only if found
            _UncalculatedParameters.erase(rem);

            //check whether the solved parameter can be used in a function

            ///@todo same code as above, except the first line
            for(auto func : todoF->_NodeParaResult->_NodesFuncOutput) {
                ///@todo only if function not in calculated
                bool allCalculated(true);
                for(auto input : func.second->_NodesParaInput) {
                    if(!input.second->_Calculated) {
                        allCalculated = false;
                        break;
                    }
                }
                if(allCalculated)
                    _ToBeCalculatedFunctions.insert(func.second);
            }
        }
        //while functions in todo
        //calculate their parameters
        //move calculated parameters to calculated group
    }


};

#endif //AUTO_SOLVE_CONTROLLER_H_INCLUDED
