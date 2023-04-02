// add ifndef
#ifndef __LRUCACHE__H__
#define __LRUCACHE__H__

#include <unordered_map>
#include <mutex>
#include <string>

#include "ResponseInfo.h"
#include "helper.h"
#include "httpcommand.h"

class Node {
public:
    std::string key;
    ResponseInfo value;
    Node *prev, *next;

    Node() {}
    Node(std::string _key, ResponseInfo _val) : key(_key), value(_val) {
        prev = NULL;
        next = NULL;
    }
};

class DoubleLinkedList {
public:
    Node *head, *tail;
    int size;

    DoubleLinkedList() {
        ResponseInfo *head_r = new ResponseInfo();
        ResponseInfo *tail_r = new ResponseInfo();
        head = new Node("head", *head_r);
        tail = new Node("tail", *tail_r);
        tail->prev = head;
        head->next = tail;
        size = 0;
    }

    void addLast(Node *toAdd) {
        tail->prev->next = toAdd;
        toAdd->next = tail;
        toAdd->prev = tail->prev;
        tail->prev = toAdd;
        size++;
    }

    // node must exist
    void removeNode(Node *node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        node->prev = NULL;
        node->next = NULL;
        size--;
        //delete node;
    }
    void deleteNode(Node* node) {
        delete node;
    }

    Node* getFirst() {
        return head->next;
    }

    void removeFirst() {
        if (head->next == tail) {
            return;
        } 
        Node *first = head->next;
        removeNode(first);
    }

    int getSize() {
        return size;
    }
};

class Cache {
 private: 
  std::unordered_map<std::string, Node*> cache;  // stores cached response for each URL，key is URL
  DoubleLinkedList list;

  int maxEntries;  // maximum number of entries allowed in the cache
  std::mutex cacheMutex;
  void cleanup();  // helper function to remove expired entries from the cache

 public:
  Cache(int maxEntries);
  ~Cache();
  // checks whether the cache contains a cached response for the given URL
  bool has(std::string key);

  // returns the cached response for the given URL
  ResponseInfo get(std::string key);

  // caches the response for the given URL
  // the response will be removed from the cache after maxAge seconds
  void put(std::string key, ResponseInfo response);

  // removes all entries from the cache
  void clear();

  bool validate(std::string key, std::string & request);

  void printCache();

  void useCache(httpcommand req, int client_fd, int thread_id);

  void makeRecent(std::string key);
  void addRecent(std::string key, ResponseInfo response);
  void deleteKey(std::string key);
  void removeLeastRecent();

};

#endif
