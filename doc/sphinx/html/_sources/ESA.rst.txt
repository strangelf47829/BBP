Embeddable Simple Assembly (language)
=====================================

.. role:: cpp(code)
   :language: cpp

.. role:: asm(code)
   :language: asm

.. role:: esa(code)
    :language: none

.. _overview:

Overview
--------

    The **Embeddable Simple Assembly** language was developed as a simple assembler language that can be virtualized reasonably easily.

Compiling .esa files
--------------------

    .esa files can be compiled by using the **EmbeddabLe Simple Assembler** (or **ELSA** for short) compiler, which can be invoked either from
    a BBP shell, or directly through an OS Shell (assuming ELSA is installed).


Installing ELSA
^^^^^^^^^^^^^^^

Just build your own ELSA lel

How to use ELSA
^^^^^^^^^^^^^^^

Invoking ELSA
"""""""""""""

        +----------+---------------------------------------------+---------------+
        |  Option  |                 Description                 |     Notes     |
        +----------+---------------------------------------------+---------------+
        | -h       |    Displays a help page on the terminal     |               |
        +----------+---------------------------------------------+---------------+


Memory model
------------

    ESA has three seperate memory "domains": the **physical** memory, the **virtual** memory, and **meta** memory.

Physical memory
^^^^^^^^^^^^^^^

    Physical memory is the memory physically accessible by the host hardware.
    This hardware is not directly accessible to ESA processes, but can be manipulated by syscalls and special instructions.

    Physical memory is managed by BBP and does not need to be accessed directly.

    .. Danger::
        
        **You are heavily discouraged** from accessing physical memory directly, as BBP has no C++ signal handling capabilities.

        Instead, use the :cpp:`std::read` and :cpp:`std::write` functions provided by BBP, as they will handle exceptions for you.

Virtual memory
^^^^^^^^^^^^^^

    Virtual memory is memory which lives on physical memory but is addressable independantly of where it lives.

    That is to say, physical address :cpp:`0x105` can only be the 6th element of a page living at physical address :cpp:`0x100`,
    but virtual memory :cpp:`0x005` will *always* be index 5, regardless of where in memory the page lives.

    Virtual memory is always relative to *something* (which in turn lives on physical memory).
    Virtual memory is said to be relative to *segments*, essentially named 'blocks' of memory that may share a specific function.

    .. note::

        Although ESA refers to memory *segments*, they are not the same as ELF *segments*, but rather ELF *sections*.

    When ESA multiple binaries are loaded into memory, the :ref:`sections <stub>` contained within are linked together,
    forming one large *segment* comprising of multiple *pages*.

    This relies on the fact that the relative addressing of virtual memory addresses stays the same regardless of where the initial offset is:
    *Virtual memory is memory which lives on physical memory but is addressable independantly of where it lives.*

Meta memory
"""""""""""

    Some memory addresses are neither neither "physical" nor "virtual". These memory addresses are said to be *meta*.

    One such example are the :ref:`registers <stub>`. Functionally, they live on virtual memory, just like .text or .rodata, however
    it would be semantically incorrect to refer to meta memory as virtual memory, `because I say so. <https://imgs.xkcd.com/comics/argument_victory.png>`_

    The reason for this distinction is that because registers (among other values) are never stored in virtual memory, their addresses cannot be represented in ESA.

Addressing schemes
""""""""""""""""""

    Virtual memory can be represented using the following scheme:

    .. code-block:: none

        "[<section>[+ offset]] | 0" <base> <index>.

    Essentially, if the memory address is not relative to any specific segment (such as offsets / indicies), it is prefixed with a 0.
    Otherwise, a 2 digit identifier is used as the prefix. :ref:`Here <stub>` is a list of commonly used identifiers.

    The base of the index is then appended:
        - 'b' for binary numbers;
        - 'x' for hexadecimal numbers;
        - 'o' for octal numbers;
        - '_' for decimal numbers.
    
    The index is then appended in the specified base.

    For example, :esa:`0x05` refers to virtual memory offset 5, relative to no specific segment.
    :esa:`BS_10` refers to virtual memory offset 10, relative to the .bss segment.
    :esa:`BS+50_10` refers to virtual memory offset 10, relative to the .bss segment offset by 50.

    Physical memory cannot be represented in EAS, and so have no representation.
    Notwithstanding, this document does sometimes represent physical memory in hexadecimal notation.
    This is possible because this document is written in english. (Yes I actually wrote that, it is not AI generated).

Syntax
------

    ESA has a rather simple syntax (hence the *simple assembly*).

Comments
^^^^^^^^

    Comments can be defined anywhere on a line, and extend until the end of the current line.
    ESA has no multiline comments.

    .. code-block:: none
        :linenos:

        ; This is a comment
        mov eax, 3 ; This is also a comment

lvalues, rvalues, and xvalues
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    Although there is no real notion of 'lvalues' or 'rvalues' in ESA, certain notation exists which cannot be described without defining what an lvalue or rvalue is.

    The syntax to dereference an expression is [expression]. This will return the value stored in virtual memory.
    The syntax to get the address of an rvalue is %expression%. This will return the address of the rvalue.
    It is only possible to get the address of a value that lives on virtual memory.

    An **lvalue** is any value that is either a register or a *dereferenced* value, while an **rvalue** is a value that immediately exists on *virtual memory*, a constant expression, or a register.
    An **xvalue** is a value that is both an **lvalue** and an **rvalue**.

    Consider the following C code

    .. code-block:: C
        :linenos:

        int a = 0;
        int **b = &&a;

    On line 1, the value *0* is stored into *a*. 
    Since *a* is a dereferenced (in EAS), *a* is an lvalue.
    Since *a* can also exist on virtual memory, *a* is also an rvalue.
    Because *a* is both an lvalue and rvalue, *a* is said to be an **xvalue**.

    On line 2, the address of the address of *a* is being stored into *b*.
    However, the "address of *a*" is never stored in virtual memory and so is not an rvalue.
    This means that this assignment cannot be made, and so generates an error.

    In ESA, the internal representation of a symbol is the dereferenced offset of the symbol,
    while the internal representation of a register is a physical memory address.
    
    Since that is technically a 'dereferenced' value, all symbols (except for quasi-symbols) are *xvalues*.

    .. code-block:: none
        :linenos:

        static (myInt) db 42
            ; Internally, 'myInt' represents 'dereferenced offset 0'.
        
        mov myInt, eax
            ; OK, because myInt and eax are xvalues.

        mov eax, myInt
            ; OK, because myInt and eax are xvalues.

        mov eax, %myInt%
            ; OK, because 'offset 0' is a constant expression

        mov eax, %%myInt%%
            ; Not OK, because 'offset 0' is never stored on virtual memory.

Directives
^^^^^^^^^^

    Directives are commands given to ELSA that do not translate directly into bytecode.

    .. important::

        Directives must be given at the start of each line, and always start with a pound (#) sign.

"Using" directive
"""""""""""""""""

    The "using" directive functions identically to C/C++'s :cpp:`#include` directive.

"Handler" directive
"""""""""""""""""""

    The handler directive tells ELSA what :ref:`procedure <stub>` to call for a given :ref:`signal <stub>`.

    .. code-block:: none
        :linenos:

        procedure (myHandler) (0)

        end

        #handler SIGINT myHandler

    The handler directive must come after a procedure declaration; The following code has the same effect as the code above.

    .. code-block:: none
        :linenos:

        extern procedure (myHandler) (0)
        #handler SIGINT myHandler

        procedure (myHandler) (0)

        end

    .. attention::

        ELSA may fail to compile and/or ignore a handler directive if the procedure is declared but not defined within a translation unit,
        or if the procedure referenced takes an incorrect amount of arguments.

Symbol definitions
^^^^^^^^^^^^^^^^^^

    Symbols are defined using the following syntax:

    .. code-block:: none

        <access> <modifiers> (<Identifier>) <size> <data>

    The access keyword can be either :cpp:`static`, :cpp:`virtual`, or nothing, in which case the symbol is said to be a quasi-symbol.
    ELSA will attempt to expand all references of quasi-symbols where found in source code.
    Quasi-symbols are not exported by ELSA; How other symbols are exported depends on their access and modifier keywords.

    .. code-block:: none
        :linenos:

        (myInt) db 42
        mov eax, myInt

            ; is equivalent to ...
        
        mov eax, 42
    
    .. note::

        Specifying any modifiers on quasi-symbols will result in compilation errors.

        Keep in mind that this is simply a macro expansion, so 

        .. code-block:: none
            :linenos:

            (myTwoInts) db 42, 42
            mov eax, myTwoInts

            ; Expands to:
            mov eax, 42, 42

        will trigger an error because 'mov' only expects two arguments.

    Static symbols are simply symbols which are initialized before code execution. In other words, static symbols are stored in the ELF binary.

    .. attention::

        Contents of static symbols can easily be viewed and modified externally.

        **Do not store security sensitive data as static symbols.**

    Static symbols are emitted inside of the .data section.

    Virtual symbols are symbols which also need to be statically-allocated, but not necessarily initialized.
    Virtual symbols live on the .bss section, as opposed to the .data section.

    Please refer to :ref:`stub` to learn how .bss and .data is treated.

    Modifiers modify the default behaviour of ELSA.

    the following modifiers are valid:

        | - Const
        | - Static (only valid directly after :cpp:`virtual`)
        | - Extern 

    The *const* modifier forces ELSA to emit data on .rodata (read only data) instead of .data.

    The *static* modifier is only valid on virtual symbols and *must* come directly after the virtual keyword.
    If a virtual symbol is modified by *static*, the virtual symbol transforms into a pointer that holds the offset of the data initialized at .bss.

    This is the C equivalent of the following:

    .. code-block:: C
        :linenos:

        int bssData[];
        int *virtualSymbol = &bssData;
    
    If a virtual symbol is modified by *static* and *const*, the symbol is stored in .rodata instead of .data.

    The extern modifier tells ELSA that the definition for that symbol is not available within this translation unit.

    **Size** refers to the physical size of **data**.
    Valid sizes are listed in the table below.

    +-----------+--------------+-------------------------------------+
    |  Keyword  |  Size (bit)  |     notes                           |
    +-----------+--------------+-------------------------------------+
    |    db     |       8      | Define Bytes                        |
    +-----------+--------------+-------------------------------------+
    |    dh     |      16      | Define Halfword                     |
    +-----------+--------------+-------------------------------------+
    |    dw     |      32      | Define Word                         |
    +-----------+--------------+-------------------------------------+
    |           |      0       | Only valid for quasi-symbols        |
    +-----------+--------------+-------------------------------------+
    
    The size of a symbol can be referred to during compile time by encapsulating a symbol identifier in angled brackets.
    If a single bracket is used, it returns the size of the symbol in bytes.
    If a double bracket is used, it returns the size of the symbol in bits.

    .. code-block:: none
        :linenos:

        static (myVar) db 42

        mov eax, <myVar>
        ; Is equivalent to:
        mov eax, 1

        mov eax, <<myVar>>
        ; Is equivalent to:
        mov eax, 8

    **Data** can be any sequence of constant expressions.
    Constant expressions are values that are known at compile time.
    Valid constant expressions are integer constants, <> expressions, <<>> expressions, hexadecimal expressions, and (multi)-char sequences.

    .. code-block:: none
        :linenos:

        'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!', 0
        ; Is equivalent to
        "Hello, World!", 0

    .. note::
        multi-char sequences are not null-terminated and must be terminated explicitly by adding a 0 after the sequence

        .. code-block::
            
            "Hello, World"    ; Not null terminated
            "Hello, World", 0 ; Null terminated

    The size (in bytes) of a data sequence is known at compile time and can be expressed by encapsulating the symbol identifier in curly braces

    .. code-block::
        :linenos:

        static (myString) db "Hello, world!", 0

        mov eax, {myString}
        ; Is equivalent to
        mov eax, 14

