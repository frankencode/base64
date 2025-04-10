#!/bin/bash -ex
clear && cc base64_example.c base64.c -Wall -o base64_example && ./base64_example
