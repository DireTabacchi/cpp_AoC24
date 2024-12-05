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
    std::vector<int> list1, list2, similarity_scores;

    int loc1, loc2;
    while (input_file >> loc1 >> loc2) {
        list1.push_back(loc1);
        list2.push_back(loc2);
    }
    
    std::sort(list1.begin(), list1.end());
    std::sort(list2.begin(), list2.end());

    // compute similarity scores
    for (int ida : list1) {
        int id_sim_score = 0;
        for (int idb : list2) {
            if (ida == idb) {
                id_sim_score++;
            }
        }

        id_sim_score *= ida;
        similarity_scores.push_back(id_sim_score);
    }

    int total_score = 0;
    for (int score : similarity_scores) {
        total_score += score;
    }
    std::cout << "Similarity score of lists: " << total_score << "\n";
}
