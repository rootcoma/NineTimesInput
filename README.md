# kxm
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
 - netcat or similar (OpenSSL instructions soon, with encryption)

Windows:
 - Clang http://releases.llvm.org/download.html
 - netcat or similar https://eternallybored.org/misc/netcat/