Procedures
^^^^^^^^^^

    Procedures are blocks of instructions that can be called from other sections.

    .. note::

        The actual inner workings of procedures are defined in the ABI.

        This section only describes the syntax of procedures, and not the ABI.

    Procedures are defined using the following structure

    .. code-block:: none
        :linenos:

        <modifiers> procedure (<identifier>) (<arguments>)
            <code>
        end

    The only valid modifier is :cpp:`extern`. In such case, ELSA will ignore any provided function body (but emit a warning), and can safely be omitted.

    .. code-block:: none
        :linenos:

        extern procedure (myFunc) (0)
        ; ^^ Compiles OK

        extern procedure (myFunc) (0)
        end
        ; ^^ Compiles OK, but generates warning "Body defined for procedure marked extern".

    **Arguments** is any valid integer constant expression and defines how many arguments the function takes.
    These arguments are not stored in any register, but are instead placed on the argument stack (please see the :ref:`ABI <stub>` for more details).

    Procedures can be overloaded by simply specifying different amount of arguments.

    .. code-block:: none
        :linenos:

        procedure (myFunc) (1)
        end

        procedure (myFunc) (2)
        end

Instructions
^^^^^^^^^^^^

    The bytecode executed by the :ref:`virtual processor <stub>` is represented by instructions.
    Instructions are composed in the following structure:

    .. code-block:: none

        <mnemonic> <arg1>, <arg2>, ...

    The **mnemonic** of an instruction is the plain-text name of an instruction. This differs from an *opcode* because a single **mnemonic** can map to multiple *opcodes*.

    **arg1** and **arg2** are arguments passed onto the instruction, however the instruction may take more or less than the demonstrated 2.

    arguments can be of any type except for mnemonic or arrays.


Registers
---------

    The virtual processor has registers that can be written to or read from.

    Registers live on physical memory, and as such are **xvalues**.
    Some registers may refer to the same physical memory. Which register references what section of what memory is explicitly defined in this section.

    Registers may also have different sizes, which are either 32 bit, 16 bit, or 8 bit;
    The :esa:`<>` and :esa:`<<>>` notation can be used to retrieve register sizes.

General purpose registers
^^^^^^^^^^^^^^^^^^^^^^^^^

    Some registers, such as :asm:`eax` are *general purpose*, meaning that they have no specific purpose and can be used freely.
    The following registers are general purpose:
    
    
    +-----------------------------------------------------------------------------------------+
    |  Accumulator register                                                                   |
    +--------------------------+-------------+-------------------------------------+----------+
    |  Arithmetic operations.  |             |                                     |          |
    |                          |             |        (cannot be addressed)        |          |
    |                          |             |                                     |          |
    |                          |    EAX      +-----------+------+-------+----------+  32 bit  |
    |                          |             |           |  AH  | 8 bit |          |          |
    |                          |             |     AX    +------+-------+  16 bit  |          |
    |                          |             |           |  AL  | 8 bit |          |          |
    +--------------------------+-------------+-----------+------+-------+----------+----------+

    +-----------------------------------------------------------------------------------------+
    |  Counter register                                                                       |
    +--------------------------+-------------+-------------------------------------+----------+
    |  Indexing purposes,      |             |                                     |          |
    |  such as for 'for' loops.|             |        (cannot be addressed)        |          |
    |                          |             |                                     |          |
    |                          |    ECX      +-----------+------+-------+----------+  32 bit  |
    |                          |             |           |  CH  | 8 bit |          |          |
    |                          |             |     CX    +------+-------+  16 bit  |          |
    |                          |             |           |  CL  | 8 bit |          |          |
    +--------------------------+-------------+-----------+------+-------+----------+----------+

    +-----------------------------------------------------------------------------------------+
    |  Data register                                                                          |
    +--------------------------+-------------+-------------------------------------+----------+
    |  I/O operations, as well |             |                                     |          |
    |  as arithmetic           |             |        (cannot be addressed)        |          |
    |  operations.             |             |                                     |          |
    |                          |    EDX      +-----------+------+-------+----------+  32 bit  |
    |                          |             |           |  DH  | 8 bit |          |          |
    |                          |             |     DX    +------+-------+  16 bit  |          |
    |                          |             |           |  DL  | 8 bit |          |          |
    +--------------------------+-------------+-----------+------+-------+----------+----------+

    +-----------------------------------------------------------------------------------------+
    |  Base register                                                                          |
    +--------------------------+-------------+-------------------------------------+----------+
    |  General pointer to data.|             |                                     |          |
    |                          |             |        (cannot be addressed)        |          |
    |                          |             |                                     |          |
    |                          |    EBX      +-----------+------+-------+----------+  32 bit  |
    |                          |             |           |  BH  | 8 bit |          |          |
    |                          |             |     BX    +------+-------+  16 bit  |          |
    |                          |             |           |  BL  | 8 bit |          |          |
    +--------------------------+-------------+-----------+------+-------+----------+----------+

    +-----------------------------------------------------------------------------------------+
    |  Stack pointer register                                                                 |
    +--------------------------+-------------+-------------------------------------+----------+
    |  **readonly**            |             |                                     |          |
    |                          |             |        (cannot be addressed)        |          |
    |                          |             |                                     |          |
    |                          |    ESP      +-----------+------+-------+----------+  32 bit  |
    |                          |             |           |  --  | 8 bit |          |          |
    |                          |             |     --    +------+-------+  16 bit  |          |
    |                          |             |           |  --  | 8 bit |          |          |
    +--------------------------+-------------+-----------+------+-------+----------+----------+

    +-----------------------------------------------------------------------------------------+
    |  Stack Base register                                                                    |
    +--------------------------+-------------+-------------------------------------+----------+
    |  **readonly**            |             |                                     |          |
    |                          |             |        (cannot be addressed)        |          |
    |                          |             |                                     |          |
    |                          |    EBX      +-----------+------+-------+----------+  32 bit  |
    |                          |             |           |  --  | 8 bit |          |          |
    |                          |             |     --    +------+-------+  16 bit  |          |
    |                          |             |           |  --  | 8 bit |          |          |
    +--------------------------+-------------+-----------+------+-------+----------+----------+

    +-----------------------------------------------------------------------------------------+
    |  Source register                                                                        |
    +--------------------------+-------------+-------------------------------------+----------+
    |                          |             |                                     |          |
    |                          |             |        (cannot be addressed)        |          |
    |                          |             |                                     |          |
    |                          |    ESI      +-----------+------+-------+----------+  32 bit  |
    |                          |             |           |  --  | 8 bit |          |          |
    |                          |             |     --    +------+-------+  16 bit  |          |
    |                          |             |           |  --  | 8 bit |          |          |
    +--------------------------+-------------+-----------+------+-------+----------+----------+

    +-----------------------------------------------------------------------------------------+
    |  Destination register                                                                   |
    +--------------------------+-------------+-------------------------------------+----------+
    |                          |             |                                     |          |
    |                          |             |        (cannot be addressed)        |          |
    |                          |             |                                     |          |
    |                          |    EDI      +-----------+------+-------+----------+  32 bit  |
    |                          |             |           |  --  | 8 bit |          |          |
    |                          |             |     --    +------+-------+  16 bit  |          |
    |                          |             |           |  --  | 8 bit |          |          |
    +--------------------------+-------------+-----------+------+-------+----------+----------+

    Although these are *general purpose*, they are used with a generally consistent purpose.
    It is up to the programmer to follow the conventions specified within these tables.

