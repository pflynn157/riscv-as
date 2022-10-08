cmake_minimum_required(VERSION 3.0.0)
project(riscv-as)

set(SRC
    lex.cpp
    main.cpp
    pass1.cpp
    pass2.cpp
)

add_executable(rvas ${SRC})

