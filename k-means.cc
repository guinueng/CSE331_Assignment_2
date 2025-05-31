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
long double calc_2d_dist(std::pair<long double, long double> first, std::pair<long double, long double> second) {
    long double x_diff = std::abs(first.first - second.first);
    long double y_diff = std::abs(first.second - second.second);
    return std::sqrt(x_diff * x_diff + y_diff * y_diff);
}

bool cmp_weight(std::pair<int, long double> a, std::pair<int, long double> b){
    return a.second < b.second;
}

void k_means(std::vector<std::pair<int, std::pair<long double, long double>>>& vertex, int k, int iter,
            std::vector<std::vector<int>>& cluster, std::vector<std::pair<long double, long double>>& centroid){
    for(size_t i = 0; i < iter; i++){
        for(size_t j = 0; j < vertex.size(); j++){  // 1. Assign cluster.
            int closest = 0;
            int closest_dist = INF;
            for(size_t k = 0; k < centroid.size(); k++){
                long double tmp_dist = calc_2d_dist(vertex[j].second, centroid[k]);
                if(tmp_dist < closest_dist){
                    closest = k;
                    closest_dist = tmp_dist;
                }
            }

            cluster[closest].push_back(j);
        }

        for(size_t j = 0; j < cluster.size(); j++){ // 2. Update centroid.
            long double x_sum = 0;
            long double y_sum = 0;
            size_t qty = cluster[j].size();
            for(size_t k = 0; k < qty; k++){
                x_sum += vertex[cluster[j][k]].second.first;
                y_sum += vertex[cluster[j][k]].second.second;
            }

            x_sum /= qty;
            y_sum /= qty;

            centroid[j].first = x_sum;
            centroid[j].second = y_sum;
        }
    }
}

void k_means_init(std::vector<std::pair<int, std::pair<long double, long double>>>& vertex, int k,
            std::vector<std::vector<int>>& cluster, std::vector<std::pair<long double, long double>>& centroid){
    
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
