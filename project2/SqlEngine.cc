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
#include <set>

// student defined test suite
// #include "Test.h"

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

// struct containing all value constraints
class VC {
  public:
    VC():i_upper_set(false), i_lower_set(false), e_upper_set(false), e_lower_set(false), equals_set(false) {};
    string i_upper;
    string i_lower;
    string e_upper; 
    string e_lower; 
    string equals;
    set<string> not_equals;
    bool i_upper_set;
    bool i_lower_set;
    bool e_upper_set;
    bool e_lower_set; 
    bool equals_set;
};

// reads in the value at rid and checks if it satisfies the value constraints
bool check_value(const RecordId& rid, const VC& vc, const RecordFile& rf, string& val) {
  int key;
  
  // reads rid to get string
  rf.read(rid, key, val);

  // checks string against the constraints
  if (vc.equals_set && strcmp(val.c_str(), (vc.equals).c_str()) != 0) {
    return false;
  }
  if (vc.e_upper_set && strcmp(val.c_str(), (vc.e_upper).c_str()) >= 0) {
    return false;
  }
  if (vc.e_lower_set && strcmp(val.c_str(), (vc.e_lower).c_str()) <= 0) {
    return false;
  }
  if (vc.i_upper_set && strcmp(val.c_str(), (vc.i_upper).c_str()) > 0) {
    return false;
  }
  if (vc.i_lower_set && strcmp(val.c_str(), (vc.i_lower).c_str()) < 0) {
    return false;
  }
  if ((vc.not_equals).find(val) != (vc.not_equals).end()) {
    return false;
  }

  return true;
}

