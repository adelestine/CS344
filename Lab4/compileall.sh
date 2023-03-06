#!/bin/bash
gcc -std=c99 -o enc_client enc_client.c extras.c
gcc -std=c99 -o enc_server enc_server.c extras.c
gcc -std=c99 -o dec_client dec_client.c extras.c
gcc -std=c99 -o dec_server dec_server.c extras.c
gcc -std=c99 -o keygen keygen.c
echo Done compiling