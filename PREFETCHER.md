1. Describe how your prefetcher works.
My prefetcher is a Next Line + Predicted Next Miss (NL+PNM) prefetcher. What this means is that I have implemented a sequential prefetcher that takes an
input, however, my prefetcher also predicts what the next miss will be and prefetches those lines as well. It does this by checking what the next missed 
line is after an address is called. If the next missed line after an address is called is the same twice in a row, previously then my prefetcher will 
prefetch this consecutively missed line as well. The prefetcher will prefetcher will prefetch the two most recently consecutively missed lines if there is
more than one along with however many lines are requested sequentially as well. 

2. Explain how you chose that prefetch strategy.
I chose this prefetching strategy because many of the prefetching strategies I researched improved prefetching efficiency by finding clever ways to optimize
prefetching with regards to prefetching latency and when lines need to be prefetched in regards to when they may be needed, however, it seemed like the 
simulator implemented in this project would not capitalize on this kind of prefetcher efficiency, or they relied on using the program counter, which again this simulator did not seemd to support. So I chose this one becuase it did not improve prefetcher efficiency using those methods. 

3. Discuss the pros and cons of your prefetch strategy.
Pros: My prefetcher does a good job of handline things like linked lists, where there may be lines that are often called sequentially that are not 
spacially near each other in memory.
Cons: If there is no pattern in how the data is called, or the data is mostly spacially local, then the Predicted Next Miss portion of my prefetcher induces
additional overhead from a normal Sequential prefetcher without much added benefit. 

4. Demonstrate that the prefetcher could be implemented in hardware (this can be
   as simple as pointing to an existing hardware prefetcher using the strategy
   or a paper describing a hypothetical hardware prefetcher which implements
   your strategy).
The paper "The FNL+MMA Instruction Cache Prefetcher" by Andre Seznec details how this prefetcher could work using what the author calls an I-Shadow cache, although the paper mostly focuses on the implementation of a more complicated version of the prefetcher I implemented called the FNL+MMA prefetcher. 

5. Cite any additional sources that you used to develop your prefetcher.
The only source I used to develop my prefetcher was the paper mentioned in question 4.
