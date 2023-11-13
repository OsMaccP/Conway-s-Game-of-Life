#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <array>

int main()
{
    const int windowWidth = 1200;
    const int windowHeight = 800;
    sf::RenderWindow window1(sf::VideoMode(windowWidth, windowHeight), "Test window", sf::Style::Default);
    window1.setVerticalSyncEnabled(true);


    const int verticaCellNum = 32;
    const int horizontalCellNum = 48;

    std::array<std::array<sf::RectangleShape, verticaCellNum>, horizontalCellNum> square;
    std::array<std::array<sf::FloatRect, verticaCellNum>, horizontalCellNum> boundingBox;
    for(int j = 0; j < verticaCellNum; j++) {
        for(int i = 0; i < horizontalCellNum; i++) {
            square[i][j].setSize(sf::Vector2f(float(windowHeight) / float(verticaCellNum), float(windowWidth) / float(horizontalCellNum)));
            square[i][j].setFillColor(sf::Color::Black);
            square[i][j].setOutlineThickness(-1.f);
            square[i][j].setOutlineColor(sf::Color::Red);

            square[i][j].setPosition(i * (float(windowHeight) / float(verticaCellNum)), j * (float(windowWidth) / float(horizontalCellNum)));
            boundingBox[i][j] = square[i][j].getGlobalBounds();
        }
    }

    while(window1.isOpen()) {

        bool pausedGame = false;
        while(1) {

            // bool pausedGame = false;
            
            // clear window with black color
            window1.clear(sf::Color::Black);

            sf::Event event1;
            while(window1.pollEvent(event1)) {
                if(event1.type == sf::Event::Closed) {
                    window1.close();
                }
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    sf::Vector2f clickLeft = sf::Vector2f(sf::Mouse::getPosition(window1));
                    for(int j = 0; j < verticaCellNum; j++) {
                        for(int i = 0; i < horizontalCellNum; i++) {
                            if(boundingBox[i][j].contains(clickLeft)) {
                                square[i][j].setFillColor(sf::Color::White);
                            }
                        }
                    }
                }    
                else if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                    sf::Vector2f clickRight = sf::Vector2f(sf::Mouse::getPosition(window1));
                    for(int j = 0; j < verticaCellNum; j++) {
                        for(int i = 0; i < horizontalCellNum; i++) {
                            if (boundingBox[i][j].contains(clickRight)) {
                                square[i][j].setFillColor(sf::Color::Black);
                            }
                        }
                    }
                }
            }
         
            // draw entitys
            // like a coordinate system with +y pointing down
            // sf::Time t1 = sf::seconds(0.2);
            for(int j = 0; j < verticaCellNum; j++) {
                for(int i = 0; i < horizontalCellNum; i++) {
                    window1.draw(square[i][j]);
                    // sf::sleep(t1);
                }
            }
        
            // display draws
            window1.display();


            if(pausedGame) {
                break;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space)) {
                pausedGame = true;
            }
        }        
         
        
        sf::Time t1 = sf::seconds(0.25);
        sf::sleep(t1);
        
        int cellCount = 0;
        int count[horizontalCellNum][verticaCellNum];
        bool isChanged[horizontalCellNum][verticaCellNum];
        for(int j = 0; j < verticaCellNum; j++) {
            for(int i = 0; i < horizontalCellNum; i++) {
                isChanged[i][j] = false;
                count[i][j] = 0;
                // int cellCount = 0;
                //for death cells -> no cuenta caso limites
                if(square[i][j].getFillColor() == sf::Color::Black && (i != 0 && i != horizontalCellNum - 1) && (j != 0 && j != verticaCellNum - 1)) {
                    if(square[i-1][j-1].getFillColor() == sf::Color::White) {
                        cellCount++;
                    }
                    if(square[i-1][j].getFillColor() == sf::Color::White) {
                        cellCount++;
                    }
                    if(square[i-1][j+1].getFillColor() == sf::Color::White) {
                        cellCount++;
                    }
                    if(square[i][j-1].getFillColor() == sf::Color::White) {
                        cellCount++;
                    }
                    if(square[i][j+1].getFillColor() == sf::Color::White) {
                        cellCount++;
                    }
                    if(square[i+1][j-1].getFillColor() == sf::Color::White) {
                        cellCount++;
                    }
                    if(square[i+1][j].getFillColor() == sf::Color::White) {
                        cellCount++;
                    }
                    if(square[i+1][j+1].getFillColor() == sf::Color::White) {
                        cellCount++;
                    }

                    if(cellCount == 3) {
                        count[i][j] = cellCount;
                        isChanged[i][j] = true;
                        // square[i][j].setFillColor(sf::Color::White);
                    }
                }

                // for alive cells -> no considera limites
                else if(square[i][j].getFillColor() == sf::Color::White && (i != 0 && i != horizontalCellNum - 1) && (j != 0 && j != verticaCellNum - 1)) {
                    if(square[i-1][j-1].getFillColor() == sf::Color::White) {
                        cellCount++;
                    }
                    if(square[i-1][j].getFillColor() == sf::Color::White) {
                        cellCount++;
                    }
                    if(square[i-1][j+1].getFillColor() == sf::Color::White) {
                        cellCount++;
                    }
                    if(square[i][j-1].getFillColor() == sf::Color::White) {
                        cellCount++;
                    }
                    if(square[i][j+1].getFillColor() == sf::Color::White) {
                        cellCount++;
                    }
                    if(square[i+1][j-1].getFillColor() == sf::Color::White) {
                        cellCount++;
                    }
                    if(square[i+1][j].getFillColor() == sf::Color::White) {
                        cellCount++;
                    }
                    if(square[i+1][j+1].getFillColor() == sf::Color::White) {
                        cellCount++;
                    }

                    if(cellCount < 2 || cellCount > 3) {
                        count[i][j] = cellCount;
                        isChanged[i][j] = true;
                        // square[i][j].setFillColor(sf::Color::Black);
                    }
                }
            cellCount = 0;
            }
        }
        //el problema es que los colores de célular se van cambiando mientras el grid se actualiza y van cambiando de color aún sin mostrarse en pantalla, por lo tanto hay que cambiar los colores todod juntos al final de barrer la pantalla

        for(int j = 0; j < verticaCellNum; j++) {
            for(int i = 0; i < horizontalCellNum; i++) {
                if (count[i][j] == 3 && isChanged[i][j] == true) {
                        square[i][j].setFillColor(sf::Color::White);
                }
                if ((count[i][j] < 2 || count[i][j] > 3) && isChanged[i][j] == true) {
                        square[i][j].setFillColor(sf::Color::Black);
                }
            }
        }










/*
        // draw entitys
        // like a coordinate system with +y pointing down
        // sf::Time t1 = sf::seconds(0.2);
        for(int j = 0; j < verticaCellNum; j++) {
            for(int i = 0; i < horizontalCellNum; i++) {
                window1.draw(square[i][j]);
                // sf::sleep(t1);
            }
        }
        
        // display draws
        window1.display();
*/
    }

    return 0;
}