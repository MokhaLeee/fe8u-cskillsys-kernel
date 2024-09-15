
# FE-Clib fit to Decomp

C header files for **FireEmblem8U** port from [decomp](https://github.com/FireEmblemUniverse/fireemblem8u)

## Usage (for WSL & ubuntu):

1. Suppose you have a **.c** file (named as *test.c*), put it to repo directory and then:

2. **.c** to **.o**
    ```sh
    make -f template.mk test.o
    ```
    Now you get the obj file *test.o* and then:

3. **.o** to **.lyn.event**

    Get [lyn](https://github.com/StanHash/lyn) and then:

    ```bash
    ./lyn test.o reference/fireemblem8.ref.o > test.lyn.event
    ```

## See also

* [**fe8-chax-template**](https://github.com/MokhaLeee/fe8-chax-template), an example to use this C-Lib to compile your own source files.
* [**FE-CBuild**](https://github.com/MokhaLeee/FE-CBuild), project to insert everything through EA for CHAX.
* [**FireEmblemUniverse/fireemblem8u**](https://github.com/FireEmblemUniverse/fireemblem8u), a decompilation of Fire Emblem: The Sacred Stones (US)
