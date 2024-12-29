# Continue: 
    - edit page template should fetch values from web.c input

# Make sure to have NULL checks and check return values in source
    - There are functions that return status code and/or NULL values. Act accordingly! ✓

# Change DB
    - MongoDB → SQLite

# Migrate FSDB
    - FSDB → SQL

# Data structure
    - AoS -> SoA
    - No dynamic memory

# Clean up buttons to reflect only avaliable options
    - Only have relevant buttons on each page

# Memory leaks
    - get_xxx() returns strcpy if timer is running, else the actual string from timer result
