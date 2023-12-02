#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <array>

int main()
{
    const int windowWidth = 1200;
    const int windowHeight = 800;
    sf::RenderWindow window1(sf::VideoMode(windowWidth, windowHeight), "Test window", sf::Style::Default);
    window1.setVerticalSyncEnabled(true);

    // It works for all combinations of numbers that satisfy -> verticalCellNum == f(horizontalCellNum) = 800/1200 * horizontalCellNum. verticalCellNum <= 60.
    // I think it has something to do with memory overload and my not very optimal implementation.
    const int horizontalCellNum = 90;
    const int verticaCellNum = 60;
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

    std::size_t generation = 0;

    bool pausedGame = false;

    while(window1.isOpen()) {

        // state evolution timer
        sf::Time t1 = sf::seconds(0.025); // 0.10 great value
        sf::sleep(t1);

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
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
                generation = 0;
                
                for(int j = 0; j < verticaCellNum; j++) {
                    for(int i = 0; i < horizontalCellNum; i++) {
                        square[i][j].setFillColor(sf::Color::Black);
                    }
                }
            }
            if(event1.type == sf::Event::KeyPressed) {
                if (event1.key.scancode == sf::Keyboard::Scan::Space) {
                    pausedGame = !pausedGame;
                }
            }
        }
         
        // draw entitys
        // like a coordinate system with +y pointing down
        for(int j = 0; j < verticaCellNum; j++) {
            for(int i = 0; i < horizontalCellNum; i++) {
                window1.draw(square[i][j]);
            }
        }

        // display draws
        window1.display();

        if(pausedGame) {
            int cellCount = 0;
            int count[horizontalCellNum][verticaCellNum];
            bool isChanged[horizontalCellNum][verticaCellNum];
            for(int j = 0; j < verticaCellNum; j++) {
                for(int i = 0; i < horizontalCellNum; i++) {
                    isChanged[i][j] = false;
                    count[i][j] = 0;
    
                    //for death cells
                    if(square[i][j].getFillColor() == sf::Color::Black) {
    
                        // for general case, no limits
                        if((i != 0 && i != horizontalCellNum - 1) && (j != 0 && j != verticaCellNum - 1)) {
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
                            }
    
                        }
                        // left wall
                        else if(i == 0 && j != 0 && j != verticaCellNum - 1) {
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
                        }
                        // right wall
                        else if(i == horizontalCellNum - 1 && j != 0 && j != verticaCellNum -1) {
                            if(square[i][j-1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i][j+1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i-1][j-1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i-1][j].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i-1][j+1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                        }
                        // ceiling
                        else if(j == 0 && i != 0 && i != horizontalCellNum -1) {
                            if(square[i-1][j].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i+1][j].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i-1][j+1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i][j+1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i+1][j+1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                        }
                        // floor 
                        else if(j == verticaCellNum - 1 && i != 0 && i != horizontalCellNum -1) {
                            if(square[i-1][j].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i+1][j].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i-1][j-1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i][j-1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i+1][j-1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                        }
                        // up-left corner
                        else if(i == 0 && j == 0) {
                            if(square[i][j+1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i+1][j].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i+1][j+1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                        }
                        // up-right corner
                        else if(i == horizontalCellNum - 1 && j == 0) {
                            if(square[i][j+1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i-1][j].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i-1][j+1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                        }
                        // down-left corner
                        else if(i == 0 && j == verticaCellNum - 1) {
                            if(square[i][j-1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i+1][j-1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i+1][j].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                        }
                        // down-right corner
                        else if(i == horizontalCellNum - 1 && j == verticaCellNum - 1) {
                            if(square[i][j-1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i-1][j-1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i-1][j].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                        }

                        if(cellCount == 3) {
                            count[i][j] = cellCount;
                            isChanged[i][j] = true;
                        }
                    }

                    // for alive cells
                    else if(square[i][j].getFillColor() == sf::Color::White) {

                        // for general case, no limits
                        if((i != 0 && i != horizontalCellNum - 1) && (j != 0 && j != verticaCellNum - 1)) {
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
                        }
                        // left wall
                        else if(i == 0 && j != 0 && j != verticaCellNum - 1) {
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
                        }
                        // right wall
                        else if(i == horizontalCellNum - 1 && j != 0 && j != verticaCellNum -1) {
                            if(square[i][j-1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i][j+1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i-1][j-1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i-1][j].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i-1][j+1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                        }
                        // ceiling
                        else if(j == 0 && i != 0 && i != horizontalCellNum -1) {
                            if(square[i-1][j].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i+1][j].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i-1][j+1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i][j+1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i+1][j+1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                        }
                        // floor 
                        else if(j == verticaCellNum - 1 && i != 0 && i != horizontalCellNum -1) {
                            if(square[i-1][j].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i+1][j].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i-1][j-1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i][j-1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i+1][j-1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                        }
                        // up-left corner
                        else if(i == 0 && j == 0) {
                            if(square[i][j+1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i+1][j].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i+1][j+1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                        }
                        // up-right corner
                        else if(i == horizontalCellNum - 1 && j == 0) {
                            if(square[i][j+1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i-1][j].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i-1][j+1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                        }
                        // down-left corner
                        else if(i == 0 && j == verticaCellNum - 1) {
                            if(square[i][j-1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i+1][j-1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i+1][j].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                        }
                        // down-right corner
                        else if(i == horizontalCellNum - 1 && j == verticaCellNum - 1) {
                            if(square[i][j-1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i-1][j-1].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                            if(square[i-1][j].getFillColor() == sf::Color::White) {
                                cellCount++;
                            }
                        }

                        if(cellCount < 2 || cellCount > 3) {
                            count[i][j] = cellCount;
                            isChanged[i][j] = true;
                        }
                    }

                    cellCount = 0;
                }
            }

            // It is necessary to change all colors only after calculating the state for each cell.
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

            generation++;
        }
    }

    return 0;
}