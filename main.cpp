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


template<typename T>
Node<T>::Node(){
}

template<typename T>
Node<T>::~Node(){
}

template<typename T>
Node<T>::Node(T x):val(x){
    next=nullptr;
}

template <typename T>
ListIterator<T>::ListIterator()
{
}

template <typename T>
ListIterator<T>::ListIterator(std::shared_ptr<Node<T> > node){
    now=node;
}

template <typename T>
ListIterator<T>::~ListIterator()
{
}


template<typename T>
LinkedList<T>::LinkedList()
{
    head=std::make_shared<Node<T> >();
    size=0;
}

template<typename T>
LinkedList<T>::~LinkedList()
{
}



// Add the specified element at the end of the list
template <typename T>
void LinkedList<T>::Add(T& value){
    auto curr=head;
    for(int i=0;i<size;i++)
        curr=curr->next;

    curr->next=std::make_shared<Node<T> >(value);

    ++size;
    return;
}

// Add the specified element at the specified index
template <typename T>
void LinkedList<T>::Insert(T& value, int index){
    if(index>=size){
        throw std::out_of_range("Insert index out of range");
        return;
    }

    auto curr=head;
    for(int i=0;i<index;++i){
        curr=curr->next;
    }

    auto rest=curr->next;
    curr->next=std::make_shared<Node<T> >(value);
    curr->next->next=rest;
    ++size;
    return;
}

// Get the element at the specified index
template <typename T>
T& LinkedList<T>::Get(int index){
    if(index>=size){
        throw std::out_of_range("Get index out of range");
    }

    auto curr=head;
    for(int i=0;i<index;++i){
        curr=curr->next;
    }

    return curr->next->val;
}

// Retrieve the index of the specified element (-1 if it does not exist in the list
template <typename T>
int LinkedList<T>::IndexOf(T& value){
    auto curr=head;
    int index=0;
    while(curr->next){
        curr=curr->next;
        if(curr->val==value){
            return index;
        }
        ++index;
    }
    return -1;
}

// Remove the element at the specified index and return it
template <typename T>
T& LinkedList<T>::Remove(int index){
    if(index>=size){
        throw std::out_of_range("Remove index out of range");
    }

    auto curr=head;
    for(int i=0;i<index;++i){
        curr=curr->next;
        --index;
    }

    auto temp=curr->next;
    curr->next=curr->next->next;

    --size;
    return temp->val;
}

// Return the size of the list
template <typename T>
int LinkedList<T>::Size(){
    return size;
}


// Return an iterator on this list
template <typename T>
ListIterator<T> LinkedList<T>::Iterator(){
    ListIterator<T> res(head);
    return res;
}

// Return whether there is another element to return in this iterator
template <typename T>
bool ListIterator<T>::HasNext(){
    if(now->next!=nullptr)
        return true;
    else
        return false;
}

// Return the next element in this iterator
template <typename T>
T& ListIterator<T>::Next(){
    now=now->next;
    return now->val;
}


template<typename T>
ListIterator<T>& ListIterator<T>::operator=(ListIterator<T>& l){
    now=l.now;
    return *this;
}

int main() {
}
