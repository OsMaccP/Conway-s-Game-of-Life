#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <array>

int main()
{
    const int windowWidth = 1200;
    const int windowHeight = 800;
    sf::RenderWindow window1(sf::VideoMode(windowWidth, windowHeight), "Test window", sf::Style::Default);
    window1.setVerticalSyncEnabled(true);


    const int horizontalCellNum = 90; //if I set the number of cells to 96 or any other number that makes CellDix_X not an integer, then the exe won't run
    const int verticaCellNum = 60;
    // windowWidth / horizontalCellNum = windowHeight / verticalCellNum = constant --> 1200 / horizontalCellNum = 800 / verticalCellNum --> if one of these two numbers is given, the other is automatically set.
    // --> 1200/800 * given verticalCellNum = horizontalCellNum, try whit integer until result is integer. Only works if the given value meets 800 % vCN --> 1200 % hCN = 0.
    // Dividers of 800 --> 1, 2, 4, 5, 8, 10, 16, 20, 25, 32, 40, 50, 80, 100, 160, 200, 400 y 800. Why does not work using this hypotesis?
    const float CellDim_X = static_cast<float>(windowWidth) / static_cast<float>(horizontalCellNum);
    const float CellDim_Y = static_cast<float>(windowHeight) / static_cast<float>(verticaCellNum);

    std::array<std::array<sf::RectangleShape, verticaCellNum>, horizontalCellNum> square;
    std::array<std::array<sf::FloatRect, verticaCellNum>, horizontalCellNum> boundingBox;
    for(int j = 0; j < verticaCellNum; j++) {
        for(int i = 0; i < horizontalCellNum; i++) {
            square[i][j].setSize(sf::Vector2f(CellDim_X, CellDim_Y));
            square[i][j].setFillColor(sf::Color::Black);
            square[i][j].setOutlineThickness(-1.f);
            square[i][j].setOutlineColor(sf::Color(37, 80, 40));

            square[i][j].setPosition(i * (CellDim_X), j * (CellDim_Y));
            boundingBox[i][j] = square[i][j].getGlobalBounds();
        }
    }

    while(window1.isOpen()) {

        // evolution delay
        sf::Time t1 = sf::seconds(0.10);
        sf::sleep(t1);

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
         
        
        // sf::Time t1 = sf::seconds(0.15);
        // sf::sleep(t1);
        
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