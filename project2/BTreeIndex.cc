/*
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */
 
#include "BTreeIndex.h"
#include "BTreeNode.h"
#include <cstring>

using namespace std;

/*
 * BTreeIndex constructor
 */
BTreeIndex::BTreeIndex()
{
    rootPid = -1;
    treeHeight = -1;
}

BTreeIndex::BTreeIndex(const string& indexname, char mode)
{
  open(indexname, mode);
}

// TODO: remove before submission
PageId BTreeIndex::getRootPid() {
  return rootPid;
}
int BTreeIndex::getTreeHeight() {
  return treeHeight;
}
PageId BTreeIndex::getPfEndPid() {
  return pf.endPid();
}

/*
 * Open the index file in read or write mode.
 * Under 'w' mode, the index file should be created if it does not exist.
 * @param indexname[IN] the name of the index file
 * @param mode[IN] 'r' for read, 'w' for write
 * @return error code. 0 if no error
 */
RC BTreeIndex::open(const string& indexname, char mode)
{
  RC rc;
  char page[PageFile::PAGE_SIZE];

  // open the index file
  if ((rc = pf.open(indexname, mode)) < 0) return rc;

  // get the end pid of the index file
  PageId endPid = pf.endPid();

  // if the end pid is zero, the index file is empty.
  //   set rootPid to 1
  //   set treeHeight to 0
  //   store these values in the 0th page of the index file
  if (endPid == 0) {
    rootPid = 1;
    treeHeight = 0;
    
    // Initialize page with zeros
    memset(page, 0, PageFile::PAGE_SIZE);
    // Save rootPid
    memcpy(page, &rootPid, sizeof(PageId));
    // Save treeHeight
    memcpy(page + sizeof(PageId), &treeHeight, sizeof(int));
    // Write to disk
    if ((rc = pf.write(0, page)) < 0) return rc;

  // else, the index file is not empty
  //   Set rootPid and treeHeight to the values stored in the 0th page
  //   of the index file
  } else {
    // Read from disk
    if ((rc = pf.read(0, page)) < 0) return rc;
    // Set rootPid
    memcpy(&rootPid, page, sizeof(PageId));
    // Set treeHeight
    memcpy(&treeHeight, page + sizeof(PageId), sizeof(int));

  }
  return 0;
}

/*
 * Close the index file.
 * @return error code. 0 if no error
 */
RC BTreeIndex::close()
{
  rootPid = -1;
  treeHeight = -1;

  return pf.close();
}

/*
 * Insert (key, RecordId) pair to the index.
 * @param key[IN] the key for the value inserted into the index
 * @param rid[IN] the RecordId for the record being inserted into the index
 * @return error code. 0 if no error
 */
RC BTreeIndex::insert(int key, const RecordId& rid)
{
    return 0;
}

/**
 * Run the standard B+Tree key search algorithm and identify the
 * leaf node where searchKey may exist. If an index entry with
 * searchKey exists in the leaf node, set IndexCursor to its location
 * (i.e., IndexCursor.pid = PageId of the leaf node, and
 * IndexCursor.eid = the searchKey index entry number.) and return 0.
 * If not, set IndexCursor.pid = PageId of the leaf node and
 * IndexCursor.eid = the index entry immediately after the largest
 * index key that is smaller than searchKey, and return the error
 * code RC_NO_SUCH_RECORD.
 * Using the returned "IndexCursor", you will have to call readForward()
 * to retrieve the actual (key, rid) pair from the index.
 * @param key[IN] the key to find
 * @param cursor[OUT] the cursor pointing to the index entry with
 *                    searchKey or immediately behind the largest key
 *                    smaller than searchKey.
 * @return 0 if searchKey is found. Othewise an error code
 */
RC BTreeIndex::locate(int searchKey, IndexCursor& cursor)
{
  RC rc;
  PageId current_pid = rootPid;
  char page[PageFile::PAGE_SIZE];
  char flag;

  // This loop runs until current_pid is set to the PageId of the
  // leaf node that could contain the searchKey
  while (true) {

    // Read in the current page using current_pid
    // Check the leaf/internal_node flag
    // If it's a leaf, then we break out of the loop
    if ((rc = pf.read(current_pid, page)) < 0) return rc;
    memcpy(&flag, page, sizeof(char));
    if (flag == 'l') {
      break;
    }
    
    // Otherwise, we create a BTNonLeafNode based on the current_pid
    // and locate the child pointer using the searchKey
    // If all goes well, then current_pid should be updated to the
    // next node to be checked
    BTNonLeafNode* non_leaf_node = new BTNonLeafNode(current_pid, pf);

    if ((rc = non_leaf_node->locateChildPtr(searchKey, current_pid)) < 0) return rc;

    delete non_leaf_node;
  }

  // We have reached a leaf node, so we create a BTLeafNode based on
  // the current_pid
  BTLeafNode* leaf_node = new BTLeafNode(current_pid, pf);

  // Set cursor.pid to the pid of the leaf node
  cursor.pid = current_pid;
  // Call locate() on the leaf_node to set cursor.eid to the correct value
  // Save the return code in rc
  rc = leaf_node->locate(searchKey, cursor.eid);

  delete leaf_node;

  // Return rc (BTLeafNode::locate() will either return 0 or an error code)
  return rc;
}

/*
 * Read the (key, rid) pair at the location specified by the index cursor,
 * and move foward the cursor to the next entry.
 * @param cursor[IN/OUT] the cursor pointing to an leaf-node index entry in the b+tree
 * @param key[OUT] the key stored at the index cursor location.
 * @param rid[OUT] the RecordId stored at the index cursor location.
 * @return error code. 0 if no error
 */
RC BTreeIndex::readForward(IndexCursor& cursor, int& key, RecordId& rid)
{
  RC rc;

  // Initialize a new leaf node and read in the contents using cursor.pid
  BTLeafNode* leaf_node = new BTLeafNode();
  rc = leaf_node->read(cursor.pid, pf);
  if (rc < 0) return rc;

  // Call readEntry using cursor.eid to update key and rid
  rc = leaf_node->readEntry(cursor.eid, key, rid);
  if (rc < 0) return rc;

  // Move the cursor forward
  // If this eid is the last entry in this leaf node,
  //   then we set cursor.pid to the next node (using getNextNodePtr())
  //   and set cursor.eid to 0
  // Else
  //    we increment cursor.eid
  if ((cursor.eid + 1) == leaf_node->getKeyCount()) {
    cursor.pid = leaf_node->getNextNodePtr();
    cursor.eid = 0;
  } else {
    cursor.eid++;
  }

  delete leaf_node;
  return 0;
}
