#include "ES.h"

int tc = 0;

string file_name(){
    string f;
    cout<<"Enter the file name (default: .bin file): ";
    cin>>f;
    bool flag = 1;
    for (int i=0; i<f.length(); i++)
        if (f[i]=='.') {
            flag = 0;
            break;
        }
    if (flag){
        f.append(".bin");
    }
    return f;
}

string new_pass(){
    string p;
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
        return new_pass();
    }
    return p;
}

char* to_char(string t){
    char *k = new char[t.length()+1];
    for (int i=0; i<t.length(); i++)
    k[i] = t[i];
    k[t.length()] = '\0';
    return k;
}

int hex_to_dec(char c){
    if (c>='0' && c<='9') return c-'0';
    return c+10-'a';
}

uint8_t* sha(string s){
    SHA256 a;
    a.update(s);
    return a.digest();
}

uint8_t* sha(uint8_t* s, int length){
    SHA256 a;
    a.update(s, length);
    return a.digest();
}

uint8_t* pass_check_util(string pw){
    auto s2 = sha(sha(pw), pwlen);
    auto s3 = sha(s2, pwlen);
    for (char i = 0; i<pwlen; i++) s3[i] ^= s2[i];
    return s3;
}

void encrypt(ifstream &inp, string f, string pw){
    string res;
    uint8_t t,k, pw_counter = 0;

    for (int i=0; i<tc; i++){
        inp>>t;
        res += t;
    }
    inp.close();
    remove(to_char(f));
    ofstream outp;
    outp.open(f);
    auto s = pass_check_util(pw);
    for (char i = 0; i<pwlen; i++){
        outp << hex << (s[i]>>4) << dec;
        outp << hex << (s[i]%(1<<4)) << dec;
    }
    s = sha(pw);
    for (auto i:res){
        t = (i^s[pw_order[pw_counter++]]);
        outp << hex << (t>>4) << dec;
        outp << hex << (t%(1<<4)) << dec;
        if (pw_counter>=pwlen) pw_counter = 0;
    }
    outp.close();
    cout<<"File encrypted successfully!\n\n";
}

bool verify_pass(string f, string pw){
    ifstream read;
    read.open(f);
    auto s = pass_check_util(pw);
    char t;
    uint8_t x;
    for (char i=0; i<pwlen; i++){
        read>>t;
        x = hex_to_dec(t);
        x <<= 4;
        read>>t;
        x += hex_to_dec(t);
        if (x!=s[i]) {
            read.close();
            return false;
        }
    }
    read.close();
    return true;
}

void display(ifstream &k){
    char t;
    while (!k.eof())
    {
        k >> t;
        cout << t;
    }
    k.close();
    return;
}

int count_alph(ifstream &inp){
    int t = -1;
    uint8_t c;
    while (!inp.eof()) {
        t++;
        inp>>c;
    }
    inp.close();
    return t;
}

string decrypt(ifstream &inp, string f, string pw){
    string res;
    char t, pw_counter = 0;
    uint8_t x;
    auto s = sha(pw);
    inp.seekg(2*pwlen);
    for (int i = 2*pwlen; i<tc; i+=2){
        inp>>t;
        x = hex_to_dec(t);
        x <<= 4;
        inp>>t;
        x += hex_to_dec(t);
        res += (x^s[pw_order[pw_counter++]]);
        if (pw_counter>=pwlen) pw_counter = 0;
    }
    inp.close();
    return res;
}

void enter(){
    cout<<"\n\nPress enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    return;        
}
