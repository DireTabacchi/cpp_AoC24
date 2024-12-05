#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>

class Report {
    std::vector<int> levels;

public:
    Report(std::vector<int> levels) :levels{levels}
    {}

    // Determine if this report is safe.
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

    // Check if one level of unsafeness can be tolerated in this report.
    bool tolerate()
    {
        for (int i = 0; i < levels.size(); i++) {
            std::vector<int> modified_levels = levels;
            modified_levels.erase(modified_levels.begin() + i);
            Report modified_report {modified_levels};
            if (modified_report.isSafe()) return true;
        }

        return false;
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

    auto start {std::chrono::steady_clock::now()};
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
        bool is_safe = report.isSafe();
        if (!is_safe)
            is_safe = report.tolerate();

        if (is_safe)
            safe_report_count++;
    }
    auto end {std::chrono::steady_clock::now()};

    std::chrono::duration<double> elapsed_time {end - start};

    std::cout << "There are " << safe_report_count << " safe reports out of " << reports.size()
              << " reports.\n";
    std::cout << "Elapsed analysis time: " << elapsed_time.count() << "s\n";
}