Thread specific registers
^^^^^^^^^^^^^^^^^^^^^^^^^

    Some registers are used to keep track of information which is specific to the thread hosting the application.

    .. warning::

        Although the name *thread specific* implies that only these registers are "specific" to this thread (as opposed to all of them),
        this is incorrect.

        **Every register is "specific" to its thread**. *Thread specific* refers to how the host treats these registers between threads.

    Thread specific registers cannot be modified by the application directly (I.E., these registers are all readonly).
    Some registers, such as :esa:`eip` can be modified *indirectly* by instructions like :asm:`call` or :asm:`ret`.

    The following registers are thread specific:

    +-----------------------------------------------------------------------------------------+
    |  Thread information register                                                            |
    +--------------------------+-------------+-----------+------+-------+----------+----------+
    |  *TP* (Thread Parent)    |             |           |  OB  | 8 bit |          |          |
    |                          |             |     ST    +------+-------+  16 bit  |          |
    |  *TI* (Thread ID)        |             |           |  TB  | 8 bit |          |          |
    |                          |    ETI      +-----------+------+-------+----------+  32 bit  |
    |  **readonly**            |             |           |  TP  | 8 bit |          |          |
    |                          |             |     PI    +------+-------+  16 bit  |          |
    |                          |             |           |  TI  | 8 bit |          |          |
    +--------------------------+-------------+-----------+------+-------+----------+----------+

    The :esa:`PI` register holds the *Process Information*. The *thread parent* is the id of the thread
    that spawned this thread, and the *thread id* is the PID of the currently running thread.

    The :esa:`ST` register holds the process information of the process from which a raised signal comes from.

    +-----------------------------------------------------------------------------------------+
    |  Signal state register                                                                  |
    +--------------------------+-------------+-----------+--------------+----------+----------+
    |  Signal handling and     |             |           |              |          |          |
    |  state.                  |             |     HP    |              |  16 bit  |          |
    |                          |             |           |              |          |          |
    |                          |    SSX      +-----------+------+-------+----------+  32 bit  |
    |                          |             |           |  CS  | 8 bit |          |          |
    |                          |             |     SS    +------+-------+  16 bit  |          |
    |                          |             |           |  SF  | 8 bit |          |          |
    +--------------------------+-------------+-----------+------+-------+----------+----------+

    The :esa:`SSX` register holds information about the current signalling state of the thread.
    
    The :esa:`SS` (Signal State) register holds the integer representation of the current signal (:esa:`CS`),
    and any associated flags (:esa:`SF`).

    The handler pointer (:esa:`HP`) register points to the address of the procedure that gets called when a signal is raised.

    Please refer to the :ref:`ABI <stub>` to learn how signals are processed, and :ref:`here <stub>` to learn what signals are.

    +-----------------------------------------------------------------------------------------+
    |  Stack unwinding register                                                               |
    +--------------------------+-------------+-----------+--------------+----------+----------+
    |                          |             |           |              |          |          |
    |                          |             |    ASU    |              |  16 bit  |          |
    |                          |             |           |              |          |          |
    |                          |    SUD      +-----------+--------------+----------+  32 bit  |
    |                          |             |           |              |          |          |
    |                          |             |    GSU    |              |  16 bit  |          |
    |                          |             |           |              |          |          |
    +--------------------------+-------------+-----------+--------------+----------+----------+

    The :esa:`SUD` register holds information about how the virtual processor must unwind the stack.

    The :esa:`ASU` register corresponds to the argument stack, and the :esa:`GSU` refers to the general stack.
    Please refer to :ref:`this <stub>` page to learn about stack unwinding.

    +-----------------------------------------------------------------------------------------+
    |  Instruction pointer                                                                    |
    +--------------------------+-------------+-------------------------------------+----------+
    |  Points to the next      |             |                                     |          |
    |  instruction.            |             |        (cannot be addressed)        |          |
    |                          |             |                                     |          |
    |                          |    EIP      +-----------+------+-------+----------+  32 bit  |
    |  **readonly**            |             |           |  IH  | 8 bit |          |          |
    |                          |             |     IP    +------+-------+  16 bit  |          |
    |                          |             |           |  IL  | 8 bit |          |          |
    +--------------------------+-------------+-----------+------+-------+----------+----------+

    The instruction pointer points to the next instruction to be run, assuming no branching takes place.

    The reason for this is that it simplifies the control flow between procedure calls. 
    Please refer to the :ref:`ABI <stub>` for a more detailed explanation.

    +-----------------------------------------------------------------------------------------+
    |  Flags register                                                                         |
    +--------------------------+-------------+-------------------------------------+----------+
    |  The state of the        |             |                                     |          |
    |  Processor.              |             |        (cannot be addressed)        |          |
    |                          |             |                                     |          |
    |                          |    FLG      |                                     |  32 bit  |
    |  **readonly**            |             |                                     |          |
    |                          |             |                                     |          |
    |                          |             |                                     |          |
    +--------------------------+-------------+-------------------------------------+----------+

    The Flags register is used internally by the virtual processor to store the executing state of the thread.
    The internal structuring of the FLG register can be found :ref:`here <stub>`.

Segmentation registers
^^^^^^^^^^^^^^^^^^^^^^

    Segmentation registers are registers that store the size and **physical address** of physical memory pages.

    The purpose of these registers is to give the user a way to interface with the actual hardware of the host.

    .. note::

        The only way to read and write to the data pointed to by these registers is by using the dedicated :esa:`read` and :esa:`write`
        instructions. Attempting to read data outside of the below listed pages (ie, past their size) will result in a segfault.

    +-----+------------------------------+-----------------------+--------------------------------+
    | ID  |  name                        | physical size         | Holds the size of              |
    +=====+==============================+=======================+================================+
    | SPS |  Stack page size             | :cpp:`sizeof(size_t)` | Size of .stack                 |
    +-----+------------------------------+-----------------------+--------------------------------+
    | CPS |  Code page size              | :cpp:`sizeof(size_t)` | Size of .text                  |
    +-----+------------------------------+-----------------------+--------------------------------+
    | DPS |  Data page size              | :cpp:`sizeof(size_t)` | Size of .data                  |
    +-----+------------------------------+-----------------------+--------------------------------+
    | RPS |  Read only Data page size    | :cpp:`sizeof(size_t)` | Size of .rodata                |
    +-----+------------------------------+-----------------------+--------------------------------+
    | EPS |  Extra page size             | :cpp:`sizeof(size_t)` | Size of .bss                   |
    +-----+------------------------------+-----------------------+--------------------------------+
    | UAS |  User allocated page size    | :cpp:`sizeof(size_t)` | Size of alloc'd data by user   |
    +-----+------------------------------+-----------------------+--------------------------------+
    | SAS |  System allocated page size  | :cpp:`sizeof(size_t)` | Size of alloc'd data by system |
    +-----+------------------------------+-----------------------+--------------------------------+

    +-----+--------------------------------+-------------------------+--------------------------------------------+
    | ID  |  name                          | physical size           | Holds a pointer to                         |
    +=====+================================+=========================+============================================+
    | SPP |  Stack page pointer            | :cpp:`sizeof(uint8_t*)` | Physical address of .stack                 |
    +-----+--------------------------------+-------------------------+--------------------------------------------+
    | CPP |  Code page pointer             | :cpp:`sizeof(uint8_t*)` | Physical address of .text                  |
    +-----+--------------------------------+-------------------------+--------------------------------------------+
    | DPP |  Data page pointer             | :cpp:`sizeof(uint8_t*)` | Physical address of .data                  |
    +-----+--------------------------------+-------------------------+--------------------------------------------+
    | RPP |  Read only Data page pointer   | :cpp:`sizeof(uint8_t*)` | Physical address of .rodata                |
    +-----+--------------------------------+-------------------------+--------------------------------------------+
    | EPP |  Extra page pointer            | :cpp:`sizeof(uint8_t*)` | Physical address of .bss                   |
    +-----+--------------------------------+-------------------------+--------------------------------------------+
    | UAP |  User allocated page pointer   | :cpp:`sizeof(uint8_t*)` | Physical address of page alloc'd by user   |
    +-----+--------------------------------+-------------------------+--------------------------------------------+
    | SAP |  System allocated page pointer | :cpp:`sizeof(uint8_t*)` | Physical address of page alloc'd by system |
    +-----+--------------------------------+-------------------------+--------------------------------------------+

Extended registers
""""""""""""""""""

    Some implementations of BBP may need extra registers, which are defined as *extended* registers (not to be confused with registers that are extended).
    These registers may offer direct control of GPIO states, for example, by setting physical addresses.

    Please refer to ':ref:`Extending BBP <stub>`' to learn how to implement extended registers.

