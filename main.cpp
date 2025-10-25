#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>

#include "network/BanchoServer.h"

void startServer();
void handleClient(int client_sock_fd, int volunteer_id);

int main() {
    startServer();
}

void startServer() {
    BanchoServer bancho_server;

    if (bancho_server.trigger_bind() == -1) return;
    if (bancho_server.start_listening() == -1) return;

    int bancho_sock_fd = bancho_server.get_sock_fd();

    std::cout << "Bancho started running. Don't know why!\nAsk him?\n";
    //  << "PORT:  \t" << PORT << std::endl
    //  << "BINDED:\t" << bind_addr << std::endl
    //  << "LISTEN:\t" << val << std::endl;

    int volunteer_count = 1;

    while (true) {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_sock_fd = accept(bancho_sock_fd, (sockaddr*)&client_addr, &client_len);

        if (client_sock_fd < 0) {
            perror("accept failed");
            continue;
        }

        std::cout << "* Bancho found one volunteer. Should we give him drinks?\n";

        std::thread(handleClient, client_sock_fd, volunteer_count++).detach();
        // ++volunteer_count;
    }
    close(bancho_sock_fd);

    std::printf("Bancho stopped running. I think he must've been tired.\n");
}

void handleClient(int client_sock_fd, int volunteer_id) {
    std::string to_volunteer = 
        ">> Bancho: Hi volunteer " + std::to_string(volunteer_id) + 
        ". Are you gonna run with Bancho?\n>> You: ";
    
    send(client_sock_fd, to_volunteer.c_str(), to_volunteer.size(), 0);

    char buf[1024];
    ssize_t read_val = read(client_sock_fd, buf, sizeof(buf) - 1);

    if (read_val > 0) {
        buf[read_val] = '\0';
        std::cout << "Volunteer " << volunteer_id << " said: " << buf << std::endl;
    } else {
        std::cout << "Volunteer " << volunteer_id << " disconnected.\n";
    }

    close(client_sock_fd);
}