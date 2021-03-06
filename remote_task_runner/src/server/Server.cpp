#include "Server.h"
#include "protocol.h"

#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fstream>
#include <fcntl.h>
#include <sys/sendfile.h>

Server::Server() {
    // Create new socket
    if (!Socket()) {
        return;
    }

    // Initialize needed value
    SetupSocketData();
    // Bind to local port
    bool result = Bind();

    // If successfully binded
    if (result) {
        // Start listening
        result = Listen();
        if (!result) {
            std::cout << "Something broke on listening" << std::endl;
        } else {
//            std::cout << "Server: Started listening.." << std::endl;
        }
    } else {
        std::cout << "Something broke on binding" << std::endl;
    }
}

bool Server::Socket() {
    // Create a socket, IPv4, Stream sockets, TCP
    socketFd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Check if it is valid
    if (socketFd < 0) {
        perror("Cannot create socket");
        return false;
    }
    return true;
}

void Server::SetupSocketData() {
    // Make sure the socket address struct is empty
    memset(&sa, 0, sizeof sa);
    // IPv4
    sa.sin_family = AF_INET;
    // Set a port number and convert it from host to network notation
    sa.sin_port = htons(PortNumber);
    // Setting the IP address of the socket to localhost?
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
}

bool Server::Bind() {
    if (bind(socketFd, (struct sockaddr *) &sa, sizeof sa) < 0) {
        perror("Bind failed");
        CloseServerSocket();
        return false;
    } else {
        return true;
    }
}

bool Server::Listen() {
    if (listen(socketFd, 10) < 0) {
        perror("listen failed");
        CloseServerSocket();
        return false;
    } else {
        return true;
    }
}

int Server::Accept() {

    int connectFd = accept(socketFd, (struct sockaddr *) &their_addr, &addr_size);

    if (connectFd < 0) {
        perror("accept failed");
        CloseServerSocket();
        exit(EXIT_FAILURE);
    }
    return connectFd;
}


void Server::CloseServerSocket() {
    close(socketFd);
}

bool Server::CloseClientSocket(int connectFd) {

    if (shutdown(connectFd, SHUT_RDWR) < 0) {
//        perror("shutdown failed");
        close(connectFd);
//        CloseServerSocket();
//        return false;
    }
    close(connectFd);
    return true;
}

bool Server::Send(int connectFd, std::string message) {
    // Send message
    size_t nrBytes = send(connectFd, message.c_str(), message.length(), 0);

    return nrBytes == message.length();
}

bool Server::SendFile(int connectFd, std::string fileName) {
    fileName = "./" + fileName;

    // The open() func requires a char *
    const char *convertedFileName = fileName.c_str();

    int fileFd = open(convertedFileName, O_RDONLY);

    if (fileFd < 0) {
        std::cout << "File does not exit" << std::endl;
        return false;
    } else {
        const int BufferSize = DataBufferSize;

        sendfile(connectFd, fileFd, NULL, (size_t) BufferSize);
        return true;
    }
}

std::string Server::Receive(int connectFd, std::string &message) {

    const int BufferSize = DataBufferSize;
    char buffer[BufferSize];

    int nrBytes = recv(connectFd, buffer, BufferSize - 1, 0);

    if (nrBytes >= 0) {
        buffer[nrBytes] = '\0';

        message = buffer;
        return message;
    } else {
        message = "Failed reading, data is empty";
        return "";
    }
}

void Server::ReceiveFile(int connectFd, std::string data) {
    std::string content;
    content = Receive(connectFd, data);

    if (!content.empty()) {
        WriteFile(content);

        while (!content.empty()) {
            content = Receive(connectFd, data);
            WriteFile(content);

        }
    } else {
        std::cout << "The content is empty" << std::endl;
    }
}


void Server::WriteFile(std::string content) {
    // Write to file
    std::ofstream outfile(DefaultFileName, std::ios::app);

    outfile << content << std::endl;

    outfile.close();
}


bool Server::IsClientAuthenticated() const {
    return clientAuthenticated;
}

void Server::SetClientAuthenticated(bool clientAuthenticated) {
    Server::clientAuthenticated = clientAuthenticated;
}

bool Server::VerifyPassphrase(std::string pass) {
    return pass == PassPhrase;
}

bool Server::DeleteFile() {
    return remove(DefaultFileName) == 0;
}


