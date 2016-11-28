/**
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include "Bruinbase.h"
#include "SqlEngine.h"
#include "BTreeIndex.h"
#include <limits.h>


// TODO: comment out before submission
// student defined test suite
#include "Test.h"

using namespace std;

// external functions and variables for load file and sql command parsing 
extern FILE* sqlin;
int sqlparse(void);


RC SqlEngine::run(FILE* commandline)
{
  fprintf(stdout, "Bruinbase> ");

  // set the command line input and start parsing user input
  sqlin = commandline;
  sqlparse();  // sqlparse() is defined in SqlParser.tab.c generated from
               // SqlParser.y by bison (bison is GNU equivalent of yacc)

  return 0;
}

// TODO: pass the count in by reference here too
// TODO: add value bounds checking to this print function
void query_print(int attr, int key, RecordId rid, RecordFile& rf) {
  string val;
  RC rc;

  // Based on the case given by attr, we print the key/value
  switch (attr) {
    // key
    case 1:
      cout << key << endl;
      break;
    // value
    case 2:
      rc = rf.read(rid, key, val);

      if (rc != 0) {
        cout << "error when printing case2" << endl;
      } else {
        cout << val << endl;
      }
      break;
    // *
    case 3:
      rc = rf.read(rid, key, val);

      if (rc != 0) {
        cout << "error when printing case3" << endl;
      } else {
        cout << key << " '" << val << "'" << endl;
      }
      break;
  }
}

RC SqlEngine::select(int attr, const string& table, const vector<SelCond>& cond)
{
  RecordFile rf;   // RecordFile containing the table
  RecordId   rid;  // record cursor for table scanning

  RC     rc;
  int    key;     
  string value;
  int    count;
  int    diff;
 
  string idx_filename = table + ".idx"; 
  ifstream idx_file(idx_filename.c_str()); // open idx file to check for existence

  int num_cond = cond.size();
  
  // implicitly inclusive (convert exclusives when processing)
  int upper_key, lower_key, equals_key;
  vector<int> not_equals_keys; 

  string i_upper_value, i_lower_value, e_upper_value, e_lower_value, equals_value;
  vector<string> not_equals_values;

  bool upper_key_set = false, lower_key_set = false, equals_key_set = false;
  bool i_upper_value_set = false, i_lower_value_set = false, e_upper_value_set = false, e_lower_value_set = false, equals_value_set = false;
  
  // early termination set flag
  bool empty_set = false;

  // preprocess conditions
  for (int i = 0; i < num_cond; i++) {

    // if key
        // reduce all ranges down to two conditions
        // ensure only one equals
        // keep all not equals in a vector of conditions
    if (cond[i].attr == 1) {
  switch(cond[i].comp) {
  case SelCond::EQ:
    // equals_key is already set
    if (equals_key_set) {
      // TODO: assumes that cond[i].value is a valid string repr of an int
      // If two differing equals keys, then result is empty set
      if (equals_key != atoi(cond[i].value)) {
        empty_set = true;
      }

    // else (first equals condition)
    } else {
      // Set equals_key and equals_key_set
      equals_key = atoi(cond[i].value);
      equals_key_set = true;
    }
    break;
  case SelCond::NE:
    // add to not_equals_keys vector
    // TODO: convert this to a set for O(1) membership lookup
    not_equals_keys.push_back(atoi(cond[i].value));
    break;
  case SelCond::LT:
    // upper_key is already set
    if (upper_key_set) {
      // If more restricted range, then update upper_key
      // (convert to inclusive)
      if (atoi(cond[i].value) - 1 < upper_key) {
        upper_key = atoi(cond[i].value) - 1;
      }

    // else (first < or <= condition)
    } else {
      // Set upper_key and upper_key_set (convert to inclusive)
      upper_key = atoi(cond[i].value) - 1;
      upper_key_set = true;
    }
    break;
  case SelCond::GT:
    // lower_key is already set
    if (lower_key_set) {
      // If more restricted range, then update lower_key
      // (convert to inclusive)
      if (atoi(cond[i].value) + 1 > lower_key) {
        lower_key = atoi(cond[i].value) + 1;
      }

    // else (first > or >= condition)
    } else {
      // Set lower_key and lower_key_set (convert to inclusive)
      lower_key = atoi(cond[i].value) + 1;
      lower_key_set = true;
    }
    break;
  case SelCond::LE:
    // upper_key is already set
    if (upper_key_set) {
      // If more restricted range, then update upper_key
      if (atoi(cond[i].value) < upper_key) {
        upper_key = atoi(cond[i].value);
      }

    // else (first < or <= condition)
    } else {
      // Set upper_key and upper_key_set
      upper_key = atoi(cond[i].value);
      upper_key_set = true;
    }
    break;
  case SelCond::GE:
    // lower_key is already set
    if (lower_key_set) {
      // If more restricted range, then update lower_key
      if (atoi(cond[i].value) > lower_key) {
        lower_key = atoi(cond[i].value);
      }

    // else (first > or >= condition)
    } else {
      // Set lower_key and lower_key_set
      lower_key = atoi(cond[i].value);
      lower_key_set = true;
    }
    break;
  }


    // else value 
        // reduce all ranges down to two conditions
        // ensure only one equals
        // keep all not equals in a vector of conditions
    } else {
  switch(cond[i].comp) {
  case SelCond::EQ:
    // equals_value is already set
    if (equals_value_set) {
      // If two differing equals values, then result is empty set
      if (equals_value != cond[i].value) {
        empty_set = true;
      }

    // else (first equals condition)
    } else {
      // Set equals_value and equals_value_set
      equals_value = cond[i].value;
      equals_value_set = true;
    }
    break;
  case SelCond::NE:
    // add to not_equals_values vector
    not_equals_values.push_back(cond[i].value);
    break;
  case SelCond::LT:
    // e_upper_value is already set
    if (e_upper_value_set) {
      // If more restricted range, then update e_upper_value
      if (strcmp(cond[i].value, e_upper_value.c_str()) < 0) {
        e_upper_value = cond[i].value;
      }

    // else (first < condition)
    } else {
      // Set e_upper_value and e_upper_value_set
      e_upper_value = cond[i].value;
      e_upper_value_set = true;	    
    }
    break;
  case SelCond::GT:
    // e_lower_value is already set
    if (e_lower_value_set) {
      // If more restricted range, then update e_lower_value
      if (strcmp(cond[i].value, e_lower_value.c_str()) > 0) {
        e_lower_value = cond[i].value;
      }

    // else (first > condition)
    } else {
      // Set e_lower_value and e_lower_value_set
      e_lower_value = cond[i].value;
      e_lower_value_set = true;
    }
    break;
  case SelCond::LE:
    // i_upper_value is already set
    if (i_upper_value_set) {
      // If more restricted range, then update i_upper_value
      if (strcmp(cond[i].value, i_upper_value.c_str()) < 0) {
        i_upper_value = cond[i].value;
      }

    // else (first <= condition)
    } else {
      // Set i_upper_value and i_upper_value_set
      i_upper_value = cond[i].value;
      i_upper_value_set = true;
    }
    break;
  case SelCond::GE:
    // i_lower_value is already set
    if (i_lower_value_set) {
      // If more restricted range, then update i_lower_value
      if (strcmp(cond[i].value, i_lower_value.c_str()) > 0) {
        i_lower_value = cond[i].value;
      }

    // else (first >= condition)
    } else {
      // Set i_lower_value and i_lower_value_set
      i_lower_value = cond[i].value;
      i_lower_value_set = true;
    }
    break;
  }
    }
  }

  // check ranges - like if > and < mismatch, etc.
  // TODO: explicit boundaries ... check w/ Ryan
  if (equals_key_set && upper_key_set && equals_key > upper_key) {
    empty_set = true;
  } else if (equals_key_set && lower_key_set && equals_key < lower_key) {
    empty_set = true;
  } else if (upper_key_set && lower_key_set && upper_key < lower_key) {
    empty_set = true;
  } else if (equals_value_set && e_upper_value_set && strcmp(equals_value.c_str(), e_upper_value.c_str()) >= 0) {
    empty_set = true;
  } else if (equals_value_set && e_lower_value_set && strcmp(equals_value.c_str(), e_lower_value.c_str()) <= 0) {
    empty_set = true;
  } else if (equals_value_set && i_upper_value_set && strcmp(equals_value.c_str(), i_upper_value.c_str()) > 0) {
    empty_set = true;
  } else if (equals_value_set && i_lower_value_set && strcmp(equals_value.c_str(), i_lower_value.c_str()) < 0) {
    empty_set = true;
  } else if (e_upper_value_set && e_lower_value_set && strcmp(e_upper_value.c_str(), e_lower_value.c_str()) <= 0) {
    empty_set = true;
  } else if (i_upper_value_set && i_lower_value_set && strcmp(i_upper_value.c_str(), i_lower_value.c_str()) < 0) {
    empty_set = true;
  }
  // TODO: check that equals key is not equal to any of the not equals keys for an empty set


  // if empty set...
  if (empty_set) {
    cout << "REMOVE THIS BEFORE SUBMISSION\nEmpty set, return 0 automatically\n";
    return 0;
  }
  // if idx exists and there are any conditions on the key
  if (idx_file.good() && (upper_key_set || lower_key_set || equals_key_set || !not_equals_values.empty())) {
    // close to release file handle
    idx_file.close();

    // open index
    BTreeIndex idx(table + ".idx", 'r');
    
    IndexCursor cursor;
    int curr_key, count = 0;
    RecordId curr_rid;

    // open the table file
    if ((rc = rf.open(table + ".tbl", 'r')) < 0) {
      fprintf(stderr, "Error: table %s does not exist\n", table.c_str());
      return rc;
    }
    
    // if equals is set. just find it
    if (equals_key_set) {
      rc = idx.locate(equals_key, cursor); 
 
      if (rc != 0) {
        cout << "equals locate error/not found" << endl;
        return 0;
      } 

      rc = idx.readForward(cursor, curr_key, curr_rid);
 
      if (rc != 0) {
        cout << "read forward error" << endl;
        return 0;
      }
       
      // TODO: print out formatting
      if (attr == 4) {
        cout << "count == 1" << endl;
      } else {
        // TODO: fix with value bounds check pass
        query_print(attr, curr_key, curr_rid, rf);
      }

    // range query
    } else { 

      // set the bounds of range query if not set prior 
      if (!lower_key_set) {
        lower_key = INT_MIN;
      }
      if (!upper_key_set) {
        upper_key = INT_MAX;
      }
      idx.locate(lower_key, cursor);
      rc = idx.readForward(cursor, curr_key, curr_rid);         
 
      if (rc != 0) {
        cout << "readForward error/empty set?" << endl;
        rf.close();
        idx.close();
        return rc;
      }      
 
      // while curr_key is within bounds
      while (curr_key <= upper_key) {
        // print checked key TODO: alter this to new impl
        query_print(attr, curr_key, curr_rid, rf);

        // read next
        rc = idx.readForward(cursor, curr_key, curr_rid);         
        
        if (rc == RC_END_OF_TREE) {
          cout << "Done!" << endl;
          break;
        }

        if (rc != 0) {
          cout << "read forward error" << endl;
          break;
        } 
      }
    }

    rf.close();
    idx.close(); 
    return rc;

  // no index
  } else {

    // open the table file
    if ((rc = rf.open(table + ".tbl", 'r')) < 0) {
      fprintf(stderr, "Error: table %s does not exist\n", table.c_str());
      return rc;
    }

    // scan the table file from the beginning
    rid.pid = rid.sid = 0;
    count = 0;
    while (rid < rf.endRid()) {
      // read the tuple
      if ((rc = rf.read(rid, key, value)) < 0) {
        fprintf(stderr, "Error: while reading a tuple from table %s\n", table.c_str());
        goto exit_select;
      }

      // check the conditions on the tuple
      for (unsigned i = 0; i < cond.size(); i++) {
        // compute the difference between the tuple value and the condition value
        switch (cond[i].attr) {
        case 1:
      diff = key - atoi(cond[i].value);
      break;
        case 2:
      diff = strcmp(value.c_str(), cond[i].value);
      break;
        }

        // skip the tuple if any condition is not met
        switch (cond[i].comp) {
        case SelCond::EQ:
      if (diff != 0) goto next_tuple;
      break;
        case SelCond::NE:
      if (diff == 0) goto next_tuple;
      break;
        case SelCond::GT:
      if (diff <= 0) goto next_tuple;
      break;
        case SelCond::LT:
      if (diff >= 0) goto next_tuple;
      break;
        case SelCond::GE:
      if (diff < 0) goto next_tuple;
      break;
        case SelCond::LE:
      if (diff > 0) goto next_tuple;
      break;
        }
      }

      // the condition is met for the tuple. 
      // increase matching tuple counter
      count++;

      // print the tuple 
      switch (attr) {
      case 1:  // SELECT key
        fprintf(stdout, "%d\n", key);
        break;
      case 2:  // SELECT value
        fprintf(stdout, "%s\n", value.c_str());
        break;
      case 3:  // SELECT *
        fprintf(stdout, "%d '%s'\n", key, value.c_str());
        break;
      }

      // move to the next tuple
      next_tuple:
      ++rid;
    }

    // print matching tuple count if "select count(*)"
    if (attr == 4) {
      fprintf(stdout, "%d\n", count);
    }
    rc = 0;

    // close the table file and return
    exit_select:
    rf.close();
    return rc;
  }
}

RC SqlEngine::load(const string& table, const string& loadfile, bool index)
{
  // TODO: comment out before submission
  if (table == "test") {
    Test* t = new Test();
    t->beginTests();
    return 0;
  }  

  // conversion for type
  const char* loadfile_name = loadfile.c_str();
  RecordFile rf_handle;
 
  // create file named table.tbl 
  rf_handle.open(table + ".tbl", 'w');

  // fill in table.tbl with each line of load file interpreted as a tuple
  // store using the RecordFile api
  ifstream infile (loadfile_name);
  
  int key;
  string value;
  RecordId rid;
  
  if (index) {
    // initialize index
    BTreeIndex bindex(table + ".idx", 'w'); 
    // for every line in file
    for (string line; getline(infile, line);) {
      // get key, value, rid
      parseLoadLine(line, key, value);
      rid = rf_handle.endRid();

      rf_handle.append(key, value, rid);  
      
      // insert the key,rid into index
      bindex.insert(key, rid);  
    }

    bindex.close();

  // no index
  } else {
    // for every line in file
    for (string line; getline(infile, line);) {
      // get key, value, rid
      parseLoadLine(line, key, value);
      rid = rf_handle.endRid();

      rf_handle.append(key, value, rid);  
    }
  }

  rf_handle.close();
  return 0;
}

RC SqlEngine::parseLoadLine(const string& line, int& key, string& value)
{
    const char *s;
    char        c;
    string::size_type loc;
    
    // ignore beginning white spaces
    c = *(s = line.c_str());
    while (c == ' ' || c == '\t') { c = *++s; }

    // get the integer key value
    key = atoi(s);

    // look for comma
    s = strchr(s, ',');
    if (s == NULL) { return RC_INVALID_FILE_FORMAT; }

    // ignore white spaces
    do { c = *++s; } while (c == ' ' || c == '\t');
    
    // if there is nothing left, set the value to empty string
    if (c == 0) { 
        value.erase();
        return 0;
    }

    // is the value field delimited by ' or "?
    if (c == '\'' || c == '"') {
        s++;
    } else {
        c = '\n';
    }

    // get the value string
    value.assign(s);
    loc = value.find(c, 0);
    if (loc != string::npos) { value.erase(loc); }

    return 0;
}
