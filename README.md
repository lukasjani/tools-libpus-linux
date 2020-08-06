# tools-libpus-linux
Client - server connection over TCP using ESROCOS' [PUS services library](https://github.com/ESROCOS/tools-libpus).


## License
GPLv2

## Installation
You need two Linux machines to communicate, they both run the same program.
For the server change the IS_SERVER flag in src/pus_test.h to 1 and specify the port.
For the client change the IS_SERVER flag in src/pus_test.h to 0, specify the port and the IP address of the server.

For both, run *make clear*.
For both, run *make*.
For both, run *Debug/tools-libpus-linux* to start the program.

## Thesis
The code was written in the scope of a Bachelors Thesis, which can be found [here](https://www.robo-technology.de/FOM/BA.pdf).