Usage examples
""""""""""""""

    .. note::

        These examples are written in ESA, but are not neccesarily compliant with the :ref:`ABI <stub>`, because
        they are intentionally made to be minimal examples.

    The following code demonstrates the use of :ref:`General purpose registers <stub>` and :ref:`Segmentation registers <stub>`.
    It reads each byte available by the system, increments its value, then overwrites that byte with the result.

    .. code-block:: none
        :linenos:

        procedure (example) (0)
            ; This is an example on how to use the segmentation registers

            push eax ; Push accumulator
            push ecx ; Push counter
            push ebx ; Push base

            mov ecx, 0 
                ;; Move into ecx the number 0
            mov ebx, sas 
                ;; Move into ebx the physical size of system allocated memory

            label loop: ; Define a label that can be jumped to
                sub ecx, ebx 
                    ;; Subtract the value of eax and ebx (result stored in eax)
                jez quitloop 
                    ;; If the 0 flag is set (I.E., ecx == ebx)
                
                ;; There is still data available

                shread ecx 
                    ;; (system-heap-read) the data at index ecx relative to system alloc'd page into eax.

                inc eax 
                    ;; Add one to eax

                shwrite ecx, eax
                    ;; Write into ecx the value of eax

                inc ecx 
                    ;; Increment ecx (index)

                jmp loop 
                    ;; Unconditional jump to 'loop'.
            label quitloop:

            shfree sap (system-heap-free) 
                ;; Tell the system to free the system allocated data, since we are done with it.

            pop ebx ; Pop into ebx
            pop ecx ; Pop into counter
            pop eax ; Pop into accumulator

            ret 
                ;; Return control to previous procedure
        end

    +-------+---------------------------------------------------------+
    | Lines | Purpose                                                 |
    +=======+=========================================================+
    |       |  Push the values of eax, ebx, and ecx onto the stack    |
    |  4-6  |  so they can later be retrieved.                        |
    |       |                                                         |
    +-------+---------------------------------------------------------+
    |       |  Write 0 to ecx to start at index 0.                    |
    |  8-11 |  Write the size of the allocated page into ebx.         |
    |       |                                                         |
    +-------+---------------------------------------------------------+
    |       |  Define a for loop by providing a label to jump to and  |
    | 13-17 |  the conditions to exit the for loop.                   |
    |       |                                                         |
    +-------+---------------------------------------------------------+
    |       |  Read one byte from allocated page with offset ebx into |
    | 21-22 |  eax.                                                   |
    |       |                                                         |
    +-------+---------------------------------------------------------+
    |       |  Increment the value of eax (the byte that was read)    |
    | 24-25 |                                                         |
    |       |                                                         |
    +-------+---------------------------------------------------------+
    |       |  Write the value of eax back into the physical memory.  |
    | 27-28 |                                                         |
    |       |                                                         |
    +-------+---------------------------------------------------------+
    |       |  Increment the index by one (ecx), and continue the     |
    | 30-34 |  loop.                                                  |
    |       |                                                         |
    +-------+---------------------------------------------------------+
    |       |  Free the memory allocated by the system.               |
    | 37-38 |                                                         |
    |       |                                                         |
    +-------+---------------------------------------------------------+
    |       |  Pop the values pushed in lines 4-6 back into the       |
    | 40-42 |  registers.                                             |
    |       |                                                         |
    +-------+---------------------------------------------------------+

    the equivalent C code is:

    .. code-block:: C
        :linenos:

        int *data;
        int size;

        int example()
        {
            int ebx = size;

            for (int ecx = 0; ecx < ebx; ecx++)
                data[ecx] = data[ecx] + 1;

            return 0;
        }

The stack
---------

    .. note::

        This section describes the model ESA uses when running a program, and not the :cpp:`std::Stack<>` data type.

        For more information on how the stack is implemented, please read the :ref:`ABI <stub>`.
        For more information on the :cpp:`std::Stack<>` datatype, please read :ref:`here <stub>`.

    ESA models a stack as a pointer that can move up and down a memory page.
    When adding a value to the stack (called *pushing* a value), the pointer is incremented.
    When reading a value to the stack (called *popping* a value), the pointer is decremented.

    Attempts to read past the boundaries of the page will result in a signal being raised for the offending thread.

    ESA has two distinct stacks: the general stack and the argument stack.

    The *general* stack can be accessed by the user using the :esa:`push` and :esa:`pop` instructions.
    The :esa:`push` command takes an rvalue, and places it on top of the stack.
    The :esa:`pop` command takes an lvalue, and assigns it the rvalue last pushed into the stack (FILO).

    .. code-block:: none
        :linenos:

        push 42
            ;; Pushes the rvalue '42' onto the stack.

        pop eax
            ;; Pops the top element of the stack into lvalue 'eax'.

    The *argument* stack is a stack that cannot be pushed or popped by the user. 
    Instead, the vitual processor pushes and pops values according to the :ref:`ABI <stub>`.

    In summary, whenever a procedure is called, the virtual processor pushes the arguments onto the *argument* stack, as well as the return value address
    and return instruction pointer.

    There are two ways to call procedures: using the :esa:`call` instruction, and the :esa:`callp` (call promise) instruction.

    When calling a procedure using :esa:`call`, the processor only pushes :esa:`eip` twice, and any arguments afterwards.
    When calling a procedure using :esa:`callp`, the processor pushes both :esa:`eip` and a return address, along with any arguments.

    .. code-block:: none
        :linenos:

        callp eax, add5, 5
            ;; Stack looks like this:
            ;; +    -- top --    +
            ;; |        5        | ; Argument one
            ;; |       eax       | ; Where to put the returned value
            ;; |       eip       | ; Where to return to

        call add5, 5
            ;; +    -- top --    +
            ;; |        5        | ; Argument one
            ;; |       eip       | ; Where to put the returned value
            ;; |       eip       | ; Where to return to

    The user can access the arguments by using the :esa:`arg` instruction (:ref:`main page <stub>`).

    The :esa:`arg` instruction takes two arguments, a constant expression and an lvalue.

    .. code-block:: none
        :linenos:

        procedure (add5) (1)
            ;; When add5, 5 is called...
            ;; +    -- top --    +
            ;; |        5        | (element 0)
            ;; |       eip       | (element 1)
            ;; |       eip       | (element 2)

            arg 0, eax
                ;; When this is called, the processor will move the value '5' into eax, because '5' is on top of the stack

            arg 1, eax
                ;; Will raise the SIGILL signal (Unless the process runs with priveliges).

            ;; The stack is unchanged, because arg does not pop, but peek.

    The user can indirectly pop elements from the argument stack by using the :esa:`ret` instruction. 
    This instruction pops all arguments from the argument stack, 
    then moves an rvalue into the return lvalue stored on the argument stack if one is provided, then moves the last rvalue into eip, and then the processor cycles.

    .. important::

        The :esa:`end` keyword is functionally identical to :esa:`ret 0`, and is not just syntactic fluff.

        The programmer could, if they so desire, omit the :esa:`end` keyword and create nested functions :ref:`like this <stub>`
        unless an EOF would be encountered first.

        Unfortunately, ELSA does not currently have this functionality, but is a planned feature.

        .. warning::

            Be careful not to callp functions that do not expect it.

            Assume any function may return the equivalent of :cpp:`(void *)0`;

    This has the effect of returning the argument stack back to how it was before the function call.
    This is also the reason why :esa:`eip` points to the next instruction: When the processor cycles, it will execute the next instruction immediately
    after the :esa:`call` instruction.

    .. note::

        Although this example is written in ESA, it is not neccesarily compliant with the :ref:`ABI <stub>`, because
        it is made to be a minimal example.

    .. code-block:: none
        :linenos:

        ;; Assuming _entry gets called on entry...

        procedure (_entry) (0)
            ;; Pretend the argument stack is empty here.

            push eax
                ;; Since we are returning to eax, push eax

            callp eax, add5, 5
                ;; eax is now 5+5

            pop eax
                ;; Return eax to how it was

        end

        procedure (add5) (1)

            arg 0, eax
                ;; Get the first argument, and move it into eax

            add eax, 5
                ;; Adds 5 to eax, and stores the result in eax

        ret eax

    The code will execute as follows:

    +-------------------+-------------+-------------------------+--------------------------------+
    | Instruction cycle | clock cycle |      current line       |           micro code           |
    +===================+=============+=========================+================================+
    |         1         |      1      |   arg 0, eax            |  peek argument 0 into eax      |
    +-------------------+-------------+-------------------------+--------------------------------+
    |                   |      2      |   callp eax, add5, 5    |  push eip onto the arg stack   |
    |                   +-------------+                         +--------------------------------+
    |                   |      3      |                         |  push %eax% onto the arg stack |
    |         2         +-------------+                         +--------------------------------+
    |                   |      4      |                         |  push 5 onto the arg stack     |
    |                   +-------------+                         +--------------------------------+
    |                   |      5      |                         |  move address of add5 into eip |
    +-------------------+-------------+-------------------------+--------------------------------+
    |         3         |      6      |   arg 0, eax            |  peek argument 0 into eax      |
    +-------------------+-------------+-------------------------+--------------------------------+
    |                   |      7      |   add eax, 5            |  compute eax + 5               |
    |                   +-------------+                         +--------------------------------+
    |                   |      8      |                         |  move result into eax          |
    |         4         +-------------+                         +--------------------------------+
    |                   |      9      |                         |  update FLG register           |
    +-------------------+-------------+-------------------------+--------------------------------+
    |                   |      10     |   ret eax               |  pop 5 from the argument stack |
    |                   +-------------+                         +--------------------------------+
    |                   |      11     |                         |  push eax into %eax%           |
    |         5         +-------------+                         +--------------------------------+
    |                   |      12     |                         |  pop %eax% from argument stack |
    |                   +-------------+                         +--------------------------------+
    |                   |      13     |                         |  pop argument stack into eip   |
    +-------------------+-------------+-------------------------+--------------------------------+
    |         6         |      14     |   pop eax               |  pop stack into eax            |
    +-------------------+-------------+-------------------------+--------------------------------+
    |                   |      ..     |   end                   |  (end of example)              |
    +-------------------+-------------+-------------------------+--------------------------------+

Signals
-------

    Signals in ESA handle much like exceptions do in C++.

    .. important::

        This documentation uses terminology like *raises the signal* or *causes a SEGFAULT*.
        These signals are not the C++ signals, but rather these "virtualized" signals.

        The documentation will state explicitly if it is referring to a C++ signal:
        ":cpp:`*((char*)0)` will cause a C++ signal".

    Signals are just integer constants that can be passed between threads to mean different things.
    What makes signals worthwhile is the way ESA handles them.

    The virtual processor stores all relevant signal handling information in the :esa:`SSX` register.

    The user may raise a signal for it or another thread using the following syntax:

    .. code-block:: none
        :linenos:

        raise SIGABRT
            ;; Raises the signal SIGABRT for this thread

        raise SIGABRT, tp
            ;; Raises the signal SIGABRT for thread with the pid corresponding to register 'tp'
    
    The virtual processor will only react to received signals when the thread resumes execution, which is not instant.

    If the thread receives two signals before the thread resumes execution, the virtual processor will immediately execute the
    signal handler for the first signal, then the second one immediately after. The virtual processor will not switch to a different
    thread before both signals are resolved.

    If a signal is raised by the signal handler, a double fault will occur. In such cases, the process is killed normally and its resources free'd.

    If another signal is raised while killing the process, a triple fault will occur. If this happens, it is likely that some section of the
    loaded binary has been corrupted. Since the binary has been corrupted, it is unlikely that continuing to execute any code without hinderence.

    Because of this, the processor will trigger a kernel panic if a triple fault occurs.

    If a C++ signal is raised during normal execution, the offending process is killed immediately without executing .fini,
    however the resources used by the process will still be free'd.

