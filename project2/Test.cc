#include <iostream>
#include <assert.h>

#include "Test.h"
using namespace std;

void Test::beginTests() {
  //testLeafNode();
  //testNonLeafNode();
  testIndex();
}

void testPrint(BTLeafNode* leaf2) {
  int key;
  RecordId rid;
  cout << "\nPrinting all values...\n";
  for (int i = 0; i < leaf2->getKeyCount(); i++) {
    leaf2->readEntry(i, key, rid);
    cout << " eid = " << i << endl;
    cout << " key = " << key << endl;
    cout << " rid.pid = " << rid.pid << endl;
    cout << " rid.sid = " << rid.sid << "\n\n";
  }
}

void testInsert(BTLeafNode* leaf, int key, int pid, int sid) {
  RecordId rid;
  rid.pid = pid;
  rid.sid = sid;
  cout << "Inserting pair... Key = " << key << ", Rid.pid = " << pid << ", Rid.sid = " << sid << endl;
  leaf->insert(key, rid);
  cout << "          Key count is: " << leaf->getKeyCount() << endl;
}

void testInsertAndSplit(BTLeafNode* leaf, int key, int pid, int sid, BTLeafNode& sibling, int& siblingKey) {
  RecordId rid;
  rid.pid = pid;
  rid.sid = sid;
  cout << "Inserting and splitting... Key = " << key << ", Rid.pid = " << pid << ", Rid.sid = " << sid << endl;
  leaf->insertAndSplit(key, rid, sibling, siblingKey);
  cout << "\n\nOriginal node:\n";
  cout << "  ";
  testPrint(leaf);
  cout << "  Key Count is: " << leaf->getKeyCount();

  cout << "\n\nSibling node:\n";
  cout < "  ";
  testPrint(&sibling);
  cout << "  Key Count is: " << sibling.getKeyCount();

  cout << "\n\nsiblingKey is set to: " << siblingKey << endl;

}

void testLocate(BTLeafNode* leaf) {
  int eid;
  cout << "\n\n///////////////////////////////////\n";
  cout << "  locate() test:\n    -1?\n";
  if (leaf->locate(-1, eid) < 0) {
    cout << "      Not found. eid is: " << eid << endl;
  } else {
    cout << "      Found. eid is: " << eid << endl;
  }

  cout << "\n    1?\n";
  if (leaf->locate(1, eid) < 0) {
    cout << "      Not found. eid is: " << eid << endl;
  } else {
    cout << "      Found. eid is: " << eid << endl;
  }

  cout << "\n    5?\n";
  if (leaf->locate(5, eid) < 0) {
    cout << "      Not found. eid is: " << eid << endl;
  } else {
    cout << "      Found. eid is: " << eid << endl;
  }

  cout << "\n    10?\n";
  if (leaf->locate(10, eid) < 0) {
    cout << "      Not found. eid is: " << eid << endl;
  } else {
    cout << "      Found. eid is: " << eid << endl;
  }

  cout << "\n    100?\n";
  if (leaf->locate(100, eid) < 0) {
    cout << "      Not found. eid is: " << eid << endl;
  } else {
    cout << "      Found. eid is: " << eid << endl;
  }

}

