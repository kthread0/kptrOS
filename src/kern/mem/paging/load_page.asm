global loadPageDirectory loadPageDirectory : push rbp mov rbp, rsp mov rax, rdi;
First argument in rdi mov cr3, rax mov rsp, rbp pop rbp ret
