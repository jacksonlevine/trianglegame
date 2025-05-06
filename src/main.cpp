

#include "PrecompHeader.h"
#include "core/App.h"

int main() {
    try {
        App app;
        app.run();
    } catch (std::exception &e) {
        std::cout << e.what() << '\n';
        return 1;
    }
    return 0;
}
