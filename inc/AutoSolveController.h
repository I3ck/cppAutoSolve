/*
Copyright 2015 Martin Buck

This file is part of cppAutoSolve.

cppAutoSolve is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

cppAutoSolve is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with cppAutoSolve.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef AUTO_SOLVE_CONTROLLER_H_INCLUDED
#define AUTO_SOLVE_CONTROLLER_H_INCLUDED

#include "../cppAutoSolve.h"

#include <set>
#include <map>
#include <queue>
#include <stdexcept>
#include <sstream>
#include <string>

//------------------------------------------------------------------------------

template <typename T>
class AutoSolveController {
private:

    std::map<std::string, ParameterNode<T>*>
        _UnknownIdentifiers; //all the identifiers of unknown parameternodes, linking to them

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

    //variadic function to add any nodes
    template <typename Node, typename ... Nodes>
    void add(Node node, Nodes... nodes) {
        add(node);
        add(nodes ...);
    }

    //add a parameter node to the system
    void add(ParameterNode<T>* pNode) {
        if(pNode->_Known) //if already known, insert to known set
            _KnownParameters.insert(pNode);
        else { //else insert to unknown set
            _UnknownParameters.insert(pNode);
            _UnknownIdentifiers.insert(std::make_pair(pNode->_Identifier, pNode));
        }
    }

    //add a function node to the system
    void add(FunctionNode<T>* fNode) {
        if(fNode->_Calculated) //all function nodes should be uncalculated in the beginning
            throw std::runtime_error("Do not add already calculated function nodes");
        _UncalculateableFunctions.insert(fNode);
    }

//------------------------------------------------------------------------------

    //variadic function to connect many inputs
    template <typename FNode, typename PNode, typename ... PNodes>
    void connect_inputs(FNode fNode, PNode pNode, PNodes... pNodes) {
        connect_inputs(fNode, pNode);
        connect_inputs(fNode, pNodes ...);
    }

    //define connections between function- and parameter nodes
    void connect_inputs(FunctionNode<T>* fNode, ParameterNode<T>* pNode) {
        if(_UncalculateableFunctions.find(fNode) == _UncalculateableFunctions.end()) //only allow connections to added nodes
            throw std::runtime_error("Function node you are trying to connect not added yet. Make sure to add THEN connect");

        if(_UnknownParameters.find(pNode) == _UnknownParameters.end()
            && _KnownParameters.find(pNode) == _KnownParameters.end()) //only allow connections to added nodes
            throw std::runtime_error("Parameter node you are trying to connect not added yet. Make sure to add THEN connect");

        fNode->connect_inputs(pNode);
    }

    //define connections between function- and parameter nodes
    void connect_output(FunctionNode<T>* fNode, ParameterNode<T>* pNode) {
        if(_UncalculateableFunctions.find(fNode) == _UncalculateableFunctions.end()) //only allow connections to added nodes
            throw std::runtime_error("Function node you are trying to connect not added yet. Make sure to add THEN connect");

        if(_UnknownParameters.find(pNode) == _UnknownParameters.end()
            && _KnownParameters.find(pNode) == _KnownParameters.end()) //only allow connections to added nodes
            throw std::runtime_error("Parameter node you are trying to connect not added yet. Make sure to add THEN connect");

        fNode->connect_output(pNode);
    }

//------------------------------------------------------------------------------

    //try to solve the system (true if all parameters could be calculated, false in all other cases)
    bool solve() {
        if(_Solved)
            return false; //don't try to solve an already solved system

        //make sure the system is valid before solving it
        check_validity();

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

            ///@todo cache find results, currently always searching twice (might happen in several spots)
            ///@todo code duplicated in parse method
            _KnownParameters.insert(todoF->_ResultParameterNode); //add its output to known parameters
            if(_UnknownParameters.find(todoF->_ResultParameterNode) != _UnknownParameters.end()) { //and remove it from the unknown parameters
                _UnknownParameters.erase(_UnknownParameters.find(todoF->_ResultParameterNode));

                if(_UnknownIdentifiers.find(todoF->_ResultParameterNode->_Identifier) != _UnknownIdentifiers.end())
                    _UnknownIdentifiers.erase(_UnknownIdentifiers.find(todoF->_ResultParameterNode->_Identifier));
            }

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

    void parse_text(const std::string &text,
                    const std::string &unknownVal = "NAN") {
        std::stringstream textSs(text);
        std::string line("");

        while(std::getline(textSs, line)) {
            std::stringstream lineSs(line);

            if(!lineSs)
                continue; //ignore empty lines

            std::string identifier;
            lineSs >> identifier;

            if(!lineSs)
                throw std::runtime_error("line '" + line + "'" + " is invalid");

            std::string delimiter;
            lineSs >> delimiter;

            if(!lineSs)
                throw std::runtime_error("line '" + line + "'" + " is invalid");

            std::string sValue;

            if(!lineSs)
                throw std::runtime_error("line '" + line + "'" + " is invalid");

            lineSs >> sValue;

            if(sValue == unknownVal)
                continue;

            std::stringstream ssValue(sValue);


            T value(0.0);
            if(!(ssValue >> value))
                throw std::runtime_error("line '" + line + "'" + " is invalid, value could not be converted");

            ///@todo cache find iterator
            if(_UnknownIdentifiers.find(identifier) == _UnknownIdentifiers.end())
                throw std::runtime_error("Identifier '" + identifier + "'" + " either not in the system, or the value is already set");

            auto itPParaNode = _UnknownIdentifiers.find(identifier);

            ///@todo more checks?
            itPParaNode->second->set(value);
            _KnownParameters.insert(itPParaNode->second);
            _UnknownParameters.erase(_UnknownParameters.find(itPParaNode->second));
            _UnknownIdentifiers.erase(itPParaNode);
        }
}

//------------------------------------------------------------------------------

    std::string results_text(const std::string &delimiter = " : ",
                             const std::string &before = "",
                             const std::string &after = "\n",
                             bool printUnknown = true,
                             const std::string &unknownVal = "NAN") const {
        std::stringstream result("");

        for(auto knownP : _KnownParameters)
            result << before << knownP->_Identifier << delimiter << knownP->_Val << after;

        if(printUnknown) {
            for(auto unknownP : _UnknownParameters)
                result << before << unknownP->_Identifier << delimiter << unknownVal << after;
        }

        return result.str();
    }

//------------------------------------------------------------------------------

    std::string as_dot_syntax() const {
            //to name functions by their output parameters
            //with raising numbers
            //e.g. for parameter b => fb1, fb2, fb3, ...
            std::map<std::string, int> functionCounts;

            std::string result("digraph G {\n");

            std::set<FunctionNode<T>*> fNodes;
            for(auto knownP : _KnownParameters) {
                //find the functions they're used as parameter in
                for(auto func : knownP->_OutputFunctionNodes) {
                    //and add them to the todo if they can be calculaed
                    fNodes.insert(func);
                }
            }

            for (const auto &func : fNodes) {
                std::string& identifier = func->_ResultParameterNode->_Identifier;
                int count = functionCounts[identifier]++;

                if(func->_ResultParameterNode != nullptr)
                    result += "f" + identifier + std::to_string(count+1) + " -> " + identifier + ";\n";

                for(const auto& inputParameterNode : func->_InputParameterNodes)
                    result += inputParameterNode.first + " -> " + "f" + identifier + std::to_string(count+1) + ";\n";

            }
            result += "\n }";

            return result;
    }


//------------------------------------------------------------------------------
private:

    void check_validity() { //checking whether the initial state of the system is fine
        if(_UnknownParameters.size() == 0 || _UncalculateableFunctions.size() == 0)
            return; //nothing to do in this case

        for(auto unknownP : _UnknownParameters) {
            if(!unknownP->is_valid())
                throw std::runtime_error("'" + unknownP->_Identifier + "'" + " is not calculateable with any given function and its value is not set");
        }

        for(auto knownP : _KnownParameters) {
            if(!knownP->is_valid())
                throw std::runtime_error("'" + knownP->_Identifier + "'" + " is invalid. Unknown cause for this");
        }

        for(auto unknownF : _UncalculateableFunctions) {
            if(!unknownF->is_valid())
                throw std::runtime_error("A function node has either no input or not output parameters");
        }
    }

//------------------------------------------------------------------------------

};

#endif //AUTO_SOLVE_CONTROLLER_H_INCLUDED
