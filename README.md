*This project has been created as part of the 42 curriculum by elbrunis.*

<div align="center">

# 🌐 Webserv

**A lightweight, non-blocking HTTP/1.1 server written from scratch in C++98.**

[![Language](https://img.shields.io/badge/language-C%2B%2B98-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)](https://en.cppreference.com/w/cpp/98)
[![Protocol](https://img.shields.io/badge/protocol-HTTP%2F1.1-orange?style=for-the-badge&logo=http&logoColor=white)](https://datatracker.ietf.org/doc/html/rfc9112)
[![I/O](https://img.shields.io/badge/I%2FO-poll()-blueviolet?style=for-the-badge)](https://man7.org/linux/man-pages/man2/poll.2.html)
[![Build](https://img.shields.io/badge/build-Makefile-success?style=for-the-badge&logo=gnu&logoColor=white)](Makefile)
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20macOS-lightgrey?style=for-the-badge&logo=linux&logoColor=white)](#)
[![School](https://img.shields.io/badge/42-Madrid-000000?style=for-the-badge&logo=42&logoColor=white)](https://www.42madrid.com)

[Description](#-description) •
[Features](#-features) •
[Tech Stack](#-tech-stack) •
[Instructions](#-instructions) •
[Configuration](#%EF%B8%8F-configuration) •
[Environment Variables](#-environment-variables) •
[Project Structure](#-project-structure) •
[Roadmap](#%EF%B8%8F-roadmap) •
[Resources](#-resources)

</div>

---

## 📖 Description

**Webserv** is an HTTP server inspired by **NGINX**, built without any external libraries. A single `poll()` event loop drives every socket (listening sockets, clients and CGI pipes), so the server never blocks, never hangs a request and never crashes.

It serves fully static websites, handles file uploads and deletions, executes CGI scripts (Python and PHP), and is configured through an NGINX-style configuration file that supports multiple servers, ports and per-route rules.

> 💡 Point any standard web browser at it — Webserv speaks real HTTP and returns accurate status codes.

---

## ✨ Features

| | Feature | Details |
|:-:|---|---|
| ⚡ | **Non-blocking I/O** | One single `poll()` for all reads & writes, including `listen` sockets and CGI pipes |
| 🔌 | **Multi-port / multi-server** | Listen on several `host:port` pairs at the same time |
| 📨 | **HTTP methods** | `GET`, `POST` and `DELETE` |
| 🧩 | **Incremental request parser** | State machine (`REQUEST_LINE → HEADERS → BODY`) that works with partial reads |
| 📦 | **Chunked transfer encoding** | Requests are de-chunked before being handled or forwarded to CGI |
| 🗂️ | **Static website hosting** | MIME types, index files and optional directory listing (autoindex) |
| 📤 | **File uploads** | Configurable upload directory per route |
| 🐍 | **CGI execution** | Python & PHP, selected by file extension, run in the script's directory |
| ↪️ | **HTTP redirections** | Per-route `return` directive |
| 🚦 | **Accurate status codes** | `200`, `201`, `204`, `301`, `400`, `403`, `404`, `405`, `413`, `414`, `500`, `501`, `504`, `505`… |
| 🎨 | **Custom error pages** | With built-in default pages as fallback |
| 🛡️ | **Body size limit** | `client_max_body_size` per server |
| ⏱️ | **Timeouts** | Idle clients and stuck CGI processes are never left hanging |
| 🧱 | **Robustness** | Graceful handling of client disconnects, malformed requests and resource exhaustion |

---

## 🛠 Tech Stack

| Layer | Technology | Purpose |
|---|---|---|
| 💻 Language | **C++98** | Core implementation, compiled with `-Wall -Wextra -Werror -std=c++98` |
| 🔁 Event loop | **`poll()`** | Readiness notification for every file descriptor |
| 🌍 Networking | **BSD sockets** (`socket`, `bind`, `listen`, `accept`, `recv`, `send`) | TCP connections |
| 🧵 Processes | **`fork` / `execve` / `pipe` / `waitpid`** | CGI execution (only place where `fork` is used) |
| 🐍 CGI runtimes | **Python 3**, **php-cgi** | Dynamic content |
| 🏗️ Build | **GNU Make** | Automatic dependency tracking (`-MMD -MP`), no relinking |
| 🧪 Testing | **Python**, `curl`, `telnet`, `siege` | Stress & behaviour tests, compared against NGINX |

---

## 🚀 Instructions

### 1️⃣ Requirements

- A C++ compiler supporting C++98 (`c++`, `g++` or `clang++`)
- `make`
- *(optional)* `python3` and `php-cgi` to run the CGI examples

### 2️⃣ Clone

```bash
git clone <repository-url> webserv
cd webserv
```

### 3️⃣ Build

```bash
make          # builds ./webserv
make clean    # removes object files
make fclean   # removes object files and the binary
make re       # full rebuild
```

### 4️⃣ Run

```bash
./webserv                        # uses config/default.conf
./webserv config/test_cgi.conf   # uses a custom configuration file
```

### 5️⃣ Try it

```bash
# 🌍 Browser
open http://localhost:8080

# 📨 GET
curl -i http://localhost:8080/

# 📤 Upload a file
curl -i -X POST -F "file=@photo.png" http://localhost:8080/uploads

# 🗑️ Delete it
curl -i -X DELETE http://localhost:8080/uploads/photo.png

# 🐍 CGI
curl -i "http://localhost:8080/cgi-bin/hello.py?name=42"

# 📦 Chunked request
curl -i -H "Transfer-Encoding: chunked" --data-binary @big.txt http://localhost:8080/uploads

# 🔧 Raw request with telnet
telnet localhost 8080
GET / HTTP/1.1
Host: localhost

```

### 6️⃣ Stress test

```bash
python3 tests/stress_test.py
siege -b -c 100 -t 30s http://localhost:8080/
```

---

## ⚙️ Configuration

Webserv uses an NGINX-like syntax. Each `server` block can define several `location` blocks.

```nginx
server {
    listen                127.0.0.1:8080;
    server_name           localhost;
    client_max_body_size  10M;

    error_page 403 /error_pages/403.html;
    error_page 404 /error_pages/404.html;
    error_page 413 /error_pages/413.html;
    error_page 500 /error_pages/500.html;

    location / {
        root        ./www;
        index       index.html;
        methods     GET;
        autoindex   off;
    }

    location /uploads {
        root        ./www;
        methods     GET POST DELETE;
        upload_store ./www/uploads;
        autoindex   on;
    }

    location /cgi-bin {
        root        ./;
        methods     GET POST;
        cgi         .py /usr/bin/python3;
        cgi         .php /usr/bin/php-cgi;
    }

    location /old {
        return 301 /;
    }
}
```

| Directive | Context | Description |
|---|---|---|
| `listen` | server | `host:port` to bind (repeatable) |
| `server_name` | server | Name(s) of the virtual server |
| `client_max_body_size` | server | Maximum request body size (`413` if exceeded) |
| `error_page` | server | Custom page for a given status code |
| `root` | location | Filesystem directory mapped to the route |
| `index` | location | Default file served for directories |
| `methods` | location | Allowed HTTP methods (`405` otherwise) |
| `autoindex` | location | Enable / disable directory listing |
| `upload_store` | location | Where uploaded files are saved |
| `cgi` | location | Extension → interpreter mapping |
| `return` | location | HTTP redirection (`code URL`) |

---

## 🔐 Environment Variables

Webserv itself needs no environment variables — everything lives in the configuration file. When running a CGI script, the server exposes the following **CGI/1.1 meta-variables** ([RFC 3875](https://datatracker.ietf.org/doc/html/rfc3875)):

| Variable | Example | Description |
|---|---|---|
| `GATEWAY_INTERFACE` | `CGI/1.1` | CGI version |
| `SERVER_PROTOCOL` | `HTTP/1.1` | Protocol of the request |
| `SERVER_SOFTWARE` | `webserv/1.0` | Server name and version |
| `SERVER_NAME` | `localhost` | Host name of the server |
| `SERVER_PORT` | `8080` | Port that received the request |
| `REQUEST_METHOD` | `POST` | HTTP method |
| `SCRIPT_NAME` | `/cgi-bin/form.php` | Virtual path of the script |
| `SCRIPT_FILENAME` | `/abs/path/cgi-bin/form.php` | Absolute path of the script |
| `PATH_INFO` | `/extra/path` | Extra path after the script name |
| `QUERY_STRING` | `name=42&lang=en` | Everything after `?` in the URI |
| `CONTENT_TYPE` | `application/x-www-form-urlencoded` | Body media type |
| `CONTENT_LENGTH` | `27` | Body size in bytes (after de-chunking) |
| `REMOTE_ADDR` | `127.0.0.1` | Client IP address |
| `REDIRECT_STATUS` | `200` | Required by `php-cgi` |
| `HTTP_*` | `HTTP_USER_AGENT=curl/8.5` | Every request header, upper-cased and prefixed |

The request body is sent to the script through `stdin`; the script's `stdout` becomes the response (EOF marks its end when no `Content-Length` is given).

---

## 📂 Project Structure

```text
webserv/
├── 📄 Makefile                 # Build rules: all, clean, fclean, re
├── 📘 README.md                # You are here
├── 📋 rules.md                 # Subject requirements summary
│
├── 📁 inc/                     # Header files
│   ├── Headers.hpp             # Shared system includes & global constants
│   ├── Server.hpp              # poll() event loop, sockets, client lifecycle
│   ├── Client.hpp              # Per-connection state (buffers, bytes sent…)
│   ├── 📁 Config/
│   │   ├── ConfigParser.hpp    # Tokenizer & parser for .conf files
│   │   ├── ServerConfig.hpp    # `server {}` block model
│   │   └── LocationConfig.hpp  # `location {}` block model
│   ├── 📁 Http/
│   │   ├── HttpParser.hpp      # Incremental request state machine
│   │   ├── HttpRequest.hpp     # Parsed request representation
│   │   └── HttpResponse.hpp    # Response builder (status, headers, body)
│   ├── 📁 Cgi/
│   │   └── CgiHandler.hpp      # fork/execve/pipe CGI runner
│   └── 📁 Utils/
│       ├── Logger.hpp          # Coloured, timestamped logging
│       └── Utils.hpp           # String helpers (trim, to_lower…)
│
├── 📁 src/                     # Implementation (mirrors inc/)
│   ├── main.cpp                # Entry point: load config & start servers
│   ├── Server.cpp
│   ├── Client.cpp
│   ├── 📁 Config/
│   ├── 📁 Http/
│   ├── 📁 Cgi/
│   └── 📁 Utils/
│
├── 📁 config/                  # Ready-to-use configuration files
│   ├── default.conf            # Default configuration
│   └── test_cgi.conf           # CGI showcase
│
├── 📁 cgi-bin/                 # Sample CGI scripts
│   ├── hello.py                # Python CGI
│   └── form.php                # PHP form handler
│
├── 📁 www/                     # Static website root
│   ├── index.html
│   ├── 📁 error_pages/         # 403, 404, 413, 500
│   └── 📁 uploads/             # Upload destination
│
└── 📁 tests/
    └── stress_test.py          # Concurrent load tester
```

---

## 🗺️ Roadmap

- [x] 🔌 Listening socket setup (`socket`, `bind`, `listen`, `SO_REUSEADDR`)
- [x] 🔁 Single `poll()` event loop for every descriptor
- [x] 👥 Non-blocking client lifecycle (accept → read → write → close)
- [x] 🧩 Incremental HTTP request parser (request line, headers, body)
- [x] 📦 Chunked transfer decoding
- [x] ⚙️ NGINX-style configuration parser
- [x] 🌍 Multiple servers & ports
- [x] 📨 `GET`, `POST`, `DELETE`
- [x] 🗂️ Static files, index & autoindex
- [x] 📤 File uploads
- [x] ↪️ Redirections
- [x] 🎨 Default & custom error pages
- [x] 🐍 CGI (Python & PHP)
- [x] ⏱️ Client & CGI timeouts
- [ ] 🍪 **Bonus:** cookies & session management
- [ ] 🧪 **Bonus:** additional CGI interpreters (Perl, Ruby, Bash)
- [ ] 🏷️ Name-based virtual hosts
- [ ] 🔄 HTTP keep-alive connection pooling
- [ ] 🚀 `epoll` / `kqueue` backends

---

## 📚 Resources

- 📄 [RFC 9110 — HTTP Semantics](https://datatracker.ietf.org/doc/html/rfc9110)
- 📄 [RFC 9112 — HTTP/1.1](https://datatracker.ietf.org/doc/html/rfc9112)
- 📄 [RFC 3875 — The Common Gateway Interface (CGI) 1.1](https://datatracker.ietf.org/doc/html/rfc3875)
- 📘 [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
- 🔧 [NGINX documentation](https://nginx.org/en/docs/) — reference behaviour & config syntax
- 🐧 `man 2 poll`, `man 2 socket`, `man 2 accept`, `man 2 recv`, `man 2 send`

### 🤖 AI usage

AI tools were used as a support resource for:

- Clarifying HTTP / CGI RFC details and expected status codes.
- Reviewing edge cases in the request parser.
- Drafting and formatting this README.

All code was written, reviewed and understood by the authors.

---

<div align="center">

Made with ☕ and `poll()` at **42 Madrid**

</div>
