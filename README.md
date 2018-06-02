# Coal-Block-Auction

Coal Block Auctioning Problem using Hill Climbing Search

## Problem Statement

The Ministry for Power, Coal, New and Renewable Energy needs help with the auctioning of the
coal blocks whose allotments were cancelled by the Supreme Court. Being the smartest AI
student, the Ministry has approached you for writing software that will take as inputs the different
bids and outputs an allotment that will maximize the government’s revenue. The Ministry has
decided to auction N coal blocks and has asked companies to bid for each block. However, the
companies with aim of maximizing their profit submit bids that combine multiple blocks. The
companies would like to have ownership of as many blocks in a state so as to minimize their costs
for developing the infrastructure. For example, a company might bid 400 (in crores) for blocks 1,
3, 4, and 7, but bid 100 for only 1, 3 and 4.
	
The software should be able to tell the Ministry the bids that should be accepted. The Ministry has decided
not to accept more than one bid from any company and not every company needs to win a bid. A
company can submit any number of bids. It might also happen that some of the coal blocks do
not get allotted at the end of the auction.

The problem is formulated as a state space search for a single agent and implemented hill climbing
with random restarts.

### Input Format:
    A text file that will be provided as command argument.The input format is as follows
    
    T
    N
    B
    C
    cid ncid
    cid nbid bid-value block-id block id ... block id
    cid nbid bid-value block-id block-id ... block-id

Here,

T refers to the wallclock time in minutes that will be given to your software to find a solution. 
N is the number of coal blocks
C is the number of companies; B is the number of bids. The bids of a single company are grouped together. 

The first line is the company id (cid) and the total number of bids made this company (ncid). Each line thereafter corresponds to a bid. Each bid starts with the company id, the number of coal blocks bid by the company in this bid (nbid), the bid-value,and the coal block ids. The bids are implicitly numbered. This is repeated for every company. 

### For example,
```
3
7
2
6
0 3
0 4 400 0 2 3 6
0 3 100 0 2 3
0 3 50 2 3 4
1 3
1 3 300 1 4 5
1 2 200 1 5
1 3 40 1 3 4
```


### Output Format:
```
The output of code (the winning bids) is written to a text file is provided as a command argument in the following format:

revenue bid-id bid-id ... bid-id
```

For the example problem, this would be
```700 0 3```


### Program Execution Format:###

```
g++ final.c
./a.out inputfile outputfile
```

### Formulation of the problem
Hill climbing search is used to optimise the problem with following modifications. 

#### State:
A state in coal block auctioning, represents an assignment of 1’s and 0’s to the total number of bids. So, each state represents whether a particular bid is taken to count the profit or not.   

#### State Representation:
Assignment of 1: Bid is included
Assignment of 0: Bid is not included 

#### Initial State:
Random bit generation of 1’s and 0’s of the total bids with probability of 0.5. 

#### Consistent State:
A state is consistent, if for the state assignment it follows the below constraints:
1. Only one bid from one company
2. No two bids assign the same blocks

#### Neighbours:
Next state could be obtained by changing one bit of any of the bids. So, if a bid is taken it is converted to not taken and vice versa.

#### Final State:
A consistent state with maximum profit is our output

#### Algorithm:
1. Start timer with an initial random state.
2. Check if state is consistent. If not, search for a consistent state.
If we aren’t able to find a consistent state in some given time, the probability of assigning 1 to the bids is decreased, so as to get a consistent assignment (to remove moving in infinite loops). It is like simulated annealing where the number of random restarts decreases with time.
3. For the new consistent state get all the neighbours 
4. Pick the neighbour which is consistent and has highest profit value
5. Repeat 4 and 5 for a given time interval
6. Check for the profit. If profit is more than previous iteration. Update the new profit.
7. Compute the time left. If still there is time remaining. Repeat 2 to 7.


By repeating steps from 2 to 7, we ensure random restarts are done in hill climbing to reach the optimal state. For increasing number of iterations, the time available for search reduces.
