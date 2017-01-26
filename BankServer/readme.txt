This version is able to handle multiple clients. This program uses processes in the form of the fork command in order to handle the different clients on the server side and uses fork to split the read and write threads

USAGE:

Make will compile both the client and server executables.

server: ./run_server <port #>

client: ./run_client <host name> <port #>

The client takes commands as specified in the project description. "exit" will close both the client and server.
The server side does not take any inputs until it connects to a client. "ctrl-c" will close the server cleanly (close sockets, disconnect from port).