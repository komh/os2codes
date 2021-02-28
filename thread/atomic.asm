;
; Atomic functions
;
; Copyright (C) 2021 KO Myung-Hun <komh@chollian.net>
;
; This program is free software. It comes without any warranty, to
; the extent permitted by applicable law. You can redistribute it
; and/or modify it under the terms of the Do What The Fuck You Want
; To Public License, Version 2, as published by Sam Hocevar. See
; http://www.wtfpl.net/ for more details.
;

%include "os2section.inc"

SECTION_TEXT

        global atomicLoad

atomicLoad:
        mov     edx, [esp + 4]          ; p
        mov     eax, [edx]              ; load and return *p
        ret

        global atomicExchange

atomicExchange:
        mov     eax, [esp + 8]          ; v
        mov     edx, [esp + 4]          ; p
        xchg    [edx], eax              ; store v to *p, then return old *p
        ret

        global atomicCompareExchange

atomicCompareExchange:
        mov     eax, [esp + 12]         ; e
        mov     edx, [esp + 8]          ; d
        mov     ecx, [esp + 4]          ; p
        lock \
        cmpxchg [ecx], edx              ; compare *p to e,
                                        ; if same, exchange with d
        setz    al                      ; assigned ? then set al to 1 or 0
        movzx   eax, al                 ; return the result
        ret
