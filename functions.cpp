///////////////////////////////////////////////////////////////////////////////////////////////////
// Function Inlining and Devirtualization
// Inlining replaces a function call with its body, reducing call overhead and enabling optimizations.
// Devirtualization removes virtual function overhead when the compiler can determine the exact function at compile-time.
///////////////////////////////////////////////////////////////////////////////////////////////////

class Base {
public:
    virtual int compute(int x) const {
        return x * 2; // A normal virtual function with runtime dispatch
    }
};

class Derived : public Base {
public:
    int compute(int x) const override {
        return x * 3; // Overrides base class method
    }
};

int devirtualized_compute(const Base &obj, int x) {
    return obj.compute(x); // If obj is a known type at compile-time, the compiler can inline the call.
}

///////////////////////////////////////////////////////////////////////////////////////////////////
