#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

struct Player {
    int index;
    char symbol;
    int wins;
    int draws;
    int loses;
    int points;
};

char checkWinner(const std::string& moves) {

    if (moves.size() != 9) {
        return '.';
    }

    char board[3][3] = { {moves[0], moves[1], moves[2]},
                        {moves[3], moves[4], moves[5]},
                        {moves[6], moves[7], moves[8]} };

    for (int i = 0; i < 3; ++i) {
        if (board[i][0] != '.' && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            return board[i][0];
        }
    }

    for (int i = 0; i < 3; ++i) {
        if (board[0][i] != '.' && board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            return board[0][i];
        }
    }

    if (board[0][0] != '.' && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        return board[0][0];
    }
    if (board[0][2] != '.' && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        return board[0][2];
    }

    return '.';
}

std::vector<Player> loadPlayers(const std::string& filename) {
    std::vector<Player> players;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int index;
        char symbol;
        iss >> index >> symbol;
        players.push_back({ index, symbol, 0, 0, 0, 0 });
    }
    return players;
}

std::vector<std::string> loadMoves(const std::string& filename) {
    std::vector<std::string> moves;
    std::ifstream file(filename);
    std::string line;
    std::string currentMove = "";
    while (std::getline(file, line)) {
        if (!line.empty()) {
            currentMove += line;
        }
        else {
            moves.push_back(currentMove);
            currentMove = "";
        }
    }
    if (!currentMove.empty()) {
        moves.push_back(currentMove);
    }
    return moves;
}

void calculateResults(std::vector<Player>& players, const std::vector<std::string>& moves) {
    for (const auto& move : moves) {
        char winner = checkWinner(move);
        if (winner != '.') {
            for (auto& player : players) {
                if (player.symbol == winner) {
                    player.wins += 1;
                    player.points += 3;
                }
                else {
                    if (move.find(player.symbol) != std::string::npos) {
                        player.loses += 1;
                        player.points -= 3;
                    }
                }
            }
        }
        else {
            std::vector<char> participatingSymbols;
            for (char c : move) {
                if (c != '.' && std::find(participatingSymbols.begin(), participatingSymbols.end(), c) == participatingSymbols.end()) {
                    participatingSymbols.push_back(c);
                }
            }
            for (auto& player : players) {
                if (std::find(participatingSymbols.begin(), participatingSymbols.end(), player.symbol) != participatingSymbols.end()) {
                    player.draws += 1;
                    player.points += 1;
                }
            }
        }
    }
}

void sortPlayers(std::vector<Player>& players) {
    std::sort(players.begin(), players.end(), [](const Player& a, const Player& b) {
        return a.points > b.points;
        });
}

void showTable(const std::vector<Player>& players) {
    std::cout << "ID         Sign    Wins      Draws       Loses     Points\n";
    for (const auto& player : players) {
        std::cout << player.index << "     " << player.symbol << "       "
            << player.wins << "       " << player.draws << "        "
            << player.loses << "        " << player.points << "\n";
    }
}

void showResults(const std::vector<std::string>& moves) {
    std::cout << "Results:\n";
    for (const auto& move : moves) {
        for (size_t i = 0; i < move.size(); ++i) {
            std::cout << move[i];
            if ((i + 1) % 3 == 0) {
                std::cout << std::endl;
            }
        }
        std::cout << std::endl;
    }
}

void showPlayerStats(const std::vector<Player>& players) {
    int index;
    std::cout << "Enter player index: ";
    std::cin >> index;
    for (const auto& player : players) {
        if (player.index == index) {
            std::cout << "Player stats:\n";
            std::cout << player.index << " " << player.symbol << " "
                << player.wins << " " << player.draws << " "
                << player.loses << " " << player.points << "\n";
            return;
        }
    }
    std::cout << "Player not found!\n";
}

int main() {
    std::vector<Player> players = loadPlayers("players.txt");
    std::vector<std::string> moves = loadMoves("tictactoe.txt");

    calculateResults(players, moves);
    sortPlayers(players);

    int choice;
    do {
        std::cout << "\nMenu:\n";
        std::cout << "1. Show table\n";
        std::cout << "2. Show results\n";
        std::cout << "3. Show player stats\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        switch (choice) {
        case 1:
            showTable(players);
            break;
        case 2:
            showResults(moves);
            break;
        case 3:
            showPlayerStats(players);
            break;
        case 4:
            std::cout << "Exiting...\n";
            break;
        default:
            std::cout << "Invalid choice!\n";
            break;
        }
    } while (choice != 4);

    return 0;
}