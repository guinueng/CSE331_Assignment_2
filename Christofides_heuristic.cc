#include <iostream>
#include <fstream>
#include <cstdlib> // for EXIT_FAILURE
#include <vector>
#include <cstring>
#include <sstream>
#include <cmath>

float calc_2d_dist(std::pair<float, float> first, std::pair<float, float> second){
    float x_dif = abs(first.first - second.first);
    float y_dif = abs(first.second - second.second);
    return sqrt(x_dif * x_dif + y_dif * y_dif);
}

int main(int argc, char* argv[]){
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <dataset> <opt>\n";
        return EXIT_FAILURE;
    }

    std::ifstream file(argv[1]);
    if(!file.is_open()){
        std::cerr<< "Unable to open file.\n" << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<std::pair<int, std::pair<float, float>>> vertex;

    std::string line;
    while(std::getline(file, line)){ // Print information in first part of file.
        std::cout << line << std::endl;
        if(line.compare(0, 18, "NODE_COORD_SECTION", 18) == 0){
            break;
        }
    }

    int city;
    float x, y;
    while(std::getline(file, line)){ // Extract coord info in tsp file.
        if(line.compare("EOF") == 0)
            break;
    
        std::istringstream iss(line);
        iss >> city >> x >> y;

        // std::cout << "city: " << city << " x: " << x << " y: " << y << std::endl;
        vertex.emplace_back(city, std::make_pair(x, y));
    }

    // for(const auto i: vertex){ // Print vertex info.
    //     std::cout << i.first << "th city with loc: " << i.second.first << ", " << i.second.second << std::endl;
    // }

    // std::cout << vertex.size() << std::endl;
    int n = vertex.size();
    float adj_matrix[n][n];
    for(size_t i = 0; i < n; i++){
        adj_matrix[i][i] = 0;

        for(size_t j = i + 1; j < n; j++){
            float dist = calc_2d_dist(vertex[i].second, vertex[j].second);
            adj_matrix[i][j] = dist;
            adj_matrix[j][i] = dist;
        }
    }

    // for(size_t i = 0; i < n; i++){
    //     std::cout << adj_matrix[1][i] << " ";
    // }

    return 0;
}