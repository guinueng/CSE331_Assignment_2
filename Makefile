#SANFLAGS = -fsanitize=address
CXXFLAGS += -Wall -O2 -std=c++23 -g $(SANFLAGS)
LDFLAGS += $(SANFLAGS)

TARGETS = Christofides_heuristic chris_auto_eval #auto_eval #Held-Karp_algorithm

all: $(TARGETS)

auto_eval: auto_eval.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

auto_eval.o: auto_eval.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

chris_auto_eval: chris_auto_eval.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

chris_auto_eval.o: chris_auto_eval.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

Christofides_heuristic: Christofides_heuristic.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

Christofides_heuristic.o: Christofides_heuristic.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

Held-Karp_algorithm: Held-Karp_algorithm.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

Held-Karp_algorithm.o: Held-Karp_algorithm.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(TARGETS) *.o
