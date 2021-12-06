# http_server_gpio
***client and server gpio toggle with index.html***
***this is not complete***

## Usage

## first try
```bash
# install library
tlpi-dist

# create build folder
mkdir build

# build
cmake ../ (in build-folder)
make

# run (2 terminals)
sudo ./Client localhost 8001
sudo ./Server 8001
 ```
 
 ## second try
```bash
# install library
tlpi-dist

# build
sudo gcc ./http-server.c

# run (2 terminals)
./a.out
 ```
 
