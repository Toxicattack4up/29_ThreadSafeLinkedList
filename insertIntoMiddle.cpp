#include <iostream>
#include <mutex>


struct Node
{
  int value;
  Node* next;
  std::mutex* node_mutex;
};

class FineGrainedQueue
{
public:
  Node* head;
  std::mutex* queue_mutex;

public:
    FineGrainedQueue() {
        head = nullptr;
        queue_mutex = new std::mutex();
    }

    ~FineGrainedQueue() {
        std::lock_guard<std::mutex> lock(*queue_mutex);
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current->node_mutex;
            delete current;
            current = next;
        }
        delete queue_mutex;
    }

    void insertIntoMiddle(int value, int pos) {
        Node* newNode = new Node();
        newNode->value = value;
        newNode->next = nullptr;
        newNode->node_mutex = new std::mutex();

        std::lock_guard<std::mutex> lock(*queue_mutex); 
        
        Node* current = head;
        if (head == nullptr) return;
        
        current->node_mutex->lock();
        
        for(int i = 0; i < pos - 1; i++) 
        {
            if (current->next == nullptr) break;

            current->next->node_mutex->lock();
            current->node_mutex->unlock();
            current = current->next;
        }

        if (current->next != nullptr) current->next->node_mutex->lock();

        newNode->next = current->next;
        current->next = newNode;

        if(newNode->next != nullptr) newNode->next->node_mutex->unlock();

        current->node_mutex->unlock();
    }

    void printList() {
    Node* temp = head;
    while (temp != nullptr) {
        std::cout << temp->value << " ";
        temp = temp->next;
    }
    std::cout << std::endl;
}
};


int main() {
    FineGrainedQueue queue;
   
    queue.head = new Node();
    queue.head->value = 5;
    queue.head->next = nullptr;
    queue.head->node_mutex = new std::mutex();

    queue.printList();
    queue.insertIntoMiddle(10, 0);
    queue.printList();
    queue.insertIntoMiddle(20, 2);
    queue.printList();
    queue.insertIntoMiddle(15, 3);
    queue.printList();
    queue.insertIntoMiddle(32, 1);
    queue.printList();
    queue.insertIntoMiddle(22, 3);
    queue.printList();
    queue.insertIntoMiddle(11, 10);
    queue.printList();
    queue.insertIntoMiddle(22, 10);
    queue.printList();
    queue.insertIntoMiddle(44, 55);
    queue.printList();
    queue.insertIntoMiddle(55, 50);
    queue.printList();
    queue.insertIntoMiddle(66, 100);
    queue.printList();



    return 0;
}