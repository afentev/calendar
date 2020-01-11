#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>
#include <vector>
#include <unistd.h>

using iterator = std::vector<sf::Color>::iterator;
using uniform_int_distribution = std::uniform_int_distribution<std::mt19937::result_type>;

const int FontSize = 40;
const int WindowSize = 800 + FontSize;
const int TileSize = 9;

const int NColours = 6;
const int FirstDrop = 3;
const int MinimumDestroyRequired = 3;
const int DropNew = 5;

int score = 0;
bool is_game = true;

sf::Color colours[TileSize][TileSize];

const double Gap = (WindowSize / TileSize) * 0.1;
const int SquareSize = (WindowSize / TileSize) * 0.9;
sf::Vector2f CPoints[TileSize][TileSize];

sf::Color RGBColours[NColours];

const sf::Color White = sf::Color(255, 255, 255, 255);

std::vector<std::vector<int>> already;
std::vector<std::vector<int>> roads;

sf::Font font;
sf::Text text("", font, FontSize - 5);


void lee(std::vector<int> destination) {
    bool checked = false;
    bool changed = true;
    int d = 0;
    while (!checked && changed) {
        changed = false;
        for (int i = 0; i < TileSize; ++i) {
            for (int j = 0; j < TileSize; ++j) {
                if (roads[i][j] == d) {
                    if (i - 1 >= 0 && roads[i - 1][j] > d + 1 && colours[i - 1][j] == White) {
                        roads[i - 1][j] = d + 1;
                        changed = true;
                    }
                    if (i + 1 < TileSize && roads[i + 1][j] > d + 1 && colours[i + 1][j] == White) {
                        roads[i + 1][j] = d + 1;
                        changed = true;
                    }
                    if (j - 1 >= 0 && roads[i][j - 1] > d + 1 && colours[i][j - 1] == White) {
                        roads[i][j - 1] = d + 1;
                        changed = true;
                    }
                    if (j + 1 < TileSize && roads[i][j + 1] > d + 1 && colours[i][j + 1] == White) {
                        roads[i][j + 1] = d + 1;
                        changed = true;
                    }
                }
            }
        }
        checked = roads[destination[0]][destination[1]] != 1e8;
        ++d;
    }
}


sf::Color HSVtoRGB(float H, float S, float V)
{
    float C = S * V; // Chroma
    float HPrime = std::fmod(H / 60, 6.f); // H'
    float X = C * (1 - std::fabs(std::fmod(HPrime, 2.f) - 1));
    float M = V - C;

    float R = 0.f;
    float G = 0.f;
    float B = 0.f;

    switch (static_cast<int>(HPrime))
    {
        case 0: R = C; G = X;        break; // [0, 1)
        case 1: R = X; G = C;        break; // [1, 2)
        case 2:        G = C; B = X; break; // [2, 3)
        case 3:        G = X; B = C; break; // [3, 4)
        case 4: R = X;        B = C; break; // [4, 5)
        case 5: R = C;        B = X; break; // [5, 6)
    }

    R += M;
    G += M;
    B += M;

    sf::Color color;
    color.r = static_cast<sf::Uint8>(std::round(R * 255));
    color.g = static_cast<sf::Uint8>(std::round(G * 255));
    color.b = static_cast<sf::Uint8>(std::round(B * 255));

    return color;
}


void draw(sf::RenderWindow* window, int* selected, int scope) {
    for (int i = 0; i < TileSize; ++i) {
        sf::Vertex horizontal_line[] = {sf::Vertex(sf::Vector2f(0, i * (Gap + SquareSize) + FontSize)),
                                        sf::Vertex(sf::Vector2f(WindowSize, i * (Gap + SquareSize) + FontSize))};
        horizontal_line[0].color = sf::Color(0, 0, 0, 32);
        horizontal_line[1].color = sf::Color(0, 0, 0, 32);

        sf::Vertex vertical_line[] = {sf::Vertex(sf::Vector2f(i * (Gap + SquareSize), FontSize)),
                                      sf::Vertex(sf::Vector2f(i * (Gap + SquareSize), WindowSize + FontSize))};
        vertical_line[0].color = sf::Color(0, 0, 0, 32);
        vertical_line[1].color = sf::Color(0, 0, 0, 32);

        window->draw(horizontal_line, 2, sf::Lines);
        window->draw(vertical_line, 2, sf::Lines);

        for (int j = 0; j < TileSize; ++j) {
            if (colours[i][j] != White) {
                int add = (std::find(already.rbegin(), already.rend(), std::vector<int>{i, j}) != already.rend()) * scope;
                sf::CircleShape circle(SquareSize / 2 + add);
                circle.setPosition(CPoints[i][j].x - add, CPoints[i][j].y - add);
                circle.setFillColor(colours[i][j]);

                if (i == *selected && j == *(selected + 1)) {
                    circle.setOutlineThickness(Gap / 3);
                    circle.setOutlineColor(sf::Color(0, 0, 0, 255));
                }
                window->draw(circle);
            }
        }
    }
    if (is_game)
        text.setString("Your score is " + std::to_string(score));
    window->draw(text);
}


