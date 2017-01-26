This is an incomplete version of the bank server. It only consists of a single client session process which can handle a single client.

USAGE:

Make will compile both the client and server executables.

server: ./run_server <port #>

client: ./run_client <host name> <port #>

The client takes commands as specified in the project description. "exit" will close both the client and server.
The server side does not take any inputs until it connects to a client. "ctrl-c" will close the server cleanly (close sockets, disconnect from port).