void Test::testLeafNode() {
  cout << "Testing Leaf Node Code" << endl;

  PageFile* pf_handle = new PageFile();
  pf_handle->open("leaf_node.test", 'w');

  BTLeafNode* leaf = new BTLeafNode();
  cout << "New leaf. Key count is: " << leaf->getKeyCount() << endl;
  cout << "          Next Node Ptr is: " << leaf->getNextNodePtr() << endl;
  cout << "Setting Next Node Ptr to 3.\n";
  PageId temp = 3;
  leaf->setNextNodePtr(temp);
  cout << "          Next Node Ptr is now: " << leaf->getNextNodePtr() << endl;

  testLocate(leaf);

  testInsert(leaf, 12, 3, 9);
  testPrint(leaf);

  testLocate(leaf);

  testInsert(leaf, 7, 3, 2);
  testPrint(leaf);
  testLocate(leaf);

  testInsert(leaf, 10, 9, 17);
  testPrint(leaf);
  testLocate(leaf);

  leaf->write(1, *pf_handle);
  cout << endl;
  BTLeafNode* leaf2 = new BTLeafNode(1, *pf_handle);
  cout << "New leaf. Key count is: " << leaf->getKeyCount() << endl;
  cout << "          Next Node Ptr is: " << leaf2->getNextNodePtr() << endl;

  testPrint(leaf2);

  cout << "\nNow testing insertAndSplit...\n\n";
  BTLeafNode* leaf3 = new BTLeafNode();

  testInsert(leaf3, 1, 101, 101);
  testInsert(leaf3, 3, 303, 303);
  testInsert(leaf3, 5, 505, 505);
  testInsert(leaf3, 7, 707, 707);
  testInsert(leaf3, 9, 909, 909);
  testInsert(leaf3, 11, 11011, 11011);

  testLocate(leaf3);

  cout << "\nNow, leaf3 has 6 pairs. If we insert and split one pair, leaf3 should have 4 pairs, and the new sibling node should have 3 pairs.\n\nTrying with key value = 10, which should be inserted into the sibling node.\n\n";
  
  BTLeafNode leaf3sibling;
  int leaf3siblingKey;
  testInsertAndSplit(leaf3, 10, 10010, 10010, leaf3sibling, leaf3siblingKey);

  cout << "\n==================================================\n\n";
  cout << "Another insertAndSplit test...\n\n";
  BTLeafNode* leaf4 = new BTLeafNode();
  
  testInsert(leaf4, 1, 101, 101);
  testInsert(leaf4, 3, 303, 303);
  testInsert(leaf4, 5, 505, 505);
  testInsert(leaf4, 7, 707, 707);
  testInsert(leaf4, 9, 909, 909);
  testInsert(leaf4, 11, 11011, 11011);
  testLocate(leaf4);

  cout << "\nNow, leaf4 has 6 pairs. If we isnert and split one pair, leaf4 should have 4 pairs, and the new sibling node should have 3 pairs.\n\nTrying with key value = 2, which should be inserted into the original node.\n\n";

  BTLeafNode leaf4sibling;
  int leaf4siblingKey;
  testInsertAndSplit(leaf4, 2, 202, 202, leaf4sibling, leaf4siblingKey);

  cout << "\n==================================================\n\n";
  cout << "Another insertAndSplit test...\n\n";
  BTLeafNode* leaf5 = new BTLeafNode();
  
  testInsert(leaf5, 1, 101, 101);
  testInsert(leaf5, 3, 303, 303);
  testInsert(leaf5, 5, 505, 505);
  testInsert(leaf5, 7, 707, 707);
  testInsert(leaf5, 9, 909, 909);
  testInsert(leaf5, 11, 11011, 11011);
  testInsert(leaf5, 13, 13013, 13013);

  cout << "\nNow, leaf5 has 7 pairs. If we isnert and split one pair, leaf5 should have 4 pairs, and the new sibling node should have 4 pairs.\n\nTrying with key value = 8, which should be inserted into the sibling node.\n\n";

  BTLeafNode leaf5sibling;
  int leaf5siblingKey;
  testInsertAndSplit(leaf5, 8, 808, 808, leaf5sibling, leaf5siblingKey);

  cout << "\n==================================================\n\n";
  cout << "Another insertAndSplit test...\n\n";
  BTLeafNode* leaf6 = new BTLeafNode();
  
  testInsert(leaf6, 1, 101, 101);
  testInsert(leaf6, 3, 303, 303);
  testInsert(leaf6, 5, 505, 505);
  testInsert(leaf6, 7, 707, 707);
  testInsert(leaf6, 9, 909, 909);
  testInsert(leaf6, 11, 11011, 11011);
  testInsert(leaf6, 13, 13013, 13013);

  cout << "\nNow, leaf6 has 7 pairs. If we isnert and split one pair, leaf6 should have 4 pairs, and the new sibling node should have 4 pairs.\n\nTrying with key value = 6, which should be inserted into the original node.\n\n";

  BTLeafNode leaf6sibling;
  int leaf6siblingKey;
  testInsertAndSplit(leaf6, 6, 606, 606, leaf6sibling, leaf6siblingKey);

  pf_handle->close();
  delete pf_handle;
  delete leaf;
  delete leaf2;
  delete leaf3;
  delete leaf4;
  delete leaf5;
  delete leaf6;
  cout << "All Leaf Test Cases Passed" << endl;
}

