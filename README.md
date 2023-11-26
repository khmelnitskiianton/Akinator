# Akinator

This is cool program that can generate data base on binary tree (yes/no)
You can upload data, guess elements or add them, see data(or log), take definition or compare objects, and save data, and vocalize all text with Festival!

- [Introduction](#inroduction)
- [Commands and options](#commands_and_options)
- [Options of compiling](#options_of_compiling)
- [Myassert](#myassert)
- [Veryficator](#verificator)
- [Logs](#logs)

## Introduction

To start program you need to use CMake and run program

1.  Clone repository:

    ```c
    $ git clone https://github.com/khmelnitskiianton/Akinator.git
    ```

2.  Compile CMake, write command in main directory in repo:

    ```c
    $ cmake .
    ```

3.  Run CMake (compile program), write command in main directory in repo:

    ```c
    $ make
    ```

4.  Run program:

    ```c
    $ ./akinator.exe <file_to_save_data_base>.txt
    ```
    (before you need to create this file)

5.  Program use voice overing with festival!
    You can turn it off, undef `INCLUDE_FESTIVAL` in `./hdr/func.h`
    -   If you have Linux install it:
        ```c
        $ sudo apt install festival festvox-ru 
        ```
    -   If you have Windows maybe you can install it, but easily use Linux
    -   If you have WSL may God help you
        Take a chance with this guide `https://research.wmz.ninja/articles/2017/11/setting-up-wsl-with-graphics-and-audio.html`
        If any errors search them in internet (I spent 2 days to connect audio in WSL with Windows with PulseAudio)

6. Answer questions and work(or have fun) with Akinator!!!

# Commands and options

0.  First program ask you about uploading data base from file
    Write `Y/N` to do it or don't.

1. `[У]гадать` You need to write `У`
    This command guessing your object, while asking questions.
    Answer `Y/N`.
    If object doesn't exist Akinator ask you to add object and it difference (So you can add objects like this).

2. `[Д]ать определение` You nees to write `Д`
    This command ask you name of object and gives you definition of object from data base.
    You need to enter its name, and its gives you full path.

3. `[С]равнить объекты` You nees to write `С`
    This command ask you name of two objects and gives you their similar features and diffrences.
    You need to enter two names.

4. `[П]оказать дерево` You nees to write `П`
    This command generates png picture of data base in `show_graph.png`
    If you want svg file, open log.html with integrated vector graphic

5. `[В]ыйти с сохранением или [Б]ез сохранения` 
    Write `В` to end programm with saving data base in txt file.
    Write `Б` to end programm without saving data base.

Thats all about working with Akinator!

## Options of compiling

INCLUDE_FESTIVAL
DEBUG_LOG
DEBUG_MYASSERT
DEBUG_VERIFY

## Myassert

`myassert.h`

Now it relized like MYASSERT() that take all errors with enum and write it to console

## Verificator

`verificator.cpp` `verificator.h`

Checking tree for loops and other errors. Functions returns EnumOfErrors 

## Logs

`log.cpp` `log.h`

Logs use GraphViz to vizualized graphs in pictures
After generating image it includes to html file `log.html`

![Пример графического лога!](https://github.com/khmelnitskiianton/Akinator/blob/main/example_log.svg)