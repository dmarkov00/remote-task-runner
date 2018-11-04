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


        } else if (data == "#put_file_on_tv#") {


        } else {   // Print received messages

            std::cout << "Received data: " << data << std::endl;

            // Send a message back
            server.Send(connectFd, "From server: Communication works!");
        }

        bool closed = server.CloseClientSocket(connectFd);

        if (!closed) {
            break;
        }
    }
    return 0;
}