void Test::testNonLeafNode() {
  PageFile* pf_handle = new PageFile();
  pf_handle->open("internal_node.test", 'w');

  // tests default constructor
  BTNonLeafNode* node = new BTNonLeafNode();
  assert(node->getKeyCount() == 0); 

  // tests insertion and getKeyCount
  PageId pid1 = 1;
  PageId pid2 = 2;
  assert(node->insert(1, pid1) == 0); 
  assert(node->getKeyCount() == 1);
  assert(node->insert(2, pid2) == 0);
  assert(node->getIthPid(0) == 1 && node->getIthKey(0) == 1);
  assert(node->getIthPid(1) == 2 && node->getIthKey(1) == 2);
  
  // tests initializeRoot
  BTNonLeafNode* root = new BTNonLeafNode();
  root->initializeRoot(1, 100, 2);
  assert(root->getIthPid(0) == 1 && root->getIthPid(1) == 2);
  assert(root->getIthKey(0) == 100);
  
  // tests locateChildPtr
  PageId childPtr;
  root->locateChildPtr(99, childPtr);
  assert(childPtr == 1);
  root->locateChildPtr(101, childPtr);
  assert(childPtr == 2);
  node->insert(100, pid2);
  node->locateChildPtr(50, childPtr);
  assert(childPtr == 2);

  // tests insertAndSplit
  BTNonLeafNode* test_sibling = new BTNonLeafNode();
  int mid_key;
  PageId pid3 = 3;
  assert(node->insertAndSplit(3, pid3, *test_sibling, mid_key) == 0);
  assert(node->getIthPid(0) == 1 && node->getIthKey(0) == 1);
  assert(node->getIthPid(1) == 2 && node->getIthKey(1) == 2);
  assert(mid_key == 2);
  assert(test_sibling->getIthPid(0) == 3 && test_sibling->getIthKey(0) == 3);

  pf_handle->close(); 
  cout << "All Non-Leaf Test Cases Passed" << endl; 
}

void Test::testIndex() {
  BTreeIndex* test1 = new BTreeIndex();
  assert(test1->getPfEndPid() == 0);
  assert(test1->getRootPid() == -1);
  assert(test1->getTreeHeight() == 0);

  BTreeIndex* test2 = new BTreeIndex("testindex", 'w');
  assert(test2->getPfEndPid() == 1);
  assert(test2->getRootPid() == 1);
  assert(test2->getTreeHeight() == 0);
  test2->close();

  test1->open("testindex", 'w');
  assert(test1->getRootPid() == 1);
  assert(test1->getTreeHeight() == 0);
  test1->close();

  BTreeIndex* test3 = new BTreeIndex("testindex", 'w');
  RecordId rid1;
  rid1.pid = 2;
  rid1.sid = 3;
  int key1 = 1;
  test3->insert(key1, rid1);

  for(int i = 0; i < 40000; i++) {
    RecordId* rid2 = new RecordId();
    rid2->pid = i;
    rid2->sid = i;
    int key2 = i;
    test3->insert(key2, *rid2);
  }  
  for(int i = 0; i < 5325; i++) {
    IndexCursor* cur = new IndexCursor();
    RC rc = test3->locate(i, *cur);
    assert(rc != RC_NO_SUCH_RECORD);
  }
  cout << test3->getTreeHeight() << endl;
  cout << test3->getRootPid() << endl;
  cout << test3->getPfEndPid() << endl;

  cout << "Passed all BTreeIndex test cases.\n";
}
