#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

class Report {
    std::vector<int> levels;

public:
    Report(std::vector<int> levels) :levels{levels}
    {}

    bool isSafe()
    {
        int last_level = levels[0];
        bool increasing = levels[0] < levels[1];
        for (int i = 1; i < this->levels.size(); i++) {
            if ((increasing && last_level > levels[i]) || (!increasing && last_level < levels[i])) {
                return false;
            }
            int level_diff = std::abs(levels[i] - last_level);
            if (level_diff < 1 || level_diff > 3) {
                return false;
            }

            last_level = levels[i];
        }

        return true;
    }
};

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Enter an input filename.\n";
        return 1;
    } else if (argc > 2) {
        std::cerr << "Too many arguments.\n";
    }

    std::string filename = argv[1];
    std::ifstream input_file {filename};

    std::vector<Report> reports{};
    std::string report_src {};
    while (getline(input_file, report_src, '\n')) {
        std::stringstream src {report_src};
        std::vector<int> report_levels {};
        int level;
        while (src >> level) {
            report_levels.push_back(level);
        }
        Report report {report_levels};
        reports.push_back(report);
    }

    int safe_report_count = 0;
    for (Report report : reports) {
        if (report.isSafe())
            safe_report_count++;
    }

    std::cout << "There are " << safe_report_count << " safe reports out of " << reports.size()
              << " reports.\n";
}
