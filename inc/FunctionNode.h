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

    std::string _Identifier;

    bool _Calculated;

//------------------------------------------------------------------------------

public:
    FunctionNode(const std::string& identifier) :
        _ResultParameterNode(nullptr),
        _Identifier(identifier),
        _Calculated(false)
    {}

//------------------------------------------------------------------------------
protected:

//------------------------------------------------------------------------------
    virtual void solve() {
        if(can_be_calculated() && !_Calculated) {
            _Calculated = true;
            _ResultParameterNode->_Val = calc();
            _ResultParameterNode->_Known = true;
        }
    }

//------------------------------------------------------------------------------

    void connect_with_input(ParameterNode<T>* paraNode) {
        _InputParameterNodes[paraNode->_Identifier] = paraNode;
        paraNode->_OutputFunctionNodes.insert(this);
    }
    void connect_with_output(ParameterNode<T>* paraNode) {
        _ResultParameterNode = paraNode;
        paraNode->_InputFunctionNodes.insert(this);
    }

    virtual T calc() = 0;

//------------------------------------------------------------------------------

    bool can_be_calculated() {
        for(auto i : _InputParameterNodes) {
            if (!i.second->_Known)
                return false;
        }
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
