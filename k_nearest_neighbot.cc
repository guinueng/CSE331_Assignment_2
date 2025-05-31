#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <cmath>
#include <cfloat>
#include <iomanip>
#include <algorithm>
#include <limits> // Required for numeric_limits

#define INF DBL_MAX

// Use long double for distance calculation to improve precision.
long double calc_2d_dist(long double first_x, long double first_y, long double second_x, long double second_y) {
    long double x_diff = std::abs(first_x - second_x);
    long double y_diff = std::abs(first_y - second_y);
    return std::sqrt(x_diff * x_diff + y_diff * y_diff);
}

bool cmp_weight(std::pair<int, long double> a, std::pair<int, long double> b){
    return a.second < b.second;
}

void find_hc(std::vector<std::vector<std::pair<int, long double>>>& child, std::vector<int>& mst_hc, std::vector<bool>& visited, int idx){
    visited[idx] = true;
    mst_hc.push_back(idx);
    for(auto i: child[idx]){
        if(!visited[i.first]){
            find_hc(child, mst_hc, visited, i.first);
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <dataset>\n";
        return EXIT_FAILURE;
    }

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

    

    long double aprx_weight = 0;
    for(size_t i = 0; i < n - 1; i++){
        aprx_weight += calc_2d_dist(vertices[mst_hc[i]].second.first, vertices[mst_hc[i]].second.second, vertices[mst_hc[i + 1]].second.first, vertices[mst_hc[i + 1]].second.second);
    }
    aprx_weight += calc_2d_dist(vertices[n - 1].second.first, vertices[n - 1].second.second, vertices[0].second.first, vertices[0].second.second);

    for(auto i: mst_hc){
        std::cout << i << "\t";
    }

    std::cout << "\naprx dist: " << aprx_weight << std::endl;

    return 0;
}
