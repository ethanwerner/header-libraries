# C Header Libraries

The following libraries are *highly* experimental and should be treated as such. The intent of this repository is archival in nature. Although I do use many of the following libraries in my projects, they are not production ready.

## ann.h - Artifical Neural Network

This library provides an implementation of a fully connected, artifical neural network with backpropagation. The entirety of the ANN is stored in a single memory allocation, allowing for ease in persisting models and training via external methods.

### Activation Functions

The activation function for hidden layers can be chosen independently from that used for the output layer.

- Identity
- Binary
- Sigmoid
- RELU
- ELU
- LRELU
- Tanh

## bin.h - Flat File Block Storage

This library is an experiment in storing ordered numerical data as binary "flat files". The contained data must be of a fixed block size. The primary goal of the library is to provide a simple method of persisting and caching data that is time-series in nature. The resulting files should be short lived. 

### Format

The file header contains two 64 bit unsigned integers, block count and block size. The rest of the file contains the block data. 

Each block must possess a numerical key/index as the first element. Prior to using the library, a ```typedef [numerical_type] bin_key_t``` must be declared in the implementation file.

**Operations:** read, write, insert, append, search, and fuzzy search

This is a naive library, and as such, does not provide input validation. If data is placed out of order, *there it will remain.*

## escape.h - ANSI Escape Codes

This library provides utility functions/macros for an assortment of ANSI escape codes.

## flat.h - Directory Traversal

This library provides a simple wrapper for traversing POSIX file systems. The intention for this library is to be used alongside bin.h, for the purpose of traversing hierarchial directory structures.

## genetic.h - Genetic Algorithm

An implementation of a genetic algorithm. The library exposes a single function for creating a new generation of individuals. The function is "generic", meaning that the respective operations for crossover, mutation, and fitness are passed as as function pointers.

### Algorithm Overview

**1. Crossover**

- Determine the selection cutoff for the population.
- Replace all unfit individuals by preforming a crossover operation with two randomly selected fit indivuals.

**2. Mutation**

- Determine the number of mutation operations, and perform those on a randomly selected individual.

**3. Fitness**

- Sort the population by descending fitness, in preparation for the next generation.

## pool.h - Memory Pool

A free-list based memory pool. The pool has a fixed size after allocation.

**Note:** The size of a block must be greater than or equal to ```sizeof(void *)```

## stack.h - Stack Implementation

A stack implementation to be used with pushdown.h

**Note:** A pop operation on an empty stack will return 0/NULL.

## pushdown.h - A Pushdown/Stack Automata

This file provides an implementation of a pushdown/stack automata.

## ufo.h - Universal Functions Originator

 An implementation of the symbolic regression technique known as a Universal Functions Originator. The originating paper can be found [here](https://doi.org/10.1016/j.asoc.2020.106417).

## uri.h - URI Builder

A simple, and likely incomplete, implementation of a "builder" pattern to generate URIs as defined by [RFC 3986](https://datatracker.ietf.org/doc/html/rfc3986). The URIs are stored interally as separate blocks, enabling sequential URIs to be generated with ease.
