#pragma once

#include <netinet/in.h>

class BanchoServer
{
public:
    BanchoServer();

    int trigger_bind();
    int start_listening();
    int get_sock_fd();
    int stop_server();

private:
    sockaddr_in server_addr;

    // The values are not supposed to be like that.
    // We later need to remove hardcoding.
    sa_family_t server_fam      = AF_INET;    // The IPv4 config
    in_addr_t   server_orgins   = INADDR_ANY; // Listen from every address
    int         server_port     = 666;        // The port which it will run

    sockaddr*   sock_addr;
    socklen_t   sock_len;
    int         sock_fd;

    sa_family_t sock_domain     = AF_INET;
    int         sock_type       = SOCK_STREAM;
    int         sock_protocol   = 0;

    int         server_q_limit  = 60;         // The limit before before further requests are refused
};