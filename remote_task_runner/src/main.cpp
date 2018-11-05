#include "server/Server.h"
#include <iostream>
#include <cstring>


void RefreshFileDescriptorList(fd_set &readfds, int &masterSocket, int &maxsd, int &sd, int &maxClients,
                               int clientSockets[]);

void ExecuteOperation(Server &server, int connectFd);

int main() {
    // Init server
    Server server;

    // It is going to be updated when a data is received
    std::string data;

    std::cout << "Server: Waiting for messages..." << std::endl;

    //set of socket descriptors
    int maxClients = 30;

    int clientSockets[maxClients];
    fd_set readfds;
    int masterSocket = server.GetMasterSocket();
    int maxsd, sd, activity, newSocket;


    // Initialise all sockets to 0
    for (int i = 0; i < maxClients; i++) {
        clientSockets[i] = 0;
    }


    while (true) {


        // Update file descriptors data
        RefreshFileDescriptorList(readfds, masterSocket, maxsd, sd, maxClients, clientSockets);

        // Wait for an activity on one of the sockets
        activity = select(maxsd + 1, &readfds, NULL, NULL, NULL);

        if (activity < 0) {
            std::cout << "Select method error" << std::endl;
        }

        // If something happened on the master socket , then its an incoming connection
        if (FD_ISSET(masterSocket, &readfds)) {
            if ((newSocket = server.Accept()) < 0) {
                break;
            } else {
                std::cout << "New client was connected.." << std::endl;

                // Tell the client that he got connected
                server.Send(newSocket, "You got connected to the server..");

                // Add a new socket to array of sockets
                for (int i = 0; i < maxClients; i++) {
                    // If the position is empty
                    if (clientSockets[i] == 0) {
                        clientSockets[i] = newSocket;
                        break;
                    }
                }
            }
        } else {
            // Operation on a client socket
            for (int i = 0; i < maxClients; i++) {

                sd = clientSockets[i];

                // If something happened on one of the client sockets
                if (FD_ISSET(sd, &readfds)) {
                    ExecuteOperation(server, sd);
                }
            }
        }

    }
    return 0;
}


void RefreshFileDescriptorList(fd_set &readfds, int &masterSocket, int &maxsd, int &sd, int &maxClients,
                               int clientSockets[]) {
    // Clear the socket set
    FD_ZERO(&readfds);

    // Add master socket to set
    FD_SET(masterSocket, &readfds);
    maxsd = masterSocket;


    // Add child sockets to set
    for (int i = 0; i < maxClients; i++) {
        // Socket descriptor
        sd = clientSockets[i];

        // If it is a  valid socket descriptor then add to read list
        if (sd > 0) {
            FD_SET(sd, &readfds);
        }

        // Highest file descriptor number, its needed for the select() function
        if (sd > maxsd) {
            maxsd = sd;
        }
    }
}

void ExecuteOperation(Server &server, int connectFd) {
    std::string data;

//     Receive new data
    server.Receive(connectFd, data);

//     When #passphrase# keyword is received, a second piece of data is expected containing the pass phrase
    if (data == "#passphrase#") {
        server.Receive(connectFd, data);
        std::cout << "Passphrase received!" << std::endl;
        if (server.VerifyPassphrase(data)) {

            server.Send(connectFd, "successful");
            std::cout << "Client authenticated!" << std::endl;
            server.SetClientAuthenticated(true);

        } else {
            server.Send(connectFd, "failed, incorrect pass");
            std::cout << "Client was NOT authenticated!" << std::endl;
        }

    } else if (data == "#lights_on#") {

        std::cout << "Request for turning on the lights was made" << std::endl;

        // Call methods

        server.Send(connectFd, "Lights turned on");

    } else if (data == "#lights_off#") {

        std::cout << "Request for turning off the lights was made" << std::endl;

        // Call methods

        server.Send(connectFd, "Lights turned off");

    } else if (data == "#put_file_on_tv#") {

        std::cout << "Request for showing file on the screen was made" << std::endl;

        server.ReceiveFile(connectFd, data);

        // Call methods
        server.Send(connectFd, "F");

    } else if (data == "#remove_file_from_tv#") {

        std::cout << "Request for removing file from the screen was made" << std::endl;

        server.DeleteFile();

    } else {   // Print received messages

        std::cout << "Received data: " << data << std::endl;

        // Send a message back
        server.Send(connectFd, "From server: Communication works!");
    }

}