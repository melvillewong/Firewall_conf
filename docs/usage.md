## Getting Started

### Prerequisites

- GCC compiler
- Criterion (for unit testing)
- TCP/IP networking tools (e.g., telnet, netcat for testing)

### Compilation

To build the project locally:

`make`

### Running Server Interactive Mode

`./server.out -i`

## Running with Docker (optional)

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

#### With script

1. Run Docker Desktop
2. `./runDocker.sh`

### Running Server Interactive Mode

`./server.out -i`
