#SANFLAGS = -fsanitize=address
CXXFLAGS += -Wall -O2 -std=c++23 -g $(SANFLAGS)
LDFLAGS += $(SANFLAGS)

TARGETS = k-means_mod k-means min_weight_loop_w_insertion brute_force 2_Approximation_Algorithm Christofides_heuristic total_eval Held-Karp_algorithm

all: $(TARGETS)

total_eval: total_eval.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

total_eval.o: total_eval.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

2_Approximation_Algorithm: 2_Approximation_Algorithm.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

2_Approximation_Algorithm.o: 2_Approximation_Algorithm.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

Christofides_heuristic: Christofides_heuristic.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

Christofides_heuristic.o: Christofides_heuristic.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

Held-Karp_algorithm: Held-Karp_algorithm.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

Held-Karp_algorithm.o: Held-Karp_algorithm.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

min_weight_loop_w_insertion: min_weight_loop_w_insertion.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

min_weight_loop_w_insertion.o: min_weight_loop_w_insertion.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

brute_force: brute_force.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

brute_force.o: brute_force.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

k-means: k-means.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

k-means.o: k-means.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

k-means_mod: k-means_mod.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

k-means_mod.o: k-means_mod.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(TARGETS) *.o
