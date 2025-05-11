#include <iostream>
#include <fstream>
#include <cstdlib> // for EXIT_FAILURE
#include <vector>
#include <sstream>
#include <cmath>
#include <cfloat>   // Used for FLT_MAX
#include <climits>  // Used for INT_MAX
#include <chrono>   // Used for calculating execution time.

#define INF FLT_MAX

float calc_2d_dist(std::pair<float, float> first, std::pair<float, float> second){
    float x_dif = abs(first.first - second.first);
    float y_dif = abs(first.second - second.second);
    return sqrt(x_dif * x_dif + y_dif * y_dif);
}

float prim_jarnik_mst(std::vector<std::vector<float>> &adj_matrix, std::vector<std::pair<int, float>> &city, int size){
    city[0].second = 0;
    for(int i = 1; i < 5; i++){
        city[i].second = INF;
    }
    return 0;
}

int main(int argc, char* argv[]){
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <dataset> <opt>\n";
        return EXIT_FAILURE;
    }

    std::ifstream file(argv[1], std::ios::in);
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

        vertex.emplace_back(city, std::make_pair(x, y));
    }

    // for(const auto i: vertex){ // Print vertex info.
    //     std::cout << i.first << "th city with loc: " << i.second.first << ", " << i.second.second << std::endl;
    // }

    size_t n = vertex.size(); // Check total input.
    std::vector<std::vector<float>> adj_matrix(n, std::vector<float>(n, 0.0000f));

    // Make adj_matrix. Since n>5, n + m = n + (n - 1)! (due to TSP deals with complete graph) > n^2, adj_matrix is better choice than edge list/adj list.
    for(size_t i = 0; i < n; i++){
        adj_matrix[i][i] = 0;

        for(size_t j = i + 1; j < n; j++){
            float dist = calc_2d_dist(vertex[i].second, vertex[j].second);
            adj_matrix[i][j] = dist;
            adj_matrix[j][i] = dist;
        }
    }

    // Find MST
    std::vector<std::vector<float>> mst(n, std::vector<float>(n, 0.0000f));
    std::vector<bool> inside(n, false);
    std::vector<int> key(n, INT_MAX);
    std::vector<int> parent(n, -1);
    
    mst = adj_matrix;
    inside[0] = true;
    key[0] = 0;
    int idx = 0;
    int min_idx;
    // std::pair<int, float> min = {-1, INT_MAX};

    for(size_t i = 0; i < n; i++){
        min_idx = -1;
        for(size_t j = 0; j < n; j++){
            if(i == 0){
                key[j] = adj_matrix[idx][j];
                if(j == 0 || adj_matrix[idx][j] < adj_matrix[idx][min_idx]){
                    min_idx = j;
                }
            }
            else{
                if(j == idx){
                    continue;
                }

                if()

                if(j == 0 || adj_matrix[idx][j] < adj_matrix[idx][min_idx]){
                    min_idx = j;
                }

            }
        }

        inside[min_idx] = true;
        idx = min_idx;
    }
    

    return 0;
}