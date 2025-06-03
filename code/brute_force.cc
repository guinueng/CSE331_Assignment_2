#include <iostream>
#include <fstream>
#include <cstdlib> // for EXIT_FAILURE
#include <vector>
#include <sstream>
#include <chrono>
#include <cmath>

void print(const auto comment, const auto& container)
{
    auto size = std::size(container);
    std::cout << comment << "{ ";
    for (const auto& element : container)
        std::cout << element << (--size ? ", " : " ");
    std::cout << "}\n";
}

float calc_2d_dist(std::pair<float, float> first, std::pair<float, float> second){
    float x_dif = abs(first.first - second.first);
    float y_dif = abs(first.second - second.second);
    // printf("calc: %f\n", sqrt(x_dif * x_dif + y_dif * y_dif));
    return sqrt(x_dif * x_dif + y_dif * y_dif);
}

float TSP(std::vector<int> &vertex, std::vector<int> &tour, std::vector<std::pair<int, std::pair<float, float>>>& vertex_list, int target){
    if(vertex.size() == 1){
        tour.push_back(1);
        tour.push_back(vertex[0]);
        // print("Final: ", tour);
        // printf("min: %f\n", adj_matrix[0][(vertex[0] - 1)]);
        return calc_2d_dist(vertex_list[0].second, vertex_list[vertex[0] - 1].second) + calc_2d_dist(vertex_list[vertex[0] - 1].second, vertex_list[target - 1].second);
        // return adj_matrix[0][(vertex[0] - 1)] + adj_matrix[vertex[0] - 1][target - 1];
        // Base case. Return a_1l. Since vertex saves city in range [1, n], need to distract 1.
    }

    // Recursive case. Need to find minimum tour.
    float min;
    float tmp_dist;

    int tmp, min_vertex;
    std::vector<int> tmp_vertex = vertex;
    int del_pos = tmp_vertex[0];
    tmp_vertex.erase(tmp_vertex.begin());

    for(size_t i = 0; i <= tmp_vertex.size(); i++){
        std::vector<int> tmp_tour;
        // printf("tmp: ", tmp_vertex);
        // printf("selected: %d\n", del_pos);
        // printf("target: %d, del_pos: %d\n", target, del_pos);
        tmp_dist = TSP(tmp_vertex, tmp_tour, vertex_list, del_pos) + calc_2d_dist(vertex_list[target - 1].second, vertex_list[del_pos - 1].second);
        // printf("a_ml: %f\n", adj_matrix[target - 1][del_pos - 1]);
        if(i == 0){
            min = tmp_dist;
            tour = tmp_tour;
            min_vertex = del_pos;
            // print("1st", tour);
        }
        else{
            if(min > tmp_dist){
                min = tmp_dist;
                tour = tmp_tour;
                min_vertex = del_pos;
                // print("up", tour);
            }
        }

        if(i != tmp_vertex.size()){
            tmp = tmp_vertex[i];
            tmp_vertex[i] = del_pos;
            del_pos = tmp;
        }
    }

    tour.push_back(min_vertex);
    // print("Final: ", tour);
    // printf("min: %f\n", min);
    return min;
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

    // for(const auto &i: vertex){ // Print vertex info.
    //     std::cout << i.first << "th city with loc: " << i.second.first << ", " << i.second.second << std::endl;
    // }
    // std::cout << "\n";

    // int n = vertex.size(); // Check total input.
    // std::vector<std::vector<float>> adj_matrix(n, std::vector<float>(n, 0.0000f));

    // // Make adj_matrix. Since n>5, n + m = n + (n - 1)! (due to TSP deals with complete graph) > n^2, adj_matrix is better choice than edge list/adj list.
    // for(size_t i = 0; i < n; i++){
    //     adj_matrix[i][i] = 0;

    //     for(size_t j = i + 1; j < n; j++){
    //         float dist = calc_2d_dist(vertex[i].second, vertex[j].second);
    //         adj_matrix[i][j] = dist;
    //         adj_matrix[j][i] = dist;
    //     }
    // }

    // for(const auto i: adj_matrix){ // Print adj_matrix info.
    //     for(const auto j: i){
    //         std::cout << j << " ";
    //     }
    //     std::cout << "\n";
    // }

    auto file_open_end = std::chrono::high_resolution_clock::now();

    float min_dist, tmp_dist;
    std::vector<int> tour;
    int tmp_vertex = vertex_list[1];
    int min_vertex, tmp;
    vertex_list.erase(vertex_list.begin(), vertex_list.begin() + 2);
    // print("original vertex list", vertex_list);
    tour.push_back(1);
    
    for(size_t i = 0; i <= (size_t)vertex_list.size(); i++){
        // print("vertex_list", vertex_list);
        std::vector<int> tmp_tour;
        tmp_dist = TSP(vertex_list, tmp_tour, vertex, tmp_vertex) + calc_2d_dist(vertex[0].second, vertex[tmp_vertex - 1].second);
        if(i == 0){
            min_dist = tmp_dist;
            min_vertex = tmp_vertex;
            tour = tmp_tour;
            // std::cout << "first dist: " << min_dist << "\n";
        }
        else{
            if(min_dist > tmp_dist){
                min_dist = tmp_dist;
                min_vertex = tmp_vertex;
                tour = tmp_tour;
                // std::cout << "updated: "<< min_dist << "\n";
            }
        }

        if(i != vertex_list.size()){
            tmp = vertex_list[i];
            vertex_list[i] = tmp_vertex;
            tmp_vertex = tmp;
        }
    }

    tour.push_back(min_vertex);
    tour.push_back(1);

    std::cout << "\nPath: \n";
    for(auto i : tour){
        std::cout << i << " ";
    }
    std::cout << "\n";

    printf("Dist: %.4f", min_dist);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    auto TSP_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - file_open_end).count();
    std::cout << "Execution time: " << duration << "(ms)\n";
    std::cout << "TSP algorithm time: " << TSP_duration << "(ms)\n";

    return 0;
}