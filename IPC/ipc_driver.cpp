#include <iostream>
#include <sstream>
#include <vector>
#include <signal.h>
#include <unistd.h>


// Accept input command
void InputCommand() {
    while(!std::cin.eof()) {
        std::string line;
        std::getline(std::cin, line);
        if (line.size () > 0)
            std::cout << line << std::endl;   
    }
}


int main (int argc, char **argv) {
    // Initialize pipe
    std::vector<pid_t> kids;
    int p1[2];
    int p2[2];
    pipe(p1);
    pipe(p2);

    // Fork process for a1
    pid_t a1_pid = fork();
    if(a1_pid == 0) {
        // Redirect stdin to pipe1 and stdout to pipe2
        dup2(p1[0], STDIN_FILENO);
        dup2(p2[1], STDOUT_FILENO);
        close(p1[1]);
        close(p1[0]);
        close(p2[0]);
        close(p2[1]);

        char* pyarg[3];
        pyarg[0] = (char*)"python2.7";
        pyarg[1] = (char*)"./intersection.py";
        pyarg[2] = nullptr;
        execv("/usr/bin/python2.7", pyarg);
        perror("Error:");
        return 1;
    }
    kids.push_back(a1_pid);

    // Fork process for a2
    pid_t a2_pid = fork();
    if(a2_pid == 0) {
        // Redirect stdin to pipe2
        dup2(p2[0], STDIN_FILENO);
        close(p2[1]);
        close(p2[0]);

        execv("./graph_parser", nullptr);
        return 1;
    }
    kids.push_back(a2_pid);

    // Fork process for rgen
    pid_t rgen_pid = fork();
    if(rgen_pid == 0) {
        // Redirect stdout to pipe1
        dup2(p1[1], STDOUT_FILENO);
        close(p1[0]);
        close(p1[1]);      

        execv("./rgen", argv);
        return 1;
    }
    kids.push_back(rgen_pid);

    // Redirect stdout to pipe2
    dup2(p2[1], STDOUT_FILENO);
    close(p2[0]);
    close(p2[1]);

    InputCommand();

    for (pid_t k : kids) {
        //int status;
        kill (k, SIGTERM);
        //waitpid(k, &status, 0);
    }

    return 0;
}
