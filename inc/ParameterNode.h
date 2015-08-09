#ifndef PARAMETER_NODE_H_INCLUDED
#define PARAMETER_NODE_H_INCLUDED

#include "../cppAutoSolve.h"

#include <map>

template <typename T>
class FunctionNode;

template <typename T>
class AutoSolveController;

template <typename T>
class ParameterNode {
    friend class FunctionNode<T>;
    friend class AutoSolveController<T>;
protected:
    ///@todo double linkage might not be required
    ///@todo mark all pointer members as such
    std::map<std::string, FunctionNode<T>*>
        _NodesFuncInput,
        _NodesFuncOutput;

    std::string _Identifier;
    bool _Calculated;
    T _Val; ///@todo define getter

    bool is_valid() {
        if(_NodesFuncInput.size() == 0)
            return false;
        return true;
    }

public:
    ParameterNode(const std::string& identifier) :
        _Identifier(identifier),
        _Calculated(false)
    {}
    void set_val(T val) {
        _Val = val;
        _Calculated = true;
    }

    T get_val() const {
        return _Val;
    }
};

#endif //PARAMETER_NODE_H_INCLUDED
