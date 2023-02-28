// Project Started: 27 March 2022
// Last worked on: 1 April 2022
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

void new_pass(string &p)
{
    cout<<"Enter a password for your file: ";
    cin>>p;
    system(clear);
    string check;
    cout<<"Confirm password: ";
    cin>>check;
    system(clear);
    if (p.compare(check))
    {
        cout<<"Passwords don't match! Try again!\n";
        new_pass(p);
    }
    return;
}

void encrypt(ifstream &inp, string f, string pw)
{
    ofstream res;
    char t;
    int total_char=0, n, pw_len = pw.length(), pw_counter = 0;
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
        n *= (int)pw[pw_counter++];
        if (pw_counter==pw_len) pw_counter=0;
        for (int j=0; j<14; j++)
        outp<<((n & 1<<j)>>j);
        if (!(i%10)) outp<<'\n';
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

void display_1(string s)
{
    char t;
    ifstream k;
    k.open(s);
    k.unsetf(ios_base::skipws);
    for (int i=0; i<100; i++)
    {
        k >> t;
        cout << t;
    }
    k.close();
    return;
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
    // cout<<"\n\nPress ENTER to Continue ...";
    // cin.ignore(numeric_limits<streamsize>::max(),'\n');
    return;
}

int decrypt(ifstream &inp, string f, string pw)
{
    ofstream res;
    char t;
    int total_char=0, n=0, bit_counter=0, pw_len = pw.length(), pw_counter = 0;
    string de_fn = create_str(f);

    res.open(de_fn);
    while(!inp.eof())
    {
        inp>>t;
        n |= (t-48)<<bit_counter;
        if (bit_counter==13)
        {
            n /= (int)pw[pw_counter++];
            if (pw_counter==pw_len) pw_counter=0;
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
    system(clear);
    cout<<"Displaying the first 100 characters of the file, based on the password enetered.\n\n";
    // sleep(2);
    // system(clear);
    display_1(de_fn);
    // sleep(2);
    // system(clear);
    cout<<"\n\nPress 'Y' to merge this file to original one or 'N' to re-enter the password.\nNote: If you press 'Y' and the password was incorrect, then the information will be lost forever!\nNote 2: You can also check the temporary file with the name: "<<de_fn<<"\n(Y/N)-> ";
    cin>>t;
    if (t=='N' || t=='n')
    {
        remove(to_char(de_fn));
        system(clear);
        cout<<"Re-enter the ";
        return 1;
    }
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

bool is_encrypted(string s)
{
    ifstream t(s);
    char c;
    for (int i=0; i<100 && !t.eof(); i++)
    {
        t >> c;
        if (c!='0' && c!='1') return 0;
    }
    return 1;
}

int main()
{
    while(1)
    {
        short int z;
        ifstream inp;
        string f, pw;

        cout<<"1. Secure a new file\n2. Display a secured file\n3. Convert a secured file to normal file\n4. Exit\n-> ";
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
                new_pass(pw);
                encrypt(inp,f,pw);
                sleep(1);
                break;
            case 2:
                cout<<"Password: ";
                cin>>pw;
                system(clear);
                display_2(create_str(f),decrypt(inp,f,pw));
                cout<<"\n\n\nFile displayed above is decrypted using the password entered. Incorrect password leads to incorrect decryption.\n\n";
                cout<<"Press ENTER to Continue ...";
                cin.ignore(numeric_limits<streamsize>::max(),'\n');
                cin.ignore(numeric_limits<streamsize>::max(),'\n');
                system(clear);
                break;
            case 3:
                do
                {
                    inp.open(f);
                    cout<<"Password: ";
                    cin>>pw;
                } while(perm_conv(f,decrypt(inp,f,pw)));
                sleep(1);
                system(clear);
                break;
        }
    }
    return 0;
}