#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>
#include <string>


class Server {

public:
    Server();

    int Accept();

    void CloseServerSocket();

    bool CloseClientSocket(int connectFd);

    bool Send(int connectFd, std::string message);

    void Receive(int connectFd, std::string &message);

    std::string WriteFile(std::string content);

    bool SendFile(int connectId, std::string fileName);

    bool DeleteFile(std::string fileName);

    bool IsClientAllowedToSendFiles() const;

    void SetClientAllowedToSendFiles(bool clientAllowedToSendFiles);

private:
    // Data of the server
    int socketFd;

    sockaddr_in sa{};
    // Data of the connected client
    sockaddr_storage their_addr;

    socklen_t addr_size;

    bool clientAllowedToSendFiles;

    static int fileNameId;


private:

    void Socket();

    void SetupSocketData();

    bool Bind();

    bool Listen();

};


#endif
