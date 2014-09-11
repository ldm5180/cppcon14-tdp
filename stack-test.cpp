#include <deque>
#include <iostream>
#include <list>
#include <type_traits>
#include <vector>
#include <cxxabi.h>
#include <sys/time.h>

double curTime() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec * 1e-6;
}

template <typename Container> void tester(int iterations) {
  srandom(1337); // Seed PRNG to a known value for reproducibility.
  Container nodesToProcess(1); // Start with the first node to process.
  for (auto i = 0; i < iterations; ++i) {
    // Nodes are provided. Isolate and commonize generation.
    auto rand = random() % 128; // Determine number of nodes at this level.
    std::vector<typename Container::value_type> nodes(rand);

    // Process a node.
    if (!nodesToProcess.empty()) {
      nodesToProcess.pop_back();
    }
    // Queue up all the children nodes to process.
    std::for_each(nodes.rbegin(), nodes.rend(),
                  [&nodesToProcess](typename Container::value_type &v) {
      nodesToProcess.push_back(v);
    });
  }

  // All nodes which have children have been processed. Process all the nodes
  // which remain.
  std::for_each(nodesToProcess.rbegin(), nodesToProcess.rend(),
                [&nodesToProcess](typename Container::value_type &v) {});
}

template <typename Container> void timed_tester(const std::vector<int> &il) {
  for (auto i : il) {
    auto start = curTime();
    tester<Container>(i);
    auto stop = curTime();
    auto tn(abi::__cxa_demangle(typeid(Container).name(), 0, 0, nullptr));
    std::cout << tn << " : Iterations = " << i << " : " << stop - start
              << std::endl;
  }
}

int main() {
  std::vector<int> il = {200000, 2000000, 20000000};
  timed_tester<std::vector<int>>(il);
  timed_tester<std::deque<int>>(il);

  il.pop_back(); // Too much std::list hurts.
  timed_tester<std::list<int>>(il);
  return 0;
}
