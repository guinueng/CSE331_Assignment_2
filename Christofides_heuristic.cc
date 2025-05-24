#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <cmath>
#include <cfloat>
#include <algorithm>
#include <iomanip>
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

    std::vector<int> parent(n);
    std::vector<std::vector<std::pair<int, long double>>> child(n);
    std::vector<long double> key(n, INF); // Use long double
    std::vector<bool> in_mst(n, false);

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
            long double dist = calc_2d_dist(vertices[u].second.first, vertices[u].second.second, vertices[v].second.first, vertices[v].second.second);
            if (!in_mst[v] && dist < key[v]) {
                parent[v] = u;
                key[v] = dist;
            }
        }
    } // Helped by gemini in google.inc(url: https://g.co/gemini/share/1528ec3e4e49)

    long double total_weight = 0; // Use long double
    std::cout << "Edge \tWeight\n";
    std::cout << std::fixed << std::setprecision(15); // Increase precision

    long double w = 0;
    for(size_t i = 0; i < n; i++){
        w += key[i];
    }
    std::cout << "Total MST weight in key: " << w << std::endl;

    for(size_t i = 1; i < n; i++){
        child[parent[i]].emplace_back(i, key[i]);
    }
    printf("1\n");

    std::vector<int> odd_vertex;
    for(size_t i = 0; i < n; i++){
        if(child[i].size() % 2 != 0){
            odd_vertex.push_back(i);
        }
    }

    int odd_vertex_qty = odd_vertex.size();
    std::vector<std::pair<int, long double>> radius; // Radius of each vertex/moats. 0+ for vertex(discs), -1- for moats(blossoms).
    std::vector<std::pair<int, bool>> matched;
    std::vector<std::pair<int, int>> matching;  // Pair of matching.
    std::vector<std::pair<std::pair<int, int>, int>> tight; // <<u, v>, weight>
    std::vector<std::vector<int>> moats;    // Vertex inside in blossoms.
    std::vector<std::pair<int, int>> even;  // target idx, what to connected
    std::vector<std::pair<int, int>> odd;   // target idx, what to connected
    for(size_t i = 0; i < odd_vertex_qty; i++){
        radius.emplace_back(odd_vertex[i],  0);
        matched.emplace_back(odd_vertex[i], false);
    }

    while(true){
        // if(radius.size() / 2 == matching.size()){
        //     break;
        // }
        int root = 0;
        bool all_matched = true;
        for(size_t i = 0; i < matched.size(); i++){
            if(!matched[i].second){
                all_matched = false;
                root = matched[i].first;
            }
        }

        if(all_matched){ // 1. Find termination cond.
            break;
        }

        root
        while(true){    // 2. Expand/shrink all tree until 3 cases happens.
            ;
        }


    }

    // while (!isPerfectMatching()){
    //     // 팽팽한 간선만으로 alternating tree 확장
    //     growAlternatingTreesWithTightEdges();
    //     if (!augmentingPathOrBlossomFound()) {
    //         updateDualVariables(); // 새로운 팽팽한 간선이 생기도록 dual 조정
    //     }
    //     if (augmentingPathFound()) {
    //         augmentMatching();
    //     }
    //     if (blossomFound()) {
    //         contractBlossom();
    //     }
    // }



    return 0;
}