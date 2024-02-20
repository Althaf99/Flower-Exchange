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
    string clientOrderId;
    string orderId;
    string instrument;
    int side;
    double price;
    int quantity;
    int status;
    string reason;
    string transactionTime;

    void setClientOrderId(string clientOrderId)
    {
        this->clientOrderId = clientOrderId;
    }

    void setOrderId(string orderId)
    {
        this->orderId = orderId;
    }

    void setInstrument(string instrument)
    {
        this->instrument = instrument;
    }

    void setReason(string reason)
    {
        this->reason = reason;
    }

    void setTransactionTime(string transactionTime)
    {
        this->transactionTime = transactionTime;
    }

    void setSide(int side)
    {
        this->side = side;
    }

    void setQuantity(int quantity)
    {
        this->quantity = quantity;
    }

    void setStatus(int status)
    {
        this->status = status;
    }

    void setPrice(double price)
    {
        this->price = price;
    }

    string getClientOrderId()
    {
        return clientOrderId;
    }

    string getOrderId()
    {
        return orderId;
    }

    string getInstrument()
    {
        return instrument;
    }

    int getSide()
    {
        return side;
    }

    double getPrice()
    {
        return price;
    }

    int getQuantity()
    {
        return quantity;
    }

    int getStatus()
    {
        return status;
    }

    string getReason()
    {
        return reason;
    }

    string getTransactionTime()
    {
        return transactionTime;
    }

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
            file << report.orderId << ","
                 << report.clientOrderId << ","
                 << report.instrument << ","
                 << report.side << ","
                 << report.status << ","
                 << report.quantity << ","
                 << std::fixed << std::setprecision(2) << report.price << ","
                 << report.reason << ","
                 << report.transactionTime << std::endl;
        }

        file.close();
    }
};

#endif // EXECUTIONREPORT_H