void neighbors(int x, int y) {
    sf::Color base_color = colours[x][y];

    std::vector<std::vector<int>> tmp;
    int counter = 1;

    int delta = 1;
    while (x + delta < TileSize) {
        if (colours[x + delta][y] == base_color) {
            tmp.push_back(std::vector<int> {x + delta, y});
        } else {
            break;
        }
        ++delta;
        ++counter;
    }

    delta = 1;
    while (x - delta >= 0) {
        if (colours[x - delta][y] == base_color) {
            tmp.push_back(std::vector<int> {x - delta, y});
        } else {
            break;
        }
        ++delta;
        ++counter;
    }

    if (counter >= MinimumDestroyRequired) {
        for (int i = 0; i < tmp.size(); ++i) {
            already.push_back(tmp[i]);
        }
    }

    std::vector<std::vector<int>>().swap(tmp);
    counter = 1;

    delta = 1;
    while (y + delta < TileSize) {
        if (colours[x][y + delta] == base_color) {
            tmp.push_back(std::vector<int> {x, y + delta});
        } else {
            break;
        }
        ++delta;
        ++counter;
    }

    delta = 1;
    while (y - delta >= 0) {
        if (colours[x][y - delta] == base_color) {
            tmp.push_back(std::vector<int> {x, y - delta});
        } else {
            break;
        }
        ++delta;
        ++counter;
    }

    if (counter >= MinimumDestroyRequired) {
        for (int i = 0; i < tmp.size(); ++i) {
            already.push_back(tmp[i]);
        }
    }

    std::vector<std::vector<int>>().swap(tmp);
    counter = 1;

    delta = 1;
    while (y + delta < TileSize && x + delta < TileSize) {
        if (colours[x + delta][y + delta] == base_color) {
            tmp.push_back(std::vector<int> {x + delta, y + delta});
        } else {
            break;
        }
        ++delta;
        ++counter;
    }

    delta = 1;
    while (y - delta >= 0 && x - delta >= 0) {
        if (colours[x - delta][y - delta] == base_color) {
            tmp.push_back(std::vector<int> {x - delta, y - delta});
        } else {
            break;
        }
        ++delta;
        ++counter;
    }

    if (counter >= MinimumDestroyRequired) {
        for (int i = 0; i < tmp.size(); ++i) {
            already.push_back(tmp[i]);
        }
    }

    std::vector<std::vector<int>>().swap(tmp);
    counter = 1;

    delta = 1;
    while (y - delta >= 0 && x + delta < TileSize) {
        if (colours[x + delta][y - delta] == base_color) {
            tmp.push_back(std::vector<int> {x + delta, y - delta});
        } else {
            break;
        }
        ++delta;
        ++counter;
    }

    delta = 1;
    while (y + delta >= 0 && x - delta >= 0) {
        if (colours[x - delta][y + delta] == base_color) {
            tmp.push_back(std::vector<int> {x - delta, y + delta});
        } else {
            break;
        }
        ++delta;
        ++counter;
    }

    if (counter >= MinimumDestroyRequired) {
        for (int i = 0; i < tmp.size(); ++i) {
            already.push_back(tmp[i]);
        }
    }

    already.push_back(std::vector<int> {x, y});
    std::vector<std::vector<int>>().swap(tmp);
}


