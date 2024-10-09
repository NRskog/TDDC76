#ifndef expression_h
#define expression_h


#include "calc.h"
#include <string>
#include <stack>
#include <memory>




class Expression
{
    public:
    Expression(std::string const& exp);
    Expression();
    Expression(Expression const& exp)= delete;
    Expression(Expression && exp);
    ~Expression();

    Expression& operator=(Expression && exp);
    Expression& operator=(Expression const& exp) = delete;

    std::string postfix() const;
    std::string infix() const;
    std::string prefix() const;
    double evaluate() const;
    
    private:
    Node* root;
    Node* get_root() const;
    void empty_root() ;
};

#endif