Signal Handler
^^^^^^^^^^^^^^

    The *signal handler* is the handler that handles signals. 
    That is to say, **it is the handler that calls the user defined handler procedure, or if none is present, the default signal action.**

    When the virtual processor resumes the execution of a thread, it will check the :esa:`CS` register.
    If a signal is present, the signal handler is called.

    First, the signal handler looks up the corresponding procedure on the handler vector table,
    which is either a default procedure or one set by the :esa:`#handler` directive.

    What happens next depends on the type of signal (I.E., the :esa:`SF` register).

    Some signals are just that, *signals*. They can be used to pass information between processes.
    These types of signals can be ignored safely, after which the process resumes as normal.

    Other signals cannot be ignored and must be handled.
    Whether the thread can continue execution after receiving depends on the 'unwind' flag.

    If the unwind flag is not set, the call to the handler is treated as a normal procedure call.
    If the unwind flag is set, the virtual processor will unwind both the argument stack and general stack according to the :esa:`SUD` register.

    .. note::

        The virtual processor will not perform multi-threading while a signal handler is running.

    Stack unwinding is just moving the stack pointer to a known state.

    ESA, unlike C++, does not have destructors (or constructors). This means that no destructors have to be called when unwinding the stack.
    Although it is theoretically possible for a process to continue executing after a stack unwinding, such functionality is not currently supported.

Signal Types
^^^^^^^^^^^^

    Signals are really just integer constants built into ELSA with a predefined meaning (similar to how errno.h works in C).

Referencing signals
"""""""""""""""""""

    Signals are referenced like any other value, and are treated like constant expressions.

    .. code-block:: none
        :linenos:

        mov eax, SIGINT
            ;; Moves the value of 'sigint' into eax

    Is a syntactically valid statement.

    .. attention::

        Because signals are basically fancy macros, you cannot declare a symbol with an identifier identical to that of a signal.

        .. code-block:: none
            :linenos:

            (SIGINT) db 3
                ;; To ELSA, this is the same as:
            (.. some integer constant ..) db 3
                ;; Which is an illegal statement

SIGABRT
"""""""

    +--------+------------------+------------------------+-------------------------+--------------+-------------------+-----------+
    | Signal | integer constant | Can be raised by user? | Can be handled by user? | Recoverable? | Causes unwinding? | Arguments |
    +========+==================+========================+=========================+==============+===================+===========+
    |SIGABRT |        1         |          Yes           |           Yes           |     No       |       Yes         |     0     |
    +--------+------------------+------------------------+-------------------------+--------------+-------------------+-----------+

    The SIGABRT signal is used by a process to tell the virtual processor that it should stop executing the process.
    Although the user can define a signal handler, the process is not allowed to resume execution.

    This signal cannot be called for other processes.

SIGCONT
"""""""

    +--------+------------------+------------------------+-------------------------+--------------+-------------------+-----------+
    | Signal | integer constant | Can be raised by user? | Can be handled by user? | Recoverable? | Causes unwinding? | Arguments |
    +========+==================+========================+=========================+==============+===================+===========+
    |SIGCONT |        2         |          Yes           |           Yes           |     Yes      |        No         |     0     |
    +--------+------------------+------------------------+-------------------------+--------------+-------------------+-----------+

    The SIGCONT signal tells the virtual processor to continue executing a previously stopped process.
    Although the user can define a signal handler for this signal, it should only be used to check if the thread *can* continue executing.

    A user could, if they wish, abort the contiuation of the thread by raising a SIGABRT, since it would cause a :ref:`double fault <stub>`.

SIGILL
""""""

    +--------+------------------+------------------------+-------------------------+--------------+-------------------+-----------+
    | Signal | integer constant | Can be raised by user? | Can be handled by user? | Recoverable? | Causes unwinding? | Arguments |
    +========+==================+========================+=========================+==============+===================+===========+
    |SIGILL  |        3         |          Yes           |           No            |     No       |       Yes         |     0     |
    +--------+------------------+------------------------+-------------------------+--------------+-------------------+-----------+

    The SIGILL signal is raised when an illegal instruction is encountered, such as an onrecognized op-code, unrecognized register value,
    or other generally ill-formed instructions.

    The user cannot handle this signal, because it is assumed that the process that caused this signal to begin with has been corrupted.
    The .fini code does get executed, since it lives on a different page than .text.

    Although the user is not meant to be able to call SIGILL, doing so anyway will cause a SIGILL because :esa:`raise SIGILL` is an illegal instruction.

SIGINT
""""""

    +--------+------------------+------------------------+-------------------------+--------------+-------------------+-----------+
    | Signal | integer constant | Can be raised by user? | Can be handled by user? | Recoverable? | Causes unwinding? | Arguments |
    +========+==================+========================+=========================+==============+===================+===========+
    |SIGINT  |        4         |          Yes           |           Yes           |     Yes      |        No         |     2     |
    +--------+------------------+------------------------+-------------------------+--------------+-------------------+-----------+

    The SIGINT signal is a "general purpose" signal. It is generally used for I/O purposes.

    It takes one argument with no predefined purpose.

    It can be safely ignored.

SIGKILL
"""""""

    +--------+------------------+------------------------+-------------------------+--------------+-------------------+-----------+
    | Signal | integer constant | Can be raised by user? | Can be handled by user? | Recoverable? | Causes unwinding? | Arguments |
    +========+==================+========================+=========================+==============+===================+===========+
    |SIGKILL |        5         |          Yes           |           No            |     No       |       No          |     0     |
    +--------+------------------+------------------------+-------------------------+--------------+-------------------+-----------+

    The SIGKILL signal functions like the SIGABRT signal, except it causes the process to be killed immediately.

SIGPOLL
"""""""

    +--------+------------------+------------------------+-------------------------+--------------+-------------------+-----------+
    | Signal | integer constant | Can be raised by user? | Can be handled by user? | Recoverable? | Causes unwinding? | Arguments |
    +========+==================+========================+=========================+==============+===================+===========+
    |SIGPOLL |        6         |          No            |           Yes           |     Yes      |       No          |     1     |
    +--------+------------------+------------------------+-------------------------+--------------+-------------------+-----------+

    The SIGPOLL signal is raised by the virtual processor if a watched file descriptor has been written to.

    The signal handler must take one argument, which is the INode of the file descriptor.

SIGSEGV
"""""""

    +--------+------------------+------------------------+-------------------------+--------------+-------------------+-----------+
    | Signal | integer constant | Can be raised by user? | Can be handled by user? | Recoverable? | Causes unwinding? | Arguments |
    +========+==================+========================+=========================+==============+===================+===========+
    |SIGSEGV |        7         |          No            |           No            |     No       |       Yes         |     0     |
    +--------+------------------+------------------------+-------------------------+--------------+-------------------+-----------+

    `I don't think this signal needs an introduction. <https://ih1.redbubble.net/image.706543508.5629/st,small,507x507-pad,600x600,f8f8f8.u3.jpg>`_

SIGSTOP
"""""""

    +--------+------------------+------------------------+-------------------------+--------------+-------------------+-----------+
    | Signal | integer constant | Can be raised by user? | Can be handled by user? | Recoverable? | Causes unwinding? | Arguments |
    +========+==================+========================+=========================+==============+===================+===========+
    |SIGSTOP |        8         |          Yes           |           No            |     Yes      |       No          |     0     |
    +--------+------------------+------------------------+-------------------------+--------------+-------------------+-----------+

    The SIGSTOP signal tells the virtual processor to stop executing this thread.

    This signal cannot be handled since the processor stops executing this thread.

SIGSYS
""""""

    +--------+------------------+------------------------+-------------------------+--------------+-------------------+-----------+
    | Signal | integer constant | Can be raised by user? | Can be handled by user? | Recoverable? | Causes unwinding? | Arguments |
    +========+==================+========================+=========================+==============+===================+===========+
    |SIGSYS  |        9         |          No            |           Yes           |     Yes      |       No          |     0     |
    +--------+------------------+------------------------+-------------------------+--------------+-------------------+-----------+

    The SIGSYS signal is raised when a system call is made with ill-formed arguments, to a non-existant system call, or any other
    error occurs when calling a system call.

    By default, the binary is assumed to be incompatible with the host system or has been corrupted, which is grounds for immediate termination.

SIGTERM
"""""""

    +--------+------------------+------------------------+-------------------------+--------------+-------------------+-----------+
    | Signal | integer constant | Can be raised by user? | Can be handled by user? | Recoverable? | Causes unwinding? | Arguments |
    +========+==================+========================+=========================+==============+===================+===========+
    |SIGTERM |        10        |          Yes           |           Yes           |     Yes      |       No          |     0     |
    +--------+------------------+------------------------+-------------------------+--------------+-------------------+-----------+

    Unlike the SIGKILL or SIGABRT signals, the SIGTERM acts as a *request* to terminate. This request can safely be ignored.

    The process can cause a double fault by raising a SIGABRT when handling a SIGTERM.

SIGAOVF
"""""""

    +--------+------------------+------------------------+-------------------------+--------------+-------------------+-----------+
    | Signal | integer constant | Can be raised by user? | Can be handled by user? | Recoverable? | Causes unwinding? | Arguments |
    +========+==================+========================+=========================+==============+===================+===========+
    |SIGAOVF |        11        |          No            |           Yes           |     Yes      |       No          |     0     |
    +--------+------------------+------------------------+-------------------------+--------------+-------------------+-----------+

    SIGAOVF is raised when the processor encounters an overflow when performing arithmetic operations.