void destroy(sf::RenderWindow* window, int* first_click, int x, int y, bool user_action) {
    std::random_device dev;
    std::mt19937 rng(dev());
    uniform_int_distribution dist(0, TileSize - 1);
    uniform_int_distribution rgb_dist(0, NColours - 1);

    if (colours[x][y] != White) {
        neighbors(x, y);


        if (already.size() >= MinimumDestroyRequired) {
            score += already.size() - 1;
            for (int scope = 0; scope < (WindowSize - SquareSize) / 10; ++scope) {
                window->clear(White);
                draw(window, first_click, scope);
                window->display();
            }
            for (std::vector<int> i: already)
                colours[i[0]][i[1]] = White;
            std::vector<std::vector<int>>().swap(already);
            window->clear(White);
            draw(window, first_click, 0);
            window->display();
        } else if (user_action) {
            int counter = 0;
            for (int i = 0; i < TileSize; ++i) {
                for (int j = 0; j < TileSize; ++j) {
                    counter += colours[i][j] == White;
                }
            }

            if (counter >= DropNew) {
                int index = 0;
                int attempts = 0;
                while (index < DropNew) {
                    int p = dist(rng);
                    int q = dist(rng);
                    if (colours[p][q] == White) {
                        int ar_index = rgb_dist(rng);
                        colours[p][q] = RGBColours[ar_index];
                        neighbors(p, q);
                        if (already.size() < MinimumDestroyRequired) {
                            ++index;
                        }
                        else {
                            colours[p][q] = White;
                        }
                        std::vector<std::vector<int>>().swap(already);

                        ++attempts;

                        if (attempts > 100) {
                            text.setString("Game over! Your score is " + std::to_string(score) + ". Press F5 to restart");
                            text.setPosition(20, 0);
                            is_game = false;
                            break;
                        }
                    }
                }
            } else {
                is_game = false;
                text.setPosition(20, 0);
                text.setString("Game over! Your score is " + std::to_string(score) + ". Press F5 to restart");
            }
        }
        std::vector<std::vector<int>>().swap(already);
    }
    int counter[NColours];

    for (int i = 0; i < NColours; ++i) {
        counter[i] = 0;
    }
}


