#include "LRUCache.h"

#include "helper.h"

Cache::Cache(int maxEntries) : maxEntries(maxEntries) {
}

Cache::~Cache() {
}

void Cache::makeRecent(std::string key) {
    Node* node = cache.at(key);
    list.removeNode(node);
    list.addLast(node);
}

void Cache::addRecent(std::string key, ResponseInfo value) {
    Node* node = new Node(key, value);
    list.addLast(node);
    cache[key] = node;
}

void Cache::deleteKey(std::string key) {
    Node* node = cache.at(key);
    list.removeNode(node);
    cache.erase(key);
    list.deleteNode(node);
}

void Cache::removeLeastRecent() {
    Node* node = list.getFirst();
    std::string deletekey = list.getFirst()->key;
    list.removeFirst();
    cache.erase(deletekey);
    list.deleteNode(node);
}

/**
 * This function checks whether the cache contains a cached response
 * for the given URL.
 * @param key representing the URL.
 * @return If the cache contains a response for the given URL,
 * the function returns true. Otherwise, it returns false.
 */
bool Cache::has(std::string key) {
    std::lock_guard<std::mutex> lock(cacheMutex);
    cleanup();
    if (cache.count(key) > 0) {
        return true;
    } else {
        return false;
    }
}

/**
 * This function retrieves a cached response for the given URL.
 * @param key representing the URL.
 * @return The cached response for the given URL.
 */
ResponseInfo Cache::get(std::string key) {
    std::lock_guard<std::mutex> lock(cacheMutex);
    // cleanup();
    // return cache.at(key);

    makeRecent(key);
    return cache.at(key)->value;
}

/**
 * This function adds a new cached response for the given URL.
 * @param key representing the URL.
 * @param content the response content to be cached.
 * @param maxAge the maximum age of the cached response in seconds.
 * @param lastModified the last modified time of the response, if available.
 * @param eTag the ETag of the response, if available.
 */
void Cache::put(std::string key, ResponseInfo response) {
    std::lock_guard<std::mutex> lock(cacheMutex);
    cleanup();

    auto it = cache.find(key);
    if (it != cache.end()) {
        deleteKey(key);
        addRecent(key, response);
        return;
    }

    if (list.getSize() >= maxEntries) {
        //cache.erase(cache.begin());
        removeLeastRecent();
    }
    addRecent(key, response);
    //cache[key] = response;
}

/**
 * This function removes all entries from the cache.
 */
void Cache::clear() {
    std::lock_guard<std::mutex> lock(cacheMutex);
    cache.clear();
}

/**
 * This function removes all expired entries from the cache.
 */
void Cache::cleanup() {
    std::vector<std::string> expiredKeys;
    TimeMake t;
    for (auto const& item : cache) {
        if (t.timeMinus(item.second->value.expirationTime, t.getTime()) <= 0) {
            expiredKeys.push_back(item.first);
        }
    }

    for (auto const& key : expiredKeys) {
        deleteKey(key);
        //cache.erase(key);
    }
}

/**
 * Validates the cached response for the given key based on the response headers.
 * If the cached response is still valid, the method returns true.
 * Otherwise, it returns false and removes the cached response for the given key.
 *
 * @param key The key for the cached response.
 * @param lastModified the last modified time of the response, if available.
 * @param eTag the ETag of the response, if available.
 * @return True if the cached response is still valid, false otherwise.
 */
bool Cache::validate(std::string key,
                     std::string& request) {  // request -- is request sent by broser
    std::lock_guard<std::mutex> lock(cacheMutex);
    cleanup();
    //  Check if the cache has the requested URL
    if (cache.count(key) == 0) {
        return false;
    }

    ResponseInfo cachedItem = cache.at(key)->value;
    TimeMake t;
    if (t.timeMinus(cachedItem.expirationTime, t.getTime()) <= 0) {
        // Cached response has expired.
        return false;
    }
    if (cachedItem.eTag == "" && cachedItem.lastModified == "") {
        return true;
    }
    if (cachedItem.eTag != "") {
        // ETag mismatch.
        std::string ifNoneMatch = "If-None-Match: " + cachedItem.eTag.append("\r\n");
        std::string request = request.insert(request.length() - 2, ifNoneMatch);
    }

    if (cachedItem.lastModified != "") {
        // Last-Modified time mismatch.
        std::string ifModifiedSince =
            "If-Modified-Since: " + cachedItem.lastModified.append("\r\n");
        std::string request = request.insert(request.length() - 2, ifModifiedSince);
    }
    return false;
}

void Cache::printCache() {
    std::unordered_map<std::string, Node*>::iterator it = cache.begin();
    std::cout << "=================Cache===================" << std::endl;
    while (it != cache.end()) {
        std::cout << "Request: " << it->first << std::endl;
        ++it;
    }
    std::cout << "=================Cache Size===================" << std::endl;
    std::cout << "cache.size=" << cache.size() << std::endl;
    std::cout << "=================Cache End===================" << std::endl;
}

void Cache::useCache(httpcommand req, int client_fd, int thread_id) {
    // Logger logFile;
    std::lock_guard<std::mutex> lock(cacheMutex);
    char res[get(req.url).response.size()];
    strcpy(res, get(req.url).response.c_str());
    send(client_fd, res, get(req.url).response.size(), 0);
    
    makeRecent(req.url);
    
    std::string msg = std::to_string(thread_id) + ": Responding \"" + req.url + "\"";
    Logger::getInstance().log(msg);
}
