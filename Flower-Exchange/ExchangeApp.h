#ifndef EXCHANGEAPP_H
#define EXCHANGEAPP_H

#include "Order.h"
#include "ExecutionReport.h"
#include "Book.h"
#include "getCurrentTransactionTime.h"
#include <vector>
#include <unordered_map>
using namespace std;

static const vector<string> validInstruments = {
    "Rose", "Lavender", "Lotus", "Tulip", "Orchid"};

unordered_map<string, vector<Book>> buyBooks;  // Map for buy orders
unordered_map<string, vector<Book>> sellBooks; // Map for sell orders

bool isValidInstrument(const string &instrument)
{
  return find(validInstruments.begin(), validInstruments.end(), instrument) != validInstruments.end();
}

void ExchangeApp(vector<Order> &orders, vector<ExecutionReport> &reports)
{

  // Object for book class
  Book book;

  // Function to process and execute orders to generate Execution Reports

  int orderid = 0;
  int OrderSize = orders.size();
  for (int i = 0; i < OrderSize; i++)
  {
    orderid++;
    ExecutionReport report;
    report.setOrderId("ord" + to_string(orderid));
    report.setClientOrderId(orders[i].ClientOderID);
    report.setInstrument(orders[i].Instrument);
    report.setSide(orders[i].Side);

    // Check for invalid orders
    if (orders[i].Price <= 0 || report.getClientOrderId().size() > 5 ||
        (report.getSide() != 1 && report.getSide() != 2) ||
        orders[i].Quantity < 0 ||
        orders[i].Quantity > 1000 ||
        orders[i].Quantity % 10 != 0 ||
        !isValidInstrument(report.getInstrument()))
    {

      // Handle invalid order
      string reason;
      if (orders[i].Price <= 0)
      {
        reason = "Invalid price";
      }
      else if (report.getClientOrderId().size() > 5)
      {
        reason = "Invalid ID";
      }
      else if (report.getSide() != 1 && report.getSide() != 2)
      {
        reason = "Invalid side";
      }
      else if (!isValidInstrument(report.getInstrument()))
      {
        reason = "Invalid instrument"; // Specific reason for invalid instrument
      }
      else
      {
        reason = "Invalid size";
      }

      report.setPrice(orders[i].Price);
      report.setQuantity(orders[i].Quantity);
      report.setStatus(1); // Reject
      report.setReason(reason);
      report.setTransactionTime(getCurrentTransactionTime());
      reports.push_back(report);
      continue; // Skip to the next order
    }

    else
    {
      // Execute valid orders
      Book flower(report.getOrderId(), orders[i].Quantity, orders[i].Price);

      string flowertype = report.getInstrument();

      // Access or create the appropriate order books using the map
      vector<Book> &buyBook = buyBooks[flowertype];
      vector<Book> &sellBook = sellBooks[flowertype];

      if (report.getSide() == 1)
      {
        book.MakeBook(1, report, reports, flower, buyBook, sellBook);
      }
      else
      {
        book.MakeBook(2, report, reports, flower, sellBook, buyBook);
      }
    }
  }
}

#endif // EXCHANGEAPP_H