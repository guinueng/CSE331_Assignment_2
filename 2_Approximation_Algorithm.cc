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
    std::vector<std::vector<long double>> adj_matrix(n, std::vector<long double>(n)); // Use long double

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            long double dist = calc_2d_dist(vertices[i].second.first, vertices[i].second.second, vertices[j].second.first, vertices[j].second.second);
            adj_matrix[i][j] = dist;
            adj_matrix[j][i] = dist;
        }
        adj_matrix[i][i] = 0.0;
    }

    std::vector<int> parent(n);
    std::vector<std::vector<std::pair<int, long double>>> child(n);
    std::vector<long double> key(n, INF); // Use long double
    std::vector<bool> in_mst(n, false);
    std::vector<int> conn_stat(n, 0);

    key[0] = 0;
    parent[0] = -1;

    for (size_t count = 0; count < n - 1; count++) { // 1. Find MST
        int u = -1;
        long double min_key = INF; // Use long double
        for (size_t v = 0; v < n; v++) {
            if (!in_mst[v] && key[v] < min_key) {
                min_key = key[v];
                u = v;
            }
        }

        if (u == -1) {
            std::cerr << "Error: MST construction failed. Graph is not connected.\n";
            return EXIT_FAILURE;
        }
        in_mst[u] = true;

        for (size_t v = 0; v < n; v++) {
            if (!in_mst[v] && adj_matrix[u][v] < key[v]) {
                parent[v] = u;
                key[v] = adj_matrix[u][v];
            }
        }
    } // Helped by gemini in google.inc(url: https://g.co/gemini/share/1528ec3e4e49)

    long double total_weight = 0; // Use long double
    std::cout << "Edge \tWeight\n";
    std::cout << std::fixed << std::setprecision(15); // Increase precision
    for (size_t i = 1; i < n; ++i) {
        std::cout << parent[i] + 1 << " - " << i + 1 << "\t" << adj_matrix[i][parent[i]] << std::endl;
        total_weight += adj_matrix[i][parent[i]];
        conn_stat[parent[i]]++;
    }
    std::cout << "Total MST weight: " << total_weight << std::endl;

    long double w = 0;
    for(size_t i = 0; i < n; i++){
        w += key[i];
    }
    std::cout << "Total MST weight in key: " << w << std::endl;

    for(size_t i = 1; i < n; i++){
        child[parent[i]].emplace_back(i, key[i]);
    }
    printf("1\n");

    for(size_t i = 0; i < n; i++){
        std::sort(child[i].begin(), child[i].end(), cmp_weight);
        for(auto j: child[i]){
            std::cout << i + 1 << " - " << j.first + 1 << "\t" << j.second << std::endl;
        }
    }

    std::vector<std::vector<std::pair<int, long double>>> child_ = child;

    printf("2\n");

    std::vector<int> mst_hc(n); // 2. Find HC in MST.
    int idx = 0;
    long double aprx_weight = 0;
    for(size_t i = 0; i < n; i++){
        if(child_[idx].empty()){
            idx = parent[idx];
            continue;
        }
        int tmp = child_[idx].front().first;
        child_[idx].erase(child_[idx].begin());
        if(i != n - 1){
            mst_hc[i + 1] = tmp;
        }

        idx = tmp;
    }

    printf("7\n");

    for(size_t i = 0; i < n - 1; i++){
        printf("%d\t", i);
        // for(auto j: child[mst_hc[i]]){
        //     if(j.first == mst_hc[i + 1]){
        //         aprx_weight += j.second;
        //     }
        // }
        aprx_weight += adj_matrix[mst_hc[i]][mst_hc[i + 1]];
    }
    aprx_weight += adj_matrix[mst_hc[n - 1]][mst_hc[0]];

    std::cout << "aprx dist: " << aprx_weight << std::endl;

    return 0;
}