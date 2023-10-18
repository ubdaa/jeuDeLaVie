#include <SFML/Graphics.hpp>

#include <iostream>
#include <random>
#include <vector>
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    //initialisation des variables par défaut qui vont servir au code du jeu
    int boxSize = 4;

    int colonnes = 150;
    int lignes = 150;

    int resize = 1;

    //initialisation de la fenêtre principale
    sf::RenderWindow window(sf::VideoMode(boxSize * colonnes * resize, boxSize * lignes * resize), "Jeu de la vie");
    window.setFramerateLimit(144);
    window.setView(sf::View(sf::FloatRect(boxSize, boxSize, boxSize * colonnes, boxSize * lignes)));

    //nos matrices (l'ancienne puis la nouvelle)
    std::vector<std::vector<int>> matrice(colonnes + 2, std::vector<int>(lignes + 2));
    std::vector<std::vector<int>> nMatrice(colonnes + 2, std::vector<int>(lignes + 2));

    //initialisation des variables liées au temps -> pour les ticks
    sf::Clock clock;
    sf::Time elapsed;

    //initialisation de l'état par défaut des matrices
    for (int x = 0; x < colonnes + 2; x++) {
        for (int y = 0; y < lignes + 2; y++) {
            matrice[x][y] = 0;
            nMatrice[x][y] = 0;
        }
    }

    //variables liées à la génération
    std::random_device dev;
    std::mt19937 rng(dev());

    //génère ici les points vivants/morts dans la situation initiale
    for (int x = 0; x <= colonnes + 1; x++) {
        for (int y = 0; y <= lignes + 1; y++) {

            //1 chance sur 6
            std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 6);

            if (dist6(rng) == 1) {
                matrice[x][y] = 1;
            }
            else {
                matrice[x][y] = 0;
            }
        }
    }

    //boucle principale
    while (window.isOpen())
    {
        //chaque événement de la fenêtre sfml
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //update
        elapsed = clock.getElapsedTime();

        //chaque tick (pour éviter que le programme s'éxécute trop vite -> le cpp va très vite)
        if (elapsed.asSeconds() >= 0.08) {
            for (int x = 1; x < colonnes + 1; x++) {
                for (int y = 1; y < lignes + 1; y++) {

                    int count = 0;

                    //cherche pour chacun des voisins de chaque cellule dans notre tableau
                    for (int i = -1; i < 2; i++) {
                        for (int j = -1; j < 2; j++) {
                            if (!(i == 0 && j == 0)) {
                                if (matrice[x + i][y + j] == 1) {
                                    count++;
                                }
                            }
                        }
                    }

                    //règles

                    //si vivant
                    if (matrice[x][y] == 1) {
                        if (count == 2 || count == 3) {
                            nMatrice[x][y] = 1;
                        }
                        if (count < 2 || count > 3) {
                            nMatrice[x][y] = 0;
                        }
                    } 
                    //si mort
                    else if (matrice[x][y] == 0) {
                        if (count == 3) {
                            nMatrice[x][y] = 1;
                        }
                        if (count <= 2 || count > 3) {
                            nMatrice[x][y] = 0;
                        }
                    }

                }
            }

            //on remet à jour la matrice
            matrice = nMatrice;

            //restart le tick
            clock.restart();
        }
        
        //render

        //une cellule
        sf::RectangleShape cell(sf::Vector2f(boxSize, boxSize));

        //netotie la frame
        window.clear();

        //pour chaque case de notre matrice
        for (int x = 1; x < colonnes + 1; x++) {
            for (int y = 1; y < lignes + 1; y++) {

                //on met la position de la case selon sa cellule
                cell.setPosition(x * boxSize, y * boxSize);

                //on choisit selon l'état vivant ou mort puis on va lui assigner une couleur
                switch (matrice[x][y]) {

                    //dans le cas où la cellule est morte
                    case 0 :
                        cell.setFillColor(sf::Color(154, 135, 157));
                        break;
                    //dans le cas où la cellule est vivante
                    case 1 :
                        cell.setFillColor(sf::Color(86, 52, 64));
                        break;
                }

                //enfin on dessine notre cellule
                window.draw(cell);
            }
        }

        //affiche la nouvelle frame
        window.display();
    }

    return 0;
}