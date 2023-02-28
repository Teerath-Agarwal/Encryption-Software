// Project Started: 27 March 2022
// Last worked on: 26 November 2022 (major update)
#include "ES.h"

int tc = 0;
uint64_t int_code1 = 834725;
uint64_t int_code2 = 140737351116532;
string str_code = "Safest_passcode_ever@$p@#ker";

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
    cout<<"Enter a password: ";
    cin>>p;
    system(clr);
    string check;
    cout<<"Confirm password: ";
    cin>>check;
    system(clr);
    if (p.compare(check))
    {
        cout<<"Passwords don't match! Try again!\n";
        return new_pass();
    }
    // p.append(str_code);
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

void encrypt(ifstream &inp, string f, string pw){
    string res;
    uint8_t t,k, pw_counter1 = 0, pw_counter2 = 0;

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
        t = (i^s[pw_order[pw_counter1][pw_counter2++]]);
        outp << hex << (t>>4) << dec;
        outp << hex << (t%(1<<4)) << dec;
        if (pw_counter2>=pwlen) {
            pw_counter2 = 0;
            pw_counter1++;
            if (pw_counter1>=100) pw_counter1 = 0;
        }
    }
    outp.close();
    return;
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

void display2(string t){
    stringstream s(t);
    string pr;
    bool check = 1;
    while (!s.eof()){
        getline(s,pr);
        system(clr);
        cout << pr;
        cout<<"\n\n\n(Press ENTER...)";
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        if (check){
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            check = 0;
        }
    }
    system(clr);
    return;
}

int count_alph(ifstream &inp){
    int t = -1;
    uint8_t c;
    while (!inp.eof()) {
        t++;
        inp>>c;
    }
    // inp.close();
    return t;
}

string decrypt(ifstream &inp, string pw){
    string res;
    char t, pw_counter1 = 0, pw_counter2 = 0;
    uint8_t x;
    auto s = sha(pw);
    inp.seekg(2*pwlen);
    for (int i = 2*pwlen; i<tc; i+=2){
        inp>>t;
        x = hex_to_dec(t);
        x <<= 4;
        inp>>t;
        x += hex_to_dec(t);
        res += (x^s[pw_order[pw_counter1][pw_counter2++]]);
        if (pw_counter2>=pwlen) {
            pw_counter2 = 0;
            pw_counter1++;
            if (pw_counter1>=100) pw_counter1 = 0;
        }
    }
    inp.close();
    return res;
}

string decrypt(string fl, string pw){
    string res;
    char t, pw_counter1 = 0, pw_counter2 = 0;
    uint8_t x;
    auto s = sha(pw);
    for (int i = 2*pwlen; i<fl.size();){
        t = fl[i++];
        x = hex_to_dec(t);
        x <<= 4;
        t = fl[i++];
        x += hex_to_dec(t);
        res += (x^s[pw_order[pw_counter1][pw_counter2++]]);
        if (pw_counter2>=pwlen) {
            pw_counter2 = 0;
            pw_counter1++;
            if (pw_counter1>=100) pw_counter1 = 0;
        }
    }
    return res;
}

void enter(){
    cout<<"\n\nPress enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    return;
}

string edit_name(string f){
    int i=f.length()-1;
    while (f[i]!='.') i--;
    f.insert(i,"_tl");
    return f;
}

int d_to_m(uint8_t m){
    switch (m){
        case 1: return 0;
        case 2: return 31;
        case 3: return 59;
        case 4: return 90;
        case 5: return 120;
        case 6: return 151;
        case 7: return 181;
        case 8: return 212;
        case 9: return 243;
        case 10: return 273;
        case 11: return 304;
        case 12: return 334;
        default: return 365;
    }
}

uint64_t epoch_conv(uint64_t t){
    if (!t) return 0;
    uint64_t ep = 0;
    uint8_t yrs = ((t/10000)%100);
    int days = (yrs*365) + ((yrs+3)/4) + 10957;
    days += d_to_m((t/1000000)%100) + (t/100000000) - 1;
    if ((!(yrs%4)) && ((t/1000000)%100)>2) days++;
    ep = (((days*24)+((t/100)%100))*60 + (t%100))*60;
    ep -= 19800;
    return ep;
}

void time_lock(string f, string pw){
    uint64_t ts,te;
    string newpw;
    sleep(1);
    system(clr);
    cout<<"\nEnter the time for which file will be unlocked.\nFormat: DDMMYYHHMM, 24-hour format, no spaces. Enter 0 for no limit.\nStart:  ";
    cin>>ts;
    cout<<"Ending: ";
    cin>>te;
    cout<<"Generalized for time locked file,\n";
    newpw = new_pass();
    ts = epoch_conv(ts) + int_code1;
    te = epoch_conv(te) + int_code2;
    ofstream outp;
    outp.open(f);
    outp << pw << endl;
    outp << ts << endl;
    outp << te;
    outp.close();

    ifstream inp;
    inp.open(f);
    inp.unsetf(ios_base::skipws);
    tc = count_alph(inp);
    inp.clear();
    inp.seekg(0);
    inp.unsetf(ios_base::skipws);
    encrypt(inp,f,str_code+newpw+str_code+str_code);

    inp.open(f);
    inp.unsetf(ios_base::skipws);
    tc = count_alph(inp);
    inp.clear();
    inp.seekg(0);
    inp.unsetf(ios_base::skipws);
    encrypt(inp,f,newpw+str_code);
    return;
}

string time_unlock(ifstream &inp, string pw, string f){
    stringstream s(decrypt(decrypt(inp,pw+str_code),str_code+pw+str_code+str_code));
    getline(s,pw);
    uint64_t ts,te,tn;
    s>>ts;
    s>>te;
    ts -= int_code1;
    te -= int_code2;
    tn = time(NULL);
    if (ts>tn){
        system(clr);
        cout<<"You are not allowed to open the file yet!\n";
        return "0";
    }
    if (te && te<tn){
        system(clr);
        cout<<"Sorry, it's too late, the files have expired!\n";
        remove(to_char(edit_name(f)));
        return "1";
    }
    inp.open(f);
    inp.unsetf(ios_base::skipws);
    tc = count_alph(inp);
    inp.clear();
    inp.seekg(0);
    inp.unsetf(ios_base::skipws);
    system(clr);
    return decrypt(decrypt(inp,pw+str_code),pw);
}

void readme(){
    system(clr);
    cout<<"This software is designed to encrypt and secure files. Default format used is .bin\nThere are several other formats that work, such as .txt and .dat. For Linux platform, .wav (audio) and .mp4 (video) can also be encrypted.\nMost of the options are self explainatory. Note that once a file is encrypted, there is no way to decrypt it if the password is forgotten. Even if the source code is available.\nExplaining option 7 and 8, it is basically a feature where the user can unlock their encrypted file(probably for someone else) for specific period of time, and set a different password for them. This way, user can show their encrypted file to someone else for any period of time (might be in future), and still not tell them the original password, just the password that was set later. This maintains data security."<<endl;
    enter();
    return;
}