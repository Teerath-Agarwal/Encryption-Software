# Encryption-Software
This is a software to encrypt and decrypt files, wth ultra high security which none other can provide. If you lose the password, these is absolutely no way to get your data back! NO WAY! (Honestly, I doubt if the world's best hackers can crack this algorithm).

Don't believe in anyone, except yourself for securing your utmost important files. Most of the softwares out there store your data with security risks. Some directly store password, some use password protected database. More or less, each one of them has some vulnerability.

But this one is truely different. The two encryption algorithm uses SHA-256 one way hashing as the underlying security mechanism. If you wanna break it, break SHA-256 first!

For this reason, I particularly suggest to either use one single password (atleast 8 characters) or store all your passwrod in a file and lock it with one password which you must remember whatsoever. I myself use the latter method find it safe.

## Table of Contents

- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)

## Features

- Shipped with a GUI as well as a CLI version.
- High speed and performance, since the core algorithm is written in C++.
- Work with multiple files at once, no issues.

## Requirements

1. GNU GCC Compiler
2. Make
4. Node Package Manager (npm)
5. Any suitable web browser.
6. Linux or MacOS

Run the following commands, if you don't have the above dependecies already installed.
1. For Linux - Fedora (You may use get-apt instead of dnf in case of Ubuntu, similarly for other distributions):
   ```shell
   sudo dnf update
   sudo dnf install git
   sudo dnf install gcc
   sudo dnf install make
   sudo dnf install nodejs npm
   ```
2. For MacOS:
   ```shell
   xcode-select --install
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   brew install git
   brew install gcc
   brew install make
   brew install node
   ```
3. For windows:
   
   Manually install minGW C++ compiler, make, git and nodeJS npm.
   The open git bash for further installation.

   
## Installation

1. Clone this repository.
2. Set the working directory to ./Encryption-Software/.
3. Execute $ make and $ npm install.
4. To start the application, run $ npm start.

     Alternatively, run the following commands:
     ```shell
     git clone https://github.com/Teerath-Agarwal/Encryption-Software.git
     cd Encryption-Software
     make
     npm install
     npm start
     ```

5. To stop the execution, either kill the terminal or press Ctrl + C.
6. To start again, navigate inside the folder Encryption-Software, and run:
   ```shell
   npm start
   ```

## Usage

1. Open you web-browser and in the address bar, type 'localhost:3000'.
2. Upload any file/files you want to encrypt.
3. Click on 'Encrypt' button.
4. Enter password and click submit.
5. Download the 'result.zip' file. Extract the contents
6. These are the encrypted files. If you change its content even slightly, it will probably corrupt instantly.
7. To decrypt, select those files to upload, (the actual encrypted files, not the .zip file)
8. Enter correct password. Click submit.
9. Save the result .zip file. Extract the contents.
10. The extracted content will consist of the original decrypted files. Thank You!
