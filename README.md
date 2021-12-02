# socket-fuzzing
fuzzing network stack(socket) library

## ToDo list
- [x] fuzzer tutorial
- [x] run basic server and client example
- [ ] only fuzzing client codes, server is up 
    - [ ] no input   
- [ ] socket api list
    - [ ] socket, connect, listen, epoll 
- [ ] expected behavior in Fuzzer 
    - [ ] verify the behaviors before connection setup between nodes on server/client end
    - [ ] verify the behaviors after socket connecton is setup between nodes on server/client end
    - [ ] compare behaviors in linux socket and customized network stack library(such as slim, MTCP): cross check in libfuzzer
    - [ ] how to pass in multiple paramaters in libfizzer? Currently, the fuzzer api only provides chars array
    - [ ] How to setup fuzzer to work with third party library?
- [ ] design ACE-like framework to automatically generate correct workflows
    - [ ] what is a correct workflow? what are the variations?
    - [ ] how many socket apis are picked to generate the workflow: a meaningful combinaiton of socket API.
    - [ ] Do we generate C codes directly or use an intermediate layer like ACE? So far, not sure why ACE use an intermediate layer.
    - [ ] If there is no bug in normal workflow, what others api(e.g. fork, epoll) are necessary to include? To be specific, how many threads do we need to fork? how often do we need to fork them? Which port do i need to pick (reuse the port or start new port)?
- [ ] Search space for the workflow when fuzzing
    - [ ] which meaningful paramaters in socket API are needed to changed/fuzzed
    - [ ] what are the classic problems in socket? search famous bugs in linux socket library
    - [ ] when developers/company create their own network stack, what could be the common issues?
    - [ ] Can SYN flood attacks apply into socket libraray
- [ ] issue with simple fuzzer which just creates socket and always fail
- [ ] generate correct workflows: correct server and client behaviors
- [ ] test workflows

### Expected results
- [ ] find at least one bug in Slim socket api.  Danyang mentioned epoll would be a good choice
- [ ] find more bugs in other libraries such as MTCP
- [ ] Build the framework that is easier to use with any third party library. (least priority)

### Mini fuzzer
- simple fuzzer with corpus
- cross check with other library

### Question
- Does exit(0) matter in fuzzer? If fuzzer exit, it will report error. Should we use exit(0)(cli line 59) or not?

### How to run fuzzer
- clang -g -fsanitize=address,fuzzer client-fuzzer.c 
- clang++ -g -fsanitize=address,fuzzer client-fuzzer.c 


### Note for wei
- slim: fuzzer 17285 (test client first) (gcc server.c -o server)
- client 19832
- I have two directions to dive into the problems
    - One, figure out expected fuzzer behaviros: pass in multiple parameters, only fuzzing after socket connection setuping
    - Another, generate random workflows with python

### issues
- sudo ufw allow portnum
- netstat -na | grep "8080"
### reference
- tcp/ip
- google [libfuzzer tutorial](https://github.com/google/fuzzing/blob/master/tutorial/libFuzzerTutorial.md)