SIGDBY0
"""""""

    +--------+------------------+------------------------+-------------------------+--------------+-------------------+-----------+
    | Signal | integer constant | Can be raised by user? | Can be handled by user? | Recoverable? | Causes unwinding? | Arguments |
    +========+==================+========================+=========================+==============+===================+===========+
    |SIGDBY0 |        12        |          No            |           Yes           |     Yes      |       No          |     0     |
    +--------+------------------+------------------------+-------------------------+--------------+-------------------+-----------+

    SIGAOVF is raised when the processor is instructed to divide by 0.

Byte code representation
------------------------

    This section defines the binary representation of instructions, and what mnemonic maps to what op-code.

Morphemes
^^^^^^^^^

    A morpheme is the smallest unit of a language that still holds any meaningful sense.

    Defining ESA bytecode in terms of morphemes allows for higher level structures to emerge naturally from these morphemes.

    ESA bytecode, as the name suggests, is expressed in bytes. As such, so are morphemes.
    A morpheme is said to be *independant* if it maintains meaning even when taken out of context.
    A morpheme is said to be a *modifier* if a morpheme does not have a meaning itself, but meaning is removed if ignored.
    A morpheme is said to be a *lexeme* if substituting the morpheme for an equivalent value would not change the idea of the phrase.

    .. note::

        Consider the phrase "10 bears".

        The phrase "10 bears" consists of a lexeme, independant morpheme, and a modifying morpheme.

        Although substituting "10" with "9" would change the meaning of the phrase, it does not change the idea.
        The phrase *10 bears* conveys the same idea as *9 bears*: "a collection of bears".

        "bear" is an independant morpheme because "bear" still holds meaning in and of itself.

        "-s" is a modifying morpheme because "-s" holds no meaning, but modifies the meaning of "bear".

Statements
^^^^^^^^^^

    Programs are made up of statements, which in turn are made up of morphemes.

    .. note::

        The term "statement" in this context means *The smallest subdivision of morphemes which the virtual processor can step over*.

        It does not have the same meaning as the commonly used meaning of the word "statement", as in, *statements and expressions*.

    Each statement starts with one word (called the *header*), structured like this:

    +-------------+--------------+-----------------+--------------+-----------------+------------+
    |  **name:**  |    Prefix    |     Op-code     |    Suffix    |      flags      |    args    |
    +-------------+--------------+-----------------+--------------+-----------------+------------+
    |  **type:**  |  *modifier*  |  *independant*  |  *modifier*  |  *indepentant*  |  *lexeme*  |
    +-------------+--------------+-----------------+--------------+-----------------+------------+
    |  **size:**  |    8 bits    |     8 bits      |    8 bits    |      4 bits     |   4 bits   |
    +-------------+--------------+-----------------+--------------+-----------------+------------+

    The *op-code* field is a single byte that tells the processor what the instruction it wants to execute is (such as :esa:`mov` or :esa:`jmp`).

    The *suffix* modifier is a modifier that modifies the behaviour of an instruction. The effect of the suffix modifier depends on the op-code,
    and are described :ref:`here <stub>`.

    The *flags* field, in contrast to *suffix*, does not modify the behaviour of the instruction but signals various things to the processor.
    Bit 1 is reserved for future use, bit 2 asks the processor not to perform multithreading tasks, bit 3 asks the processor to switch to another task,
    and bit 4 is reserved for future use.

    The *args* lexeme is a single (unsigned) 4 bit lexeme that encodes the amount of arguments the processor should expect.

    The prefix modifier is a bitfield that represents whether an argument is a numerical constant or not: A *0* designates a numerical constant, and a *1*
    designates elsewise.

    .. note::

        This means that instructions are limited to 8 arguments; Procedure calls are also limited to 8 arguments because it could
        otherwise not be called by a :esa:`call` instruction.

    A statement header is then followed by **at least** *args* amount of words, where the type of argument n is determined by the nth bit in the prefix field.

    If the argument is of type '*numerical constant*', it is simply treated as a numerical constant.

    Otherwise, the argument is read using the following structure:

    +-------------+--------------+----------------+-------------------+------------+-----------------+
    |  **name:**  |  referenced  |  dereferenced  |       offset      |  trailers  |      data       |
    +-------------+--------------+----------------+-------------------+------------+-----------------+
    |  **type:**  |  *modifier*  |   *modifier*   |   *indepentant*   |  *lexeme*  |  *independant*  |
    +-------------+--------------+----------------+-------------------+------------+-----------------+
    |  **size:**  |    1 bit     |     1 bit      |       4 bits      |   2 bits   |     24 bits     |
    +-------------+--------------+----------------+-------------------+------------+-----------------+

    The *referenced* and *dereferenced* flags are used to tell the virtual processor to either reference, or dereference, a certain address.

    .. note::

        Although technically supported, the *referenced* flag is usually used by the virtual processor internally to represent references of things
        the programmer cannot represent, such as :esa:`%eip%`.

    If the *referenced* and *dereferenced* flags are both set, a SIGILL signal will be raised.

    Since a referenced value is an rvalue, an atomic reference to a reference cannot exist; Setting the *referenced* flag is enough to reference a value.
    
    Dereferencing, on the other hand, can be done as many times as the user desires. In order to support multiple dereferences (atomically), an extra
    argument is needed. The amount of extra arguments taken is stored in the *trailers* field. Although currently, only the *dereferenced* flag needs
    trailers, future revisions may need extra fields and thus 2 bits have been allocated to this field.

    Trailers are 32-bit signed numerical expressions, **and are considered an extension of the argument**; the header does not need to specify the type of a trailer,
    nor does it need to count it in the *args* field.

    The *offset* and *data* fields are what make up the actual argument.

    The *offset* field is used to specify where the processor should fix the 0th index of the page specified by *data*.

    *Data* specifies both the page and index of that page using the following structure:

    +-------------+----------+-----------+
    |  **name:**  |   Page   |   Index   |
    +-------------+----------+-----------+
    |  **size:**  |  4 bits  |  20 bits  |
    +-------------+----------+-----------+

    The *page* field specifies which page *index* is relative to.

Statement example
"""""""""""""""""

    Consider the following statement:

    :esa:`0x02C10002 0x84000001 0x0000002A`

    This statement comprises of a header and 2 arguments.

    The header decodes as follows:

    +-------------+--------------+-----------------+--------------+-----------------+------------+
    |  **name:**  |    Prefix    |     Op-code     |    Suffix    |      flags      |    args    |
    +-------------+--------------+-----------------+--------------+-----------------+------------+
    | **value:**  |     0x02     |      0xC1       |     0x00     |       0x0       |     0x2    |
    +-------------+--------------+-----------------+--------------+-----------------+------------+

    This tells the virtual processor to execute op-code :esa:`0xC1` with 2 arguments of which argument 1 is not a literal, does not ask
    the processor to switch thread, nor does it ask to remain on this thread.

    Argument 1, :esa:`0x84000001` is not a constant and so is decoded as follows:

    +-------------+--------------+----------------+-------------------+------------+-----------------+
    |  **name:**  |  referenced  |  dereferenced  |       offset      |  trailers  |      data       |
    +-------------+--------------+----------------+-------------------+------------+-----------------+
    | **value:**  |     0b1      |      0b0       |       0b0001      |    0b00    |    0x000001     |
    +-------------+--------------+----------------+-------------------+------------+-----------------+

    Argument 1 is a reference to index '1' of page '0', with an offset of type '1', and has no trailers.

    Argument 2 is the literal constant of '42' (:esa:`0x2A`).

Mnemonic mapping
^^^^^^^^^^^^^^^^

    This section contains the list of mnemonics used by ESA, their respective op-code, and any other information regarding the instruction.

nil
"""

    - Usage: :esa:`nil`
    - Stands for: nill (null).

    +---------+------+
    | Op-code | 0x00 |
    +=========+======+
    |  Priv?  |  no  |
    +---------+------+
    |  args?  |  0   |
    +---------+------+
    | atomic? | yes  |
    +---------+------+

    The :esa:`nil` instruction is an instruction that does not exist.

    Raises SIGILL if executed.

nop
"""

    - Usage: :esa:`nop`
    - Stands for: no operation.

    +---------+------+
    | Op-code | 0x01 |
    +=========+======+
    |  Priv?  |  no  |
    +---------+------+
    |  args?  |  0   |
    +---------+------+
    | atomic? | yes  |
    +---------+------+

    The :esa:`nop` instruction does nothing.

hlt
"""

    - Usage: :esa:`hlt`
    - Stands for: halt.

    +---------+------+
    | Op-code | 0x02 |
    +=========+======+
    |  Priv?  |  no  |
    +---------+------+
    |  args?  |  0   |
    +---------+------+
    | atomic? | yes  |
    +---------+------+

    The :esa:`hlt` instruction tells the processor to stop executing the thread, but not to kill it.
    It is analogous to :esa:`dec eip`.

hcf
"""

    - Usage: :esa:`hcf`
    - Stands for: halt catch fire.

    +---------+------+
    | Op-code | 0x03 |
    +=========+======+
    |  Priv?  |  no  |
    +---------+------+
    |  args?  |  0   |
    +---------+------+
    | atomic? | yes  |
    +---------+------+

    The :esa:`hcf` instruction forces the processor to do nothing indefinately, until reset.

mov
"""

    - Usage: :esa:`mov [dst] [src]`
    - Stands for: move into *dst*, *src*.

    +---------+------+
    | Op-code | 0x04 |
    +=========+======+
    |  Priv?  |  no  |
    +---------+------+
    |  args?  |  2   |
    +---------+------+
    | atomic? | yes  |
    +---------+------+

    The :esa:`mov` instruction moves a value from one location into another.

    :esa:`mov` takes two arguments, *src* and *dst*, where:

    - *src* must be an lvalue;
    - *dst* must be an rvalue.

    +----------+----------------------------------+
    |  Raises  |  If                              |
    +==========+==================================+
    |  SIGILL  | - *src* is not an lvalue         |
    |          | - *dst* is not an rvalue         |
    +----------+----------------------------------+
    |  SIGSEGV | - *src* does not exist           |
    |          | - *dst* does not exist           |
    |          | - *src* cannot be written to     |
    |          | - *dst* cannot be read from      |
    +----------+----------------------------------+

