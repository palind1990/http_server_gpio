cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
# set the project name
project(sockets)
#add the libraries
link_libraries(libtlpi.a PJ_RPI)
# add the client executable
add_executable(Client client.c)
# add the server executable
add_executable(Server server.c)
