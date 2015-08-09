#define CATCH_CONFIG_MAIN
#include "../dependencies/Catch.h" //https://github.com/philsquared/Catch

using namespace std;

#include <iostream>

#include "../cppAutoSolve.h"

template <typename T>
class FN1 : public FunctionNode<T> {
public:
    T calc(std::map<std::string, ParameterNode<T>*> &inputs) const {
        return inputs["x"]->get() * inputs["y"]->get();
    }
};

template <typename T>
class FN2 : public FunctionNode<T> {
public:
    T calc(std::map<std::string, ParameterNode<T>*> &inputs) const {
        return 18.0 * inputs["y"]->get();
    }
};

template <typename T>
class FN3 : public FunctionNode<T> {
public:
    T calc(std::map<std::string, ParameterNode<T>*> &inputs) const {
        return inputs["x"]->get() * inputs["y"]->get() + inputs["z"]->get();
    }
};

TEST_CASE("first case") {
    FN1<double> fn1;
    FN2<double> fn2;
    FN3<double> fn3;
    ParameterNode<double>
        x("x"), y("y"), z("z"), a("a");

    y.set(7.5);

    AutoSolveController<double> controller;

    controller.add(&fn1);
    controller.add(&fn2);
    controller.add(&fn3);
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

    REQUIRE(controller.solve());
    REQUIRE(x.get() == 135.0);
    REQUIRE(y.get() == 7.5);
    REQUIRE(z.get() == 1012.5);
    REQUIRE(a.get() == 2025.0);
}
