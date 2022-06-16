#include <iostream>
#include "source/game.h"
#include "windows.h"
#include <SFML/Network.hpp>

sf::TcpListener listener;
sf::TcpSocket player1;
sf::TcpSocket player2;

using namespace std;

GameBoard game = GameBoard();

void SendBoard(char * board,int n){
    std::string message;
    for (int i = 0; i < n; i++){
        message += board[i];
    }
    game.move == white ? message += 'w' : message += 'b';
    player1.send(message.c_str(), message.size() + 1);
    player2.send(message.c_str(), message.size() + 1);
}

vector<string> Decode(char * message, int size){
    vector<string> res;
    string s;
    s+= message[0];
    s+= message[1];
    res.push_back(s);
    s.clear();
    s+= message[2];
    s+= message[3];
    res.push_back(s);
    return res;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    using namespace sf;


    sf::SocketSelector selector;

    if (listener.listen(53000) != sf::Socket::Done){
        std::cout << 1 << std::endl;
    }
    if (listener.accept(player1) != sf::Socket::Done){
        std::cout << 2 << std::endl;
        return 1;
    } else {
        std::cout << "player 1 was connected" << std::endl;
    }
    if (listener.accept(player2) != sf::Socket::Done){
        std::cout << 3 << std::endl;
        return 1;
    } else {
        std::cout << "player 2 was connected" << std::endl;
    }

    listener.close();
    selector.add(player1);
    selector.add(player2);
    std::vector<sf::TcpSocket*> sockets = {&player1, &player2};

    char buffer[1024]; std::size_t received = 0; bool status = false;

    SendBoard(game.packager(), 65);

    while (true) {
        if (game.EndGame()) break;
        if (selector.wait(sf::seconds(5))) {
            for (auto & socket : sockets) {
                if (selector.isReady(*socket)) {
                    socket->receive(buffer, sizeof(buffer), received);
                    std::vector<std::string> message = Decode(buffer, sizeof(buffer));
                    cout << message[0] << " " << message[1] << endl;
                    game.Move(message[0], message[1]);
                }
            }
            SendBoard(game.packager(), 65);
        }
    }
    cout << "Игра оконченна"  << endl;
    return 0;
}
