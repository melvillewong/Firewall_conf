## Running Docker

1. Run Docker Desktop
2. `docker build -t i_firewall .`
3. `docker run -it --rm --name firewall i_firewall`

> Docker build and rebuild image (named i_firewall).
> Docker start and run container (named firewall).

    - `-it` = interactive mode
    - `--rm` = remove container after exit

## Running Server Interactive Mode

1. valgrind ./main -i
