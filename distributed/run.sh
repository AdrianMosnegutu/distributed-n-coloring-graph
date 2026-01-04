#!/bin/bash

make clean;                 # Clean build artefacts
clear;                      # Clear the console
make run;                   # Run the app
make clean;                 # Clean build artefacts
cat graph.out | wl-copy     # Copy the results of `graph.out` to Wayland's clipboard
