#include "client/Client.h"
#include <iostream>
#include <cstring>


static void PrintClientInfo() {
    std::cout << "Welcome to the file dropper client! Choose from the menu.\n";
    std::cout << "\n";
    std::cout << ("(1) Send a test message\n");
    std::cout << ("(2) Ask the server for a permission to send a file\n");
    std::cout << ("(3) Send a file to server\n");
    std::cout << ("(4) Request a file from the server\n");
    std::cout << ("(5) Delete a file on the server\n");
    std::cout << ("(6) Exit program\n");
    std::cout << "\n";
    std::cout << ("Waiting for a choice..");
    std::cout << "\n";

}


int main() {
    bool quit = false;

    std::string data;

    Client client;

    // Print menu
    PrintClientInfo();

    while (!quit) {

        // Get input
        char menuChoice;
        std::cin >> menuChoice;

        // Establish connection
        bool connected = client.Connect();
        if (!connected) {
            std::cout << "Client: error when trying to connect to server" << std::endl;
        }

        switch (menuChoice) {

            case '1': {
                std::cout << "Client: input a message please" << std::endl;

                std::string message;
                std::cin >> message;

                bool messageSent = client.Send(message);

                if (!messageSent) {
                    std::cout << "Client: error not the whole message was sent or some other error occurred"
                              << std::endl;
                }
                client.Receive(data);

                // Print the received message
                std::cout << data << std::endl;

            }
                break;
            case '2': {
                bool messageSent = client.Send("#canIsendafile#");


                std::cout << "Waiting for the server to answer.." << std::endl;

                if (!messageSent) {
                    std::cout << "Client: error not the whole message was sent or some other error occurred"
                              << std::endl;
                    break;
                }

                client.Receive(data);

                // If the received message is equal to 'yes'
                if (data == "yes") {
                    std::cout << "Client: the server responded with 'yes', you can send files now" << std::endl;
                } else {
                    std::cout << "Client: the server did not allow you to send files" << std::endl;
                }
            }
                break;
            case '3': {
                // Indicate to the server that a file is going to be sent
                client.Send("#file#");

                // Send a file with hardcoded file path
                client.SendFile();
                std::cout << "File request was made.." << std::endl;

                client.Receive(data);

                std::cout << data << std::endl;
            }
                break;
            case '4': {
                std::cout << "Specify the id of your file" << std::endl;

                std::string fileId;
                std::cin >> fileId;

                // Tell the server that a file is wanted
                client.Send("#filerequest#");

                // Send a file name
                client.Send(fileId);

                std::cout << "The request was made.." << std::endl;

                // Receive response
                client.Receive(data);

                if (data == "No file with this id") {
                    std::cout << "The file does not exist on the server" << std::endl;
                } else {
                    client.WriteFile(data);
                    std::cout << "The requested file was received and written on the file system" << std::endl;
                }
            }
                break;
            case '5': {
                std::cout << "Specify the id of the file to be deleted" << std::endl;

                std::string fileId;
                std::cin >> fileId;

                // Tell the server that a file is wanted
                client.Send("#filedelete#");

                // Send a file name
                client.Send(fileId);

                std::cout << "The request for deletion was made.." << std::endl;

                // Receive response
                client.Receive(data);

                if (data == "No file with this id") {
                    std::cout << "The file does not exist on the server" << std::endl;
                } else {
                    std::cout << "The requested file was deleted successfully" << std::endl;
                }
            }
                break;
            case '6':
                quit = true;
                break;
            default:
                std::cout << "Client: your choice was incorrect" << std::endl;
                break;
        }

        bool closed = client.Close();
        if (!closed) {
            std::cout << "Client: error on closing" << std::endl;
        }
        // Need to reinitialize the object, since once socket is closed it cannot be reused?
        client = Client();
    }

    return 0;
}