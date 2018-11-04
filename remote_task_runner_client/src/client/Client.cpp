#include "Client.h"
#include "../common/protocol.h"

#include <cstring>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <fstream>

Client::Client() {
    // Create new socket
    Socket();
    // Initialize needed value
    SetupSocketData();
}

void Client::Socket() {

    // Create a socket, IPv4, Stream sockets, TCP
    socketFd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socketFd == -1) { // Some error checks
        perror("cannot create socket");
        exit(EXIT_FAILURE);
    }
}

void Client::SetupSocketData() {

    // Make sure the socket address struct is empty
    memset(&sa, 0, sizeof sa);
    // IPv4
    sa.sin_family = AF_INET;
    // Set a port number and convert it from host to network notation
    sa.sin_port = htons(PortNumber);

    // Specify address of server to connect, convert it and set it to the socket address
    int result = inet_pton(AF_INET, "127.0.0.1", &sa.sin_addr);
    if (result != 1) {
        perror("could not convert ip address to network address structure");
        exit(EXIT_FAILURE);
    }
}

bool Client::Connect() {
    int result = connect(socketFd, (struct sockaddr *) &sa, sizeof sa);

    if (result < 0) {
        perror("connect failed");
        close(socketFd);
        return false;
    } else {
        return true;
    }
}

bool Client::Send(std::string message) {
    // Send message
    size_t nrBytes = send(socketFd, message.c_str(), message.length(), 0);

    return nrBytes == message.length();
}

bool Client::Close() {

    if (shutdown(socketFd, SHUT_RDWR) < 0) {
        perror("shutdown failed");
        close(socketFd);
        return false;
    }

    close(socketFd);
    return true;
}

void Client::Receive(std::string &data) {

    const int BufferSize = DataBufferSize;
    char buffer[BufferSize];

    int nrBytes = recv(socketFd, buffer, BufferSize - 1, 0);

    if (nrBytes >= 0) {
        buffer[nrBytes] = '\0';

        data = buffer;
    } else {
        data = "Failed reading, data is empty";
    }
}

bool Client::SendFile(std::string fileName) {

//    fileName = "/" + fileName;

    // The open() func requires a char *
    const char *convertedFileName = fileName.c_str();

    std::cout << convertedFileName << std::endl;

    int fileFd = open(convertedFileName, O_RDONLY);

    std::cout << fileFd << std::endl;

    if (fileFd < 0) {
        std::cout << "File does not exist" << std::endl;
    } else {

        char buffer[DataBufferSize];

        while (true) {

            // Attempts to read up to count bytes from file descriptor fd
            // into the buffer starting at buf
            int bytesRead = read(fileFd, buffer, sizeof(buffer)-1);
            std::cout << "Buffer filled" << std::endl;
            if (bytesRead == 0) // We're done reading from the file
                break;

            if (bytesRead < 0) {
                std::cout << "An error occurred while reading from file:" << fileName << std::endl;
            }

            char *positionInBuffer;
            positionInBuffer = &buffer[0];

            while (bytesRead > 0) {

                // Write to socketFd
                int bytesWritten = write(socketFd, positionInBuffer, bytesRead);

                if (bytesWritten <= 0) {
                    std::cout << "Error when writing the file to a socket" << std::endl;

                }
                bytesRead -= bytesWritten;
                positionInBuffer += bytesWritten;
            }
        }
    }

    return false;
}

void Client::WriteFile(std::string content) {

    // Write to file
    std::ofstream outfile("received_file");

    outfile << content << std::endl;

    outfile.close();
}

