#include <memory>
#include <iostream>
#include <stdexcept>
using namespace std;

template<typename T>
class Node
{
public:
    Node();
    Node(T x);
    ~Node();

    T val;
    std::shared_ptr<Node<T> > next;

};

template<typename T>
class ListIterator
{
private:
    std::shared_ptr<Node<T> > now;
public:
    ListIterator();
    ListIterator(std::shared_ptr<Node<T> > node);
    ~ListIterator();

    bool HasNext();

    T& Next();

    ListIterator<T>& operator=(ListIterator<T>& l);
};


template<typename T>
class LinkedList
{
private:
    std::shared_ptr<Node<T> > head;
    int size;
public:
    LinkedList();
    ~LinkedList();

    void Add(T& value);

    void Insert(T& value, int index);

    T& Get(int index);

    int IndexOf(T& value);

    T& Remove(int index);

    ListIterator<T> Iterator();

    int Size();
};

template <typename T>
class DNode : public Node<T>
{
public:
    DNode(){}            //default constructor
    DNode(T x):val(x) {
        next = nullptr;
        prev = nullptr;
    }         //constructor with new value
    virtual ~DNode(){}   //destructor

    T val;
    std::shared_ptr<DNode<T> > next;    //point to the next one
    std::shared_ptr<DNode<T> > prev;    //point to the previous one
};

template <typename T>
class DoublyLinkedList : public LinkedList<T>
{
private:
    std::shared_ptr<DNode<T> > head;           //head pointer
    std::shared_ptr<DNode<T> > tail;           //tail pointer
    int size;                                  //size of the doubly linked list

public:
    DoublyLinkedList() {
        head = std::make_shared<DNode<T> >();
        tail = std::make_shared<DNode<T> >();
        size = 0;
    }
    virtual ~DoublyLinkedList(){}

    // Add the specified element at the end of the list
    void Add(T& value);

    // Add the specified element at the specified index
    void Insert(T& value, int index);

    // Get the element at the specified index
    T& Get(int index);

    // Retrieve the index of the specified element (-1 if it does not exist in the list
    int IndexOf(T& value);

    // Remove the element at the specified index and return it
    T& Remove(int index);

    // Return an iterator on this list
    ListIterator<T> Iterator();

    // Return the size of the list
    int Size();
};

template<typename T>
Node<T>::Node() {
}

template<typename T>
Node<T>::~Node() {
}

template<typename T>
Node<T>::Node(T x) :val(x) {
    next = nullptr;
}

template <typename T>
ListIterator<T>::ListIterator()
{
}

template <typename T>
ListIterator<T>::ListIterator(std::shared_ptr<Node<T> > node) {
    now = node;
}

template <typename T>
ListIterator<T>::~ListIterator()
{
}


template<typename T>
LinkedList<T>::LinkedList()
{
    head = std::make_shared<Node<T> >();
    size = 0;
}

template<typename T>
LinkedList<T>::~LinkedList()
{
}



// Add the specified element at the end of the list
template <typename T>
void LinkedList<T>::Add(T& value) {
    auto curr = head;
    for (int i = 0; i < size; i++)
        curr = curr->next;

    curr->next = std::make_shared<Node<T> >(value);

    ++size;
    return;
}

// Add the specified element at the specified index
template <typename T>
void LinkedList<T>::Insert(T& value, int index) {
    if (index >= size) {
        throw std::out_of_range("Insert index out of range");
        return;
    }

    auto curr = head;
    for (int i = 0; i < index; ++i) {
        curr = curr->next;
    }

    auto rest = curr->next;
    curr->next = std::make_shared<Node<T> >(value);
    curr->next->next = rest;
    ++size;
    return;
}

// Get the element at the specified index
template <typename T>
T& LinkedList<T>::Get(int index) {
    if (index >= size) {
        throw std::out_of_range("Get index out of range");
    }

    auto curr = head;
    for (int i = 0; i < index; ++i) {
        curr = curr->next;
    }

    return curr->next->val;
}

// Retrieve the index of the specified element (-1 if it does not exist in the list
template <typename T>
int LinkedList<T>::IndexOf(T& value) {
    auto curr = head;
    int index = 0;
    while (curr->next) {
        curr = curr->next;
        if (curr->val == value) {
            return index;
        }
        ++index;
    }
    return -1;
}

// Remove the element at the specified index and return it
template <typename T>
T& LinkedList<T>::Remove(int index) {
    if (index >= size) {
        throw std::out_of_range("Remove index out of range");
    }

    auto curr = head;
    for (int i = 0; i < index; ++i) {
        curr = curr->next;
        --index;
    }

    auto temp = curr->next;
    curr->next = curr->next->next;

    --size;
    return temp->val;
}

// Return the size of the list
template <typename T>
int LinkedList<T>::Size() {
    return size;
}


// Return an iterator on this list
template <typename T>
ListIterator<T> LinkedList<T>::Iterator() {
    ListIterator<T> res(head);
    return res;
}

// Return whether there is another element to return in this iterator
template <typename T>
bool ListIterator<T>::HasNext() {
    if (now->next != nullptr)
        return true;
    else
        return false;
}

// Return the next element in this iterator
template <typename T>
T& ListIterator<T>::Next() {
    now = now->next;
    return now->val;
}


template<typename T>
ListIterator<T>& ListIterator<T>::operator=(ListIterator<T>& l) {
    now = l.now;
    return *this;
}

