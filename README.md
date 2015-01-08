RaceTrack Cache Monitor
===

###1. Architecture:

It's a 3 layers architecture:

1. 0 level: Containing **file i/o** and **cache components**
2. 1 level: Combining the 0 level together, and building different RaceTrack related mechanisms here, such as the fast way method.
3. 2 level: Optimizing the whole project. Input configuration parameters and read the performance result, such as the runtime. After gathered related data, should output the best choice of parameters.

####1.1 Cache Component

This cache is dynamically configured. 

If we view this cache as a black box:

1. **Input**: operations and time ticks
2. **Output**: finish time ticks
3. **Inside the box**: operation results, including the change of data in cache, and the position shift of RaceTrack.

Cache itself is a **finite-state machine**. So the way we implement contains these 2 parts:

1. **States**: a cache state is the common way we're describing the cache. Such as cache lines, tags and data in each cache line and cache block, and so on. For RaceTrack cache, things are slightly different:
	
	RaceTrack material is quite **independent** to the cache, which means, no matter where the material's position is, it will only affect the time, but the state of the cache itself will never change.
    
    In the end, we'll split the RaceTrack state out, divide the whole state into 2 parts:
    
    1. **Cache States**: the original cache states
    	1. **Inactive**
    	2. **Comparing Marks**: Marks include the cache tags and the valid bits
    	3. **Allocation**
    	4. **Write-back**
    2. **RaceTrack States**: the state of the RaceTrack material:
    	1. **Inactive**
    	2. ****
        
2. **Next-state Function**: Unlike the **state representation** part, the 

###1.2 File I/O

Read the test data in. Output a list of data lines, with these parts inside:

1. operation: including name, type and parameters
2. time tick
