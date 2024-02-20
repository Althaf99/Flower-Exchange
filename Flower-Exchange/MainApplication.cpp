#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Order.h"
#include "ExecutionReport.h"
#include "ExchangeApp.h"
using namespace std;

int main()
{
    // Read orders from CSV file
    string inputFilename;
    cout << "Enter the input filename (csv):  ";
    cin >> inputFilename;
    inputFilename += ".csv";

    // Ensure that the rest of the input stream is empty
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Object for Order class
    Order order;
    vector<Order> orders = order.ReadOrder(inputFilename);

    // Process and execute orders to generate Execution Reports
    vector<ExecutionReport> reports;
    ExchangeApp(orders, reports);

    // Get output filename
    string outputFilename;
    cout << "Enter the output filename (csv): ";
    cin >> outputFilename;
    outputFilename += ".csv"; // Add the ".csv" extension

    // Write Execution Reports to the output CSV file

    // Object for ExecutionReport class
    ExecutionReport executionReport;
    executionReport.Output(reports, outputFilename);
    cout << "Execution reports written to " << outputFilename << endl;

    return 0;
}