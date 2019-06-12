# KXM
A network KxM (Keyboard/(void)/Mouse) server/client

## About

This was a program I wrote to forward my keyboard and mouse input from my Windows desktop to my GNU/Linux laptop. I wanted the feeling of having multiple monitors while having 2 machines at my desk.

## Usage

Linux:

Edit the IP address in start.sh to point to whatever IP you have
```
./start.sh
```

Windows:

```
.\start.bat
```

The "Pause" key will stop user input from being consumed (Windows).


## Build

Linux:

```
$ cd linux
$ make
```

Windows:

```
> cd windows
> .\build.bat
```


## Prereqs

Linux:
 - make
 - gcc
 - X11/Xlib headers
 - XTest Extension headers
 - netcat or similar (see OpenSSL Setup below)

Windows:
 - Clang http://releases.llvm.org/download.html
 - netcat or similar (see OpenSSL Setup below) https://eternallybored.org/misc/netcat/

## OpenSSL setup

Linux:
 - Obtain OpenSSL from your package manager (you probably already have it)
 - After Windows is running start-secure.bat, run start-secure.sh

Windows:
 - Download OpenSSL https://slproweb.com/products/Win32OpenSSL.html
 - Add C:\Program Files (x86)\OpenSSL-Win32\bin (or your install location) to your PATH https://superuser.com/questions/949560/how-do-i-set-system-environment-variables-in-windows-10
 - cd back to kxm\windows
 - run:
```
openssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -nodes
```
 - You can hit enter for everything and you don't need to set a password
 - run start-secure.bat