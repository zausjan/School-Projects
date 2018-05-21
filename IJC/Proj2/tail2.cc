// tail2.cc
// Řešení IJC-DU2, příklad 1 b), 24.4.2018
// Autor: Ján Zauška, FIT, xzausk00@stud.fit.vutbr.cz
#include<iostream>
#include <fstream>
#include<queue>
#include<cstring>


#define DEFAULT_LINES 10

using namespace std;

//queue bude vzdy obsahovat maximalne n riadkov
//ak ich bude viac prvy sa odstrani
queue<string> read_file(istream &in, unsigned long n){
    queue<string> q;
    string str;
    while(!in.eof())
	{
		getline(in, str);
        if(in.eof() && str.empty())
        {
            break;
        }
        q.push(str);
        if(q.size() > n)
        {
            q.pop();
        }
	}
    return q;
}

//vypise vsetky riadky ulozene v queue
void print_lines(queue<string> q){
    while(! q.empty())
    {
        cout<<q.front()<<endl;
        q.pop();
    }
}


int main(int argc, char *argv[])
{
    ios::sync_with_stdio(false);
    istream* in;
    ifstream f;
    unsigned long n = DEFAULT_LINES;

    if(argc == 1)
    {
        in = &cin;
    }
    else if(argc == 2)
    {
        f.open(argv[1]);
        in = &f;
    }
    else if(argc == 3 && strcmp("-n", argv[2]))
    {
        n = strtod(argv[2], NULL);
        in = &cin;
    }
    else if(argc == 4 && strcmp("-n", argv[2]))
    {
        n = strtod(argv[2], NULL);
        f.open(argv[3]);
        in = &f;
    }
    else
    {
        fprintf(stderr, "Zle zadane argumenty.\n");
        return 1;
    }

    if(f.fail())
    {
        fprintf(stderr, "Chyba pri otvarani suboru.\n");
        return 1;
    }

    if(n <= 0){
        fprintf(stderr, "Nespravny pocet riadkov.\n");
        return 1;
    }
    queue<string> q = read_file(*in, n);
    print_lines(q);


    return 0;
}
