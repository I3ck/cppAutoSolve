#ifndef PARAMETER_NODE_H_INCLUDED
#define PARAMETER_NODE_H_INCLUDED

#include "../cppAutoSolve.h"

#include <set>

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
