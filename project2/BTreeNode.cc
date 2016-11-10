#include "BTreeNode.h"
#include <cstring>

using namespace std;

BTLeafNode::BTLeafNode()
{
  // Initialize Node with zeros
  memset(buffer, 0, PageFile::PAGE_SIZE);

  // Set Leaf flag
  memset(buffer, 'l', sizeof(char));

  // Set key count to 0
  int temp = 0;
  memcpy(buffer + sizeof(char), &temp, sizeof(int));

  // Set next node pointer (PageId) to -1
  temp = -1;
  memcpy(buffer + (PageFile::PAGE_SIZE) - 1 - (sizeof(PageId)), &temp, sizeof(PageId));
}

BTLeafNode::BTLeafNode(PageId pid, const PageFile& pf)
{
  read(pid, pf);
}

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::read(PageId pid, const PageFile& pf)
{
  RC rc;

  // TODO: Error-checking? Out of bounds pid?

  // read the page representing the node
  if ((rc = pf.read(pid, buffer)) < 0) return rc;

  return 0; 

}
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::write(PageId pid, PageFile& pf)
{
  RC rc;

  // TODO: Error-checking? Out of bounds pid?

  // write the page to the disk
  if ((rc = pf.write(pid, buffer)) < 0) return rc;

  return 0; 

}

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTLeafNode::getKeyCount()
{
  int count;

  // The four bytes starting from the second byte of a page contains
  // the # of (rid, key) pairs in the node
  memcpy(&count, buffer + sizeof(char), sizeof(int));
  return count;
}

/*
 * Insert a (key, rid) pair to the node.
 * @param key[IN] the key to insert
 * @param rid[IN] the RecordId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTLeafNode::insert(int key, const RecordId& rid)
{
  // If node is full, return error code
  int oldCount = getKeyCount();
  if (oldCount >= PAIRS_PER_NODE) {
    return RC_NODE_FULL;
  }

  // Size of a (rid, key) pair
  size_t sizeOfPair = sizeof(RecordId) + sizeof(int);
  
  // Initialize two pointers
  // rightPtr points to the next avilable pair position (initially empty)
  // leftPtr points to the last non-empty pair position
  char* rightPtr = buffer + sizeof(char) + sizeof(int) + (oldCount * sizeOfPair);
  char* leftPtr = rightPtr - sizeOfPair;

  // curNum is set to the pair number of the second-to-last pair
  int curNum = oldCount - 1;
  int curKey;

  // Move each pair one position to the right until we reach a pair
  // that has a key value smaller than the to-be-inserted pair's key value,
  // or until there are no more pairs
  while (true) {
    // If curNum is less than 0, then we there is no key in this node
    // that is less than the to-be-inserted pair's key; break
    if (curNum < 0) {
      break;
    }

    // Set curKey to the key of the pair pointed to by leftPtr
    memcpy(&curKey, leftPtr + sizeof(RecordId), sizeof(int));

    // If curKey is less than key, then rightPtr points to the position
    // where we should insert the pair; break
    if (curKey < key) {
      break;
    }

    // Otherwise, move this current pair one position to the right
    memcpy(rightPtr, leftPtr, sizeOfPair);
    
    // Move both pointers left one position
    // Decrement curNum
    rightPtr = leftPtr;
    leftPtr = rightPtr - sizeOfPair;
    curNum--;
  }

  // rightPtr points to where we should insert the pair
  memcpy(rightPtr, &rid, sizeof(RecordId));
  memcpy(rightPtr + sizeof(RecordId), &key, sizeof(int));

  // Increment this node's key count
  int count = oldCount + 1;
  memcpy(buffer + sizeof(char), &count, sizeof(int));
  return 0;
}

/*
 * Insert the (key, rid) pair to the node
 * and split the node half and half with sibling.
 * The first key of the sibling node is returned in siblingKey.
 * @param key[IN] the key to insert.
 * @param rid[IN] the RecordId to insert.
 * @param sibling[IN] the sibling node to split with. This node MUST be EMPTY when this function is called.
 * @param siblingKey[OUT] the first key in the sibling node after split.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::insertAndSplit(int key, const RecordId& rid, 
                              BTLeafNode& sibling, int& siblingKey)
{

  // TODO: Check if sibling node is empty?

  size_t sizeOfPair = sizeof(RecordId) + sizeof(int);

  // The number of pairs we need to move into the new sibling node
  // is equal to (oldcount+1)/2
  int oldCount = getKeyCount();
  int numPairsToNewNode = (oldCount+1)/2;

  // TODO: Check if oldCount is less than 1?

  // curPtr points to the last pair in the original node
  char* curPtr = buffer + sizeof(char) + sizeof(int) + ((oldCount - 1) * sizeOfPair);
  int curKey;
  
  RecordId ridToInsert;
  int newKeyCount;

  bool insertedNewPair = false;

  // This loop moves pairs over to the new sibling node one at a time until
  //   the sibling node has the correct amount of pairs.
  // If we ever reach a key value that is smaller than the one we are trying
  //   to insert, then we insert the to-be-inserted pair instead, and continue.
  while (numPairsToNewNode > 0) {
    
    // Get current key value
    memcpy(&curKey, curPtr + sizeof(RecordId), sizeof(int));
    
    // If we've already inserted the to-be-inserted pair
    // -or-
    // If the current key is greater than the to-be-inserted pair's key
    if (insertedNewPair || curKey > key) {
      
      // Get the rid of the current pair
      // Insert the pair into the new sibling node
      memcpy(&ridToInsert, curPtr, sizeof(RecordId));
      sibling.insert(curKey, ridToInsert);

      // Decrement this node's key count
      newKeyCount = getKeyCount() - 1;
      memcpy(buffer + sizeof(char), &newKeyCount, sizeof(int));      

      // Move the curPtr left so we can look at the next pair
      // Decrement numPairsToNewNode since we've inserted one to the
      // new sibling node
      curPtr -= sizeOfPair;
      numPairsToNewNode--;

      // If we haven't inserted the to-be-inserted pair
      // -and-
      // The to-be-inserted pair's key is greater than the current key
    } else {

      // Insert the to-be-inserted pair into the new sibling node
      // Set insertedNewPair to true, so we don't do it again
      sibling.insert(key, rid);
      insertedNewPair = true;

      // Decrement numPairsToNewNode sicne we've inserted one to the
      // new sibling node
      numPairsToNewNode--;

    }

  }

  // If we've inserted the correct number of pairs into the new sibling node
  // but we haven't inserted the to-be-inserted pair, then insert that
  // pair into this node
  if (!insertedNewPair) {
    insert(key, rid);
  }

  // temp RecordId to be passed in, we don't use its values
  RecordId temp;
  // Read the first entry of the new sibling node to set siblingKey
  sibling.readEntry(0, siblingKey, temp);

  return 0;

  // TODO: Set next node ptr? I think it's done by BTreeIndex though

}

/**
 * If searchKey exists in the node, set eid to the index entry
 * with searchKey and return 0. If not, set eid to the index entry
 * immediately after the largest index key that is smaller than searchKey,
 * and return the error code RC_NO_SUCH_RECORD.
 * Remember that keys inside a B+tree node are always kept sorted.
 * @param searchKey[IN] the key to search for.
 * @param eid[OUT] the index entry number with searchKey or immediately
                   behind the largest key smaller than searchKey.
 * @return 0 if searchKey is found. Otherwise return an error code.
 */