inc
"""

    - Usage: :esa:`inc [reg]`
    - Stands for: increment *reg* by one.

    +---------+------+
    | Op-code | 0x05 |
    +=========+======+
    |  Priv?  |  no  |
    +---------+------+
    |  args?  |  1   |
    +---------+------+
    | atomic? | yes  |
    +---------+------+

    The :esa:`in` instruction increments lvalue *reg* by one.

    +----------+----------------------------------+
    |  Raises  |  If                              |
    +==========+==================================+
    |  SIGILL  | - *reg* is not an lvalue         |
    +----------+----------------------------------+
    |  SIGSEGV | - *reg* does not exist           |
    |          | - *reg* cannot be written to     |
    |          | - *reg* cannot be read from      |
    +----------+----------------------------------+

    +----------+----------+----------------------------------+
    | Modifier |  Append  | Effect                           |
    +==========+==========+==================================+
    |   0x00   |          |  Treat *reg* as an integer       |
    +----------+----------+----------------------------------+
    |   0x01   |    -f    |  Treat *reg* as a 32 bit float   |
    +----------+----------+----------------------------------+

dec
"""

    - Usage: :esa:`dec [reg]`
    - Stands for: decrement *reg* by one.

    +---------+------+
    | Op-code | 0x06 |
    +=========+======+
    |  Priv?  |  no  |
    +---------+------+
    |  args?  |  1   |
    +---------+------+
    | atomic? | yes  |
    +---------+------+

    The :esa:`dec` instruction increments lvalue *reg* by one.

    +----------+----------------------------------+
    |  Raises  |  If                              |
    +==========+==================================+
    |  SIGILL  | - *reg* is not an lvalue         |
    +----------+----------------------------------+
    |  SIGSEGV | - *reg* does not exist           |
    |          | - *reg* cannot be written to     |
    |          | - *reg* cannot be read from      |
    +----------+----------------------------------+

    +----------+----------+----------------------------------+
    | Modifier |  Append  | Effect                           |
    +==========+==========+==================================+
    |   0x-0   |          |  Treat *reg* as an integer       |
    +----------+----------+----------------------------------+
    |   0x-1   |    -f    |  Treat *reg* as a 32 bit float   |
    +----------+----------+----------------------------------+

add
"""

    - Usage: :esa:`add [a] [b]`
    - Stands for: add *a* and *b*, and store the result in eax.

    +---------+------+
    | Op-code | 0x07 |
    +=========+======+
    |  Priv?  |  no  |
    +---------+------+
    |  args?  |  2   |
    +---------+------+
    | atomic? | yes  |
    +---------+------+

    :esa:`add` also updates the FLG register.

    +----------+----------------------------------+
    |  Raises  |  If                              |
    +==========+==================================+
    |  SIGSEGV | - *a* does not exist             |
    |          | - *a* cannot be read from        |
    |          | - *b* does not exist             |
    |          | - *b* cannot be read from        |
    +----------+----------------------------------+

    +----------+----------+----------------------------------+
    | Modifier |  Append  | Effect                           |
    +==========+==========+==================================+
    |   0x-0   |          |  Treat *a* as an integer         |
    +----------+----------+----------------------------------+
    |   0x-1   |    -f    |  Treat *a* as a 32 bit float     |
    |          |          |  Treat *eax* as a 32 bit float   |
    +----------+----------+----------------------------------+
    |   0x0-   |          |  Treat *b* as an integer         |
    +----------+----------+----------------------------------+
    |   0x1-   |    -F    |  Treat *b* as a 32 bit float     |
    |          |          |  Treat *eax* as a 32 bit float   |
    +----------+----------+----------------------------------+
    |   0x-2   |    -u    |  Treat *a* as unsigned           |
    +----------+----------+----------------------------------+
    |   0x2-   |    -U    |  Treat *b* as unsigned           |
    +----------+----------+----------------------------------+

sub
"""

    - Usage: :esa:`sub [a] [b]`
    - Stands for: subtract *b* from *a*, and store the result in eax.

    +---------+------+
    | Op-code | 0x08 |
    +=========+======+
    |  Priv?  |  no  |
    +---------+------+
    |  args?  |  2   |
    +---------+------+
    | atomic? | yes  |
    +---------+------+

    :esa:`sub` also updates the FLG register.

    +----------+----------------------------------+
    |  Raises  |  If                              |
    +==========+==================================+
    |  SIGSEGV | - *a* does not exist             |
    |          | - *a* cannot be read from        |
    |          | - *b* does not exist             |
    |          | - *b* cannot be read from        |
    +----------+----------------------------------+

    +----------+----------+----------------------------------+
    | Modifier |  Append  | Effect                           |
    +==========+==========+==================================+
    |   0x-0   |          |  Treat *a* as an integer         |
    +----------+----------+----------------------------------+
    |   0x-1   |    -f    |  Treat *a* as a 32 bit float     |
    |          |          |  Treat *eax* as a 32 bit float   |
    +----------+----------+----------------------------------+
    |   0x0-   |          |  Treat *b* as an integer         |
    +----------+----------+----------------------------------+
    |   0x1-   |    -F    |  Treat *b* as a 32 bit float     |
    |          |          |  Treat *eax* as a 32 bit float   |
    +----------+----------+----------------------------------+
    |   0x-2   |    -u    |  Treat *a* as unsigned           |
    +----------+----------+----------------------------------+
    |   0x2-   |    -U    |  Treat *b* as unsigned           |
    +----------+----------+----------------------------------+

mul
"""

    - Usage: :esa:`mul [a] [b]`
    - Stands for: multiply *a* and *b*, and store the result in eax.

    +---------+------+
    | Op-code | 0x09 |
    +=========+======+
    |  Priv?  |  no  |
    +---------+------+
    |  args?  |  2   |
    +---------+------+
    | atomic? | yes  |
    +---------+------+

    :esa:`mul` also updates the FLG register.

    +----------+----------------------------------+
    |  Raises  |  If                              |
    +==========+==================================+
    |  SIGSEGV | - *a* does not exist             |
    |          | - *a* cannot be read from        |
    |          | - *b* does not exist             |
    |          | - *b* cannot be read from        |
    +----------+----------------------------------+

    +----------+----------+----------------------------------+
    | Modifier |  Append  | Effect                           |
    +==========+==========+==================================+
    |   0x-0   |          |  Treat *a* as an integer         |
    +----------+----------+----------------------------------+
    |   0x-1   |    -f    |  Treat *a* as a 32 bit float     |
    |          |          |  Treat *eax* as a 32 bit float   |
    +----------+----------+----------------------------------+
    |   0x0-   |          |  Treat *b* as an integer         |
    +----------+----------+----------------------------------+
    |   0x1-   |    -F    |  Treat *b* as a 32 bit float     |
    |          |          |  Treat *eax* as a 32 bit float   |
    +----------+----------+----------------------------------+
    |   0x-2   |    -u    |  Treat *a* as unsigned           |
    +----------+----------+----------------------------------+
    |   0x2-   |    -U    |  Treat *b* as unsigned           |
    +----------+----------+----------------------------------+

div
"""

    - Usage: :esa:`div [a] [b]`
    - Stands for: divide *a* by *b*, and store the result in eax.

    +---------+------+
    | Op-code | 0x0A |
    +=========+======+
    |  Priv?  |  no  |
    +---------+------+
    |  args?  |  2   |
    +---------+------+
    | atomic? | yes  |
    +---------+------+

    :esa:`div` also updates the FLG register.

    +----------+----------------------------------+
    |  Raises  |  If                              |
    +==========+==================================+
    |  SIGSEGV | - *a* does not exist             |
    |          | - *a* cannot be read from        |
    |          | - *b* does not exist             |
    |          | - *b* cannot be read from        |
    +----------+----------------------------------+
    |  SIGDBY0 | - *b* is 0                       |
    +----------+----------------------------------+

    +----------+----------+----------------------------------+
    | Modifier |  Append  | Effect                           |
    +==========+==========+==================================+
    |   0x-0   |          |  Treat *a* as an integer         |
    +----------+----------+----------------------------------+
    |   0x-1   |    -f    |  Treat *a* as a 32 bit float     |
    |          |          |  Treat *eax* as a 32 bit float   |
    +----------+----------+----------------------------------+
    |   0x0-   |          |  Treat *b* as an integer         |
    +----------+----------+----------------------------------+
    |   0x1-   |    -F    |  Treat *b* as a 32 bit float     |
    |          |          |  Treat *eax* as a 32 bit float   |
    +----------+----------+----------------------------------+
    |   0x-2   |    -u    |  Treat *a* as unsigned           |
    +----------+----------+----------------------------------+
    |   0x2-   |    -U    |  Treat *b* as unsigned           |
    +----------+----------+----------------------------------+

