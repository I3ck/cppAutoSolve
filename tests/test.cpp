#define CATCH_CONFIG_MAIN
#include "../dependencies/Catch.h" //https://github.com/philsquared/Catch

using namespace std;

#include <iostream>

#include "../cppAutoSolve.h"

double f1(std::map<std::string, ParameterNode<double>*> &inputs) {
    return inputs["x"]->get() * inputs["y"]->get();
}


TEST_CASE("first case") {

    FunctionNode<double> fn1(f1);
    FunctionNode<double> fn2([](std::map<std::string, ParameterNode<double>*> &inputs) {
        return 18.0 * inputs["y"]->get();
    });
    FunctionNode<double> fn3([](std::map<std::string, ParameterNode<double>*> &inputs) {
            return inputs["x"]->get() * inputs["y"]->get() + inputs["z"]->get();
    });

    ParameterNode<double>
        x("x"), y("y"), z("z"), a("a");

    std::string parseableText("y = 7.5");
    //or y = 7.5;
    //or y.set(7.5);

    AutoSolveController<double> controller;

    controller.add(&fn1, &fn2, &fn3 ,
                   &x, &y, &z, &a);

    z = fn1(&x, &y);
    //or
    //controller.connect_inputs(&fn1, &x, &y);
    //controller.connect_output(&fn1, &z);

    controller.connect_inputs(&fn2, &y);
    controller.connect_output(&fn2, &x);

    controller.connect_inputs(&fn3, &x, &y, &z);
    controller.connect_output(&fn3, &a);

    controller.parse_text(parseableText);
    REQUIRE(controller.solve());
    std::cout << controller.results_text() << std::endl;
    REQUIRE(x.get() == 135.0);
    REQUIRE(y.get() == 7.5);
    REQUIRE(z.get() == 1012.5);
    REQUIRE(a.get() == 2025.0);
}
