#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include "windows.h"

char board[1024];

std::string VectorToBoardIdx(int pos){
    int x; int y; std::string res;
    x = pos / 8;
    y = pos % 8;
    res += 'a' + y;
    res += (8 - x) + '0';
    return res;
}

int main() {
    sf::TcpSocket socket;
    sf::Socket::Status status = socket.connect("localhost", 53000);

    if (status != sf::Socket::Done){
        std::cout << "Connecting error..." << std::endl;
        return 1;
    }
    std::string from, to;


    using namespace sf;
    sf::RenderWindow window(sf::VideoMode(448, 448), "Player 2");
    window.setFramerateLimit(15);
    window.clear(sf::Color(90, 90, 90));

    struct tail{
        RectangleShape shape;
        std::string pos;
    };

    std::vector<tail> fill;

    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            std::string pos;
            pos += 'a' + 7 - j;
            pos += i + 1 + '0';
            RectangleShape xx;
            xx.setPosition(j * 56, i * 56);
            xx.setSize({56, 56});
            xx.setFillColor(((i + j) % 2 == 0) ? sf::Color(90, 90, 90) : sf::Color::White);
            fill.push_back({xx, pos});
        }
    }

    struct collection{
        Sprite fig;
        std::string pos;
        std::string color;
    };

    Texture black_texture, white_texture, white_texture_plus, black_texture_plus;
    white_texture.loadFromFile(R"(C:\Users\shiln\CLionProjects\gui-net-game\clients\source\white.png)");
    black_texture.loadFromFile(R"(C:\Users\shiln\CLionProjects\gui-net-game\clients\source\black.png)");
    black_texture_plus.loadFromFile(R"(C:\Users\shiln\CLionProjects\gui-net-game\clients\source\black+.png)");
    white_texture_plus.loadFromFile(R"(C:\Users\shiln\CLionProjects\gui-net-game\clients\source\white+.png)");

    std::size_t received = 0;
    socket.receive(board, sizeof(board), received);

    std::vector<collection> spriteCollector;

    sf::Event event{};

    std::vector<int> queue;
    bool flag = false;
    sf::SocketSelector selector;
    selector.add(socket);

    while (window.isOpen()) {
        if (from.empty()) {
            spriteCollector.clear();
            for (int i = 0; i < 64; i++) {
                Sprite Figure;
                std::string pos = VectorToBoardIdx(i), color;
                char symbol = board[i];
                if (symbol == 'o') {
                    color = "black";
                    Figure.setTexture(black_texture);
                } else if (symbol == 'O') {
                    color = "white";
                    Figure.setTexture(white_texture);
                } else if (symbol == 'x') {
                    color = "black";
                    Figure.setTexture(black_texture_plus);
                } else if (symbol == 'X') {
                    color = "white";
                    Figure.setTexture(white_texture_plus);
                } else continue;
                Figure.setPosition(((63 - i) % 8) * 56, ((63 - i) / 8) * 56);
                spriteCollector.push_back({Figure, pos, color});
            }
        }
        window.clear(sf::Color(90, 90, 90));
        for (const auto& i : fill){
            window.draw(i.shape);
        }
        for (const auto & i : spriteCollector){
            window.draw(i.fig);
        }
        window.display();

        while (window.pollEvent(event)) {
            if (board[65] == 'w') continue;
            if (event.type == sf::Event::Closed) {
                socket.disconnect();
                window.close();
                return 0;
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    socket.disconnect();
                    window.close();
                    return 0;
                }
            }
            if (event.type == Event::MouseButtonPressed){
                auto mouse_pos = sf::Mouse::getPosition(window);
                auto translated_pos = window.mapPixelToCoords(mouse_pos);
                if (!flag) {
                    for (int i = 0; i < spriteCollector.size(); i++) {
                        if (spriteCollector[i].fig.getGlobalBounds().contains(translated_pos)) {
                            from = spriteCollector[i].pos;
                            flag = true;
                            queue.push_back(i);
                            break;
                        }
                    }
                } else {
                    for (const auto & i : fill){
                        if (i.shape.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))){from = spriteCollector[queue.back()].pos, to = i.pos;
                            spriteCollector[queue.back()].fig.setPosition(i.shape.getPosition().x,
                                                                          i.shape.getPosition().y);
                            to = i.pos;
                            spriteCollector[queue.back()].pos = i.pos;
                            break;
                        }
                    }
                    flag = false;
                    queue.clear();
                }
            }
        }
        from += to;
        if (!from.empty() && !to.empty()) {
            if (socket.send(from.c_str(), from.size() + 1) != sf::Socket::Done) {
                std::cout << "exception while sending message";
                break;
            }
            from.clear();
            to.clear();
        }
        if (selector.wait(sf::seconds(0.2))) socket.receive(board, sizeof(board), received);
    }
    return 0;
}

