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

    std::string Receive(int connectFd, std::string &message);

    void ReceiveFile(int connectFd, std::string data);

    void WriteFile(std::string content);

    bool SendFile(int connectId, std::string fileName);

    bool IsClientAuthenticated() const;

    void SetClientAuthenticated(bool clientAuthenticated);

    bool VerifyPassphrase(std::string pass);

    bool DeleteFile();

    int GetMasterSocket();

private:
    // File descriptor of the server
    int socketFd;

    sockaddr_in sa{};
    // Data of the connected client
    sockaddr_storage their_addr;

    socklen_t addr_size;

    bool clientAuthenticated;

    const char *DefaultFileName = "file.txt";

    const std::string PassPhrase = "pass123";

    bool Socket();

    void SetupSocketData();

    bool Bind();

    bool Listen();

};


#endif
