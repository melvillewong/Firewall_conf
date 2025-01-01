#!/bin/bash

docker build -t i_firewall .
docker run -it --rm --name firewall i_firewall
