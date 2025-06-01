#include <iostream>
#include <fstream>  // Used for opening and reading file.
#include <cstdlib>  // Used for EXIT_FAILURE
#include <vector>   // Used for storing element.
#include <sstream>  // Used for dismissing " " in readed line.
#include <cmath>    // Used for sqrt.
#include <cfloat>   // Used for FLT_MAx
#include <chrono>   // Used for calculating execution time.

#define INF FLT_MAX

void print(const auto comment, const auto& container){ // For debuging purpose.
    auto size = std::size(container);
    std::cout << comment << "{ ";
    for (const auto& element : container)
        std::cout << element << (--size ? ", " : " ");
    std::cout << "}\n";
}

float calc_2d_dist(std::pair<float, float> first, std::pair<float, float> second){ // Calculate euclidean dist between two vertex.
    float x_dif = abs(first.first - second.first);
    float y_dif = abs(first.second - second.second);
    // printf("calc: %f\n", sqrt(x_dif * x_dif + y_dif * y_dif));
    return sqrt(x_dif * x_dif + y_dif * y_dif);
}

int main(int argc, char* argv[]){
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <dataset>\n";
        return EXIT_FAILURE;
    }

    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream file(argv[1], std::ios::in);
    if(!file.is_open()){
        std::cerr<< "Unable to open file.\n" << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<std::pair<int, std::pair<float, float>>> vertex;
    std::vector<int> vertex_list;

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
        vertex_list.emplace_back(city);
    }

    auto file_open_end = std::chrono::high_resolution_clock::now();

    int n = vertex.size(); // Check total input.

    auto adj_matrix_end = std::chrono::high_resolution_clock::now();

    // dp[subset][last] = minimum cost to reach 'last' having visited 'subset'
    // Representing visited subset as bitmask.
    // Found that Held-Karp algorithm is needed to memorize dp sequences.
    // std::vector<std::vector<float>> dp(1 << n, std::vector<float>(n, INF));
    // Vector is quite usefull, but too slow.
    // float dp[size][n]; <- stack size is so small that n = 25, stack overflow happens.
    float** dp = new float* [1 << n];
    // Thus need to use heap, thus used new and delete.
    for(int i = 0; i < (1 << n); ++i){
        dp[i] = new float[n];
        for(int j = 0; j < n; ++j){
            dp[i][j] = FLT_MAX;
        }
    }

    dp[1 << 0][0] = 0; // Make starting city(city 1)'s cumulative dist = 0;

    for(int subset = 0; subset < (1 << n); subset++){
        // Trying all subset case by gradually increasing subset.
        // 1 -> 2(in binary 10, which include city 2) -> 3(in binary 11, which include city 1, 2) and so on.
        for(int last = 0; last < n; ++last){
            if(subset & (1 << last)){
                // Trying S-l case.
                // Checking last visit city(will visit in this context) is included in subset.
                // If last is not included in subset, by and operation, if statement will work as false case.
                for(int prev = 0; prev < n; prev++){
                    if(prev != last && (subset & (1 << prev))){
                        // Calc cumulative distance based on prev city which will work m, and last will work as l
                        // in given dpsequencing paper.
                        // prev city should also contained in subset.
                        dp[subset][last] = std::min(
                            dp[subset][last],
                            dp[subset ^ (1 << last)][prev] + calc_2d_dist(vertex[prev].second, vertex[last].second)
                            // min dist to city 1 to prev + dist from prev to last city
                        );
                    }
                }
            }
        }
    }

    // After filling dp[subset][last] array, find last city with minimal cost
    int last = -1;
    float min_dist = INF;
    int ALL_VISITED = (1 << n) - 1;

    for(int k = 1; k < n; k++){
        float cost = dp[ALL_VISITED][k] + calc_2d_dist(vertex[k].second, vertex[0].second);
        if(cost < min_dist){
            min_dist = cost;
            last = k;
        }
    }

    // Then, backtrack dp array to find the tour
    std::vector<int> tour;
    int subset = ALL_VISITED;
    int curr = last;
    tour.push_back(1); // End city

    for(int i = n - 1; i >= 1; i--){
        tour.push_back(curr + 1);
        // Since array range is [0, n) but city's number range is [1, n], need to add 1.
        int prev_city = -1;
        for(int k = 0; k < n; k++){
            if(k != curr && (subset & (1 << k))){
                if (dp[subset][curr] == dp[subset ^ (1 << curr)][k] + calc_2d_dist(vertex[k].second, vertex[curr].second)) {
                    prev_city = k;
                    break;
                }
            }
        }
        subset ^= (1 << curr);
        curr = prev_city;
    }

    tour.push_back(1); // Start city.

    std::cout << "min dist: " << min_dist << "\n\n";

    for(auto i = tour.rbegin(); i != tour.rend(); i++){
        // Since tour is stored as backtrace way, need to print reverse way.
        // Save city sequence in each line.
        std::cout << *i << "\n";
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    // auto adj_matrix_duration = std::chrono::duration_cast<std::chrono::milliseconds>(adj_matrix_end - file_open_end).count();
    auto TSP_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - adj_matrix_end).count();
    std::cout << "Execution time: " << duration << "(ms)\n";
    // std::cout << "adj matrix time: " << adj_matrix_duration << "(ms)\n";
    std::cout << "TSP algorithm time: " << TSP_duration << "(ms)\n";

    for(int i = 0; i < (1 << n); i++){
        delete[] (dp[i]);
    }

    delete[] (dp);

    return 0;
}