// Add the specified element at the end of the list
template <typename T>
void DoublyLinkedList<T>::Add(T& value) {
    auto curr = head;
    //update curr to the end
    for (int i = 0; i < size; i++)
        curr = curr->next;

    //update the linked list to the new one
    curr->next = std::make_shared<DNode<T> >(value);
    tail = curr->next;
    tail->prev = curr;

    //increase the size by one
    ++size;
    return;
}


// Add the specified element at the specified index
template <typename T>
void DoublyLinkedList<T>::Insert(T& value, int index) {
    //If the index is out of range, throw an error
    if (index >= size || index < 0) {
        throw std::out_of_range("Insert index out of range");
        return;
    }

    std::shared_ptr<DNode<T> > curr = head;

    //find the specified index in the list
    if (index < size / 2) {//start from the head
        for (int i = 0; i < index; ++i) {
            curr = curr->next;
        }
    }
    else {//start from the end
        curr = tail;
        for (int i = size - 1; i >= index; --i) {
            curr = curr->prev;
        }
    }

    //update the list
    std::shared_ptr<DNode<T> > rest = curr->next;
    curr->next = std::make_shared<DNode<T> >(value);
    curr->next->next = rest;
    rest->prev = curr->next;
    rest->prev->prev = curr;

    //increase the size
    ++size;
    return;
}


// Get the element at the specified index
template <typename T>
T& DoublyLinkedList<T>::Get(int index) {
    if (index >= size || index < 0)
        //if the index is out of range
        throw std::out_of_range("Get index out of range");
    else if (index == 0)
        //return the first element
        return head->next->val;
    else if (index == size - 1)
        //return the tail element
        return tail->val;

    std::shared_ptr<DNode<T> > curr = head;
    //find the specified index in the list
    if (index < size / 2) {//start from the head
        for (int i = 0; i < index; ++i) {
            curr = curr->next;
        }
    }
    else {//start from the tail
        curr = tail;
        for (int i = size - 1; i >= index; --i) {
            curr = curr->prev;
        }
    }

    return curr->next->val;
}


// Retrieve the index of the specified element (-1 if it does not exist in the list
template <typename T>
int DoublyLinkedList<T>::IndexOf(T& value) {
    auto curr = head;
    int index = 0;

    while (curr->next != nullptr) {
        curr = curr->next;
        if (curr->val == value) {
            return index;
        }
        ++index;
    }

    return -1;
}


// Remove the element at the specified index and return it
template <typename T>
T& DoublyLinkedList<T>::Remove(int index) {
    if (index >= size || index < 0) {
        throw std::out_of_range("Remove index out of range");
    }

    std::shared_ptr<DNode<T> > curr = head;
    //find the specified index in the list
    if (index < size / 2) {//start from the head
        for (int i = 0; i < index; ++i) {
            curr = curr->next;
        }
    }
    else {//start from the end
        curr = tail;
        for (int i = size - 1; i >= index; --i) {
            curr = curr->prev;
        }
    }

    //the value which we want to remove
    auto temp = curr->next;

    //update the linked list
    if (curr->next->next == nullptr)
        tail = curr;
    else {
        curr->next = curr->next->next;
        curr->next->prev = curr;
    }

    //decrese the size by 1
    --size;

    //return the removed value
    return temp->val;
}

// Return the size of the list
template <typename T>
int DoublyLinkedList<T>::Size() {
    return size;
}


// Return an iterator on this list
template <typename T>
ListIterator<T> DoublyLinkedList<T>::Iterator() {
    ListIterator<T> res(head);
    return res;
}

template<typename T>
void print(T l) {
    cout << "print: ";
    auto it = l.Iterator(); int i = 0;
    while (it.HasNext() and i++ < l.Size()) {
        cout << it.Next() << " - ";
    }
    cout << endl;
    return;
}


int main(int argc, char** argv)
{
    DoublyLinkedList<int> l;
    int a = 1, b = 2, c = 3, d = 4;

    cout << "Add 1, 2, 4: ";
    l.Add(a); l.Add(b); l.Add(d);
    cout << endl;
    print<DoublyLinkedList<int> >(l);

    cout << "Get index 0: " << l.Get(0) << endl;
    cout << "Get index 1: " << l.Get(1) << endl;
    cout << "Get index 2: " << l.Get(2) << endl;

    cout << "Index of 1: " << l.IndexOf(a) << endl;
    cout << "Index of 2: " << l.IndexOf(b) << endl;
    cout << "Index of 4: " << l.IndexOf(d) << endl;

    cout << endl;
    c = 33;
    cout << "Insert 33 at index 2: ";
    l.Insert(c, 2); print(l);
    cout << "Insert 33 at index 0: ";
    l.Insert(c, 0); print(l);

    cout << "Get index 0: " << l.Get(0) << endl;
    cout << "Get index 1: " << l.Get(1) << endl;
    cout << "Get index 2: " << l.Get(2) << endl;
    cout << "Get index 3: " << l.Get(3) << endl;
    cout << "Get index 4: " << l.Get(4) << endl;

    cout << endl;
    cout << "Remove the first element: " << l.Remove(0) << endl;
    print(l);
    cout << "Remove the last element: " << l.Remove(3) << endl;
    print(l);

    cout << endl;
    cout << "Get index 0: " << l.Get(0) << endl;
    cout << "Get index 1: " << l.Get(1) << endl;
    cout << "Get index 2: " << l.Get(2) << endl;

    cout << "Index of 1: " << l.IndexOf(a) << endl;
    cout << "Index of 2: " << l.IndexOf(b) << endl;
    cout << "Index of 33: " << l.IndexOf(c) << endl;


    return 0;
}