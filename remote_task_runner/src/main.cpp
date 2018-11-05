#include "server/Server.h"
#include <iostream>
#include <cstring>

int main() {
    // Init server
    Server server;

//    int connectFd;

    // It is going to be updated when a data is received
    std::string data;

    std::cout << "Server: Waiting for messages..." << std::endl;
    /////////////////////////////////////////////the new setup///////////////////////////

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

        // Clear the socket set
        FD_ZERO(&readfds);

        // Add master socket to set
        FD_SET(masterSocket, &readfds);
        maxsd = masterSocket;


        // Add child sockets to set
        for (int i = 0; i < maxClients; i++) {
            // Socket descriptor
            sd = clientSockets[i];

            //if valid socket descriptor then add to read list
            if (sd > 0) {
                FD_SET(sd, &readfds);
            }

            // Highest file descriptor number, its needed for the select() function
            if (sd > maxsd) {
                maxsd = sd;
            }
        }

        std::cout << "Server: Waiting for messages ON SELECT" << std::endl;

        // Wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
        activity = select(maxsd + 1, &readfds, NULL, NULL, NULL);
        if (activity < 0) {
            std::cout << "Select method error" << std::endl;
        }

        //If something happened on the master socket , then its an incoming connection
        if (FD_ISSET(masterSocket, &readfds)) {
            if ((newSocket = server.Accept()) < 0) {
                std::cout << "NEW CONNECTION" << std::endl;

                //send new connection greeting message
                server.Send(newSocket, "Yo got connected");


                perror("accept");
                break;
            } else {
                std::cout << "NEW CONNECTION was made" << std::endl;


                //add new socket to array of sockets
                for (int i = 0; i < maxClients; i++) {
                    //if position is empty
                    if (clientSockets[i] == 0) {
                        clientSockets[i] = newSocket;

                        std::cout << "ADDED TO the list of client sockets" << std::endl;

                        break;
                    }
                }
            }




        }

        // Else its some IO operation on some other socket
        for (int i = 0; i < maxClients; i++) {

            sd = clientSockets[i];

            if (FD_ISSET(sd, &readfds)) {
                //Check if it was for closing , and also read the incoming message
                server.Receive(sd, data);
                std::cout << "Socket descriptor ------  " << sd << std::endl;

                std::cout << "THE data is  ------  " << data << std::endl;

                if (data == "Failed reading, data is empty") {

                    std::cout << "Somebody disconnected" << std::endl;
//                    server.CloseClientSocket(sd);
                    clientSockets[i] = 0;

                } else {
                    server.Send(sd, "Got your message");
                };

            }
        }
//
//        // Accept connection
//        connectFd = server.Accept();

        // Receive new data
//        server.Receive(connectFd, data);

        // When #passphrase# keyword is received, a second piece of data is expected containing the pass phrase
//        if (data == "#passphrase#") {
//            server.Receive(connectFd, data);
//            std::cout << "Passphrase received!" << std::endl;
//            if (server.VerifyPassphrase(data)) {
//
//                server.Send(connectFd, "successful");
//                std::cout << "Client authenticated!" << std::endl;
//                server.SetClientAuthenticated(true);
//
//            } else {
//                server.Send(connectFd, "failed, incorrect pass");
//                std::cout << "Client was NOT authenticated!" << std::endl;
//            }
//
//        } else if (data == "#lights_on#") {
//
//            std::cout << "Request for turning on the lights was made" << std::endl;
//
//            // Call methods
//
//            server.Send(connectFd, "Lights turned on");
//
//        } else if (data == "#lights_off#") {
//
//            std::cout << "Request for turning off the lights was made" << std::endl;
//
//            // Call methods
//
//            server.Send(connectFd, "Lights turned off");
//
//        } else if (data == "#put_file_on_tv#") {
//
//            std::cout << "Request for showing file on the screen was made" << std::endl;
//
//            server.ReceiveFile(connectFd, data);
//
//            // Call methods
//            server.Send(connectFd, "F");
//
//        } else if (data == "#remove_file_from_tv#") {
//
//            std::cout << "Request for removing file from the screen was made" << std::endl;
//
//            server.DeleteFile();
//
//        } else {   // Print received messages
//
//            std::cout << "Received data: " << data << std::endl;
//
//            // Send a message back
//            server.Send(connectFd, "From server: Communication works!");
//        }
//
//        bool closed = server.CloseClientSocket(connectFd);
//
//        if (!closed) {
//            std::cout << "Exiting: error closing client socket" << std::endl;
//            break;
//        }
    }
    return 0;
}