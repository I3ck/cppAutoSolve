#define CATCH_CONFIG_MAIN
#include "../dependencies/Catch.h" //https://github.com/philsquared/Catch

using namespace std;

#include <iostream>

#include "../cppAutoSolve.h"

template <typename T>
class FN1 : public FunctionNode<T> {
public:
    FN1(const std::string& id) : FunctionNode<T>(id) {}

    T calc() {
        return (FunctionNode<T>::_NodesParaInput)["x"]->get_val() * (FunctionNode<T>::_NodesParaInput)["y"]->get_val();
    }
};

template <typename T>
class FN2 : public FunctionNode<T> {
public:
    FN2(const std::string& id) : FunctionNode<T>(id) {}

    T calc() {
        return 18.0 * (FunctionNode<T>::_NodesParaInput)["y"]->get_val();
    }
};

template <typename T>
class FN3 : public FunctionNode<T> {
public:
    FN3(const std::string& id) : FunctionNode<T>(id) {}

    T calc() {
        return (FunctionNode<T>::_NodesParaInput)["x"]->get_val() * (FunctionNode<T>::_NodesParaInput)["y"]->get_val() + (FunctionNode<T>::_NodesParaInput)["z"]->get_val();
    }
};

TEST_CASE("first case") {
    FN1<double> fn1("fn1");
    FN2<double> fn2("fn2");
    FN3<double> fn3("fn3");
    ParameterNode<double>
        x("x"), y("y"), z("z"), a("a");

    y.set_val(7.5);

    AutoSolveController<double> controller;

    controller.add(&fn1);
    controller.add(&fn2);
    controller.add(&x);
    controller.add(&y);
    controller.add(&z);
    controller.add(&a);

    controller.connect_function_with_input(&fn1, &x);
    controller.connect_function_with_input(&fn1, &y);
    controller.connect_function_with_output(&fn1, &z);

    controller.connect_function_with_input(&fn2, &y);
    controller.connect_function_with_output(&fn2, &x);

    controller.connect_function_with_input(&fn3, &x);
    controller.connect_function_with_input(&fn3, &y);
    controller.connect_function_with_input(&fn3, &z);
    controller.connect_function_with_output(&fn3, &a);

    if(controller.solve()) {

        cout << "x : " << x.get_val() << endl;
        cout << "y : " << y.get_val() << endl;
        cout << "z : " << z.get_val() << endl;
        cout << "a : " << a.get_val() << endl;
    }
}
