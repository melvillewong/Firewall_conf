## Getting Started with Interactive Mode

### Prerequisites

- GCC compiler
- Criterion (for unit testing)
- TCP/IP networking tools (e.g., telnet, netcat for testing)

### Compilation

To build the project locally:

`make`

### Running Server Interactive Mode

`./server.out -i`

## Running Interactive Mode with Docker (optional)

### Prerequisites

- Docker Desktop (or equivalent docker tools)

### Initialise and Running Docker's Container

#### Manually

1. Run Docker Desktop
2. `docker build -t i_firewall .`
3. `docker run -it --rm --name firewall i_firewall`

> Docker build and rebuild image (named i_firewall).

> Docker start and run container (named firewall).

- `-it` = interactive mode
- `--rm` = remove container after exit

#### With Script of Valgrind

1. Run Docker Desktop
2. `./runDocker.sh`

### Running Server Interactive Mode

`./server.out -i`

## Get Started with Connection Mode

### Server Setup

`./server.out 2200`

- `2200` = customised port

### Client Setup

`./client.out localhost 2200 A 1.1.1.1 2`

- `2200` = port to connect
- `A 1.1.1.1 2` = Command

## Running Connection Mode with Docker (optional)

### Server

`./runDocker.sh`

#### Running Server with Connection listening

##### Manually

`./server.out 2200`

##### With Script of Valgrind

`./runS2200`

### Client

> Ensure Docker Container is Running

`docker exec -it firewall /bin/bash`

#### Running Client with Connecting Server

`./client.out localhost 2200 A 1.1.1.1 2`