void query_print(const int& attr, int key, const RecordId& rid, const RecordFile& rf, int& count, const VC& vc) {
  
  string val;
  RC rc;
  
  // if there are value restrictions
  if (vc.i_upper_set || vc.i_lower_set || vc.e_upper_set || vc.e_lower_set || vc.equals_set || !vc.not_equals.empty()) {
    // get the string from this too so we don't need to do multiple reads
    if(!check_value(rid, vc, rf, val)) {
      return;
    }

  // no value restrictions but we need val to be read in
  } else if (attr == 2 || attr == 3) {
      rc = rf.read(rid, key, val);
      if (rc != 0) {
        // cout << "error when reading value" << endl;
        return;
      }
  }

  // Based on the case given by attr, we print the key/value
  switch (attr) {
    // key
    case 1:
      cout << key << endl;
      break;
    // value
    case 2:
      cout << val << endl;
      break;
    // *
    case 3:
      cout << key << " '" << val << "'" << endl;
      break;
    case 4:
      count++;
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
  set<int> not_equals_keys; 

  bool upper_key_set = false, lower_key_set = false, equals_key_set = false;
  // value constraints struct zero'd out
  VC vc;
 
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
    not_equals_keys.insert(atoi(cond[i].value));
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
    if (vc.equals_set) {
      // If two differing equals values, then result is empty set
      if (vc.equals != cond[i].value) {
        empty_set = true;
      }

    // else (first equals condition)
    } else {
      // Set equals_value and equals_value_set
      vc.equals = cond[i].value;
      vc.equals_set = true;
    }
    break;
  case SelCond::NE:
    // add to not_equals_values vector
    vc.not_equals.insert(cond[i].value);
    break;
  case SelCond::LT:
    // e_upper_value is already set
    if (vc.e_upper_set) {
      // If more restricted range, then update e_upper_value
      if (strcmp(cond[i].value, vc.e_upper.c_str()) < 0) {
        vc.e_upper = cond[i].value;
      }

    // else (first < condition)
    } else {
      // Set e_upper_value and e_upper_value_set
      vc.e_upper = cond[i].value;
      vc.e_upper_set = true;	    
    }
    break;
  case SelCond::GT:
    // e_lower_value is already set
    if (vc.e_lower_set) {
      // If more restricted range, then update e_lower_value
      if (strcmp(cond[i].value, vc.e_lower.c_str()) > 0) {
        vc.e_lower = cond[i].value;
      }

    // else (first > condition)
    } else {
      // Set e_lower_value and e_lower_value_set
      vc.e_lower = cond[i].value;
      vc.e_lower_set = true;
    }
    break;
  case SelCond::LE:
    // i_upper_value is already set
    if (vc.i_upper_set) {
      // If more restricted range, then update i_upper_value
      if (strcmp(cond[i].value, vc.i_upper.c_str()) < 0) {
        vc.i_upper = cond[i].value;
      }

    // else (first <= condition)
    } else {
      // Set i_upper_value and i_upper_value_set
      vc.i_upper = cond[i].value;
      vc.i_upper_set = true;
    }
    break;
  case SelCond::GE:
    // i_lower_value is already set
    if (vc.i_lower_set) {
      // If more restricted range, then update i_lower_value
      if (strcmp(cond[i].value, vc.i_lower.c_str()) > 0) {
        vc.i_lower = cond[i].value;
      }

    // else (first >= condition)
    } else {
      // Set i_lower_value and i_lower_value_set
      vc.i_lower = cond[i].value;
      vc.i_lower_set = true;
    }
    break;
  }
    }
  }

  // check ranges - like if > and < mismatch, etc.
  if (equals_key_set && upper_key_set && equals_key > upper_key) {
    empty_set = true;
  } else if (equals_key_set && lower_key_set && equals_key < lower_key) {
    empty_set = true;
  } else if (upper_key_set && lower_key_set && upper_key < lower_key) {
    empty_set = true;
  } else if (vc.equals_set && vc.e_upper_set && strcmp(vc.equals.c_str(), vc.e_upper.c_str()) >= 0) {
    empty_set = true;
  } else if (vc.equals_set && vc.e_lower_set && strcmp(vc.equals.c_str(), vc.e_lower.c_str()) <= 0) {
    empty_set = true;
  } else if (vc.equals_set && vc.i_upper_set && strcmp(vc.equals.c_str(), vc.i_upper.c_str()) > 0) {
    empty_set = true;
  } else if (vc.equals_set && vc.i_lower_set && strcmp(vc.equals.c_str(), vc.i_lower.c_str()) < 0) {
    empty_set = true;
  } else if (vc.e_upper_set && vc.e_lower_set && strcmp(vc.e_upper.c_str(), vc.e_lower.c_str()) <= 0) {
    empty_set = true;
  } else if (vc.i_upper_set && vc.i_lower_set && strcmp(vc.i_upper.c_str(), vc.i_lower.c_str()) < 0) {
    empty_set = true;
  } else if (equals_key_set && not_equals_keys.find(equals_key) != not_equals_keys.end()) {
    empty_set = true;
  } else if (vc.equals_set && (vc.not_equals).find(vc.equals) != (vc.not_equals).end()) {
    empty_set = true;
  }


  // if empty set...
  if (empty_set) {
    // cout << "REMOVE THIS BEFORE SUBMISSION\nEmpty set, return 0 automatically\n";
    return 0;
  }
  // if idx exists and there are any conditions on the key or count(*) or key
  if (idx_file.good() && (attr == 1 || attr == 4 || upper_key_set || lower_key_set || equals_key_set || !not_equals_keys.empty())) {
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
        // cout << "equals locate error/not found" << endl;
        return 0;
      } 

      rc = idx.readForward(cursor, curr_key, curr_rid);
 
      if (rc != 0) {
        // cout << "read forward error" << endl;
        return 0;
      }
       
      query_print(attr, curr_key, curr_rid, rf, count, vc);

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
        // cout << "readForward error/empty set?" << endl;
        rf.close();
        idx.close();
        return rc;
      }      
 
      // while curr_key is within bounds
      while (curr_key <= upper_key) {
      // if curr_key is not part of not_equals_keys then we print it  
      if (not_equals_keys.find(curr_key) == not_equals_keys.end()) {
          // print checked key
          query_print(attr, curr_key, curr_rid, rf, count, vc);
        }
        // read next
        rc = idx.readForward(cursor, curr_key, curr_rid);         
        
        if (rc == RC_END_OF_TREE) {
          // cout << "Done!" << endl;
          break;
        }

        if (rc != 0) {
          // cout << "read forward error" << endl;
          break;
        } 
      }
    }

    // COUNT(*) query
    if (attr == 4) {
      cout << count << endl;
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
  // if (table == "test") {
  //   Test* t = new Test();
  //   t->beginTests();
  //   return 0;
  // }  

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
