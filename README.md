# kxm
A network KxM (Keyboard/(void)/Mouse) server/client

## About

A program to forward keyboard and mouse input from a Windows desktop to a GNU/Linux desktop. Allows for multiple screens on a desk, while having multiple machines at the desk sharing the same input device.

## Usage

**Instructions for Windows are listed first for a reason! By default start.bat opens a TCP socket for the linux client to connect to. Attempting to run start.sh without start.bat running will result in the client closing instantly.**

Windows:

 - If you use the netcat I linked below, add it to your PATH, or edit start.bat to manually point to it
 - Edit the port in start.bat if necessary (make sure to do so in start.bat to match)
 - Run the following:
```
.\start.bat
```

Linux:

 - Edit the IP address in start.sh to the IP address of the computer which will forward input
 - Edit the port if necessary (make sure to do so in start.bat to match)
 - run the following (make sure you run the windows batch file first):
```
./start.sh
```

#### It's running, now what?

**The "Pause" key is the current hotkey for switching between remote and local input modes. Try hitting the key a few times. If you have the client and server connected you should see your mouse move on the remote machine when input is locked locally.**

## Compiling

#### Prereqs

Windows:
 - Clang http://releases.llvm.org/download.html
 - netcat or similar https://eternallybored.org/misc/netcat/

Linux:
 - make
 - gcc
 - XTest Extension headers
 - netcat or similar


#### Build

Windows:

```
> cd windows
> .\build.bat
```

Linux:

```
$ cd linux
$ make
```


