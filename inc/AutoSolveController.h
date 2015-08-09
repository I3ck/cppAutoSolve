#ifndef AUTO_SOLVE_CONTROLLER_H_INCLUDED
#define AUTO_SOLVE_CONTROLLER_H_INCLUDED

#include "../cppAutoSolve.h"

#include <set>
#include <queue>
#include <stdexcept>

//------------------------------------------------------------------------------

template <typename T>
class AutoSolveController {
private:
    std::set<ParameterNode<T>*>
        _UncalculatedParameters, //all the parameter nodes, of which the value is unknown
        _CalculatedParameters; //all the parameter nodes, of which the value is known

    std::set<FunctionNode<T>*>
        _UncalculatedFunctions; //functions which were never called

    std::queue<FunctionNode<T>*>
        _ToBeCalculatedFunctions; //functions which have to be used

//------------------------------------------------------------------------------
public:

    //add a parameter node to the system
    void add(ParameterNode<T>* pNode) {
        if(pNode->_Calculated) //if already calculated, insert to calculated set
            _CalculatedParameters.insert(pNode);
        else //else insert to uncalculated set
            _UncalculatedParameters.insert(pNode);
    }

    //add a function node to the system
    void add(FunctionNode<T>* fNode) {
        if(fNode->_Calculated) //all function nodes should be uncalculated in the beginning
            throw std::runtime_error("Do not add already calculated function nodes");
        _UncalculatedFunctions.insert(fNode);
    }

//------------------------------------------------------------------------------

    //define connections between function- and parameter nodes
    void connect_function_with_input(FunctionNode<T>* fNode, ParameterNode<T>* pNode) {
        if(_UncalculatedFunctions.find(fNode) == _UncalculatedFunctions.end()) //only allow connections to added nodes
            throw std::runtime_error("Function node you are trying to connect not added yet. Make sure to add THEN connect");

        if(_UncalculatedParameters.find(pNode) == _UncalculatedParameters.end()
            && _CalculatedParameters.find(pNode) == _CalculatedParameters.end()) //only allow connections to added nodes
            throw std::runtime_error("Parameter node you are trying to connect not added yet. Make sure to add THEN connect");

        fNode->connect_with_input(pNode);
    }

    //define connections between function- and parameter nodes
    void connect_function_with_output(FunctionNode<T>* fNode, ParameterNode<T>* pNode) {
        if(_UncalculatedFunctions.find(fNode) == _UncalculatedFunctions.end()) //only allow connections to added nodes
            throw std::runtime_error("Function node you are trying to connect not added yet. Make sure to add THEN connect");

        if(_UncalculatedParameters.find(pNode) == _UncalculatedParameters.end()
            && _CalculatedParameters.find(pNode) == _CalculatedParameters.end()) //only allow connections to added nodes
            throw std::runtime_error("Parameter node you are trying to connect not added yet. Make sure to add THEN connect");

        fNode->connect_with_output(pNode);
    }

//------------------------------------------------------------------------------

    //try to solve the system (true if all parameters could be calculated, false in all other cases)
    bool solve() { ///@todo make sure this cant be called twice
        //make sure the system is valid before solving it
        if(!system_is_valid()) {
            return false;
        }

        //for all initally set parameters
        //go the functions requiring them
        //and check whether these can be calculated
        //add to the todo in this case

        //for all initally known parameters
        for(auto calculatedP : _CalculatedParameters) {

            //find the function they're used as parameter in
            for(auto func : calculatedP->_NodesFuncOutput) {

                //check whether these functions can be calculated
                ///@todo make this a method (or use existing one)
                bool allCalculated(true);

                //check whether all inputs for this function are known
                for(auto input : func->_NodesParaInput) {
                    if(!input.second->_Calculated) {
                        allCalculated = false;
                        break;
                    }
                }
                //add to todo if it can be solved
                ///@todo renamed container to solveable functions and other to solved functions
                if(allCalculated)
                    _ToBeCalculatedFunctions.push(func);
            }
        }

        //while there still are functions that can be solved
        while(_ToBeCalculatedFunctions.size() > 0) {
            //get a new function which can be solved
            FunctionNode<T> *todoF = _ToBeCalculatedFunctions.front();
            _ToBeCalculatedFunctions.pop();

            //make sure it is fine and can be solved
            if(!todoF->is_valid())
                return false;

            todoF->solve(); //solve the function

            _CalculatedParameters.insert(todoF->_NodeParaResult); //add its output to known parameters
            if(_UncalculatedParameters.find(todoF->_NodeParaResult) != _UncalculatedParameters.end()) //and remove it from the unknown parameters
                _UncalculatedParameters.erase(_UncalculatedParameters.find(todoF->_NodeParaResult));

            //now check whether the now known output parameter
            //can be used to solve any of its functions
            ///@todo same code as above, except the first line (make method?)

            //for all the functions this parameter is used in
            for(auto func : todoF->_NodeParaResult->_NodesFuncOutput) {
                //check whether they can be calculated
                bool allCalculated(true);
                for(auto input : func->_NodesParaInput) {
                    if(!input.second->_Calculated) {
                        allCalculated = false;
                        break;
                    }
                }
                if(allCalculated) //if they can be calculated, add them to the todo
                    _ToBeCalculatedFunctions.push(func);
            }
        }

        if(_UncalculatedParameters.size() > 0)
            return false; //system could not be fully solved
        return true; //system fully solved
    }

//------------------------------------------------------------------------------
private:

    bool system_is_valid() { //checking whether the initial state of the system is fine
        if(_UncalculatedParameters.size() == 0 || _UncalculatedFunctions.size() == 0)
            return false; //nothing to do in this case

        for(auto uncalculatedP : _UncalculatedParameters) {
            if(!uncalculatedP->is_valid())
                return false; //all uncalculated parameters have to be valid
        }

        for(auto uncalculatedF : _UncalculatedFunctions) {
            if(!uncalculatedF->is_valid())
                return false; //all uncalculated functions have to be valid
        }

        return true; //all checks passed
    }

//------------------------------------------------------------------------------

};

#endif //AUTO_SOLVE_CONTROLLER_H_INCLUDED
