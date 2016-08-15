#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <iterator>
#include <algorithm>    //for all_of
#include <cctype>
#include <sstream>         //for stringstream ss
#include <queue>

using namespace std;

void program();
void stmt(string);
void set(string);
void jump(string);
void jumpt(string);
void expr(string);
void term(string);
void factor(string);
bool is_number(string&);

ifstream file_in;     //delcraing instream
ofstream file_out;    //declaring outstream

int main(int argc, char** argv)
{
    string outF = "";

    switch(argc)
    {
        case 0: cout << "no arguments";
        break;

        case 1: cout << "no file names";
        break;

        case 2: 
                outF = argv[1];
                outF = outF + ".out";
                file_in.open(argv[1], ifstream::in);
                file_out.open(outF, ofstream::out);
                program();
                file_in.close();
                file_out.close();
        break;

        default: cout << "wrong input. please enter at least one file name";
        break;
    }
//    system("Pause");
    return 0;
}

void program()
{
    string statement[10];
    int i = 0;

    file_out << "Program" << endl;
    if (file_in.is_open())
    {
        while (getline(file_in, statement[i]))
        {
            stmt(statement[i]);
            //cout << statement[i] << endl; 
            i++;
            file_out << endl;
        }
    }

}
void stmt(string a)
{   file_out << "Statement" << endl;
    int count = 0;
    string tok;
    queue<string> q;

    //regex e("[,\+\-]|(==)");
    regex e("[,\\+\\-]|(==)");
    istringstream iss(regex_replace(a, e, " $0 "));
    while (iss >> tok)
    {
        q.push(tok);    
        count = count + 1;  
    }
    for (int i = 0; i < count - 1; i++)
    {
        tok = q.front();
        q.pop();
        if (tok == "set" || tok == "Set")
        {
            file_out << "Set" << endl;  
            tok = q.front();
            q.pop();
        }
        if (tok == "jump" || tok == "Jump")
        {
            file_out << "Jump" << endl;
            tok = q.front();
            jump(tok);
            q.pop();
        }
        else if (tok == "jumpt" || tok == "Jumpt")
        {
            file_out << "Jumpt" << endl;
            tok = q.front();
            q.pop();
            set(tok);
        }
        //else if (tok == "+" || tok == "-" || tok == ">" || tok == "<")
        else if (tok == "+" || tok == "-")
        {
            tok = q.front();
            term(tok);
            i++;
        }
        else if (tok != "set" && tok != "jump" && tok != "jumpt" && tok != "halt"
                && tok != "Set" && tok != "Jump" && tok != "Jumpt" && tok != "Halt")     //for the rest tokens after set
        {
            set(tok);   
            //cout << "tok " << tok << endl;
        }   
    }   
}
void set(string a)
{   
    if (a != "write" && a != "read" && a != "Write" && a != "Read" && a != "," 
            && a != "-" && a != "+" && a != "<" && a != ">" && a != "==")
    {
        expr(a);
    }   
}
void jump(string a)
{
    expr(a);
}
void jumpt(string a)
{
    expr(a);    
}
void expr(string a)
{
    file_out << "Expr" << endl;
    term(a);
}
void term(string a)
{
    file_out << "Term" << endl;
    factor(a);
}
void factor(string a)
{
    file_out << "Factor" << endl;
    if (is_number(a) == true)
    {
        file_out << "Number" << endl;
    }
    else if ((a.at(0) == 'D' || a.at(0) == 'd') && a.at(a.length() - 1) == ']')
    {
        file_out << "Expr" << endl;
        term(a.substr(2, a.length() - 3));  
    }
    else if ((a.at(0) == 'D' || a.at(0) == 'd') && a.at(a.length() - 1) != ']')
    {
        term(a.substr(a.length() - 2, a.length() - 1));
    }
    else
    {
        file_out << "Expr" << endl;
        //expr(a);
    }
}
bool is_number(string& a)
{
    return !a.empty() && all_of(a.begin(), a.end(), ::isdigit);
}
