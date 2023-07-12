#pragma once
#include <string>
#include <vector>

struct CashoutRecord {
    std::string username;
    int winnings;
    std::string date;
};

std::vector<CashoutRecord> readCashoutRecords(const std::string& filename);

void writeCashoutRecords(const std::string& filename, const std::vector<CashoutRecord>& records);

