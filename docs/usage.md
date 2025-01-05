## Getting Started

### Prerequisites

- GCC compiler
- Criterion (for unit testing)
- TCP/IP networking tools (e.g., telnet, netcat for testing)
- Docker (optional)

### Compilation

To build the project:

```bash
make
```

### Running with Docker

1. Run Docker Desktop
2. `docker build -t i_firewall .`
3. `docker run -it --rm --name firewall i_firewall`

> Docker build and rebuild image (named i_firewall).
> Docker start and run container (named firewall).

    - `-it` = interactive mode
    - `--rm` = remove container after exit

### Running Server Interactive Mode

1. valgrind ./main.out -i

### Testing

`./test.out`
