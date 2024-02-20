#ifndef BOOK_H
#define BOOK_H

#include <string>
#include "getCurrentTransactionTime.h"
#include <algorithm>
using namespace std;

class Book
{

private:
    string orderID;
    int quantity;
    double price;

    void setOrderId(string orderID)
    {
        this->orderID = orderID;
    }
    string getOrderId()
    {
        return orderID;
    }
    void setQuantity(int quantity)
    {
        this->quantity = quantity;
    }
    int getQuantity()
    {
        return quantity;
    }

    void setPrice(double price)
    {
        this->price = price;
    }

    double getPrice()
    {
        return price;
    }

public:
    Book()
    {
    }

    Book(string orderID, int quantity, double price)
    {
        this->orderID = orderID;
        this->quantity = quantity;
        this->price = price;
    }

    void AddToBook(vector<Book> &BookToAdd, Book flower)
    {
        // Function to add order to the relevant side of the book in the relevant order according to the price
        int n = BookToAdd.size();
        if (n == 0)
        {
            // If the relevent side of the book is empty, just add the order
            BookToAdd.push_back(flower);
        }
        else
        {
            // If the relevant side of the book is not empty, find the correct position to insert the order using Binary Search
            int left = 0;
            int right = n - 1;
            int insertionPoint = n;
            int mid = left + (right - left) / 2;
            while (left <= right)
            {
                int mid = left + (right - left) / 2;
                if (BookToAdd[mid].getPrice() >= flower.getPrice())
                {
                    right = mid - 1;
                }
                else
                {
                    left = mid + 1;
                }
            }
            insertionPoint = mid;
            BookToAdd.insert(BookToAdd.begin() + insertionPoint, flower);
        }
    }

    void MakeBook(int side, ExecutionReport &report, vector<ExecutionReport> &reports, Book flower, vector<Book> &SideToAdd, vector<Book> &OtherSide)
    {
        // Object of Book Class
        Book book;

        // Function to make book for each flower type
        if (OtherSide.empty())
        {
            // If the other side is empty, add the order to the book directly
            book.AddToBook(SideToAdd, flower);
            report.setStatus(0); // New
            report.setQuantity(flower.getQuantity());
            report.setPrice(flower.getPrice());
            report.setTransactionTime(getCurrentTransactionTime());
            reports.push_back(report);
            return;
        }

        // Sort OtherSide by price (ascending for buys, descending for sells) and then by timestamp (ascending)
        if (side == 1)
        { // Buy
            std::sort(OtherSide.begin(), OtherSide.end(),
                      [](Book a, Book b)
                      {
                          ExecutionReport aReport, bReport; // Temporary reports for timestamp access
                          // Retrieve timestamps for a and b using OrderID
                          if (a.getPrice() != b.getPrice())
                              return a.getPrice() < b.getPrice();
                          return aReport.getTransactionTime() < bReport.getTransactionTime();
                      });
        }
        else
        { // Sell
            std::sort(OtherSide.begin(), OtherSide.end(),
                      [](Book a, Book &b)
                      {
                          ExecutionReport aReport, bReport; // Temporary reports for timestamp access
                          // Retrieve timestamps for a and b using OrderID

                          if (a.getPrice() != b.getPrice())
                              return a.getPrice() > b.getPrice();                             // Prioritize higher prices for sells
                          return aReport.getTransactionTime() < bReport.getTransactionTime(); // Break ties with timestamp
                      });
        }

        // If the other side is not empty, check whether the order can be filled
        bool filled = false;
        int limit = OtherSide.size();
        int j = 0;
        while (j < limit)
        {
            if ((OtherSide[j].getPrice() > flower.getPrice() && side == 1) || (OtherSide[j].getPrice() < flower.getPrice() && side == 2))
            {
                break;
            }
            // If the order can be filled, add the order to the book and generate execution reports
            filled = true;
            if (OtherSide[j].getQuantity() >= flower.getQuantity())
            {
                // If the order can be completly filled, generate execution reports without adding to the book
                report.setPrice(OtherSide[j].getPrice());
                report.setQuantity(flower.getQuantity());
                report.setStatus(2); // fill
                report.setTransactionTime(getCurrentTransactionTime());
                reports.push_back(report);
                ExecutionReport updated;
                for (int k = reports.size() - 1; k >= 0; k--)
                {
                    if (reports[k].getOrderId() == OtherSide[j].getOrderId())
                    {
                        updated = reports[k];
                        break;
                    }
                }

                // update the status of the filled or partially filled order in the book
                updated.setStatus((OtherSide[j].getQuantity() == flower.getQuantity()) ? 2 : 3);
                int temp = OtherSide[j].getQuantity();
                updated.setQuantity(flower.getQuantity());
                updated.setPrice(OtherSide[j].getPrice());
                updated.setTransactionTime(getCurrentTransactionTime());
                reports.push_back(updated);
                if (temp - flower.getQuantity() == 0)
                {
                    // If the order is completely filled, remove the order from the book
                    OtherSide.erase(OtherSide.begin() + j);
                }
                else
                {
                    // If the order is partially filled, update the quantity of the order in the book
                    OtherSide[j].setQuantity(OtherSide[j].getQuantity() - flower.getQuantity());
                }
                break;
            }

            // If the order is partially filled, calculate the remaining order to be entered to the book and generate execution reports
            report.setPrice(OtherSide[j].getPrice());
            report.setQuantity(OtherSide[j].getQuantity());
            report.setStatus(3); // partially filled
            report.setTransactionTime(getCurrentTransactionTime());
            reports.push_back(report);
            ExecutionReport updated;
            for (int k = reports.size() - 1; k >= 0; k--)
            {
                if (reports[k].getOrderId() == OtherSide[j].getOrderId())
                {
                    updated = reports[k];
                    break;
                }
            };
            // update the status of the completly filled  order in the book
            updated.setStatus(2);
            updated.setTransactionTime(getCurrentTransactionTime());
            reports.push_back(updated);
            // update the quantity of the partially filled order
            flower.setQuantity(flower.getQuantity() - OtherSide[j].getQuantity());
            OtherSide.erase(OtherSide.begin() + j);
            limit -= 1;
        }
        if (filled == false)
        {
            // If the order cannot be filled, add the order to the book directly
            report.setStatus(0); // New
            report.setQuantity(flower.getQuantity());
            report.setPrice(flower.getPrice());
            report.setTransactionTime(getCurrentTransactionTime());
            reports.push_back(report);
        }
        if (flower.getQuantity() > 0)
        {
            // If the order is partially filled, add the remaining order to the book directly
            book.AddToBook(SideToAdd, flower);
        }
    }
};

#endif // BOOK_H
