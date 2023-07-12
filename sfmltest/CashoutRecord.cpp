#include "CashoutRecord.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <regex>

std::vector<CashoutRecord> readCashoutRecords(const std::string& filename) {
    std::ifstream inFile(filename);
    std::vector<CashoutRecord> records;
    std::string line;

    std::regex cashoutRegex(R"(Gracz: (\S+) --- Wygrana: (\d+)\$ --- Data: (\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}))");
    std::smatch matchResults;

    while (std::getline(inFile, line)) {
        if (std::regex_search(line, matchResults, cashoutRegex)) {
            std::string username = matchResults[1];
            int winnings = std::stoi(matchResults[2]);
            std::string date = matchResults[3];

            records.push_back({ username, winnings, date });
        }
    }
    return records;
}


void writeCashoutRecords(const std::string& filename, const std::vector<CashoutRecord>& records) {
    std::ofstream outFile(filename);
    int rank = 1;
    for (const auto& record : records) {
        outFile << rank++ << ". " << "Gracz: " << record.username << " --- Wygrana: " << record.winnings << "$ --- Data: " << record.date << std::endl;
    }
}