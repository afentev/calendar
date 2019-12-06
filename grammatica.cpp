#include <iostream>

bool check() {
    int index = 0;
    bool res;
    char letter;
    letter = std::cin.peek();
    if (letter == '\n') {
        std::cout << "Непредвиденный конец строки :(" << std::endl;
        exit(2);
    }
    std::cin >> letter;

    switch (letter) {
        case 't':
            std::cin >> letter;
            if (letter != 'r') {
                std::cout << "Некорректный символ: " << letter << std::endl;
                exit(2);
            }

            std::cin >> letter;
            if (letter != 'u') {
                std::cout << "Некорректный символ: " << letter << std::endl;
                exit(2);
            }

            std::cin >> letter;
            if (letter != 'e') {
                std::cout << "Некорректный символ: " << letter << std::endl;
                exit(2);
            }

            return true;
        case 'f':
            std::cin >> letter;
            if (letter != 'a') {
                std::cout << "Некорректный символ: " << letter << std::endl;
                exit(2);
            }

            std::cin >> letter;
            if (letter != 'l') {
                std::cout << "Некорректный символ: " << letter << std::endl;
                exit(2);
            }

            std::cin >> letter;
            if (letter != 's') {
                std::cout << "Некорректный символ: " << letter << std::endl;
                exit(2);
            }

            std::cin >> letter;
            if (letter != 'e') {
                std::cout << "Некорректный символ: " << letter << std::endl;
                exit(2);
            }

            return false;
        case 'n':
            std::cin >> letter;
            if (letter != 'o') {
                std::cout << "Некорректный символ: " << letter << std::endl;
                exit(2);
            }

            std::cin >> letter;
            if (letter != 't') {
                std::cout << "Некорректный символ: " << letter << std::endl;
                exit(2);
            }

            std::cin >> letter;
            if (letter != '(') {
                std::cout << "Ожидалась открывающая скобка, встречено: " << letter << std::endl;
                exit(2);
            }

            res = check();
            std::cin >> letter;
            if (letter != ')') {
                std::cout << "Ожидалась закрывающая скобка, встречено: " << letter << std::endl;
                exit(2);
            }
            return !res;
        case 'a':
            res = true;
            std::cin >> letter;
            if (letter != 'n') {
                std::cout << "Некорректный символ: " << letter << std::endl;
                exit(2);
            }

            std::cin >> letter;
            if (letter != 'd') {
                std::cout << "Некорректный символ: " << letter << std::endl;
                exit(2);
            }

            std::cin >> letter;
            if (letter != '(') {
                std::cout << "Некорректный символ: " << letter << std::endl;
                exit(2);
            }

            do {
                bool ch = check();
                res = res && ch;
                letter = std::cin.peek();
                if (letter == '\n') {
                    std::cout << "Непредвиденный конец строки" << std::endl;
                    exit(2);
                }
                std::cin >> letter;
                if (letter != ',' && letter != ')') {
                    std::cout << "Некорректный символ: " << letter << std::endl;
                    exit(2);
                }
                ++index;
            } while (letter != ')');
            if (index < 2) {
                std::cout << "Слишком мало аргументов" << std::endl;
                exit(2);
            }

            return res;
        case 'o':
            res = false;
            std::cin >> letter;
            if (letter != 'r') {
                std::cout << "Некорректный символ: " << letter << std::endl;
                exit(2);
            }

            std::cin >> letter;
            if (letter != '(') {
                std::cout << "Ожидалась открывающая скобка, встречено: " << letter << std::endl;
                exit(2);
            }

            do {
                bool ch = check();
                res = res || ch;
                letter = std::cin.peek();
                if (letter == '\n') {
                    std::cout << "Непредвиденный конец строки" << std::endl;
                    exit(2);
                }
                std::cin >> letter;
                if (letter != ',' && letter != ')') {
                    std::cout << "Неожиданный символ: " << letter << std::endl;
                    exit(2);
                }
                ++index;
            } while (letter != ')');
            if (index < 2) {
                std::cout << "Некорректный символ: " << letter << std::endl;
                exit(2);
            }

            return res;
        default:
            std::cout << "Некорректный символ: " << letter << std::endl;
            exit(2);
    }
}

int main() {
    bool result = check();
    char tail;
    tail = std::cin.peek();
    if (tail != '\n') {
        std::cout << "Неправильно";
        exit(2);
    }
    std::cout << result << std::endl;
    return 0;
}
