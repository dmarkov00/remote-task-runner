#include "server/Server.h"
#include <iostream>
#include <cstring>

int main() {
    // Init server
    Server server;

    int connectFd;

    // It is going to be updated when a data is received
    std::string data;

    std::cout << "Server: Waiting for messages..." << std::endl;

    while (true) {

        // Accept connection
        connectFd = server.Accept();

        // Receive new data
        server.Receive(connectFd, data);

        // When #passphrase# keyword is received, a second piece of data is expected containing the pass phrase
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


        } else if (data == "#filerequest#") {
            server.Receive(connectFd, data); // Calling receive for the second time to get the id of the file

            // Send the requested file
            bool fileSent = server.SendFile(connectFd, data);

            if (!fileSent) {
                server.Send(connectFd, "No file with this id");
            } else {
                std::cout << "File with name:" << data << " was requested and sent to the client" << std::endl;
            }

        } else if (data == "#filesent#") {


            server.ReceiveFile(connectFd, data);


        } else if (data == "#canIsendafile#") {   // If data is equal to "Can I send a file?"

            std::cout << "Client asks if he can send files? Type 'yes' to allow him or"
                         " any other symbol to NOT allow him!" << std::endl;

            std::string sendFile;

            std::cin >> sendFile;

            if (sendFile == "yes") {
                server.SetClientAllowedToSendFiles(true);

                server.Send(connectFd, "yes");
            } else {
                server.Send(connectFd, "no");
            }

        } else {   // Print received messages

            std::cout << "Received data: " << data << std::endl;

            // Send a message back
            server.Send(connectFd, "Communication works!");
        }

        bool closed = server.CloseClientSocket(connectFd);

        if (!closed) {
            break;
        }

    }
    return 0;
}