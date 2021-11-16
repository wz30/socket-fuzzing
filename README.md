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
- [ ] design ACE-like framework to automatically generate correct workflows
    - [ ] what is a correct workflow? what are the variations?
    - [ ] how many socket apis are picked to generate the workflow: a meaningful combinaiton of socket API.
    - [ ] Do we generate C codes directly or use an intermediate layer like ACE? So far, not sure why ACE use an intermediate layer.
    - [ ] If there is no bug in normal workflow, what others api(e.g. fork, epoll) are necessary to include?
- [ ] Search space for the workflow when fuzzing
    - [ ] which meaningful paramaters in socket API are needed to changed/fuzzed
    - [ ] what are the classic problems in socket?
    - [ ] when developers/company create their own network stack, what could be the common issues?
    - [ ] 
- [ ] issue with simple fuzzer which just creates socket and always fail
- [ ] generate correct workflows: correct server and client behaviors
- [ ] test workflows



### Note for wei
- slim: fuzzer 17285 (test client first) (gcc server.c -o server)
- client 19832
