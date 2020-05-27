/**
 * An example unit test using doctest. See documentation at
 * https://github.com/onqtam/doctest/blob/master/doc/markdown/tutorial.md
 */
/*
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_CASE("Successful Test Example") {
    int a = 5;
    CHECK(a == 5);
}
TEST_CASE("Failing Test Examples") {
    CHECK(true == false);
}
*/
#include <iostream>
#include <vector>
#include <signal.h>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <map>

void InputGenerator() {
    while(!std::cin.eof()) {
        std::string line;
        std::getline(std::cin, line);
        if (line.size () > 0)
            std::cout << line << std::endl;  
        else
        {
            std::cout << "error" << std::endl;  
            exit(1);
        }
         
    }
    std::cout << "EOF" << '\n';
}

int main() {

    std::map<int, int> m;
    char* c = "a";
    std::cout << strlen(c) << "\n";;
}