mod
"""

    - Usage: :esa:`mod [a] [b]`
    - Stands for: compute the module *b* of *a* (a % b), and store the result in eax.

    +---------+------+
    | Op-code | 0x0B |
    +=========+======+
    |  Priv?  |  no  |
    +---------+------+
    |  args?  |  2   |
    +---------+------+
    | atomic? | yes  |
    +---------+------+

    :esa:`mod` also updates the FLG register.

    +----------+----------------------------------+
    |  Raises  |  If                              |
    +==========+==================================+
    |  SIGSEGV | - *a* does not exist             |
    |          | - *a* cannot be read from        |
    |          | - *b* does not exist             |
    |          | - *b* cannot be read from        |
    +----------+----------------------------------+
    |  SIGDBY0 | - *b* is 0                       |
    +----------+----------------------------------+

    +----------+----------+----------------------------------+
    | Modifier |  Append  | Effect                           |
    +==========+==========+==================================+
    |   0x-0   |          |  Treat *a* as an integer         |
    +----------+----------+----------------------------------+
    |   0x-1   |    -f    |  Treat *a* as a 32 bit float     |
    |          |          |  Treat *eax* as a 32 bit float   |
    +----------+----------+----------------------------------+
    |   0x0-   |          |  Treat *b* as an integer         |
    +----------+----------+----------------------------------+
    |   0x1-   |    -F    |  Treat *b* as a 32 bit float     |
    |          |          |  Treat *eax* as a 32 bit float   |
    +----------+----------+----------------------------------+
    |   0x-2   |    -u    |  Treat *a* as unsigned           |
    +----------+----------+----------------------------------+
    |   0x2-   |    -U    |  Treat *b* as unsigned           |
    +----------+----------+----------------------------------+

and
"""

    - Usage: :esa:`and [a] [b]`
    - Stands for: Compute *a* and *b*, and store the result in eax.

    +---------+------+
    | Op-code | 0x0B |
    +=========+======+
    |  Priv?  |  no  |
    +---------+------+
    |  args?  |  2   |
    +---------+------+
    | atomic? | yes  |
    +---------+------+

    :esa:`and` also updates the FLG register.

    +----------+----------------------------------+
    |  Raises  |  If                              |
    +==========+==================================+
    |  SIGSEGV | - *a* does not exist             |
    |          | - *a* cannot be read from        |
    |          | - *b* does not exist             |
    |          | - *b* cannot be read from        |
    +----------+----------------------------------+

    +----------+----------+----------------------------------+
    | Modifier |  Append  | Effect                           |
    +==========+==========+==================================+
    |   0x00   |          |  perform a bitwise and (&)       |
    +----------+----------+----------------------------------+
    |   0x01   |    -b    |  perform a binary and (&&)       |
    +----------+----------+----------------------------------+
    

orr
"""

    - Usage: :esa:`orr [a] [b]`
    - Stands for: Compute *a* or *b*, and store the result in eax.

    +---------+------+
    | Op-code | 0x0C |
    +=========+======+
    |  Priv?  |  no  |
    +---------+------+
    |  args?  |  2   |
    +---------+------+
    | atomic? | yes  |
    +---------+------+

    :esa:`orr` also updates the FLG register.

    +----------+----------------------------------+
    |  Raises  |  If                              |
    +==========+==================================+
    |  SIGSEGV | - *a* does not exist             |
    |          | - *a* cannot be read from        |
    |          | - *b* does not exist             |
    |          | - *b* cannot be read from        |
    +----------+----------------------------------+

    +----------+----------+----------------------------------+
    | Modifier |  Append  | Effect                           |
    +==========+==========+==================================+
    |   0x00   |          |  perform a bitwise or (|)        |
    +----------+----------+----------------------------------+
    |   0x01   |    -b    |  perform a binary or (||)        |
    +----------+----------+----------------------------------+

xor
"""

    - Usage: :esa:`xor [a] [b]`
    - Stands for: Compute *a* exclusive or *b*, and store the result in eax.

    +---------+------+
    | Op-code | 0x0D |
    +=========+======+
    |  Priv?  |  no  |
    +---------+------+
    |  args?  |  2   |
    +---------+------+
    | atomic? | yes  |
    +---------+------+

    :esa:`xor` also updates the FLG register.

    +----------+----------------------------------+
    |  Raises  |  If                              |
    +==========+==================================+
    |  SIGSEGV | - *a* does not exist             |
    |          | - *a* cannot be read from        |
    |          | - *b* does not exist             |
    |          | - *b* cannot be read from        |
    +----------+----------------------------------+

    +----------+----------+----------------------------------+
    | Modifier |  Append  | Effect                           |
    +==========+==========+==================================+
    |   0x00   |          |  perform a bitwise xor (^)       |
    +----------+----------+----------------------------------+
    |   0x01   |    -b    |  perform a binary xor (^^)       |
    +----------+----------+----------------------------------+

not
"""

    - Usage: :esa:`not [a]`
    - Stands for: Compute not *a*, and store the result in eax.

    +---------+------+
    | Op-code | 0x0E |
    +=========+======+
    |  Priv?  |  no  |
    +---------+------+
    |  args?  |  1   |
    +---------+------+
    | atomic? | yes  |
    +---------+------+

    :esa:`not` also updates the FLG register.

    +----------+----------------------------------+
    |  Raises  |  If                              |
    +==========+==================================+
    |  SIGSEGV | - *a* does not exist             |
    |          | - *a* cannot be read from        |
    +----------+----------------------------------+

    +----------+----------+----------------------------------+
    | Modifier |  Append  | Effect                           |
    +==========+==========+==================================+
    |   0x00   |          |  perform a bitwise not (~)       |
    +----------+----------+----------------------------------+
    |   0x01   |    -b    |  perform a binary not (!)        |
    +----------+----------+----------------------------------+

neg
"""

    - Usage: :esa:`neg [a]`
    - Stands for: Negate *a*, and store the result in eax.

    +---------+------+
    | Op-code | 0x0F |
    +=========+======+
    |  Priv?  |  no  |
    +---------+------+
    |  args?  |  1   |
    +---------+------+
    | atomic? | yes  |
    +---------+------+

    :esa:`neg` also updates the FLG register.

    +----------+----------------------------------+
    |  Raises  |  If                              |
    +==========+==================================+
    |  SIGSEGV | - *a* does not exist             |
    |          | - *a* cannot be read from        |
    +----------+----------------------------------+


jmp
"""

    The jump instruction is a family of instructions that jump to an absolute or relative address based on conditions.

    The op-code for a jump instruction is 0x11, and may take 1 or 3 arguments.
    The *suffix* field dictates what condition needs to be met to jump to a certain address.

    If the suffix field has its 4th bit set, the jump is considered to be relative, otherwise it is absolute.

    An **absolute jump** is a jump that will set the eip to whatever address is specified.
    A **relative jump** is a jump that will add an offset to eip.

    Where the processor will jump to is dictated by the first argument; If the jump is absolute, the first argument must be an lvalue,
    Otherwise if the jump is relative, the first argument must be an xvalue.

    Whether the instruction takes 1 or 3 arguments depends on the type of the instruction, which is encoded in the 5th to 8th bit of
    the *suffix* field.

    The following table shows all implemented types of the jump instruction.

    +------------+----------------------+----------+-----+------------------+
    |  Mnemonic  | Relative or absolute |  suffix  | args|   Jumps if       |
    +============+======================+==========+=====+==================+
    |    jmp     |       Absolute       |   0x00   |  1  | unconditional    |
    +------------+----------------------+----------+-----+------------------+
    |    jro     |       Relative       |   0x10   |  1  | unconditional    |
    +------------+----------------------+----------+-----+------------------+
    |    jnz     |       Absolute       |   0x01   |  1  | zero flag not set|
    +------------+----------------------+----------+-----+------------------+
    |    jez     |       Absolute       |   0x02   |  1  | zero flag set    |
    +------------+----------------------+----------+-----+------------------+
    |    jgz     |       Absolute       |   0x03   |  1  | comp flag not set|
    +------------+----------------------+----------+-----+------------------+
    |    jlz     |       Absolute       |   0x04   |  1  | comp flag set    |
    +------------+----------------------+----------+-----+------------------+
    |    jnq     |       Absolute       |   0x05   |  3  | b != c           |
    +------------+----------------------+----------+-----+------------------+
    |    jeq     |       Absolute       |   0x06   |  3  | b == c           |
    +------------+----------------------+----------+-----+------------------+
    |    jgt     |       Absolute       |   0x07   |  3  | b > c            |
    +------------+----------------------+----------+-----+------------------+
    |    jlt     |       Absolute       |   0x08   |  3  | b < c            |
    +------------+----------------------+----------+-----+------------------+
    |    jno     |       Relative       |   0x11   |  1  | zero flag not set|
    +------------+----------------------+----------+-----+------------------+
    |    jeo     |       Relative       |   0x12   |  1  | zero flag set    |
    +------------+----------------------+----------+-----+------------------+
    |    jgo     |       Relative       |   0x13   |  1  | comp flag not set|
    +------------+----------------------+----------+-----+------------------+
    |    jlo     |       Relative       |   0x14   |  1  | comp flag set    |
    +------------+----------------------+----------+-----+------------------+
    |    jon     |       Relative       |   0x15   |  3  | b != c           |
    +------------+----------------------+----------+-----+------------------+
    |    joe     |       Relative       |   0x16   |  3  | b == c           |
    +------------+----------------------+----------+-----+------------------+
    |    jog     |       Relative       |   0x17   |  3  | b > c            |
    +------------+----------------------+----------+-----+------------------+
    |    jol     |       Relative       |   0x18   |  3  | b < c            |
    +------------+----------------------+----------+-----+------------------+

    If a jump instruction is found that does not belong to any of these types, or if
    the amount of arguments suppied does not match the amount expected, a SIGILL is raised.

    If the address supplied cannot be jumped to, or if any *b* and/or *c* cannot be accessed, a SIGSEGV signal is raised.

push
""""

    The push instruction has an op-code of 0x12 and takes 1 argument, which must be an rvalue.

    This instruction is atomic, and does not take any modifiers.

pop
"""

    The pop instruction has an op-code of 0x13 and takes 1 argument, which must be an lvalue.

    This instruction is atomic, and does not take any modifiers.

call
""""

    ... Fill in

ret
"""

syscall
"""""""

raise
"""""



