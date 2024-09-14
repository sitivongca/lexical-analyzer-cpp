#pragma once

#include <iostream>
#include <iomanip>


class Item
{
public:
    std::string itemValue;
    Item* nextItem;
    Item(std::string value);
    Item(std::string value, Item* last);

};

class Queue
{
public:
    int count;
    Item* tail;
    Item* head;
    Queue();

    ~Queue();

    bool isEmpty();

    void add(std::string data);

    void remove();

    std::string peek();

};


std::ostream& operator<< (std::ostream& out, const Queue& s);
