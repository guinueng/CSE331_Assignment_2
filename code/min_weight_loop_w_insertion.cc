#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <cmath>
#include <cfloat>
#include <iomanip>
#include <algorithm>
#include <chrono>
#include <limits> // Required for numeric_limits

#define INF DBL_MAX

// Use long double for distance calculation to improve precision.
long double calc_2d_dist(std::pair<int, std::pair<long double, long double>>& first, std::pair<int, std::pair<long double, long double>>& second) {
    long double x_diff = std::abs(first.second.first - second.second.first);
    long double y_diff = std::abs(first.second.second - second.second.second);
    return std::sqrt(x_diff * x_diff + y_diff * y_diff);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <dataset>\n";
        return EXIT_FAILURE;
    }

    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << argv[1] << "\n";
        return EXIT_FAILURE;
    }

    std::vector<std::pair<int, std::pair<long double, long double>>> vertices; // Use long double
    std::string line;

    while (std::getline(file, line)) {
        std::cout << line << std::endl;
        if (line.rfind("NODE_COORD_SECTION", 0) == 0) {
            break;
        }
    }

    while (std::getline(file, line)) {
        if (line.find("EOF") != std::string::npos) {
            break;
        }
        std::istringstream iss(line);
        int city;
        long double x, y; // Use long double
        if (iss >> city >> x >> y) {
            vertices.emplace_back(city, std::make_pair(x, y));
        } else {
            std::cerr << "Skipping invalid line: " << line << std::endl;
        }
    }

    size_t n = vertices.size();
    if (n == 0) {
        std::cerr << "No vertices found in the file.\n";
        return EXIT_FAILURE;
    }

    auto file_open_end = std::chrono::high_resolution_clock::now();

    std::vector<int> loop;  // 1. Find loop of city which goes to 1 or make loops.
    std::vector<bool> visited (n, false);
    size_t visit_qty = 0;
    int idx = 0;

    while(visit_qty != n){
        visited[idx] = true;
        visit_qty++;
        loop.push_back(idx);
        int small_idx;
        long double small_dist = INF;
        for(size_t i = 0; i < n; i++){
            if(!visited[i]){
                long double tmp_dist = calc_2d_dist(vertices[idx], vertices[i]);
                if(small_dist > tmp_dist){
                    small_dist = tmp_dist;
                    small_idx = i;
                }
            }
        }

        idx = small_idx;
        if(idx == 1 || visit_qty == (n - 1)){
            if(idx != 1 && visit_qty == (n - 1)){
                visited[idx] = true;
                loop.push_back(idx);
            }
            loop.push_back(0);
            break;
        }
    }

    printf("2\n");
    std::vector<std::vector<std::pair<int, long double>>> candidates(n);
    for(size_t i = 0; i < n; i++){ // 2. Insert unvisited city in loops.
        long double min_weight = INF;
        int min_idx = -1;

        if(!visited[i]){
            for(size_t j = 0; j < (size_t)(loop.size() - 1); j++){
                long double tmp_dist = calc_2d_dist(vertices[loop[j]], vertices[i]) + calc_2d_dist(vertices[loop[i]], vertices[j + 1]);
                if(tmp_dist < min_weight){
                    min_weight = tmp_dist;
                    min_idx = j + 1;
                }
            }
        }

        if(min_idx != -1){
            visited[i] = true;
            loop.insert(loop.begin() + min_idx, i);
        }
    }

    long double aprx_weight = 0;
    for(size_t i = 0; i < n - 2; i++){
        aprx_weight += calc_2d_dist(vertices[loop[i]], vertices[loop[i + 1]]);
    }

    printf("\nPath: \n");
    for(auto i: loop){
        std::cout << i << "\t";
    }

    std::cout << std::fixed << std::setprecision(15); // Increase precision
    std::cout << "\nDist: " << aprx_weight << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    auto TSP_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - file_open_end).count();
    std::cout << "Execution time: " << duration << "(ms)\n";
    std::cout << "TSP algorithm time: " << TSP_duration << "(ms)\n";

    return 0;
}
