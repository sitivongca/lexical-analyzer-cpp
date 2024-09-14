#include "Queue.h"


Item::Item(std::string value)
{
	itemValue = value;
}
Item::Item(std::string value, Item* last)
{
	nextItem = last;
	itemValue = value;
}


Queue::Queue()
{
	Item* tail = nullptr;
	Item* head = nullptr;
	count = 0;
}

Queue::~Queue()
{
	while (count != 0)
	{
		remove();
	}
}

bool Queue::isEmpty()
{
	if (count == 0)
	{
		return true;
	}
	else
	{
		return false;

	}
}


void Queue::add(std::string data)
{
	if (count != 0)
	{
		this->tail = new Item(data, this->tail);
	}

	if (count == 0)
	{
		this->tail = new Item(data);
		this->head = this->tail;
	}
	++count;

}

void Queue::remove()
{
	int counter = count;
	if (count <= 0)
	{
		std::cout << "\nCan't remove anymore.\n\n";
		count = 0;
		return;
	}
	else if (count == 1)
	{
		Item* temp = this->head;
		head = nullptr;
		tail = nullptr;
		delete temp;

	}
	else if (count == 2)
	{
		Item* temp = this->head;
		head = tail;
		delete temp;
	}
	else
	{
		Item* temp = this->head;
		Item* tempTail = this->tail;
		counter = count - 2;
		while (counter > 0)
		{
			tempTail = tempTail->nextItem;
			--counter;
		}
		head = tempTail;
		delete temp;
		tempTail = nullptr;
		delete tempTail;
	}
	--count;

}

std::string Queue::peek()
{
	if (isEmpty() == true)
	{
		return "\n(The queue is empty.)";
	}
	else
	{
		std::cout << "Queue count " << count << "\n";
		return head->itemValue;
	}
}

std::ostream& operator<< (std::ostream& out, const Queue& s)
{

	std::cout << std::setw(12) << "Queue";
	std::cout << "\n -------------------\n";
	std::cout << std::setw(4) << s.head->itemValue << " ";

	if (s.count == 2)
	{
		std::cout << s.tail->itemValue << " ";
	}
	else if (s.count != 1)
	{
		Item* tempTail = nullptr;
		int counter = s.count - 1;
		int oCounter = counter;
		while (oCounter > 0)
		{
			tempTail = s.tail;
			counter = oCounter - 1;
			while (counter > 0)
			{
				tempTail = tempTail->nextItem;
				--counter;
			}
			std::cout << tempTail->itemValue << " ";
			--oCounter;

		}


		tempTail = nullptr;
		delete tempTail;

	}

	return out;
}
