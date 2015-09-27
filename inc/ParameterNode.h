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

#ifndef PARAMETER_NODE_H_INCLUDED
#define PARAMETER_NODE_H_INCLUDED

#include "../cppAutoSolve.h"

#include <set>
#include<string>

template <typename T>
class FunctionNode;

template <typename T>
class AutoSolveController;

//------------------------------------------------------------------------------

template <typename T>
class ParameterNode {
    friend class FunctionNode<T>;
    friend class AutoSolveController<T>;

//------------------------------------------------------------------------------
protected:
    std::set<FunctionNode<T>*>
        _InputFunctionNodes,
        _OutputFunctionNodes;

    std::string _Identifier;

    bool _Known;

    T _Val;

//------------------------------------------------------------------------------
public:
    ParameterNode(const std::string& identifier) :
        _Identifier(identifier),
        _Known(false)
    {}

//------------------------------------------------------------------------------

    void set(T val) {
        _Val = val;
        _Known = true;
    }

    T get() const {
        return _Val;
    }

//------------------------------------------------------------------------------

    ParameterNode& operator = (T val)
    {
        set(val);
        return *this;
    }

//------------------------------------------------------------------------------

    FunctionNode<T>& operator = (FunctionNode<T>& funcNode)
    {
        funcNode.connect_output(this);
        return funcNode;
    }

//------------------------------------------------------------------------------
protected:

    bool is_valid() {
        if(_Known)
            return true;
        if(_InputFunctionNodes.size() == 0)
            return false;
        return true;
    }

//------------------------------------------------------------------------------

};

#endif //PARAMETER_NODE_H_INCLUDED
