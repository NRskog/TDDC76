#include "expression.h"
#include <string>
#include <sstream>
#include <iostream>
#include <cctype>
#include <algorithm>

using namespace std;


Expression::Expression(std::string const& exp)
: root{}
{   
    std::stack<Node*> my_stack;
    double d;
    
    istringstream iss {exp};  
    std::string word {};
    Node* temp {};
    
    while(iss >> word)
    {   
        istringstream iss2 {word};

        if ( std::all_of( begin(word), end(word), ::isdigit ) )
        {
          iss2 >> d;
          temp = new Integer(d);
          my_stack.push(temp);
        }
        
        else if ( isdigit(word.at(0)) )
        {
          iss2 >> d;
          temp = new Real(d);
          my_stack.push(temp);
        }

        
        else
        {
        if (my_stack.empty())
          {
            throw(logic_error("Saknar operand till operator"));
          }
        Node* rhs {my_stack.top()};
        my_stack.pop();
        if (my_stack.empty())
        {
          throw(logic_error("Saknar operand till operator"));
        }
        Node* lhs {my_stack.top()};
        my_stack.pop();
        
        if (word == "+")
            {
              temp = new Addition(lhs, rhs);
              my_stack.push(temp);
            }

        else if (word == "-")
            {
              temp = new Subtraction(lhs, rhs);
              my_stack.push(temp);
            }

        else if (word == "*")
            {
              temp = new Multiplication(lhs, rhs);
              my_stack.push(temp);
            }

        else if (word == "^")
            {
              temp = new Power(lhs, rhs);
              my_stack.push(temp);
            }

        else if (word == "/")
            {
              temp = new Division(lhs, rhs);
              my_stack.push(temp);
            }

        else
        {
          throw(logic_error("Ej giltig operator eller operand"));
        }
        root = my_stack.top();
      }

    }
    if (my_stack.size() > 1)
    {
      throw(logic_error("Saknar operator"));
    }
    else if (my_stack.size() == 0)
    {
      throw(logic_error("Tomt uttryck"));
    }
    else
    {
      root = my_stack.top();
    }
    
}

Expression::Expression()
:root{nullptr}
{

}

Expression::Expression(Expression && exp)
:root{exp.get_root()}
{

  exp.empty_root();
}

Expression::~Expression()
{
  if (!(root == nullptr))
  {
    delete root;
  }
  
}

Expression& Expression::operator=(Expression && exp)
{
  if (!(root == nullptr))
  {
    delete root;
  }
  root = exp.get_root();
  exp.empty_root();
  return *this;
}


std::string Expression::postfix() const
{
 
  return ((*root).postfix());
}

std::string Expression::infix() const
{
  
  return ((*root).infix());
}

std::string Expression::prefix() const
{
  return ((*root).prefix());
}

double Expression::evaluate() const
{
  return((*root).evaluate());
}

Node* Expression::get_root() const
{
  return root;
}

void Expression::empty_root() 
{
  root = nullptr;
}
