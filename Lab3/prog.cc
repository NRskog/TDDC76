#include "calc.h"
#include "expression.h"
#include "token.h"
#include "postfix.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;


int main()

    {
        std::string exp{};
        bool initated = false;
        bool run = true;
        int i = 0;
        Expression active{};
    

        vector<Expression> expressions {};
        
    
        while(!initated)
        {
            
            cout << "Skriv in ditt första uttryck:" << endl;
            getline(cin,exp);
            if (exp.empty())
            {
                cout << "Skriv inte tomt uttryck." << endl;
            } 
            else if (exp.at(0) == ':')
            {
                if (exp == ":quit" || exp == ":exit")
                {
                run = false;
                break;
                }
                else
                {
                cout << "Du behöver mata in uttryck först." ;
                }
            }  
            else
            {
                try
                {
             
                active =  move(Expression{Postfix(exp).to_string()});
                initated = true;
                }
                catch (const std::exception& e)
                {
                    cout << "felaktig inmatning"<<endl;
                    
                }
            }
            
            }
        
        while(run)
        {
            cout << "Skriv:" << endl;
            getline(cin,exp); 
            if (exp.empty())
            {
                cout << "Skriv inte tomt uttryck."<< endl;
            }      
            else if  (exp.at(0) == ':')
            {   
                if (exp == ":prefix")
                {
                    cout << (active).prefix() << endl; 
                }
                else if (exp == ":postfix")
                {
                    cout << (active).postfix() << endl;
                }
                else if (exp == ":infix")
                {
                    cout << (active).infix() << endl;
                }
                else if (exp == ":calc")
                {
                    cout << (active).evaluate() << endl;
                }
                else if (exp == ":quit"|| exp == ":exit")
                {
                    run = false;
                }

                else if (exp == ":save")
                {  
                   expressions.push_back((active).postfix());
            
                }

                else if (exp == ":list")
                {
                    for ( long unsigned int j{0}; j < expressions.size(); j++ )
                    {
                        cout << j  << ": " << expressions.at(j).infix() << endl;
                    }
                    
                }

                istringstream iss3 {exp};
                string temp2{};
                iss3 >> temp2;

                if (temp2 == ":activate")
                {
                    iss3 >> i;
                    active = move(expressions.at(i));
                    expressions.erase(expressions.begin() + i);
                }

            }
            else
            {
               
                try 
                {
                
                active = move(Expression {Postfix(exp).to_string()});
                
                }
                catch (const std::exception& e)
                {
                    cout << "felaktig inmatning"<<endl;
                    
                }
            }

        }
        


    }   
