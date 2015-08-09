#ifndef FUNCTION_NODE_H_INCLUDED
#define FUNCTION_NODE_H_INCLUDED

#include "../cppAutoSolve.h"

#include <map>

template <typename T>
class ParameterNode;

template <typename T>
class FunctionNode {
    friend class ParameterNode<T>;
protected:
    ParameterNode<T>* _NodeParaResult;

    std::map<std::string, ParameterNode<T>*>
        _NodesParaInput;

    std::string _Identifier;
    bool _Calculated;

public:
    FunctionNode(const std::string& identifier) :
        _Identifier(identifier),
        _Calculated(false)
    {}

    virtual void solve() {
        if(can_be_calculated() && !_Calculated) {
            _NodeParaResult->_Val = calc();
            _NodeParaResult->_Calculated = true;
        }
    }

protected:
    virtual T calc() = 0;
    bool can_be_calculated() {
        for(auto i : _NodesParaInput) {
            if (i.second->_Calculated)
                return false;
        }
        return true;
    }

public:
    void connect_with_input(const ParameterNode<T>* paraNode) {
        _NodesParaInput[paraNode._Identifier] = paraNode;
        paraNode._NodesFuncOutput[_Identifier] = this;
    }
    void connect_with_output(const ParameterNode<T>* paraNode) {
        _NodeParaResult = paraNode;
        paraNode._NodesFuncInput[_Identifier] = this;
    }
};

#endif //FUNCTION_NODE_H_INCLUDED
