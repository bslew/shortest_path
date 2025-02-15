#include "Graph.h"
#include <gtest/gtest.h>

using namespace minpath;

TEST(GraphBuildTest, fieldGraphWithoutDiagonalConnections) {
    FieldGraph<int> G(0, 0, 2, 2, 0, 2, 0, 2, Obstacles<int>({{1, 1}}), false);

    /*
    y
    2:  2-5-8
        |   |
    1:  1 4 7
        |   |
    0:  0-3-6

    x:  0 1 2
    */
    auto neigh = G.node(0).getNeighbors();

    ASSERT_TRUE(neigh.contains(1)); // Node 0 should have neighbour 1 at dist 1
    ASSERT_EQ(neigh[1], 1);
    ASSERT_TRUE(neigh.contains(3));
    ASSERT_EQ(neigh[3], 1); // Node 0 should have neighbour 3 at dist 1

    neigh = G.node(1).getNeighbors();
    ASSERT_TRUE(neigh.contains(2));  // Node 1 should have neighbour 2
    ASSERT_TRUE(neigh.contains(0));  // Node 1 should have neighbour 0
    ASSERT_FALSE(neigh.contains(4)); // Node 1 should not have neighbour 4

    neigh = G.node(4).getNeighbors();
    ASSERT_TRUE(neigh.empty()); // Node 4 should not have any neighbours
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
