#ifndef PARAMETER_NODE_H_INCLUDED
#define PARAMETER_NODE_H_INCLUDED

#include "../cppAutoSolve.h"

#include <map>

template <typename T>
class FunctionNode;

template <typename T>
class ParameterNode {
private:
    ///@todo double linkage might not be required
    std::map<std::string, FunctionNode<T>&>
        _NodesFuncInput,
        _NodesFuncOutput;

    std::string _Identifier;
    bool _Calculated;
    T _Val;
};

#endif //PARAMETER_NODE_H_INCLUDED
