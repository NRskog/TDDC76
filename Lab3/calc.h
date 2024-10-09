#include <string>
#ifndef calc_h
#define calc_h



class Node
{
    public:
    Node();
    Node(Node const&) = delete;
    virtual ~Node() = default;
    Node& operator=(Node const&) = delete;
    virtual double evaluate() const  = 0;
    virtual std::string postfix() const = 0;
    virtual std::string prefix() const = 0;
    virtual std::string infix() const = 0;
    virtual bool is_real() const = 0 ;
    private:
};


class Operand: public Node
{
    public:
    Operand();
    ~Operand();
   
    virtual std::string to_string() const =0;
    
    private:
};

class Real: public Operand
{
    public:
    Real(double const i);
    
    double evaluate() const override;
    bool is_real() const override;
    std::string to_string() const override;
    std::string postfix() const override;
    std::string prefix() const override;
    std::string infix() const override;

    private:
    double value;
};

class Integer: public Operand
{
    public:
    Integer(int const i);
    bool is_real() const override;
    double evaluate() const override;
    std::string to_string() const override;
    std::string postfix() const override;
    std::string prefix() const override;
    std::string infix() const override;

    private:
    int value;
};

class Operator: public Node
{
    public:
    Operator(Node* const& lhs, Node* const& rhs);
    Operator(const Operator&) = delete;
    ~Operator();
    Operator&operator=(const Operator&) = delete;
    
    bool is_real() const override;
    virtual char operator_type() const = 0;
    std::string postfix() const override;
    std::string prefix() const override;
    std::string infix() const override;

    protected:
    Node* lhs;
    Node* rhs;
};

class Addition: public Operator
{
    public:
    Addition(Node* const& lhs, Node* const& rhs);
    double evaluate() const override;
    char operator_type() const override;

    private:
};

class Subtraction: public Operator
{
    public:
    Subtraction(Node* const& lhs, Node* const& rhs);
    double evaluate() const override;
    char operator_type() const override;

    private:
};

class Multiplication: public Operator
{
    public:
    Multiplication(Node* const& lhs, Node* const& rhs);
    double evaluate() const override;  
    char operator_type() const override;

    private:
};

class Division: public Operator
{
    public:
    Division(Node* const& lhs, Node* const& rhs);
    double evaluate() const override;
    char operator_type() const override;

    private:
};

class Power: public Operator
{
    public:
    Power(Node* const & lhs, Node* const& rhs);
    double evaluate() const override;
    char operator_type() const override;

    private:
};

#endif
