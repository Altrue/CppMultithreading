#pragma once


 //Just use following macro in your class to enable singleton
#define IMPLEMENT_SINGLETON(className)                                                                  \
protected:                                                                                              \
    className() {}                                                                                      \
    className(className const&);                /* Don't implement */                                   \
    void operator = (className const&);         /* Don't implement */                                   \
                                                                                                        \
    static className& getInstanceRef() {                                                                \
        static className clsSingleton;          /* The only one instance with lazy instantiation */     \
        return clsSingleton;                                                                            \
    }                                                                                                   \
                                                                                                        \
public:                                                                                                 \
    static className *getInstance() {                                                                   \
        className &clsSingleton = className::getInstanceRef();                                          \
        return &clsSingleton;                                                                           \
    }                                                                                                   \


// -----------------------------------------------------------------------------
// Sample 1: convert a class to a singleton
// -----------------------------------------------------------------------------
// class CTest
// {
//     IMPLEMENT_SINGLETON(CTest)
//
// public:
//     void doSomething() { /* whatever you want */ }
// };
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Sample 2: get singleton instance and use it
// -----------------------------------------------------------------------------
// /* All in one */
// CTest::getInstance()->doSomething();
//
// /* Through class pointer */
// CTest *t;
// t = CTest::getInstance();
// t->doSomething();
// -----------------------------------------------------------------------------
