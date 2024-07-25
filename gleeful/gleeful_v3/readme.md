# About

---
Author: Eleanor Waiss  
Date: 22 July 2024  
Language: C++  
Files:

- `bigint.h`
- `cprimes.h`
- `gleeful.h`
- `int.h`
- `main.cpp`
- `parameters.h`
- `precomp.h`
- `readme.md`
- `readme.pdf`
- `rollsieve.h`
- `stack.h`

---
This program enumerates a histogram of the number of gleeful representations for all numbers less than or equal to some bound. The program is driven from `main.cpp` using precomputed parameters from `precomp.cpp`, and outputs the histogram and runtime data to the console.

From a high level, this program contains a hash table (`unordered_map`) of `gleeful` objects and successively looks into the hash table for larger and larger gleeful representations. When it finds a representation, it removes this representation from the hash table, converts the `gleeful` object to the next `sum` of the same `length`, and reinserts the object into the table if it is less than `bound`. In doing so, we guarantee every element in the hash table is larger than the last element we removed; this also has the added benefit of shrinking our table as gleeful lengths grow too large and exceed `bound`. This description is slightly misleading; the exact implementation herein has been optimized to keep the movement of data to a minimum, and the hash table is only used to guarantee O(1) lookup as the main loop iterates over `sum`, not `length`.

## `bigint.h`

foo

## `cprimes.h`

foo

## `gleeful.h`

A custom class that defines a `gleeful` object. Mathematically, this is one representation for a number as gleeful, i.e. the sum of consecutive primes. There are two categories of gleeful objects that affect how the class data gets handled and modified within `main.cpp`: if a `gleeful` is long (i.e. not short), then the individual primes used in the summand remain small enough that using the pregenerated list of prime gaps is sufficient (see: `cprimes.h` and `soe`), hence we only require metadata about the first and last prime used in the summand. If, however, the length of the gleeful representation is short, then the individual primes in the summand will exceed the largest prime given by `soe`, thus we implement a unique rolling prime sieve for each of these short gleeful representations. Note, however, the rolling sieve "forgets" primes it's already outputted, thus we store these primes in a queue of fixed length so we can "remember" intermediary primes when they're needed.

> #### **Why do we need to categorize `gleeful` objects as short or long?**  
> In early test code, we are able to use a pregenerated list of primes for every gleeful number as our `bound` was small enough to fit this list in memory. However, as our `bound` grows, the memory required to store orders of magnitude more of exact primes quickly outgrew our machine's capability, thus we reduce the total space required by shortening `sieveSize` to about `bound`^(2/3). There are some gleeful lengths (those that exceed `sizeShort`) that are still able to use this smaller sieve as the largest prime they can use without exceeding `bound` is bounded above by `sieveSize`, else we include a unique rolling prime sieve (see: *Sorenson [????]*) that outputs the **next** prime up to any arbitrary limit. This `rollsieve`, however, only outputs the desired prime once before irreversibly moving to a different state that "forgets" previous primes, thus because we need to know the lowest prime used in each `gleeful` object to compute the next correct `sum`, we need to store a queue of all primes used (see: `list`).

### Data

- `const bool isShort`: A flag for if this gleeful is small, i.e. its length is less than or equal to `sizeShort`.
- `uint64 sum`: The number of study as a gleeful representation, i.e. the sum of all primes between `minP` and `maxP`, inclusive.
- `const uint32 len`: The length of this representation; equivalent to the following:
  - `maxPI - minPI + 1` (if long)
  - `list.len()` (if short)
- `uint32 next`: The table position of the a `gleeful` object of the same sum, in effect creating an embedded linked list data structure.
- `uint64 minP`: The value of the smallest prime in the representation (only used on long gleefuls).
- `uint64 maxP`: The value of the largest prime in the representation (only used on long gleefuls).
- `uint64 minPI`: The value of the prime index of `minP`; i.e. the prime 2 has index 1, the prime 3 has index 2, the prime 5 has index 3, etc (only used on long gleefuls).
- `uint64 maxPI`: The value of the prime index of `maxP`; see `minPI` (only used on long gleefuls).
- `queue<uint64_t> list`: A queue that contains exact primes (only used on short gleefuls).
- `rollsieve roll`: A rolling prime sieve that outputs exact primes (only used on short gleefuls).

### Methods

foo

## `int.h`

foo

## `main.cpp`

The driver file runs in two phases: table implementation ("initialization"), and the "main loop" for representation counting. The first phase is quick, while the main loop is the bottle neck for this program.

Defines the following global variables:

- `CPrimes soe`: The list of prime gaps up to `sieveSize`, computed at runtime.
- `vector<gleeful> happy`: The vector (of fixed size `numGleefuls + 1`) storing all gleeful representations of interest. The `gleeful` object stored at position 1 has length `1`, the object stored in position 2 has length `2`, etc. This is to help prevent off-by-one errors when modifying the objects in the table, but hence there is an additional `gleeful` object allocated in position `0` that goes unused.
- `unordered_map<uint64, uint32> h`: The hash table for `gleeful` objects, keyed on `sum` holding the value `len`. We don't actually need the full functionality of the hash table (such as collision management), we only require O(1) lookup, insertion, and deletion.

