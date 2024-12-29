# Continue: 
    - edit page template should fetch values from web.c input
    - trackme is responsible for handling timer -> main interacts by trackMe and not directly with timer.c
    - naming for get_time_xxx{,_str} - from timer is int and from timerResult is string?

# Improve tests
    - Validate all tests - make sure they make sense

# clean up unit test init StartInfo
    - test names could probably be merged

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
    - strings returned from timer or timerresult when error is not cleaned up by stop_timer()
    - get_xxx() returns strcpy if timer is running, else the actual string from timer result
