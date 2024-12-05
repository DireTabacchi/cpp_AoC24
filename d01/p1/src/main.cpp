#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Enter an input filename.\n";
        return 1;
    }

    std::string filename = argv[1];
    // Read the input
    std::ifstream input_file {filename};
    std::vector<int> list1, list2, distances;

    int loc1, loc2;
    while (input_file >> loc1 >> loc2) {
        list1.push_back(loc1);
        list2.push_back(loc2);
    }
    
    std::sort(list1.begin(), list1.end());
    std::sort(list2.begin(), list2.end());

    for (int i = 0; i < list1.size(); i++) {
        int dist = std::abs(list1[i] - list2[i]);
        distances.push_back(dist);
    }

    int total_dist = 0;
    for (int dist : distances) {
        total_dist += dist;
    }

    std::cout << "" << total_dist << "\n";
}
