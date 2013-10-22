# Reversi

## Usage and Modifications
Run the program with `./reversi -m 3 -l 3 -s 8 -q -a 1` for the default execution 
Besides the `-l` flag, I also added an `-a` flag to represent the ai version which has 
values 1, 2, and 3 reflecting sequential, parallel for, and fork/join ai, respectively

The default usage of mode 3 is to use the inputted values of -l and -a to calculate
player 1, and to use the default lookahead of 0 and to use the default algorithm.  
This is quite a dirty fix however, and only really works for testing the differences
between algorithms.

To use `run.sh` you must specify flags for lookahead and game size (`-l` and `-s`).
Example usage:
    
```.sh
›./run.sh -l 3 -s 8

```
Run.sh runs the program 10 times with the specified flags for all three algorithms and 
calculates the average speedup.



## Performance
Best controllable performance is achieved using a lookahead of 3 and a board size of 8 with an 
average speedup of ~4 for parallel for (ranges from 3.7 to 4.4) and average speedup of 
~5.3 (ranges from 4.7 - 5.7) for fork/join, averaged over 10 tests.   

Execution speed also increases exponentially with lookahead increases.  A lookahead of 3 
provides an approximate execution time of 0.1 seconds while a lookahead of 6 brings it to 
15 seconds (for sequential).

Changes in game size have a drastic effect on game speed.  For example, a game size of 20 
has an average execution of 23 seconds.  However parallel algorithms average out at 3 seconds.
The problem is that the sporadic ranges of times (0.3 to 9 seconds) for the parallel algorithms
lead wildly incorrect results.  If we were to keep them, speedups range from 5 to 95 times!

## Known Issues
Board sizes of 6, 10 and 14 run through execution but hit an interesting error 
    free(): invalid next size(fast)
probably contributed to the irregularities in gameboard allocation.  Debugging in valgrind
lead to an error in creation and deletion of `invalid read of size 8` when creating the gameboard.
I originally thought this was due to the incorrect deconstructor but that wasn't the issue.  
I was not able to fix this problem...

There seems to be a data race for the parallel algorithms, as the results are a bit more 
unpredictable.  The issue may lie with the assignment of alpha and beta in the algorithm
or the break in the control flow for the for loops...
Nevertheless it may not be a huge issue for the program as the problem 
seems to lie with the weighting algorithm that I implemented. 

Two solutions that I tried to implement for data races was the implementation of a "breakable_range"
that incorporated a blocked range with a volatile bool to cancel execution.  This didn't seem to have
any effect however.  The second solution was to try using atomic<int> as my alpha beta values and/or using
atomic<Move> (Move holds the x and y positions, as well as the heuristic value of the current move) but those
were never implemented correctly.