RC BTLeafNode::locate(int searchKey, int& eid)
{ return 0; }

/*
 * Read the (key, rid) pair from the eid entry.
 * @param eid[IN] the entry number to read the (key, rid) pair from
 * @param key[OUT] the key from the entry
 * @param rid[OUT] the RecordId from the entry
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::readEntry(int eid, int& key, RecordId& rid)
{
  if (eid < 0 || eid >= getKeyCount()) {
    return RC_INVALID_ATTRIBUTE;
  }

  // Size of a (rid, key) pair
  size_t sizeOfPair = sizeof(RecordId) + sizeof(int);

  // Get pointer to the target pair
  // Use memcpy to set rid and key to the values in the pair
  char* pairPtr = buffer + sizeof(char) + sizeof(int) + (eid * sizeOfPair);
  memcpy(&rid, pairPtr, sizeof(RecordId));
  memcpy(&key, pairPtr + sizeof(RecordId), sizeof(int));

  return 0;
}

/*
 * Return the pid of the next slibling node.
 * @return the PageId of the next sibling node 
 */
PageId BTLeafNode::getNextNodePtr()
{
  PageId nextNodePtr;

  memcpy(&nextNodePtr, buffer + (PageFile::PAGE_SIZE) - 1 - (sizeof(PageId)), sizeof(PageId));

  return nextNodePtr;
}

/*
 * Set the pid of the next slibling node.
 * @param pid[IN] the PageId of the next sibling node 
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::setNextNodePtr(PageId pid)
{
  memcpy(buffer + (PageFile::PAGE_SIZE) - 1 - (sizeof(PageId)), &pid, sizeof(PageId));
  return 0;
}

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::read(PageId pid, const PageFile& pf)
{ return 0; }
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::write(PageId pid, PageFile& pf)
{ return 0; }

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTNonLeafNode::getKeyCount()
{ return 0; }


/*
 * Insert a (key, pid) pair to the node.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTNonLeafNode::insert(int key, PageId pid)
{ return 0; }

/*
 * Insert the (key, pid) pair to the node
 * and split the node half and half with sibling.
 * The middle key after the split is returned in midKey.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @param sibling[IN] the sibling node to split with. This node MUST be empty when this function is called.
 * @param midKey[OUT] the key in the middle after the split. This key should be inserted to the parent node.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::insertAndSplit(int key, PageId pid, BTNonLeafNode& sibling, int& midKey)
{ return 0; }

/*
 * Given the searchKey, find the child-node pointer to follow and
 * output it in pid.
 * @param searchKey[IN] the searchKey that is being looked up.
 * @param pid[OUT] the pointer to the child node to follow.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::locateChildPtr(int searchKey, PageId& pid)
{ return 0; }

/*
 * Initialize the root node with (pid1, key, pid2).
 * @param pid1[IN] the first PageId to insert
 * @param key[IN] the key that should be inserted between the two PageIds
 * @param pid2[IN] the PageId to insert behind the key
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::initializeRoot(PageId pid1, int key, PageId pid2)
{ return 0; }
