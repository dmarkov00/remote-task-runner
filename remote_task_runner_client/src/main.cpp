#include "client/Client.h"
#include <iostream>
#include <cstring>


static void PrintClientInfo() {
    std::cout << "Welcome to the remote task runner client! Choose from the menu.\n";
    std::cout << "\n";
    std::cout << ("(1) Send a test message\n");
    std::cout << ("(2) Authenticate with a passphrase\n");
    std::cout << ("(3) Turn on the lights\n");
    std::cout << ("(4) Turn off the lights\n");
    std::cout << ("(5) Show a code snippet on to the TV\n");
    std::cout << ("(6) Remove the image from the TV\n");
    std::cout << ("(7) Exit the client program\n");
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

                std::cout << "Client: please specify the passphrase" << std::endl;

                std::string pass;
                std::cin >> pass;

                // Tell the server that a pass is going to be sent
                bool messageSent = client.Send("#passphrase#");
                if (!messageSent) {
                    std::cout << "Client: error not the whole message was sent or some other error occurred"
                              << std::endl;
                }

                // Send the actual pass
                messageSent = client.Send(pass);

                if (!messageSent) {
                    std::cout << "Client: error not the whole message was sent or some other error occurred"
                              << std::endl;
                }

                // Get the result of authentication
                client.Receive(data);

                // Print the received message
                std::cout << "Authentication: " << data << std::endl;
            }
                break;
            case '3': {
                // Tell the server to turn on the lights
                client.Send("#lights_on#");


                std::cout << "Turn lights on request was made!" << std::endl;

                client.Receive(data);

                std::cout << data << std::endl;
            }
                break;
            case '4': {
                // Tell the server to turn on the lights
                client.Send("#lights_off#");

                std::cout << "Turn lights off request was made!" << std::endl;

                client.Receive(data);

                std::cout << data << std::endl;
            }
                break;
            case '5': {
                std::cout << "Specify the name of the file to be sent" << std::endl;

                std::string fileName;
                std::cin >> fileName;

                fileName = "file.txt";
                // Tell the server that a file is about to be sent
                client.Send("#put_file_on_tv#");

                client.SendFile(fileName);

                // Receive server response
                client.Receive(data);

                std::cout << data << std::endl;
            }
                break;
            case '6': {
                client.Send("#remove_file_from_tv#");

                std::cout << "Remove file from tv request was made!" << std::endl;

                client.Receive(data);

                std::cout << data << std::endl;

            }
                break;
            case '7':
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