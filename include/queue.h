#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>
// Queue is a simple queue implemented as a singled-linked list.
//
// The element type must support copy constructor.
template <typename E> 
class Queue;
// defines a template class  QueueNode which  represents a node in a queue data structure.
//  The template parameter E represents the element type of the queue.
template <typename E>  // E is the element type
class QueueNode{
    friend class Queue<E>; 
//  declares that the entire Queue class is a friend of QueueNode.

    private: 
        explicit QueueNode(const E& an_element)
            :element_(an_element), next_(nullptr){}
        const QueueNode& operator=(const QueueNode&);
        QueueNode(const QueueNode&)

        // two private member variables: E element_ which holds the element of the queue, 
        // and a pointer QueueNode* next_ which points to the next node in the queue.
        E element_;
        QueueNode* next_;
    public:
         // constructor Queue create a new node with the given element,
         // initializes the next_ptr s nullptr
        const E& element() const {return element_;}
        // Gets the next node in the queue
        QueueNode* next(){return next_;}
        const QueueNode* next(){return next_;}
};
template <typename E>  // E is the element type.
class Queue {
    public:
        // Creates an empty queue.
        Queue():head_(nullptr), last_(nullptr),size_(0){}

        // D'tor.  Clears the queue.
        ~Queue(){Clear();}

        void Clear(){
            if(size_>0){
                // 1. Deletes every node.
                QueueNode<E>* node = head_;
                QueueNode<E>* next = node->next();
                for (;;){
                    delete node;
                    node = next;
                    if (node == nullptr) break;
                    next = node -> next;
                }
                head = last =nullptr;
                size_=0;
            }
        }
        size_t Size() const 

};
#endif