;
; OS/2 section macros
;
; Copyright (C) 2021-2024 KO Myung-Hun <komh78@gmail.com>
;
; This program is free software. It comes without any warranty, to
; the extent permitted by applicable law. You can redistribute it
; and/or modify it under the terms of the Do What The Fuck You Want
; To Public License, Version 2, as published by Sam Hocevar. See
; http://www.wtfpl.net/ for more details.
;

%macro SECTION_TEXT 0-1 16
    %ifidn __OUTPUT_FORMAT__,obj
        ; FLAT property is not necessary unlike NASM manual. FLAT seems
        ; not to have any effect. It has no differences of segment ordering
        ; whether or not using it.
        ; In case of Watcom Linker such as wl.exe, defining CGROUP is important
        ; because kLIBC uses CGROUP for CODE segments. If a CODE segment is not
        ; in CGROUP, then it's not possible to access it with a single segment
        ; register.
        ; In addition, this violates segment ordering of kLIBC.
        ; kLIBC expects ________TEXT segment to be at the end of all the code
        ; segments. However, a CODE segment not in CGROUP is placed after
        ; ________TEXT segment. This may lead to unexpected problems.
        ; If using both FLAT property and GROUP directive, then NASM complains
        ; about multiple-grouping of the same segment although it has no
        ; problems.
        ; After all, omitting FLAT property and using GROUP directive is
        ; better. Or simply use TEXT32 as a CODE segment name, because kLIBC
        ; uses TEXT32 as a CODE segment name.
        ; IBM linkers such as ilink.exe and link386.exe have no CGROUP-related
        ; problems.
        ; In case of VisualAge C++ and Watcom, CGROUP is not defined. Because
        ; of this, rather, defining CGROUP causes a problem. So CGROUP should
        ; not be defined.
        ; And it's not important whether or not using FLAT property.
        ; In any case, FLAT property is not necessary. Just omit it.
        ;          ********** THE ABOVE is PARTIALLY true **********
        ; The above is true only when declaring a segment which is already
        ; declared by external module such as kLIBC startup codes and has FLAT
        ; property. Therefore FLAT property should be specified as NASM manual
        ; says.
        section TEXT32 public align=%1 use32 class=CODE flat

        %ifdef __KLIBC__
            ; On kLIBC, segments of CODE class should be included in CGROUP.
            ; NOTE that group directive does not allow to define a group twice.
            ; group CGROUP TEXT32
        %endif
    %elifidn __OUTPUT_FORMAT__,aout
        section .text
    %else
        %error Unsupported output format!
    %endif
%endmacro

%macro SECTION_DATA 0-1 16
    %ifidn __OUTPUT_FORMAT__,obj
        section DATA32 public align=%1 use32 class=DATA flat

        ; Segments of DATA class should be included in DGROUP.
        ; NOTE that group directive does not allow to define a group twice.
        ; group DGROUP DATA32
    %elifidn __OUTPUT_FORMAT__,aout
        section .data
    %else
        %error Unsupported output format!
    %endif
%endmacro

%macro SECTION_BSS 0-1 16
    %ifidn __OUTPUT_FORMAT__,obj
        section BSS32 public align=%1 use32 class=BSS flat

        ; Segments of BSS class should be included in DGROUP.
        ; NOTE that group directive does not allow to define a group twice.
        ; group DGROUP BSS32
    %elifidn __OUTPUT_FORMAT__,aout
        section .bss
    %else
        %error Unsupported output format!
    %endif
%endmacro
