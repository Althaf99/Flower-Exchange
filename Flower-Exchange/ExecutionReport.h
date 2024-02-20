#ifndef EXECUTIONREPORT_H
#define EXECUTIONREPORT_H

#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;

// Execution Report structure
class ExecutionReport
{

public:
    string ClientOderID;
    string OrderID;
    string Instrument;
    int Side;
    double Price;
    int Quantity;
    int Status;
    string Reason;
    string TransactionTime;

    void Output(const vector<ExecutionReport> &reports, const string &filename)
    {
        // Function to write execution reports to CSV file
        ofstream file(filename);
        if (!file.is_open())
        {
            cout << "Failed to open file " << filename << std::endl;
            return;
        }

        // Write the CSV header
        file << "OrderID,ClientOrderID,Instrument,Side,Status,Quantity,Price,Reason,TransitionTime" << std::endl;

        // Write each ExecutionReport to the file as a new line in CSV format
        for (const auto &report : reports)
        {
            file << report.OrderID << ","
                 << report.ClientOderID << ","
                 << report.Instrument << ","
                 << report.Side << ","
                 << report.Status << ","
                 << report.Quantity << ","
                 << std::fixed << std::setprecision(2) << report.Price << ","
                 << report.Reason << ","
                 << report.TransactionTime << std::endl;
        }

        file.close();
    }
};

#endif // EXECUTIONREPORT_H