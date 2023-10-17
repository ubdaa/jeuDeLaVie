#include <SFML/Graphics.hpp>

#include <vector>

int main()
{
    int boxSize = 10;

    int colonnes = 60;
    int lignes = 60;

    int resize = 1;

    sf::RenderWindow window(sf::VideoMode(boxSize * colonnes * resize, boxSize * lignes * resize), "Jeu de la vie");
    window.setFramerateLimit(144);
    window.setView(sf::View(sf::FloatRect(0, 0, boxSize * colonnes, boxSize * lignes)));

    std::vector<std::vector<int>> matrice(colonnes, std::vector<int>(lignes));

    sf::Clock clock;
    sf::Time elapsed;
    
    for (int x = 0; x < colonnes; x++) {
        for (int y = 0; y < lignes; y++) {
            if ((rand() % 3 + 1) == 1) {
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
        for (int x = 0; x < colonnes; x++) {
            for (int y = 0; y < lignes; y++) {
                
                int count = 0;

                for (int a = -1; a < 2; a++) {
                    if (matrice[x + 1][y + a] == 1) {
                        count = count + 1;
                    }
                }

                for (int a = -1; a < 2; a++) {
                    if (matrice[x][y + a] == 1 && a !=0) {
                        count = count + 1;
                    }
                }

                for (int a = -1; a < 2; a++) {
                    if (matrice[x - 1][y + a] == 1) {
                        count = count + 1;
                    }
                }

                if (count == 3) {
                    matrice[x][y] = 1;
                }
                else if (count < 2 || count > 3) {
                    matrice[x][y] = 0;
                }
            }
        }
        
        //render
        sf::RectangleShape cell(sf::Vector2f(boxSize, boxSize));
        cell.setFillColor(sf::Color(255, 255, 255));

        window.clear();

        for (int x = 0; x < colonnes; x++) {
            for (int y = 0; y < lignes; y++) {
                cell.setPosition(x * boxSize, y * boxSize);

                switch (matrice[x][y]) {
                case 0 :
                    cell.setFillColor(sf::Color(255, 255, 255));
                    break;
                case 1 :
                    cell.setFillColor(sf::Color(0, 0, 0));
                    break;
                }

                window.draw(cell);
            }
        }

        window.display();
    }

    return 0;
}