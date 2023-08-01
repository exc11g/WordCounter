#include <fstream>
#include <iostream>

struct Options {
    bool words = false;
    bool chars = false;
    bool lines = false;
    bool bytes = false;
};

struct Parameters {
    size_t words = 0;
    size_t chars = 0;
    size_t lines = 1;
    size_t bytes = -1;
};

//Обработчик файлов, считает все что нужно, выводит только то, что потребует пользователь
void HandleFile(const std::string &fileName, const Options &options) {
    Parameters parameters;
    std::ifstream fin(fileName, std::ios::in | std::ifstream::ate | std::ifstream::binary);
    parameters.bytes = fin.tellg();
    if (!fin.is_open()) {
        std::cout << fileName << " opening error" << std::endl;
        return;
    }
    char next;
    bool flagWord = true;
    while (fin) {
        next = fin.get();
        if (isprint(next) && !isspace(next)) {
            parameters.chars++;
            parameters.words += flagWord;
            flagWord = false;
        } else if (next == '\n') {
            parameters.lines++;
            flagWord = true;
        } else {
            flagWord = true;
        }
    }
    if (options.lines) {
        std::cout << parameters.lines << " ";
    }
    if (options.words) {
        std::cout << parameters.words << " ";
    }
    if (options.bytes) {
        std::cout << parameters.bytes << " ";
    }
    if (options.chars) {
        std::cout << parameters.chars << " ";
    }
    std::cout << fileName << std::endl;
}

//Функция обрабатывает строки и ищет в ней опции, заданные пользователем
void SearchOptions(const std::string &str, Options &options) {
    if (str[1] == '-') {
        if (str == "--lines") {
            options.lines = true;
        } else if (str == "--bytes") {
            options.bytes = true;
        } else if (str == "--words") {
            options.words = true;
        } else if (str == "--chars") {
            options.chars = true;
        } else {
            std::cout << "The option " << str.substr(2, str.length() - 1) << " does not exist" << std::endl;
        }
    } else {
        for (int i = 1; i < str.length(); i++) {
            if (str[i] == 'l') {
                options.lines = true;
            } else if (str[i] == 'c') {
                options.bytes = true;
            } else if (str[i] == 'w') {
                options.words = true;
            } else if (str[i] == 'm') {
                options.chars = true;
            } else {
                std::cout << "The option " << str[i] << " does not exist" << std::endl;
            }
        }
    }
}

//В случае того, когда опций не было задано, нам нужно вывести 3 параметра для каждого файла,
//т.к. изначально для всех опций стоит флаг false исправим его на true
void FixOptions(Options &opt) {
    if (!(opt.lines || opt.bytes || opt.words || opt.chars)) {
        opt.lines = true;
        opt.bytes = true;
        opt.words = true;
    }
}

int main(int argv, char** argc) {
    Options options;
    //Сначала ищем все возможные опции и ставим соответствующие флаги в search_options
    for (int i = 1; i < argv; i++) {
        if (argc[i][0] == '-') {
            SearchOptions(argc[i], options);
        }
    }
    //Фиксим флаги, если опции не были заданы
    FixOptions(options);
    //Осталось найти название файлов/путей и для каждого из них высчитать значения параметров
    for (int i = 1; i < argv; i++) {
        if (argc[i][0] != '-') {
            HandleFile(argc[i], options);
        }
    }

    return 0;
}