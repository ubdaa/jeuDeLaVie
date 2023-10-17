#include <SFML/Graphics.hpp>

#include <iostream>
#include <random>
#include <vector>

int main()
{
    int boxSize = 4;

    int colonnes = 150;
    int lignes = 150;

    int resize = 1;

    sf::RenderWindow window(sf::VideoMode(boxSize * colonnes * resize, boxSize * lignes * resize), "Jeu de la vie");
    window.setFramerateLimit(144);
    window.setView(sf::View(sf::FloatRect(boxSize, boxSize, boxSize * colonnes, boxSize * lignes)));

    std::vector<std::vector<int>> matrice(colonnes + 2, std::vector<int>(lignes + 2));
    std::vector<std::vector<int>> nMatrice(colonnes + 2, std::vector<int>(lignes + 2));

    sf::Clock clock;
    sf::Time elapsed;

    for (int x = 0; x < colonnes + 2; x++) {
        for (int y = 0; y < lignes + 2; y++) {
            matrice[x][y] = 0;
            nMatrice[x][y] = 0;
        }
    }

    std::random_device dev;
    std::mt19937 rng(dev());

    for (int x = 0; x <= colonnes + 1; x++) {
        for (int y = 0; y <= lignes + 1; y++) {

            std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 6);

            if (dist6(rng) == 1) {
                matrice[x][y] = 1;
            }
            else {
                matrice[x][y] = 0;
            }
        }
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //update
        elapsed = clock.getElapsedTime();

        if (elapsed.asSeconds() >= 0.08) {
            for (int x = 1; x < colonnes + 1; x++) {
                for (int y = 1; y < lignes + 1; y++) {

                    int count = 0;

                    for (int i = -1; i < 2; i++) {
                        for (int j = -1; j < 2; j++) {
                            if (!(i == 0 && j == 0)) {
                                if (matrice[x + i][y + j] == 1) {
                                    count++;
                                }
                            }
                        }
                    }

                    if (matrice[x][y] == 1) {
                        if (count == 2 || count == 3) {
                            nMatrice[x][y] = 1;
                        }
                        if (count < 2 || count > 3) {
                            nMatrice[x][y] = 0;
                        }
                    } else if (matrice[x][y] == 0) {
                        if (count == 3) {
                            nMatrice[x][y] = 1;
                        }
                        if (count <= 2 || count > 3) {
                            nMatrice[x][y] = 0;
                        }
                    }

                }
            }

            matrice = nMatrice;

            clock.restart();
        }
        
        //render
        sf::RectangleShape cell(sf::Vector2f(boxSize, boxSize));

        window.clear();

        for (int x = 1; x < colonnes + 1; x++) {
            for (int y = 1; y < lignes + 1; y++) {

                cell.setPosition(x * boxSize, y * boxSize);

                switch (matrice[x][y]) {
                    case 0 :
                        cell.setFillColor(sf::Color(154, 135, 157));
                        break;
                    case 1 :
                        cell.setFillColor(sf::Color(86, 52, 64));
                        break;
                }

                window.draw(cell);
            }
        }

        window.display();
    }

    return 0;
}