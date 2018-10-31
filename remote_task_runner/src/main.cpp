#include "Server.h"
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

        // When #file# keyword is received, a second piece of data is expected containing the file content
        if (data == "#file#") {

            if (server.IsClientAllowedToSendFiles()) {

                // Calling receive for the second time to receive the file
                server.Receive(connectFd, data);

                // Write the content of the file to the file system
                std::string fileName = server.WriteFile(data);

                std::cout << "Wrote file with name: " << fileName << std::endl;

                // Send a confirmation to the client and a the file name
                server.Send(connectFd,
                            "Your file was saved on the server. To request or delete your file use this id: " +
                            fileName);
            } else {
                server.Send(connectFd, "Sorry, you are not allowed to send files");

                std::cout << "Client tried to send a file but he was not allowed" << std::endl;
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

        } else if (data == "#filedelete#") {
            server.Receive(connectFd, data); // Calling receive for the second time to get the id of the file

            // Delete file by file name
            bool fileDeleted = server.DeleteFile(data);

            if (fileDeleted) {
                std::cout << "Deleted file with id: " << data<< std::endl;

                server.Send(connectFd, "File deleted successfully");
            } else {
                std::cout << "Client tried to delete file: " << data<<", but it was not found" << std::endl;

                server.Send(connectFd, "No file with this id");
            }


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