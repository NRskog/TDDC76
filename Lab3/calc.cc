#include "calc.h"
#include <string>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <iostream>

using namespace std;


Node::Node()
{
}

Operand::Operand()
:Node{}
{
}

Operand::~Operand()
{
}

Real::Real(double const i)
:Operand{}, value {i}
{
}

double Real::evaluate() const
{
    return value;
}

std::string Real::to_string() const 
{
    ostringstream oss{};
    oss << left << fixed <<  setprecision(3) <<setw(5) << setfill('0') << evaluate();
    return(oss.str());
}

std::string Real::postfix() const 
{
    return to_string();
}

std::string Real::prefix() const 
{
    return( to_string());
}

std::string Real::infix() const 
{
    return( to_string());
}

bool Real::is_real() const
{
    return (true);
}

Integer::Integer(int const i)
:Operand{}, value{i}
{
}

bool Integer::is_real() const
{
    return (false);
}

double Integer::evaluate() const
{
    return value;
}

std::string Integer::to_string() const 
{
    ostringstream oss{};
    oss  << evaluate();
    return(oss.str());
}

std::string Integer::postfix() const 
{
    return( to_string());
}

std::string Integer::prefix() const 
{
    return( to_string());
}
std::string Integer::infix() const 
{
    return( to_string());
}

Operator::Operator(Node* const& lhs, Node* const& rhs)
:Node{},lhs{lhs}, rhs{rhs}
{
}

Operator::~Operator()
{
    delete rhs;
    delete lhs;
    rhs =nullptr;
    lhs = nullptr;
}

bool Operator::is_real() const
{
    return(false);
}

std::string Operator::postfix() const 
{
    ostringstream oss{};
    oss << (*(lhs)).postfix() <<" "<< (*(rhs)).postfix() <<" " <<  operator_type();
    return(oss.str());
}

std::string Operator::prefix() const 
{
    ostringstream oss{};
    oss <<  operator_type() <<" "<< (*lhs).prefix() <<" "<< (*rhs).prefix();

    return(oss.str());
}

std::string Operator::infix() const 
{   
    ostringstream oss{};
    oss <<"( "<< (*(lhs)).infix() <<" "<<  operator_type()<< " " << (*(rhs)).infix() << " )";
    return(oss.str());
}

Addition::Addition(Node* const& lhs, Node* const& rhs)
:Operator(lhs,rhs)
{
}

double Addition::evaluate() const
{
    return ((*(lhs)).evaluate() +(*(rhs)).evaluate() );
}

char Addition::operator_type() const
{
    return '+';
}

Subtraction::Subtraction(Node* const& lhs, Node* const& rhs)
:Operator(lhs,rhs)
{
}

double Subtraction::evaluate() const
{
    return ((*(lhs)).evaluate() - (*(rhs)).evaluate() );
}

char Subtraction::operator_type() const
{
    return '-';
}

Multiplication::Multiplication(Node* const& lhs, Node* const& rhs)
:Operator(lhs,rhs)
{
}

double Multiplication::evaluate() const
{
    return ((*(lhs)).evaluate() * (*(rhs)).evaluate() );
}

char Multiplication::operator_type() const
{
    return '*';
}

Division::Division(Node* const& lhs, Node* const& rhs)
:Operator(lhs,rhs)
{
    if ((*rhs).evaluate() == 0)
    {
        throw(logic_error("Nolldivision"));
    }
}

double Division::evaluate() const
{   
    return ((*(lhs)).evaluate() / (*(rhs)).evaluate() );
}

char Division::operator_type() const
{
    return '/';
}

Power::Power(Node* const& lhs, Node* const& rhs)
:Operator(lhs,rhs)
{
    if (((*lhs).evaluate() <0 )&& ((*rhs).is_real()))
    {
        throw(logic_error("Negativt tal med flyttal som exponent"));
    }
    if (((*lhs).evaluate() ==0) && ((*rhs).evaluate() < 0))
    {
        throw(logic_error("Nolldivision"));
    }
}

double Power::evaluate() const
{
    return ( pow((*(lhs)).evaluate(), (*(rhs)).evaluate())  );
}

char Power::operator_type() const
{
    return '^';
}



