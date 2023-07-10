#include "ES.h"

int main(int argc, char* argv[]){
    // Format: ./a.exe <filepath> <password> <mode>
    if (argc > 1){
        string dec;
        string f = argv[1];
        string pw = argv[2];
        string mode = argv[3];
        ofstream outp;
        ifstream inp;
        inp.open(f);
        if (inp.fail()) return 1;
        if (mode=="1"){ // Mode 1 is encryption
            enc_algo1(inp,f,pw);
            return 0;
        }
        else if (mode=="2"){ // Mode 2 is decryption
            set_tc(inp);
            if (!verify_pass(f,pw+str_code))
                return 2;
            dec = decrypt(decrypt(inp,pw+str_code),pw);
            remove(to_char(f));
            outp.open(f);
            outp << dec;
            outp.close();
            return 0;
        }
        else return 3;
    }
    else
    while(1){
        uint16_t z;
        ifstream inp;
        ofstream outp;
        string f, pw, dec;

        system(clr);
        cout<<"1. Encrypt a new file\n2. Decrypt a file\n3. Display an encrypted file\n\n4. Display a file\n5. Create a new file\n6. Remove a file\n\n7. Time-lock a file (Assuming it's aleady encrypted)\n8. Show a time locked file\n\n9. Open ReadMe\n10. Exit\n\n-> ";
        cin>>z;

        if (z<9 && z!=5){
            f = file_name();
            inp.open(f);

            if (inp.fail()){
                cout<<"\nError! This file does not exist!\n\n";
                slp;
                continue;
            }
            
            if (z==8){
                inp.close();
                dec = f;
                f = edit_name(f);
                inp.open(f);
            }
            
            if ((z>=2 && z<=4) || z==8){
                set_tc(inp);
                cout<<"File opened successfully!\n";
            }
        }
        else if (z>=10){
            cout<<"\nThank You! Have a beautiful day!\nCode written by Teerath Agarwal";
            enter();
            return 0;
        }
        
        if (z==2 || z==3 || z==7 || z==8){
            if (!input_pass(f,pw)) continue;
        }

        switch (z){
            case 1:
                pw = new_pass();
                enc_algo1(inp,f,pw);
                cout<<"File encrypted successfully!\n\n";
                slp;
                break;
            case 2:
                dec = decrypt(decrypt(inp,pw+str_code),pw);
                remove(to_char(f));
                outp.open(f);
                outp << dec;
                outp.close();
                cout<<"File decrypted successfully!"<<endl;
                slp;
                break;
            case 3:
                dec = decrypt(decrypt(inp,pw+str_code),pw);
                system(clr);
                cout<<dec;
                cout<<"\n\n\nText displayed above is the decrypted file";
                enter();
                break;
            case 4:
                system(clr);
                display(inp);
                enter();
                break;
            case 5:
                f = file_name();
                cout<<"Enter the contents of the file. To end, type '#'.\n";
                cin.ignore((numeric_limits<streamsize>::max)(),'\n');
                getline(cin,dec,'#');
                outp.open(f);
                outp << dec;
                outp.close();
                break;
            case 6:
                inp.close();
                remove(to_char(f));
                break;
            case 7:
                inp.close();
                f = edit_name(f);
                time_lock(f,pw);
                break;
            case 8:
                dec = time_unlock(inp, pw, dec);
                if (!(dec.length()==1 && (dec[0]=='1' || dec[0]=='0')))
                cout<<dec;
                enter();
                break;
            case 9:
                readme();
                break;
        }
    }
    return 0;
}