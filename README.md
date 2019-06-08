# kxm
A network KxM (Keyboard/(void)/Mouse) server/client


## About

A program to forward keyboard and mouse input from a GNU/Linux or Windows device to a separate GNU/Linux or Windows device. Uses TweetNaCl `crypto_stream_xor` for encrypting network communication between server and client.


## Compiling

#### Windows

Prereqs:
 - Clang http://releases.llvm.org/download.html
 - netcat https://eternallybored.org/misc/netcat/ or similar 

Build:
```
E:\git_repos> git clone https://github.com/rootcoma/kxm
E:\git_repos> cd kxm
E:\git_repos\kxm> build
```

#### GNU/Linux

Prereqs:
 - make
 - gcc
 - xcb
 - netcat or similar

Build:
```
~/git_repos $ git clone https://github.com/rootcoma/kxm
~/git_repos $ cd kxm
~/git_repos/kxm $ make
```


## Running

Edit config.txt or config-client.txt in the build folder and make sure the values match for server and client.

There is a server and client implementation for GNU/Linux and for Windows. For running a server on GNU/Linux run `./server-start.sh` and for Windows `.\server-start.bat`. After a server is running, then you can start a client; For GNU\Linux you can run `./client-start.sh` and for Windows `.\client-start.bat`.

**Important: The default values for IP addresses in these scripts should be changed.**


## Project State

This project is approaching a dog-food state. The encryption is not meant to be considered secure. There will be bugs and inconsistencies that need to be resolved.
