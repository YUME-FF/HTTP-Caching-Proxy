# HTTP-Caching-Proxy

For this assignment you will be writing an http proxy – a server whose job it is to forward requests to the origin server on behalf of the client. Your proxy will cache responses, and, when appropriate, respond with the cached copy of a resource rather than re-fetching it.

# Table of Contents

- [1 Review](#1-Review)
  - [1.1 Socket](#1-.-1Socket)
   - [1.1.1 Socket Communication Basic](#1-.-1-.-1-Socket-Communication-Basic)
- [2 Proxy Design](#2-Proxy-Design)
- [3 Implementation](#3-Implementation)

## 1 Review

### 1.1 Socket

#### 1.1.1 Socket Communication Basic

**What should Server do:** 
- Create a ServerSocket object and bind the listening port
- Call the accept() method to monitor the client's request
- After the connection is established, read the request information sent by the client through the input stream
- Send response information to the client through the output stream
- Close related resources

**What should Client do:**
- Create a Socket object, indicating the address and port number of the server to be linked
- After the link is established, send request information to the server through the output stream
- Get the server response information through the output stream
- Close related resources

```mermaid
sequenceDiagram
    participant S as Server
    participant C as Client
    autonumber
    Note over S: Create ServerSocket
    S-->>C: Wait for request
    C->>S: Creat socket to request to connet
    S-->>C: Accept connection on socket //accept()
    rect rgb(200, 150, 255)
    loop Messages
        Note over S,C: InputStream
        Note over S,C: outputStream
        S-->C: Communication
    end
    end
    S-->C: Close socket
```

## 2 Proxy Design
## 3 Implementation