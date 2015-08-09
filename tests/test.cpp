#define CATCH_CONFIG_MAIN
#include "../dependencies/Catch.h" //https://github.com/philsquared/Catch

using namespace std;

#include <iostream>

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



TEST_CASE("first case") {
    FN1<double> fn1("fn1");
    ParameterNode<double>
        x("x"), y("y"), z("z");

    x._Val = 3.0;
    y._Val = 7.5;

    fn1.connect_with_input(&x);
    fn1.connect_with_input(&y);
    fn1.connect_with_output(&z);

    fn1.solve();

    SECTION("first section") {
        cout << "x : " << x._Val << endl;
        cout << "y : " << y._Val << endl;
        cout << "z : " << z._Val << endl;
    }
}
