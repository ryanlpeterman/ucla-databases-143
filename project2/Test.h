#ifndef TEST_H
#define TEST_H

#include "BTreeNode.h"
#include "BTreeIndex.h"
using namespace std;

class Test {
  public:
    void beginTests();

  private:
    void testLeafNode();
    void testNonLeafNode();
    void testIndex();
};


#endif
