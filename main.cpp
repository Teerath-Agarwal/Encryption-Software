// Project Started: 27 March 2022
// Version 2 started on: 26 November 2022 (major update)
// Last worked: 1 December 2022
// formats tested: .bin, .dat, .txt, .wav, .mp4 (audio & video as well)
#include "ES.h"

int main()
{
    while(1)
    {
        uint16_t z;
        ifstream inp;
        ofstream outp;
        string f, pw, dec;

        system(clear);
        cout<<"1. Encrypt a new file\n2. Decrypt a file\n3. Display an encrypted file\n4. Display a decrypted file\n5. Create a new file\n6. Exit\n-> ";
        cin>>z;

        if (z>=1 && z<=4)
        {
            f = file_name();
            inp.open(f);

            if (inp.fail())
            {
                cout<<"Error! This file does not exist!\n\n";
                continue;
            }
            else {
                inp.unsetf(ios_base::skipws);
                tc = count_alph(inp);
                // cout<<"tc = "<<tc<<endl;
                inp.open(f);
                inp.unsetf(ios_base::skipws);
                cout<<"File opened successfully!\n";
            }
        }
        else if (z!=5)
        {
            cout<<"\nThank You! Have a beautiful day!\nCode written by Teerath Agarwal";
            enter();
            return 0;
        }

        bool pass = 0;
        if (z==2 || z==3){
            bool rep = 1;
            while (rep){
                cout<<"Password: ";
                cin>>pw;
                if (verify_pass(f,pw)){
                    cout<<"Password Verified!\n";
                    rep = 0;
                }
                else {
                    char bin;
                    cout<<"Incorrect Password!\nTry Again? (Y/N) -> ";
                    cin>>bin;
                    if (bin=='n' || bin=='N'){
                        rep = 0;
                        pass = 1;
                    }
                }
            }
        }
        if (pass) continue;

        switch (z)
        {
            case 1:
                pw = new_pass();
                encrypt(inp,f,pw);
                sleep(1);
                break;
            case 2:
                dec = decrypt(inp,f,pw);
                remove(to_char(f));
                outp.open(f);
                outp << dec;
                outp.close();
                cout<<"File decrypted successfully!"<<endl;
                sleep(1);
                break;
            case 3:
                dec = decrypt(inp,f,pw);
                system(clear);
                cout<<dec;
                cout<<"\n\n\nText displayed above is the decrypted file";
                enter();
                break;
            case 4:
                system(clear);
                display(inp);
                enter();
                break;
            case 5:
                f = file_name();
                cout<<"Enter the contents of the file. To end, type '#'.\n";
                cin.ignore(numeric_limits<streamsize>::max(),'\n');
                getline(cin,dec,'#');
                outp.open(f);
                outp << dec;
                outp.close();
                sleep(1);
                break;
        }
    }
    return 0;
}
