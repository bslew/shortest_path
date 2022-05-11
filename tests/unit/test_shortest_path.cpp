#include "Graph.h"
#include <gtest/gtest.h>

using namespace minpath;

TEST(ShortestPathTest, twoWaysDifferentDistances) {
    Graph<int> G;
    Node<int> n0(0);
    Node<int> n1(1);
    Node<int> n2(2);
    Node<int> n3(3);

    /*
    (x) cost/distance to traverse between nodes
         3
         | (1)
         2
    (2) / \
       1   | (2)
    (1) \ /
         0
    */

    n0.addNeighbour(1, 1);
    n0.addNeighbour(2, 2);
    n0.setStart();

    n1.addNeighbour(0, 1);
    n1.addNeighbour(2, 2);

    n2.addNeighbour(0, 2);
    n2.addNeighbour(1, 2);
    n2.addNeighbour(3, 1);

    n3.addNeighbour(2, 1);
    n3.setEnd();

    G.addNodes({n0, n1, n2, n3});

    auto minDist = G.solveMinimalDistance();
    auto shortestPath = G.getShortestPathNodes();
    ASSERT_TRUE(minDist.has_value()); // Expected path exists
    ASSERT_EQ(minDist.value(),
              3); // Expected shortest path from node 0 to node 3 is 3
    ASSERT_EQ(
        shortestPath,
        std::vector<size_t>(
            {0, 2, 3})); // Expected shortest path goes through nodes 0, 2, 3
}

TEST(ShortestPathTest, twoWaysDifferentDistances2) {
    Graph<int> G;
    Node<int> n0(0);
    Node<int> n1(1);
    Node<int> n2(2);
    Node<int> n3(3);
    Node<int> n4(4);

    /*
    (x) cost/distance to traverse between nodes
         4
         | (1)
         3
    (3) / \ (1)
       1   2
    (3) \ / (4)
         0
    */

    n0.addNeighbour(1, 3);
    n0.addNeighbour(2, 4);
    n0.setStart();

    n1.addNeighbour(0, 3);
    n1.addNeighbour(3, 3);

    n2.addNeighbour(0, 4);
    n2.addNeighbour(3, 1);

    n3.addNeighbour(1, 3);
    n3.addNeighbour(2, 1);
    n3.addNeighbour(4, 1);

    n4.addNeighbour(3, 1);
    n4.setEnd();

    G.addNodes({n0, n1, n2, n3, n4});

    auto minDist = G.solveMinimalDistance();
    auto shortestPath = G.getShortestPathNodes();

    ASSERT_TRUE(minDist.has_value()); // Expected path exists
    ASSERT_EQ(minDist.value(),
              6); // Expected shortest path from node 0 to node 4 is 6
    ASSERT_EQ(shortestPath,
              std::vector<size_t>(
                  {0, 2, 3,
                   4})); // Expected shortest path goes through nodes 0, 2, 3, 4
}

TEST(ShortestPathTest, twoWaysDifferentDistances3) {
    Graph<int> G;
    Node<int> n0(0);
    Node<int> n1(1);
    Node<int> n2(2);

    /*
    (x) cost/distance to traverse between nodes
         2
    (2) / \
       1   | (4)
    (1) \ /
         0
    */

    n0.addNeighbour(1, 1);
    n0.addNeighbour(2, 4);
    n0.setStart();

    n1.addNeighbour(0, 1);
    n1.addNeighbour(2, 2);

    n2.addNeighbour(0, 4);
    n2.addNeighbour(1, 2);
    n2.setEnd();

    G.addNodes({n0, n1, n2});

    auto minDist = G.solveMinimalDistance();
    auto shortestPath = G.getShortestPathNodes();

    ASSERT_TRUE(minDist.has_value()); // Expected path exists
    ASSERT_EQ(minDist.value(),
              3); // Expected shortest path from node 0 to node 2 is 3
    ASSERT_EQ(shortestPath,
              std::vector<size_t>(
                  {0, 1, 2})); // Expected shortest path goes through nodes 0, 2
}

TEST(ShortestPathTest, pathDoesNotExist) {
    Graph<int> G;
    Node<int> n0(0);
    Node<int> n1(1);
    Node<int> n2(2);

    /*
        2

        1
        |
        0
    */

    n0.addNeighbour(1, 1);
    n0.setStart();

    n1.addNeighbour(0, 1);

    n2.setEnd();

    G.addNodes({n0, n1, n2});

    auto minDist = G.solveMinimalDistance();
    ASSERT_FALSE(minDist.has_value()); // Expected path does not exist
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
