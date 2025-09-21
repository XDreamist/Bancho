#include "BanchoServer.h"
#include <iostream>
#include <unistd.h>

BanchoServer::BanchoServer()
{
    server_addr.sin_family      = server_fam;
    server_addr.sin_addr.s_addr = server_orgins;
    server_addr.sin_port        = htons(server_port);

    sock_addr = (sockaddr*)&server_addr;
    sock_len = sizeof(server_addr);
    sock_fd = socket(sock_domain, sock_type, sock_protocol);
}

int BanchoServer::trigger_bind()
{
    int bind_result = bind(sock_fd, sock_addr, sock_len);
    if (bind_result == -1) perror("Bancho failed to bind");
    return bind_result;
}

int BanchoServer::start_listening()
{
    int listen_result = listen(sock_fd, server_q_limit);
    if (listen_result == -1) perror("Bancho failed to listen");
    return listen_result;
}

int BanchoServer::get_sock_fd()
{
    return sock_fd;
}

int BanchoServer::stop_server()
{
    return close(sock_fd);
}
