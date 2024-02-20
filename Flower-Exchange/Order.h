#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>
#include <sstream>
using namespace std;

// Order structure
class Order
{

public:
    string ClientOderID;
    string Instrument;
    int Side;
    double Price;
    int Quantity;

    vector<Order> ReadOrder(const string &filename)
    {
        // Function to read orders from CSV file
        std::ifstream file;
        file.open(filename);

        if (!file.is_open())
        {
            cout << "File " << filename << " not found." << endl;
            exit(-1);
        }

        vector<Order> orders;
        // Skip the first line (header)
        string header;
        getline(file, header);

        // Read each line from the file and store in the vector of orders
        string line;
        while (getline(file, line))
        {

            Order order;
            istringstream iss(line); // parse the line into separate fields based on commas.
            string clientId, instrument, side, price, quantity;

            getline(iss, clientId, ',');
            getline(iss, instrument, ',');
            getline(iss, side, ',');
            getline(iss, quantity, ',');
            getline(iss, price, ',');

            order.ClientOderID = clientId;
            order.Instrument = instrument;
            order.Side = stoi(side);
            order.Price = stod(price);
            order.Quantity = stoi(quantity);
            orders.push_back(order);
        }

        file.close();
        return orders;
    }
};

#endif // ORDER_H