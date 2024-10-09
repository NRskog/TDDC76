#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;



struct Runner
{
  string name {};
  int hours {};
  int minutes {};
  int seconds {};
  
};

//deklaration
vector<Runner> read_file(string const & filename);
void sort(vector<Runner>& runners);
bool file_exist(string const & filename);
int get_int(int max);


int main()
{
  string filename {};
  cout<< "Ange filnamn: " << flush;

  cin >> filename;
  cin.ignore(1000,'\n');

  while(!file_exist(filename)) //Läser in filnamnet till det blir korrekt
  {
    cout << "FEL: Filen gick inte att öppna! " << endl << "Ange filnamn: "<< flush;
    cin >> filename;
  }
  vector<Runner> runners {read_file(filename)}; //Lagrar data från fil på vektor
  sort(runners);

  cout << "Ange antal rader: "<< flush;

  int amount_of_rows {get_int(runners.size())}; //Läser in antal rader med hjälp av funktion

//Utskrift
  cout << "   Namn   |   Tid    "
  << endl
  << "========================="
  << endl;

  for(int i {0}; i<amount_of_rows; i++)
  {
    cout << setw(8)
    << setfill(' ')
    << runners.at(i).name
    <<  "  |  "
    << setw(2) << setfill(' ')
    << runners.at(i).hours
    <<  " : " << setw(2)
    << setfill('0')
    << runners.at(i).minutes
    << " : "
    << setw(2)
    << setfill('0')
    << runners.at(i).seconds
    << endl;

  }
  return 0;
}


bool file_exist(string const & filename) // Försöker läsa fil
{
    ifstream file{filename};

    if (file)
    {
      return true;
    }
    else
    {
      return false;
    }
}


int get_int(int max) // Hanterar inmatning av antal rader och ser till att den blir korrekt
{
  int amount_of_rows {};
  bool done {false};

  while (!done)

  {
    cin>>amount_of_rows;


    if(cin.fail())
    {

      cout << "FEL: Inmatningen måste vara ett heltal!"
      << endl
      << "Ange antal rader: "
      << flush;
    }
    else
    {

      if(!((amount_of_rows >0) && (amount_of_rows<= max)))
      {
        cout << "Det finns inte " << amount_of_rows << " rader i filen."<< endl << "Ange antal rader: " << flush;
      }

      else
      {
        done = true;
      }

    }

  cin.clear();
  cin.ignore(10000,'\n');

  }
return amount_of_rows;
}


vector<Runner> read_file(string const & filename) //Läser in fil och konverterar till vektor av typ Runner
{
  string line {};
  vector<Runner> runners {};
  ifstream file {filename};

while (getline (file , line) )
  {

    istringstream iss {line};
    string name {};
    int hours {};
    int min {};
    int sec {};

    if ( iss >> name >> hours >> min >> sec )
    {
      runners.push_back({name,hours, min, sec});

    }

  }
  file.close ();
  return runners;
}


void sort(vector<Runner>& runners) // Sorterar vektor av typ runner

{
  for (unsigned int i{}; i < runners.size() - 1; ++i)
  {
    for (unsigned int j{}; j < runners.size() - i - 1; ++j)
    {
      if (runners.at(j).hours*3600 + runners.at(j).minutes * 60+ runners.at(j).seconds 
      > runners.at(j+1).hours*3600 + runners.at(j+1).minutes * 60+ runners.at(j+1).seconds)
      {
      swap(runners.at(j), runners.at(j + 1));
      }
    }
  }


}
