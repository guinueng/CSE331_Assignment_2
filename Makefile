SANFLAGS = -fsanitize=address
CXXFLAGS += -Wall -O2 -std=c++23 -g $(SANFLAGS)
LDFLAGS += $(SANFLAGS)

TARGETS = Christofides_heuristic Held-Karp_algorithm

all: $(TARGETS)

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
