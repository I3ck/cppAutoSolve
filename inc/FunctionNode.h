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

#ifndef FUNCTION_NODE_H_INCLUDED
#define FUNCTION_NODE_H_INCLUDED

#include "../cppAutoSolve.h"

#include <map>

template <typename T>
class ParameterNode;

template <typename T>
class AutoSolveController;

//------------------------------------------------------------------------------

template <typename T>
class FunctionNode {
    friend class ParameterNode<T>;
    friend class AutoSolveController<T>;

//------------------------------------------------------------------------------

protected:
    ParameterNode<T>* _ResultParameterNode;

    std::map<std::string, ParameterNode<T>*>
        _InputParameterNodes;

    bool _Calculated;

    std::function<T (std::map<std::string, ParameterNode<T>*>&)> _Callback;

//------------------------------------------------------------------------------

public:
    FunctionNode(std::function<T (std::map<std::string, ParameterNode<T>*>&)> callback) :
        _ResultParameterNode(nullptr),
        _Calculated(false),
        _Callback(callback)
    {}

//------------------------------------------------------------------------------
protected:

//------------------------------------------------------------------------------
    virtual void solve() {
        if(can_be_calculated() && !_Calculated) {
            _Calculated = true;
            _ResultParameterNode->_Val = _Callback(_InputParameterNodes);
            _ResultParameterNode->_Known = true;
        }
    }

//------------------------------------------------------------------------------

    void connect_input(ParameterNode<T>* paraNode) {
        _InputParameterNodes[paraNode->_Identifier] = paraNode;
        paraNode->_OutputFunctionNodes.insert(this);
    }
    void connect_output(ParameterNode<T>* paraNode) {
        _ResultParameterNode = paraNode;
        paraNode->_InputFunctionNodes.insert(this);
    }

//------------------------------------------------------------------------------

    bool can_be_calculated() {
        for(auto i : _InputParameterNodes) {
            if (!i.second->_Known)
                return false;
        }
        if(_ResultParameterNode->_Known)
            return false;
        return true;
    }

//------------------------------------------------------------------------------

    bool is_valid() {
        if(_ResultParameterNode == nullptr)
            return false;
        if(_InputParameterNodes.size() == 0)
            return false;
        return true;
    }

//------------------------------------------------------------------------------

};

#endif //FUNCTION_NODE_H_INCLUDED
