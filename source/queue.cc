// Queue is a simple queue implemented as a singled-linked list.
//
// The element type must support copy constructor.
template <typename E> 
// defines a template class  QueueNode which 
// represents a node in a queue data structure.
//  The template parameter E represents the element type of the queue.
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
        const E& element() const {return element_;}
        // Gets the next node in the queue
        QueueNode* next(){return next_;}
        const QueueNode* next(){return next_;}
    
}