#define CATCH_CONFIG_MAIN
#include "../dependencies/Catch.h" //https://github.com/philsquared/Catch

using namespace std;

#include "../cppAutoSolve.h"

template <typename T>
class FN1 : public FunctionNode<T> {
public:
    FN1(const std::string& id) :
        FunctionNode<T>(id)
    {}

    T calc() {
        return (FunctionNode<T>::_NodesParaInput)["x"]->_Val * (FunctionNode<T>::_NodesParaInput)["y"]->_Val;
    }
};

FN1<double> fn1("fn1");
ParameterNode<double>
    x("x"), y("y");

TEST_CASE("first case") {

    SECTION("first section") {
        REQUIRE(true);
    }
}
