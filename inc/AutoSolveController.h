#ifndef AUTO_SOLVE_CONTROLLER_H_INCLUDED
#define AUTO_SOLVE_CONTROLLER_H_INCLUDED

#include "../cppAutoSolve.h"

#include <set>

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

        //INITIALLY or when todo is empty:
        //for each calculated parameter go to its functions
        //if all children of it are defined, move it from uncalculated to todo
        //if todo is empty, break

        //while functions in todo
        //calculate their parameters
        //move calculated parameters to calculated group
    }


};

#endif //AUTO_SOLVE_CONTROLLER_H_INCLUDED