int main() {
    font.loadFromFile("/home/afentev/CLionProjects/untitled3/arial.ttf");
    text.setString("Your score is " + std::to_string(score));
    text.setFillColor(sf::Color::Black);
    text.setStyle(sf::Text::Bold);
    text.setPosition(WindowSize / 3, 0);

    int first_click[2] = {-1, -1};

    std::random_device dev;
    std::mt19937 rng(dev());
    uniform_int_distribution dist(0, TileSize - 1);
    uniform_int_distribution rgb_dist(0, NColours - 1);

    for (int i = 0; i < NColours; ++i) {
        int hsv = 360 / NColours * i;
        RGBColours[i] = HSVtoRGB(hsv, 1, 1);
    }

    roads.resize(TileSize);
    for (int row = 0; row < TileSize; ++row) {
        roads[row].resize(TileSize);
        for (int column = 0; column < TileSize; ++column) {
            CPoints[row][column].x = row * SquareSize + Gap * (0.5 + row);
            CPoints[row][column].y = column * SquareSize + Gap * (0.5 + column) + FontSize;

            roads[row][column] = 1e8;
        }
    }

    for (int i = 0; i < TileSize; ++i) {
        for (int j = 0; j < TileSize; ++j) {
            colours[i][j] = White;
        }
    }

    int index = 0;
    while (index < FirstDrop) {
        int x = dist(rng);
        int y = dist(rng);
        if (colours[x][y] == White) {
            int ar_index = rgb_dist(rng);
            colours[x][y] = RGBColours[ar_index];
            neighbors(x, y);
            if (already.size() < MinimumDestroyRequired)
                ++index;
            std::vector<std::vector<int>>().swap(already);
        }
    }
    sf::RenderWindow window(sf::VideoMode(WindowSize - Gap, WindowSize - Gap + FontSize), "Colored lines");
    for (int x = 0; x < TileSize; ++x) {
        for (int y = 0; y < TileSize; ++y) {
            destroy(&window, first_click, x, y, false);
        }
    }

    while (window.isOpen()) {
        sf::Event event;

        sf::Vector2i position;
        int x, y, circle_x, circle_y;
        bool in_circle;

        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseButtonPressed:
                    if (is_game) {
                        position = sf::Mouse::getPosition(window);
                        x = position.x / (SquareSize + Gap);
                        y = (position.y - FontSize) / (SquareSize + Gap);
                        circle_x = CPoints[x][y].x + SquareSize / 2;
                        circle_y = CPoints[x][y].y + SquareSize / 2;
                        in_circle = (position.x - circle_x) * (position.x - circle_x) +
                                    (position.y - circle_y) * (position.y - circle_y) <= SquareSize * SquareSize / 4;
                        if (in_circle && colours[x][y] != White) {
                            if (x == first_click[0] && y == first_click[1]) {
                                first_click[0] = first_click[1] = -1;
                            } else if (first_click[0] == -1 && first_click[1] == -1) {
                                first_click[0] = x;
                                first_click[1] = y;
                            }
                        } else if (colours[x][y] == White && first_click[0] != -1 && first_click[0] != -1) {
                            std::vector<int> current = std::vector<int>{x, y};
                            std::vector<int> begin = std::vector<int>{first_click[0], first_click[1]};
                            std::vector<int> tmp;

                            roads[begin[0]][begin[1]] = 0;
                            lee(current);
                            std::vector<std::vector<int>> restore;

                            if (roads[x][y] != 1e8) {
                                do {
                                    restore.push_back(current);
                                    if (current[0] - 1 >= 0) {
                                        if (roads[current[0] - 1][current[1]] + 1 == roads[current[0]][current[1]])
                                            tmp = std::vector<int>{current[0] - 1, current[1]};
                                    }
                                    if (current[0] + 1 < TileSize) {
                                        if (roads[current[0] + 1][current[1]] + 1 == roads[current[0]][current[1]])
                                            tmp = std::vector<int>{current[0] + 1, current[1]};
                                    }
                                    if (current[1] - 1 >= 0) {
                                        if (roads[current[0]][current[1] - 1] + 1 == roads[current[0]][current[1]])
                                            tmp = std::vector<int>{current[0], current[1] - 1};
                                    }
                                    if (current[1] + 1 < TileSize) {
                                        if (roads[current[0]][current[1] + 1] + 1 == roads[current[0]][current[1]])
                                            tmp = std::vector<int>{current[0], current[1] + 1};
                                    }
                                    current = tmp;
                                } while (current != begin);
                                restore.push_back(current);

                                for (int i = restore.size() - 1; i > 0; --i) {
                                    colours[restore[i - 1][0]][restore[i -
                                                                       1][1]] = colours[restore[i][0]][restore[i][1]];
                                    colours[restore[i][0]][restore[i][1]] = White;

                                    window.clear(White);
                                    draw(&window, first_click, 0);
                                    window.display();
                                    usleep(50000);
                                }

                                window.clear(White);
                                draw(&window, first_click, 0);
                                window.display();

                                ++score;

                                destroy(&window, first_click, x, y, true);
                                first_click[0] = first_click[1] = -1;
                                window.clear(White);
                                draw(&window, first_click, 0);
                                window.display();
                            } else {
                                first_click[0] = first_click[1] = -1;
                            }
                            for (int row = 0; row < TileSize; ++row) {
                                for (int column = 0; column < TileSize; ++column) {
                                    roads[row][column] = 1e8;
                                }
                            }
                        }
                    }
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::F5) {
                        text.setPosition(WindowSize / 3, 0);
                        is_game = true;
                        score = 0;

                        for (int i = 0; i < TileSize; ++i) {
                            for (int j = 0; j < TileSize; ++j) {
                                colours[i][j] = White;
                            }
                        }

                        index = 0;
                        while (index < FirstDrop) {
                            x = dist(rng);
                            y = dist(rng);
                            if (colours[x][y] == White) {
                                int ar_index = rgb_dist(rng);
                                colours[x][y] = RGBColours[ar_index];
                                neighbors(x, y);
                                if (already.size() < MinimumDestroyRequired)
                                    ++index;
                                std::vector<std::vector<int>>().swap(already);
                            }
                        }
                    }

            }
        }
        window.clear(White);
        draw(&window, first_click, 0);
        window.display();
    }
}
