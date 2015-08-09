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
        _UnknownParameters, //all the parameter nodes, of which the value is unknown
        _KnownParameters; //all the parameter nodes, of which the value is known

    std::set<FunctionNode<T>*>
        _UncalculateableFunctions; //functions which were never called

    std::queue<FunctionNode<T>*>
        _CalculateableFunctions; //functions which have to be used

    bool _Solved;

//------------------------------------------------------------------------------
public:

    AutoSolveController() :
        _Solved(false)
    {}

    //add a parameter node to the system
    void add(ParameterNode<T>* pNode) {
        if(pNode->_Known) //if already known, insert to known set
            _KnownParameters.insert(pNode);
        else //else insert to unknown set
            _UnknownParameters.insert(pNode);
    }

    //add a function node to the system
    void add(FunctionNode<T>* fNode) {
        if(fNode->_Calculated) //all function nodes should be uncalculated in the beginning
            throw std::runtime_error("Do not add already calculated function nodes");
        _UncalculateableFunctions.insert(fNode);
    }

//------------------------------------------------------------------------------

    //define connections between function- and parameter nodes
    void connect_function_with_input(FunctionNode<T>* fNode, ParameterNode<T>* pNode) {
        if(_UncalculateableFunctions.find(fNode) == _UncalculateableFunctions.end()) //only allow connections to added nodes
            throw std::runtime_error("Function node you are trying to connect not added yet. Make sure to add THEN connect");

        if(_UnknownParameters.find(pNode) == _UnknownParameters.end()
            && _KnownParameters.find(pNode) == _KnownParameters.end()) //only allow connections to added nodes
            throw std::runtime_error("Parameter node you are trying to connect not added yet. Make sure to add THEN connect");

        fNode->connect_with_input(pNode);
    }

    //define connections between function- and parameter nodes
    void connect_function_with_output(FunctionNode<T>* fNode, ParameterNode<T>* pNode) {
        if(_UncalculateableFunctions.find(fNode) == _UncalculateableFunctions.end()) //only allow connections to added nodes
            throw std::runtime_error("Function node you are trying to connect not added yet. Make sure to add THEN connect");

        if(_UnknownParameters.find(pNode) == _UnknownParameters.end()
            && _KnownParameters.find(pNode) == _KnownParameters.end()) //only allow connections to added nodes
            throw std::runtime_error("Parameter node you are trying to connect not added yet. Make sure to add THEN connect");

        fNode->connect_with_output(pNode);
    }

//------------------------------------------------------------------------------

    //try to solve the system (true if all parameters could be calculated, false in all other cases)
    bool solve() {
        if(_Solved)
            return false; //don't try to solve an already solved system

        //make sure the system is valid before solving it
        if(!system_is_valid()) {
            return false;
        }

        //for all initally set parameters
        //go the functions requiring them
        //and check whether these can be calculated
        //add to the todo in this case

        //for all initally known parameters
        for(auto knownP : _KnownParameters) {
            //find the functions they're used as parameter in
            for(auto func : knownP->_OutputFunctionNodes) {
                //and add them to the todo if they can be calculaed
                if(func->can_be_calculated())
                    _CalculateableFunctions.push(func);
            }
        }

        //while there still are functions that can be solved
        while(_CalculateableFunctions.size() > 0) {
            //get a new function which can be solved
            FunctionNode<T> *todoF = _CalculateableFunctions.front();
            _CalculateableFunctions.pop();

            //make sure it is fine and can be solved
            if(!todoF->is_valid())
                return false;

            todoF->solve(); //solve the function

            _KnownParameters.insert(todoF->_ResultParameterNode); //add its output to known parameters
            if(_UnknownParameters.find(todoF->_ResultParameterNode) != _UnknownParameters.end()) //and remove it from the unknown parameters
                _UnknownParameters.erase(_UnknownParameters.find(todoF->_ResultParameterNode));

            //now check whether the now known output parameter
            //can be used to solve any of its functions
            for(auto func : todoF->_ResultParameterNode->_OutputFunctionNodes) {
                //check whether they can be calculated
                //and add them to the todo if they can be calculaed
                if(func->can_be_calculated())
                    _CalculateableFunctions.push(func);
            }
        }

        if(_UnknownParameters.size() > 0)
            return false; //system could not be fully solved

        //system fully solved
        _Solved = true;
        return true;
    }

//------------------------------------------------------------------------------
private:

    bool system_is_valid() { //checking whether the initial state of the system is fine
        if(_UnknownParameters.size() == 0 || _UncalculateableFunctions.size() == 0)
            return false; //nothing to do in this case

        for(auto unknownP : _UnknownParameters) {
            if(!unknownP->is_valid())
                return false; //all unknown parameters have to be valid
        }

        for(auto knownP : _KnownParameters) {
            if(!knownP->is_valid())
                return false; //all known parameters have to be valid
        }

        for(auto unknownF : _UncalculateableFunctions) {
            if(!unknownF->is_valid())
                return false; //all unknown functions have to be valid
        }

        return true; //all checks passed
    }

//------------------------------------------------------------------------------

};

#endif //AUTO_SOLVE_CONTROLLER_H_INCLUDED
