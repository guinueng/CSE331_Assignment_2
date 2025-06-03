#include <iostream>
#include <string>
#include <vector>
#include <cstdlib> // For std::system
#include <fstream> // For file handling

// Function to run a program with arguments and capture its output
void runProgram(const std::string& program, const std::string& input_arg_1) {
    // Construct the command string
    std::string command = "/usr/bin/time -v " + program + " ./dataset/" + input_arg_1; // Redirect output to a file

    // Print the results
    std::cout << "Results from " << program << " w/ input file: " << input_arg_1 << ":" << std::endl;

    // Execute the command
    int result = std::system(command.c_str());
    if (result != 0) {
        std::cerr << "Error running program: " << program << std::endl;
        return;
    }
}

int main() {
    // List of programs and their arguments
    std::vector<std::string> progs = {
        // "./Held-Karp_algorithm",
        "./Christofides_heuristic"
    };
 
    std::vector<std::string> args = {
        "a5.tsp",
        "a25.tsp",
        "a100.tsp",
        "a280.tsp",
        "xql662.tsp",
        "kz9976.tsp",
        // "mona-lisa100K.tsp",
    };

    // Run each program in sequence
    for (const auto& prog : progs) {
        for(const auto& arg_1 : args){
            runProgram(prog, arg_1);
            std::cout << "------------------------------------" << std::endl;
        }
    }

    return 0;
}
