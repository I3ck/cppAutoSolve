#cppAutoSolve
A header-only, templated C++ library which allows the automatic solving of function-parameter systems.  
This enables you to define and solve any system of equations, linear or not.



##version 0.10

##solve any system
Define your functions and pass them to the `FunctionNode` constructor, or directly use lambda expressions:  
```cpp
//using a callback function
double f1(std::map<std::string, ParameterNode<double>*> &inputs) {
    return inputs["x"]->get() * inputs["y"]->get();
}
FunctionNode<double> fn1(f1);


//directly using a lambda expression
FunctionNode<double> fn2([](std::map<std::string, ParameterNode<double>*> &inputs) {
    return 18.0 * inputs["y"]->get();
});
```


Then define your parameters by creating `ParameterNodes` with the name as identifier:
```cpp
ParameterNode<double>
    x("x"), y("y"), z("z"), a("a");
```

Set values for known parameters:
```cpp
y.set(7.5)  
//or
y = 7.5;
//or
std::string parseable("y = 7.5") //call controller.parse_text(parseable) later
```

Create the `AutoSolveController`:
```cpp
AutoSolveController<double> controller;
```

Add all the `ParameterNodes` and `FunctionNodes` to your equation system by adding to the controller:
```cpp
controller.add(&fn1, &fn2, &fn3 ,
               &x, &y, &z);
...
```

Connect functions with their input and output parameters.  
`e = f1(x,y,z)`  
=>  
`e` is output of `f1`  
`x`,`y` and`z` inputs of `f1`
```cpp
e = f1(&x, &y, &z);
//or
controller.connect_inputs(&f1, &x, &y, &z);
controller.connect_output(&f1, &e);
```

Once all connections are defined, you can start solving the system:
```cpp
controller.solve();
```

If all parameters could be calculated, `solve()` will be `true`, otherwise `false`.  
You can access the values with `ParameterNode.get()`  
Or receive them as string via `AutoSolveController.results_text()`

##building the examples / tests
`make`


##including
directly include `cppAutoSolve.h` which includes all required headers or choose the header files you want to use  

#contribute
feel free to open issues if you'd like to have new algorithms added, would like to have a new feature or found a bug

#license
LGPL V3, see `LICENSE`
