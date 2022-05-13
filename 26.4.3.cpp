#include <iostream>
class Monitor;
class Window;

class Monitor {
    unsigned width, height;
    friend class Window;

public:
    Monitor (unsigned w, unsigned h) {
        width = w;
        height = h;
    }
    friend void show_display (const Monitor & monitor, const Window & window);
};

class Window {
    int x, y;
    unsigned width, height;

public:
    Window (const Monitor & monitor) {
        x = 0;
        y = 0;
        width = monitor.width;
        height = monitor.height;
    }

    void move (const int & dx, const int & dy, const Monitor & monitor) {
        if (x + dx >= (int)monitor.width) {
            x = (int)monitor.width;
        }
        else if (x + dx <= 0){
            x = 0;
        }
        else {
            x += dx;
        }

        if (y + dy >= (int)monitor.height) {
            y = (int)monitor.height;
        }
        else if (y + dy <= 0){
            y = 0;
        }
        else {
            y += dy;
        }
    }

    void resize (const unsigned & newWidth, const unsigned & newHeight) {
        width = newWidth;
        height = newHeight;
    }

    void show_position () {
        std::cout << "window position: " << x << ',' << y << std::endl;
    }

    void show_size () {
        std::cout << "window size: " << width << 'x' << height << std::endl;
    }

    friend void show_display (const Monitor & monitor, const Window & window);
};

void show_display (const Monitor & monitor, const Window & window){
    std::cout << std::endl;
    for (int y = 0; y < monitor.height; y++){
        for (int x = 0; x < monitor.width; x++) {
            if (x >= window.x and x < window.x + window.width and y >= window.y and y < window.y + window.height) {
                std::cout << '1';
            }
            else {
                std::cout << '0';
            }
        }
        std::cout << std::endl;
    }
}


template<typename Type>
Type inputSomething (const std::string & previewText) {
    Type result;
    while (true) {
        std::cin.rdbuf()->pubsync();
        std::cin.clear();
        std::cout << previewText;
        std::cin >> result;
        if (!std::cin.fail()) {
            break;
        }
    }
    return result;
}

int main() {
    Monitor monitor(80,50);
    Window window(monitor);

    while (true) {
        auto command = inputSomething<std::string> ("Input command: ");

        if (command == "move") {
            auto x = inputSomething<int> ("Input delta x: ");
            auto y = inputSomething<int> ("Input delta y: ");
            window.move(x, y, monitor);
            window.show_position();
        }
        else if (command == "resize") {
            auto newW = inputSomething<int> ("Input new width: ");
            auto newH = inputSomething<int> ("Input new height: ");
            if (newW < 0 or newH < 0) {
                std::cout << "Sign error!" << std::endl;
                continue;
            }
            window.resize(newW, newH);
            window.show_size();
        }
        else if (command == "display"){
            show_display(monitor, window);
        }
        else if (command == "close"){
            break;
        }
        else {
            std::cout << "Unknown command" << std::endl;
        }
    }
}