# Word frequency

Count the occurrences of each word in a text file concurrently (Partitioning the text and assigning each partition to a thread) using three approaches:

  - All threads use the same global hash table with concurrency control
  - All threads have a local hash table and we merge the results
  - MapReduce (TODO)

## Usage

To compile simply type in a terminal

```bash
  make
```

Then, to run each version

  - `./global-ht.out [options ...] file`
  - `./local-ht.out [options ...] file`
  - TODO

`options` can be

  - `-j, --threads <value>` Number of threads to use
 
