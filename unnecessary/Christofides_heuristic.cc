#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <cmath>
#include <cfloat>
#include <algorithm>
#include <iomanip>
#include <queue>
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
    
    std::vector<int> matching;  // Pair of matching.
    std::vector<std::pair<int, int>> tight; // <<u, v>, weight>
    std::vector<std::vector<int>> moats;    // Vertex inside in blossoms.
    std::vector<int> label(odd_vertex_qty, -1);  // target idx, unmatched/even/odd(0/1/2) state.
    std::vector<int> parent(odd_vertex_qty, -1);
    std::vector<bool> in_que(odd_vertex_qty, false);
    std::vector<int> tree_root(odd_vertex_qty, 0);

    while(true){
        std::vector<int> radius(odd_vertex_qty, 0); // Radius of each vertex/moats. 0+ for vertex(discs), -1- for moats(blossoms).
        for(size_t i = 0; i < odd_vertex_qty; i++){
            parent[i] = -1;
            in_que[i] = false;
            label[i] = 0;
            tree_root[i] = i;
        }

        int root = 0;
        bool all_matched = true;
        for(size_t i = 0; i < matching.size(); i++){
            if(!matching[i] == -1){
                all_matched = false;
                root = i;
                break;
            }
        }

        if(all_matched){ // 1. Find termination cond.
            break;
        }

        std::queue<int> candidates; // 2. Expand/shrink moats(blossoms)/discs(augment tree).
        while(!candidates.empty()){
            int u = candidates.front(); // 1) Choose u.
            candidates.pop();   // 2) Delete candidates.
            int delta; // = calc_delta(); maybe?

            for(size_t v = 0; v < odd_vertex_qty; v++){
                if(tree_root[u] == tree_root[v] || matching[u] == v){ // case 1. matched one or locates in same tree.
                    continue;
                }

                if(v == root || (matching[v] != -1 && parent[matching[v]] != -1)){ // If found loops.
                    // Can create blossoms.
                }
                else if(parent[v] == -1){ // If no loops.
                    parent[v] = u;
                    if(matching[v] == -1){ // If selected node is not matched -> Can increase augment paths.
                        int curr_idx = v;
                        while(curr_idx != -1){
                            int parent_idx = parent[curr_idx];
                            int parent_matching = matching[parent_idx];

                            matching[curr_idx] = parent_idx;
                            matching[parent_idx] = curr_idx;

                            curr_idx = parent_matching;
                        }
                    }
                    else{   // If has matchings, but does not have parent.
                        candidates.push(matching[v]);
                        in_que[matching[v]] = true;
                    }
                }
            }
        }
    }

    return 0;
}