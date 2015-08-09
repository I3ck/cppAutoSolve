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
    ParameterNode<T>* _NodeParaResult;

    std::map<std::string, ParameterNode<T>*>
        _NodesParaInput;

    std::string _Identifier;

    bool _Calculated;

//------------------------------------------------------------------------------

public:
    FunctionNode(const std::string& identifier) :
        _NodeParaResult(nullptr),
        _Identifier(identifier),
        _Calculated(false)
    {}

//------------------------------------------------------------------------------
protected:

//------------------------------------------------------------------------------
    virtual void solve() {
        if(can_be_calculated() && !_Calculated) {
            _Calculated = true;
            _NodeParaResult->_Val = calc();
            _NodeParaResult->_Calculated = true;
        }
    }

//------------------------------------------------------------------------------

    void connect_with_input(ParameterNode<T>* paraNode) {
        _NodesParaInput[paraNode->_Identifier] = paraNode;
        paraNode->_NodesFuncOutput[_Identifier] = this;
    }
    void connect_with_output(ParameterNode<T>* paraNode) {
        _NodeParaResult = paraNode;
        paraNode->_NodesFuncInput[_Identifier] = this;
    }

    virtual T calc() = 0;

//------------------------------------------------------------------------------

    bool can_be_calculated() {
        for(auto i : _NodesParaInput) {
            if (!i.second->_Calculated)
                return false;
        }
        return true;
    }

//------------------------------------------------------------------------------

    bool is_valid() {
        if(_NodeParaResult == nullptr)
            return false;
        if(_NodesParaInput.size() == 0)
            return false;
        return true;
    }

//------------------------------------------------------------------------------

};

#endif //FUNCTION_NODE_H_INCLUDED
