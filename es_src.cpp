// Sub-part of ES
#include <iostream>
#include <fstream>
#include <string>
#include <ios>
#include <limits>
#include <unistd.h>
using namespace std;

#ifdef _WIN32
#define clear "cls"
#else
#define clear "clear"
#endif

void file_name(string &f)
{
    cout<<"Enter the file name with extension: ";
    cin>>f;
    return;
}

char* to_char(string t)
{
    char *k = new char[t.length()+1];
    for (int i=0; i<t.length(); i++)
    k[i] = t[i];
    k[t.length()] = '\0';
    return k;
}

void encrypt(ifstream &inp, string f)
{
    ofstream res;
    char t;
    int total_char=0, n;
    string temp_fn = "temp_file.txt";

    res.open(temp_fn);
    inp.unsetf(ios_base::skipws);
    while(!inp.eof())
    {
        inp>>t;
        res<<(int)t<<'\n';
        total_char++;
    }
    res.close();
    inp.close();
    remove(to_char(f));
    ifstream read;
    ofstream outp;
    read.open(temp_fn);
    outp.open(f);
    for(int i=1; i<total_char; i++)
    {
        read >> n;
        for (int j=0; j<8; j++)
        outp<<((n & 1<<j)>>j);
    }
    read.close();
    outp.close();
    remove(to_char(temp_fn));
    cout<<"File encrypted successfully!\n\n";
}

string create_str(string f)
{
    int i=f.length()-1;
    while (f[i]!='.') i--;
    f.insert(i,"_decrypted");
    return f;
}

void display_2(string s, int tc)
{
    char t;
    ifstream k;
    k.open(s);
    k.unsetf(ios_base::skipws);
    for (int i=0; i<tc; i++)
    {
        k >> t;
        cout << t;
    }
    k.close();
    remove(to_char(s));
    return;
}

int decrypt(ifstream &inp, string f)
{
    ofstream res;
    char t;
    int total_char=0, n=0, bit_counter=0;
    string de_fn = create_str(f);

    res.open(de_fn);
    while(!inp.eof())
    {
        inp>>t;
        n |= (t-48)<<bit_counter;
        if (bit_counter==7)
        {
            char x = n;
            res<<x;
            n = 0;
            bit_counter = 0;
            total_char++;
        }
        else bit_counter++;
    }
    res.close();
    inp.close();
    return total_char;
}

bool perm_conv(string f, int total_char)
{
    string de_fn = create_str(f);
    char t;
    ifstream read;
    ofstream outp;
    read.open(de_fn);
    outp.open(f);
    read.unsetf(ios_base::skipws);
    for(int i=0; i<total_char; i++)
    {
        read >> t;
        outp << t;
    }
    read.close();
    outp.close();
    remove(to_char(de_fn));
    cout<<"File decrypted successfully!\n\n";
    return 0;
}

int main()
{
    while(1)
    {
        short int z;
        ifstream inp;
        string f;

        cout<<"1. Convert a new file to binary\n2. Display a binary file\n3. Convert a binary file to normal file\n4. Exit\n-> ";
        cin>>z;
        
        if (z>=1 && z<=3)
        {
            file_name(f);
            inp.open(f);

            if (inp.fail())
            {
                cout<<"Error! This file does not exist!\n\n";
                continue;
            }
            else cout<<"File opened successfully!\n";
            if (z==3) inp.close();            
        }
        else
        {
            cout<<"\nThank You! Have a beautiful day!\n\nCode written by Teerath Agarwal\n\nPress ENTER to Continue ...";
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            return 0;
        }

        switch (z)
        {
            case 1:
                encrypt(inp,f);
                sleep(1);
                break;
            case 2:
                system(clear);
                display_2(create_str(f),decrypt(inp,f));
                cout<<"\n\n\nPress ENTER to Continue ...";
                cin.ignore(numeric_limits<streamsize>::max(),'\n');
                cin.ignore(numeric_limits<streamsize>::max(),'\n');
                system(clear);
                break;
            case 3:
                inp.open(f);
                perm_conv(f,decrypt(inp,f));
                sleep(1);
                system(clear);
                break;
        }
    }
    return 0;
}