#include "server/Server.h"
#include <iostream>
#include <cstring>
#include <tasks_runner/HomeTasksCoordinator.h>
#include <tasks_runner/tasks/HomeLight.h>
#include <tasks_runner/tasks/TVScreenFileVisualizer.h>


void RefreshFileDescriptorList(fd_set &readfds, int &masterSocket, int &maxsd, int &sd, int clientSockets[]);

void ExecuteOperation(Server &server, HomeTasksCoordinator &homeTasksCoordinator, int connectFd);

const int MaxClients = 30;

int main() {

    HomeLight homeLight;
    TVScreenFileVisualizer fileVisualizer;
    HomeTasksCoordinator homeTasksCoordinator(homeLight, fileVisualizer);

    // Init server
    Server server;

    // It is going to be updated when a data is received
    std::string data;

    std::cout << "Server: Waiting for messages..." << std::endl;

    //set of socket descriptors

    int clientSockets[MaxClients];
    fd_set readfds;
    int masterSocket = server.GetMasterSocket();
    int maxsd, sd, activity, newSocket;


    // Initialise all sockets to 0
    for (int i = 0; i < MaxClients; i++) {
        clientSockets[i] = 0;
    }


    while (true) {
        // Reset data
        RefreshFileDescriptorList(readfds, masterSocket, maxsd, sd, clientSockets);

        // Wait for an activity on one of the sockets
        activity = select(maxsd + 1, &readfds, NULL, NULL, NULL);

        if (activity < 0) {
            std::cout << "Select method error" << std::endl;
        }

        // If something happened on the master socket (socket can be read() without blocking), then its an incoming connection
        if (FD_ISSET(masterSocket, &readfds)) {
            if ((newSocket = server.Accept()) < 0) {
                break;
            } else {
                std::cout << "New client was connected.." << std::endl;

                // Tell the client that he got connected
                server.Send(newSocket, "You got connected to the server..");

                // Add a new socket to array of sockets
                for (int i = 0; i < MaxClients; i++) {
                    // If the position is empty
                    if (clientSockets[i] == 0) {
                        clientSockets[i] = newSocket;
                        break;
                    }
                }
            }
        } else {
            // Operation on a client socket
            for (int i = 0; i < MaxClients; i++) {

                sd = clientSockets[i];

                // If something happened on one of the client sockets -(the socket can be read() without blocking)
                if (FD_ISSET(sd, &readfds)) {
                    ExecuteOperation(server, homeTasksCoordinator, sd);
                }
            }
        }

    }
    return 0;
}

/**
    The fd_set need to be reset before every select() call (this is how it works)
 */
void RefreshFileDescriptorList(fd_set &readfds, int &masterSocket, int &maxsd, int &sd, int clientSockets[]) {

    // Clear the socket set
    FD_ZERO(&readfds);

    // Add master socket to set
    FD_SET(masterSocket, &readfds);
    maxsd = masterSocket;


    // Add child sockets to set
    for (int i = 0; i < MaxClients; i++) {
        // Socket descriptor
        sd = clientSockets[i];

        // If it is a  valid socket descriptor then add to read list
        if (sd > 0) {
            FD_SET(sd, &readfds);
        }

        // Get the highest file descriptor number, select() function needs number higher than the max sd as specified in the man page
        if (sd > maxsd) {
            maxsd = sd;
        }
    }
}

void ExecuteOperation(Server &server, HomeTasksCoordinator &homeTasksCoordinator, int connectFd) {
    std::string data;

//     Receive new data
    server.Receive(connectFd, data);

//     When #passphrase# keyword is received, a second piece of data is expected containing the pass phrase
//    if (data == "#passphrase#") {
//        server.Receive(connectFd, data);
//        std::cout << "Passphrase received!" << std::endl;
//        if (server.VerifyPassphrase(data)) {
//
//            server.Send(connectFd, "successful");
//            std::cout << "Client authenticated!" << std::endl;
//            server.SetClientAuthenticated(true);
//
//        } else {
//            server.Send(connectFd, "failed, incorrect pass");
//            std::cout << "Client was NOT authenticated!" << std::endl;
//        }
//
//    } else
    if (data == "#lights_on#") {

        std::cout << "Request for turning on the lights was made" << std::endl;

        // Execute task
        homeTasksCoordinator.TurnOnHomeLight();

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

        if (data.empty()) {
            std::cout << "Got empty data, maybe someone disconnected " << data << std::endl;
        }
        std::cout << "Received data: " << data << std::endl;

        // Send a message back
        server.Send(connectFd, "From server: Communication works!");
    }

}