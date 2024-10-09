#include<iostream>
#include<iomanip>
#include <string>
#include <vector>

using namespace std;


int main()

{
  int heltal {};
  float flyttal {};
  char tecken {};
  string text {};


  //1

  cout <<"Skriv in ett heltal: "
      << flush;

  cin >> heltal;


  cout << "Du skrev in talet: "
       << heltal
       <<'\n'
       << endl;
  cin.ignore(10000,'\n' );


  //2

   cout << "Skriv in fem decimaltal: "
	<< flush;

   cin >> flyttal;

   cout << "Du skrev in talen: "
	<< flyttal;

   cin >> flyttal;

   cout << ' '
	<< flyttal;

   cin >> flyttal;

   cout << ' '
	<< flyttal;

   cin >> flyttal;

   cout << ' '
	<< flyttal;

   cin >> flyttal;

   cout << ' '
	<< flyttal
	<<'\n'
	<<  endl;

   cin.ignore(10000,'\n' );


  //3

  cout << "Skriv in ett heltal och ett flyttal: "
       << flush;

  cin >> heltal
      >>flyttal;

  cout <<"Du skrev in heltalet: "
       <<  setw(11) << setfill(' ')
       << heltal
       <<"\n"
       <<"Du skrev in flyttalet: "
       << fixed
       << setw(10)
       << setfill(' ')
       << setprecision(4)
       << flyttal
       <<'\n'
       <<  endl;
  cin.ignore(10000,'\n' );


  //4

  cout << "Skriv in ett flyttal  och ett heltal: "
       << flush;

  cin >> flyttal
      >>heltal;

  cout <<"Du skrev in heltalet: "
       <<  setw(11) << setfill('-')
       << heltal
       <<"\n"
       <<"Du skrev in flyttalet: "
       << fixed
       <<  setw(10)
       << setfill('-')
       << setprecision(4)
       << flyttal
       <<'\n'
       << endl;
  cin.ignore(10000,'\n' );


  //5

  cout << "Skriv in ett tecken: "
	 << flush;

  cin >> tecken;

  cout << "Du skrev in tecknet: "
	 << tecken
	 <<'\n'
	 <<  endl;
  cin.ignore(10000,'\n' );


  //6

  cout<< "Skriv in ett ord: "
	<< flush;

    cin >> text;

    cout << "Du skrev in ordet: "
	 << text
	 <<'\n'
	 <<  endl;

    cin.ignore(10000,'\n' );


 //7

  cout << "Skriv in ett heltal och ett ord: "
       << flush;

  cin >> heltal
      >> text;

  cout << "Du skrev in talet |"
       << heltal
       << "| och ordet |"
       << text
       <<"|."
       << '\n'
       << endl;

  cin.ignore(10000,'\n' );


  //8

    cout << "Skriv in ett tecken och ett ord: "
	 << flush;

    cin >> tecken
	>> text;

    cout << "Du skrev in "
	<< '"'
	<< text
	<<'"'
	<<" och '"
	<< tecken
	<< "'.\n"
	<< endl;
    cin.ignore(10000,'\n' );


  //9

    cout << "Skriv in en rad text: "
	 << flush;

    getline(cin,text, '\n');

    cout << "Du skrev in: "
	 << '"'
	 << text
	 << '"'
	 <<'\n'
	 << endl;


   //10

    cout << "Skriv in en till rad text: "
	 << flush;

    getline(cin,text, '\n');

    cout << "Du skrev in: "
	 << '"'
	 << text
	 << '"'
	 << '\n'
	 <<  endl;


    //11

    cout << "Skriv in tre ord: "
	 << flush;

    cin >> text;

    cout <<"Du skrev in: "
	 << '"'
	 << text
	 << '"';


    cin >> text;
    cout << ", "
	 << '"'
	 << text
	 << '"'
	 << " och ";

    cin >> text;

    cout << '"'
	 << text
	 << '"'
	 << '\n'
	 <<  endl;

    cin.ignore(10000,'\n' );




  return 0;
}
