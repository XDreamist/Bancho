#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <thread>

#include "network/BanchoServer.h"

#include <SFML/Graphics.hpp>

void startServer();
void handleClient(int client_sock_fd, int volunteer_id);

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Bancho");

    // Load sprite texture
    sf::Texture texture;
    if (!texture.loadFromFile("Ludo-Idle.png")) {
        return -1;
    }

    sf::Sprite sprite(texture);
    sprite.setPosition(208, 108);

    sf::Vector2f charSize;
    charSize.x = 0.2;
    charSize.y = 0.2;
    sprite.setScale(charSize);

    // Frame size (calculated from sheet)
    const int frameWidth = 256;
    const int frameHeight = 320;
    const int columns = 4;
    const int rows = 2;

    // Start at first frame
    int frameX = 0;
    int frameY = 0;
    sprite.setTextureRect(sf::IntRect(frameX * frameWidth, frameY * frameHeight, frameWidth, frameHeight));

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float time = clock.getElapsedTime().asSeconds();

        // Animate every 0.15s
        if (time > 0.15f) {
            frameX++;
            if (frameX >= columns) {
                frameX = 0;
                frameY = (frameY + 1) % rows; // switch row
            }
            sprite.setTextureRect(sf::IntRect(frameX * frameWidth, frameY * frameHeight, frameWidth, frameHeight));
            clock.restart();
        }

        window.clear(sf::Color::Black);
        window.draw(sprite);
        window.display();
    }

    return 0;
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