# Firewall Configuration Management System

## Overview

This project implements a server-client system for managing a collection of firewall rules. The server maintains the rules and processes requests for adding, deleting, and querying them. The system supports two modes of interaction:

1. Interactive mode (`-i`) where the server accepts commands from standard input.
2. Network mode where a client program communicates with the server using TCP/IP.

The server maximises concurrency and ensures no memory leaks during execution.

---

## Features

The server supports the following commands:

### 1. **List All Requests**

- **Command:** `R`
- **Description:** Lists all requests received by the server in the order they were received.
- **Response Example:**
    ```
    Request 1: A 147.188.192.41 443
    Request 2: C 147.188.192.41 443
    ```

---

### 2. **Add a Firewall Rule**

- **Command:** `A <rule>`
- **Description:** Adds a valid firewall rule to the server's collection.
- **Response:**
    - **`Rule added`** if the rule is valid.
    - **`Invalid rule`** if the rule is malformed.

---

### 3. **Check IP and Port**

- **Command:** `C <IPAddress> <port>`
- **Description:** Checks if a given IP address and port are allowed by the rules. The query is added to the first matching rule.
- **Response:**
    - **`Connection accepted`** if the IP and port match any rule.
    - **`Connection rejected`** if the IP and port do not match any rule.
    - **`Illegal IP address or port specified`** if the inputs are invalid.

---

### 4. **Delete a Firewall Rule**

- **Command:** `D <rule>`
- **Description:** Deletes a rule if it exists exactly as specified in the server's collection.
- **Response:**
    - **`Rule deleted`** if the rule is successfully deleted.
    - **`Rule not found`** if the rule does not exist.
    - **`Rule invalid`** if the rule specification is malformed.

---

### 5. **List All Rules and Queries**

- **Command:** `L`
- **Description:** Lists all stored rules and their associated IP address-port queries.
- **Response Example:**
    ```
    Rule: 147.188.192.41 443
    Query: 147.188.192.41 443
    Rule: 147.188.193.0-147.188.194.255 21-22
    Query: 147.188.193.10 22
    ```

---

### 6. **Handle Illegal Requests**

- **Response:** `Illegal request` for invalid or unsupported commands.

---

## Firewall Rule Format

A firewall rule is defined as:
<IPAddresses> <ports>

- **IPAddresses** can be:
    - A single IP address: `xxx.xxx.xxx.xxx` (e.g., `147.188.192.41`)
    - A range of IP addresses: `<IPAddress1>-<IPAddress2>` (e.g., `147.188.193.0-147.188.194.255`)
- **Ports** can be:
    - A single port: `port` (e.g., `443`)
    - A range of ports: `<port1>-<port2>` (e.g., `21-22`)

---

## System Components

### 1. **Server Program**

The server:

- Maintains a collection of firewall rules.
- Tracks IP address-port pairs matching the rules.
- Keeps a log of all submitted requests.

### 2. **Client Program**

The client sends commands to the server over the network. Usage:
<ClientProgram> <serverHost> <serverPort> <command>

Example:
`./client localhost 2200 A 147.188.193.15 22
`

---

## Interaction Modes

1. **Interactive Mode:**

    - Start the server:
        ```
        ./server -i
        ```
    - Input commands directly into the server.

2. **Network Mode:**
    - Start the server on a specific port:
        ```
        ./server <port>
        ```
    - Use the client program to interact with the server.

---

## Concurrency and Memory Safety

- The server is designed to handle multiple client connections concurrently.
- Implements robust memory management to prevent memory leaks.

---

## Example Usage

### Server Startup

Interactive mode:
`./server -i`

Network mode:
`./server 2200`

### Commands

- Add a rule:
  `A 147.188.192.41 443`

- Check IP and port:
  `C 147.188.192.41 443`

- Delete a rule:
  `D 147.188.192.41 443`

- List all rules:
  `L`

- List all requests:
  `R`

### Client Example

`./client localhost 2200 A 147.188.193.15 22`

---

## Notes

- Ensure the server is running before using the client.
- Input rules and commands must strictly follow the specified formats.
