%macro isr_err_stub 1
isr_stub_%+%1:
    call exception_handler
    iretq
%endmacro

; if writing for 64-bit, use iretq instead
%macro isr_no_err_stub 1
isr_stub_%+%1:
    call exception_handler
    iretq
%endmacro
