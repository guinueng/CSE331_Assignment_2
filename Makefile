#SANFLAGS = -fsanitize=address
CXXFLAGS += -Wall -O2 -std=c++23 -g $(SANFLAGS)
LDFLAGS += $(SANFLAGS)

TARGETS = min_weight_loop_w_insertion #2_Approximation_Algorithm 2_apprx_eval Christofides_heuristic chris_auto_eval #auto_eval #Held-Karp_algorithm

all: $(TARGETS)

auto_eval: auto_eval.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

auto_eval.o: auto_eval.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

chris_auto_eval: chris_auto_eval.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

chris_auto_eval.o: chris_auto_eval.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

2_Approximation_Algorithm: 2_Approximation_Algorithm.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

2_Approximation_Algorithm.o: 2_Approximation_Algorithm.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

2_apprx_eval: 2_apprx_eval.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

2_apprx_eval.o: 2_apprx_eval.cc
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

.PHONY: clean
clean:
	rm -f $(TARGETS) *.o
