#include <iostream>
#include <Core/Config.h>
#include <Core/Args.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        std::cout << "Error: no option provided" << std::endl;
        exit(1);
    } else {
        HandleArgs(argc, argv);
    }
    return 0;
}