> #### **Why do we require O(1) lookup, insertion, and deletion?**  
> While the `happy[]` table is indexed based on `len`, our algorithm iterates over `sum`, which has the unfortunate consequence of occuring "randomly" along `happy[]` indexes (in the non-mathematical sense of the word). As such, by keying `sum` on value `len` (which is also the `happy[]` index), we are able to immediately access the correct position within `happy[]` without needing to interate over the entire `happy[]` table to look for the minimum `sum`.

### Initialization Phase

During the initialization phase, the list of prime gaps up to `sieveSize` is computed using a modified Sieve of Eratosthenes, which is saved to `CPrimes soe`. Using a rolling sieve that outputs exact primes up to `sieveSize`, both `numGleefuls` and `sizeShort` is computed (the former by adding small primes together until they exceed `bound`, the latter by adding large primes until they exceed `bound`. Starting with `len=1` and beginning the sum at the smallest prime, `2`, each element in `happy[]` is specified with a `sum`, `len`, and `maxP`. If the gleeful number is "short", i.e. the individual primes will exceed the largest prime given by `soe`, then `roll` is also initialized, and `list` is filled with the prime gaps given by moving `roll` forward `len` primes. If the gleeful number is "long", then there are too many primes used in the summand which bounds the largest possible prime in this representation of fixed length above by `sieveSize`, so the pregenerated list of prime gaps (`soe`) is sufficient and we additionally initialize with `minPI` and `maxPI`. Finally, all `gleeful` objects from `happy[]` are inserted into  `h`.

### Main Loop

Beginning at `s = 1`, we search for `s` in `h`. If no element is found, there are no representations for `s`, which gets noted and we increment `s`. Else, we have found `s` in `h` and thus there is at least one gleeful representation for `s`, which we call `currObj`. We remove `currObj` from `h`. If `currObj` is long, we add the next gap to `maxP`, replace `sum` with `sum + maxP - minP`, and add the appropriate gap to `minP`. If `currObj` is short, we save the value given by `roll.nextprime()`, add that saved value to `maxP`, replace `sum` with `sum + maxP - minP`, replace `minP` with `minP + list.dequeue()`, and call `list.enqueue()` on the saved prime gap from `roll`. Note we do not require the prime indexes `minPI` and `maxPI` for short `gleeful` objects. Finally, we set `next` to `0` (copying whatever was there for later), and re-insert this gleeful representation into `h` (see next paragraph). If `next` was not `0` before the previous stage, we repeat this process for the gleeful object in `happy[]` at the location we saved from `next`, keeping note of how many times we must repeat this process. Once we reach a gleeful object with `next=0`, then we have exhausted all gleeful representations for this `s`, note that amount of cycles in the histogram data, and increment `s`.

To insert a `gleeful` object into `h`, we first look into `h` to see if there is already an object with the same `sum`. If not, we can insert with no issues. If there is a repeated `sum`, we remove that object from the table, set our original `next` to the `len` of the removed object (i.e. the `happy[]` position of the removed object), and now are clear to insert the original object without causing a collision. In doing so, we form a makeshift linked list based on the values stored in `next` which hold `happy[]` indexes.

## `parameters.h`

Allocates and defines the following global variables:

- `const uint64 bound`: The largest number whose gleeful representations are computed and the parameter for which all other values are derived.
- `const uint64 sieveSize`: The number for which all primes less than this are explicitly computed
- `const uint32 numGleefuls`: The number of `gleeful` objects to allocate in the `happy[]` vector
- `const uint32 sizeShort`: The maximum length allowable for which all larger gleeful representations of a longer length are able to use the pregenerated list of exaxt primes

Also see: `precomp.cpp`.

## `precomp.cpp`

This short program is designed to be run before compiling and running `main.cpp`. It takes in two parameters, namely `bound` and `sieveSize`, and outputs a `paramaters.h` file containing these two values as well as two computed values needed at compile time for `main.cpp`. 

Allocates and explicitly defines the following global variables:

- `const uint64 bound`: The largest number whose gleeful representations are computed and the parameter for which all other values are derived.
- `const uint64 sieveSize`: The number for which all primes less than this are explicitly computed

Allocates and computes the following global variables:

- `const uint32 numGleefuls`: The number of `gleeful` objects to allocate in the `happy[]` vector
- `const uint32 sizeShort`: The maximum length allowable for which all larger gleeful representations of a longer length are able to use the pregenerated list of exaxt primes.

To modify the settings for `main.cpp`, adjust the values for `bound` and `sieveSize` in `precomp.cpp`, compile and run `precomp.cpp`, thus overriding the file `parameters.h` *before* compiling and running `main.cpp`.

## `rollsieve.h`

foo

## `stack.h`

foo
