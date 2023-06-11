#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "priorityqueue.h"

TEST_CASE("Default constructor creates empty queue") {
    priorityqueue<int> q;
    REQUIRE(q.Size() == 0);
}

TEST_CASE("Copy constructor copies the queue") {
    priorityqueue<int> q1;
    q1.enqueue(2, 10);
    q1.enqueue(4, 5);
    priorityqueue<int> q2(q1);
    REQUIRE(q1 == q2);
}

TEST_CASE("Find node returns correct node") {
    priorityqueue<int> q;
    q.enqueue(2, 10);
    q.enqueue(4, 5);
    q.enqueue(6, 15);
    q.enqueue(8, 5);
    auto node = q.findNode(6);
    REQUIRE(node->value == 2);
}

TEST_CASE("Clear empties the queue") {
    priorityqueue<int> q;
    q.enqueue(2, 10);
    q.enqueue(4, 5);
    q.clear();
    REQUIRE(q.Size() == 0);
}

TEST_CASE("Enqueue adds elements to the queue") {
    priorityqueue<int> q;
    q.enqueue(2, 10);
    q.enqueue(4, 5);
    REQUIRE(q.Size() == 2);
}

TEST_CASE("Dequeue removes and returns the highest priority element") {
    priorityqueue<int> q;
    q.enqueue(2, 10);
    q.enqueue(4, 5);
    q.enqueue(6, 15);
    auto value = q.dequeue();
    REQUIRE(value == 10);
}

TEST_CASE("Size returns the correct size of the queue") {
    priorityqueue<int> q;
    q.enqueue(2, 10);
    q.enqueue(4, 5);
    REQUIRE(q.Size() == 2);
}

TEST_CASE("Begin returns an iterator to the first element in the queue") {
    priorityqueue<int> q;
    q.enqueue(2, 10);
    q.enqueue(4, 5);
    auto it = q.begin();
    REQUIRE(it->value == 5);
}

TEST_CASE("Tostring returns a string representation of the queue") {
    priorityqueue<int> q;
    q.enqueue(2, 10);
    REQUIRE(q.tostring() == "2 value: 10");
}

TEST_CASE("Operator== returns true if two queues are equal") {
    priorityqueue<int> q1;
    q1.enqueue(2, 10);
    q1.enqueue(4, 5);
    priorityqueue<int> q2;
    q2.enqueue(2, 10);
    q2.enqueue(4, 5);
    REQUIRE(q1 == q2);
}

TEST_CASE("Operator== returns false if two queues are not equal") {
    priorityqueue<int> q1;
    q1.enqueue(2, 10);
    q1.enqueue(4, 5);
    priorityqueue<int> q2;
    q2.enqueue(2, 10);
    q2.enqueue(6, 5);
    REQUIRE_FALSE(q1 == q2);
    
TEST_CASE("Peek does not remove elements from the queue") {
    PriorityQueue<int> pq;
    pq.push(2, 10);
    pq.push(1, 20);
    pq.push(3, 5);
    REQUIRE(pq.size() == 3);
    pq.peek();
    REQUIRE(pq.size() == 3);
}
