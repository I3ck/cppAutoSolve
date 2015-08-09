#ifndef PARAMETER_NODE_H_INCLUDED
#define PARAMETER_NODE_H_INCLUDED

#include "../cppAutoSolve.h"

#include <map>

template <typename T>
class FunctionNode;

template <typename T>
class ParameterNode {
    friend class FunctionNode<T>;
protected:
    ///@todo double linkage might not be required
    ///@todo mark all pointer members as such
    std::map<std::string, FunctionNode<T>*>
        _NodesFuncInput,
        _NodesFuncOutput;

    std::string _Identifier;
    bool _Calculated;
public:
    T _Val; ///@todo define getter
    ParameterNode(const std::string& identifier) :
        _Identifier(identifier),
        _Calculated(false)
    {}
};

#endif //PARAMETER_NODE_H_INCLUDED
