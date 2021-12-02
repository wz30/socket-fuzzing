### 12/2/2021
- plan to only use single input for read and recv
- plan to use slim socket for a simple check.
  - which api should be picked
- plan to cross check them
#### summary: 
- slim and regular socket works, need to merge togethre in fuzzer
- enable slim work by dlsym and dlopn to rewrite the function.
- interesting issue when using slimsocket, there are many fialed connections: performance issue?

