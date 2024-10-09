#include "calc.h"
#include "expression.h"
#include "token.h"
#include "postfix.h"
#include <iostream>
#include <string>



using namespace std;


int run()
{
    std::string exp;
    getline(cin ,exp);
    Postfix post {exp};

    Expression e {post.to_string()};
    cout << "postfix:" << e.postfix()
    <<"\n value:"<<e.evaluate() << endl;
    return 0;
}
