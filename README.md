# socket-fuzzing
fuzzing network stack(socket) library

## ToDo list
- [x] fuzzer tutorial
- [x] run basic server and client example
- [ ] only fuzzing client codes, server is up 
    - [ ] no input   
- [ ] expected behavior in Fuzzer 
    - [ ] verify the behaviors before connection setup between nodes on server/client end
    - [ ] verify the behaviors after socket connecton is setup between nodes on server/client end
    - [ ] compare behaviors in linux socket and customized network stack library(such as slim, MTCP)
- [ ] design ACE-like framework to automatically generate correct workflows
    - [ ] what is a correct workflow? what are the variations?
    - [ ] how many socket apis are picked to generate the workflow
    - [ ] Do we generate C codes directly or use an intermediate layer like ACE? So far, not sure why ACE use an intermediate layer.
- [ ] issue with simple fuzzer which just creates socket and always fail
- [ ] generate correct workflows: correct server and client behaviors
- [ ] test workflows



### Note for wei
- slim: fuzzer 17285 (test client first) (gcc server.c -o server)
- client 19832
