#ifndef expression_h
#define expression_h


#include "calc.h"
#include <string>
#include <stack>
#include <memory>

// Komplettering: Ni läcker Node-klassen i det publika gränssnittet.        KLAR
// Komplettering: Ett Expression-objekt behöver endast bestå av en pekare till den översta noden. KLAR
// Komplettering: hjälpfunktioner ska ligga privat i klassen.           KLAR
// Komplettering: Parameteröverföring av sammansatta datatyper ska ske som konstant referens om möjligt.    KLAR


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