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

void find_hc(std::vector<int>& target, std::vector<std::vector<std::pair<int, long double>>>& child, std::vector<int>& mst_hc, std::vector<bool>& visited, int idx){
    visited[idx] = true;
    mst_hc.push_back(target[idx]);
    for(auto i: child[idx]){
        if(!visited[i.first]){
            find_hc(target, child, mst_hc, visited, i.first);
        }
    }
}

int mst(std::vector<int>& target, std::vector<std::pair<int, std::pair<long double, long double>>>& vertex, std::vector<int>& result, int start_city){
    int n = target.size();
    std::vector<int> parent(n);
    std::vector<std::vector<std::pair<int, long double>>> child(n);
    std::vector<long double> key(n, INF); // Use long double
    std::vector<bool> in_mst(n, false);
    std::vector<int> conn_stat(n, 0);

    key[start_city] = 0;
    parent[start_city] = -1;

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
            long double dist = calc_2d_dist(vertex[target[u]].second, vertex[target[v]].second);
            if (!in_mst[v] && dist < key[v]) {
                parent[v] = u;
                key[v] = dist;
            }
        }
    }

    // long double total_weight = 0; // Use long double
    // std::cout << "Edge \tWeight\n";
    // std::cout << std::fixed << std::setprecision(15); // Increase precision

    // long double w = 0;
    // for(size_t i = 0; i < n; i++){
    //     w += key[i];
    // }
    // std::cout << "Total MST weight in key: " << w << std::endl;

    for(size_t i = 1; i < n; i++){
        child[parent[i]].emplace_back(i, key[i]);
    }
    // printf("1\n");

    for(size_t i = 0; i < n; i++){
        std::sort(child[i].begin(), child[i].end(), cmp_weight);
        // for(auto j: child[i]){
        //     std::cout << i + 1 << " - " << j.first + 1 << "\t" << j.second << std::endl;
        // }
    }

    // std::vector<int> result; // 2. Find HC in MST.
    std::vector<bool> visited (n, false);
    find_hc(target, child, result, visited, 0);
}

void k_means(std::vector<std::pair<int, std::pair<long double, long double>>>& vertex, int k, int iter,
            std::vector<std::vector<int>>& cluster, std::vector<std::pair<long double, long double>>& centroid, int& one_cont_vertex){
    std::vector<std::pair<long double, long double>> prev_centroid = centroid;
    for(size_t i = 0; i < iter; i++){
        std::vector<std::vector<int>> new_cluster(k);
        for(size_t j = 0; j < vertex.size(); j++){  // 1. Assign cluster.
            int closest = 0;
            long double closest_dist = INF;
            for(size_t l = 0; l < centroid.size(); l++){
                long double tmp_dist = calc_2d_dist(vertex[j].second, centroid[l]);
                if(tmp_dist < closest_dist){
                    closest = l;
                    closest_dist = tmp_dist;
                }
            }

            new_cluster[closest].push_back(j);
        }

        cluster = new_cluster;
        int cnt = 0;    // Counting early termination cond met.
        for(size_t j = 0; j < k; j++){ // 2. Update centroid.
            long double x_sum = 0;
            long double y_sum = 0;
            size_t qty = cluster[j].size();
            for(size_t l = 0; l < qty; l++){
                int tmp_idx = cluster[j][l];
                x_sum += vertex[tmp_idx].second.first;
                y_sum += vertex[tmp_idx].second.second;
                if(tmp_idx == 0){
                    one_cont_vertex = tmp_idx;
                }
            }

            if(qty > 0){
                x_sum /= qty;
                y_sum /= qty;
                
                centroid[j].first = x_sum;
                centroid[j].second = y_sum;
                
                if(abs(prev_centroid[j].first - x_sum) < 0.0001 && abs(prev_centroid[j].second - y_sum) < 0.0001){
                    cnt++;
                }
            }
        }

        if(cnt == k){
            break;
        }
    }
}

void k_means_init(std::vector<std::pair<int, std::pair<long double, long double>>>& vertex, int k, int iter,
            std::vector<std::vector<int>>& cluster, std::vector<std::pair<long double, long double>>& centroid, int& one_cont_vertex){
    size_t vertex_size = vertex.size() / k;
    for(size_t i = 0; i < centroid.size(); i++){
        centroid[i].first = vertex[vertex_size * i].second.first;
        centroid[i].second = vertex[vertex_size * i].second.second;
    }

    k_means(vertex, k, iter, cluster, centroid, one_cont_vertex);
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

    int k = 10;
    int iter = 100;
    int one_cont_vertex = 0;
    std::vector<std::vector<int>> cluster(k);
    std::vector<std::pair<long double, long double>> centroid(k);
    k_means_init(vertices, k, iter, cluster, centroid, one_cont_vertex);

    std::vector<int> aprx_tour;
    std::vector<bool> visited(k, false);
    int idx = 0;
    visited[one_cont_vertex] = true;
    // for(size_t i = 0; i < cluster[one_cont_vertex].size(); i++){
    std::sort(cluster[one_cont_vertex].begin(), cluster[one_cont_vertex].end());
    mst(cluster[one_cont_vertex], vertices, aprx_tour, 0);
    int last_city = aprx_tour.back();
        // Find opt tour of inside cluster which contains 1.
    // }

    // Find remainder's opt path.
    for(size_t i = 0; i < k; i++){
        int min_dist_cluster = 0;
        long double min_dist = INF;
        for(size_t j = 0; j < k; j++){  // Find min dist cluster.
            if(!visited[j]){
                long double tmp_dist = calc_2d_dist(vertices[last_city].second, centroid[j]);
                if(tmp_dist < min_dist){
                    min_dist_cluster = j;
                    min_dist = tmp_dist;
                }
            }
        }

        // Find closest city.
        visited[min_dist_cluster] = true;
        int start_city = cluster[min_dist_cluster].front();
        int min_intercity_dist = INF;
        for(size_t j = 0; j < cluster[min_dist_cluster].size(); j++){
            long double tmp_dist = calc_2d_dist(vertices[last_city].second, vertices[cluster[min_dist_cluster][j]].second);
        }

        // Find hc of target cluster.
        std::vector<int> tmp_rst;
        mst(cluster[min_dist_cluster], vertices, tmp_rst, start_city);
        
        // Append result into aprx_tour.
        aprx_tour.insert(aprx_tour.end(), tmp_rst.begin(), tmp_rst.end());
        last_city = aprx_tour.back();
    }

    // Add connection btw last city and first city.
    aprx_tour.push_back(0);

    long double aprx_weight = 0;
    for(size_t i = 0; i < n - 1; i++){
        aprx_weight += calc_2d_dist(vertices[aprx_tour[i]].second, vertices[aprx_tour[i + 1]].second);
    }

    for(auto i: aprx_tour){
        std::cout << i + 1 << "\t";
    }

    std::cout << "\naprx dist: " << aprx_weight << std::endl;

    return 0;
}
