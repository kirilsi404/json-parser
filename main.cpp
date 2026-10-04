#include <iostream>
#include "Commands/Commands.h"

int main() {
    try {
        Commands app;
        app.start();
    }
    catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "An unknown critical error occurred!" << std::endl;
    }

    return 0;
}