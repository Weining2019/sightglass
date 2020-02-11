mkdir -p out

export bench=$1
export aot_target=$2

export cflags_innative="disable_tail_call check_int_division check_memory_access \
                        check_float_trunc check_indirect_call check_stack_overflow \
                        noinit sandbox strict"

export cflags_innative1="strict noinit library"

gcc -O3 -o out/${bench}_native -Dblack_box=set_res \
        -Dbench=${bench} \
        -I../../include ${bench}.c main/main_${bench}.c main/my_libc.c

clang-8 -O3 --target=wasm32 -nostdlib \
        -Dblack_box=set_res \
        -DINNATIVE_WASM \
        -I../../include \
        -Wno-unknown-attributes \
        -Wno-incompatible-library-redeclaration \
        -Wl,--initial-memory=1310720,--allow-undefined \
        -Wl,--strip-all,--no-entry \
        -o out/${bench}.wasm \
        -Wl,--export=app_main \
        ${bench}.c main/main_${bench}.c main/my_libc.c

wamrc --target=${aot_target} --format=aot -o out/${bench}.wamr-aot out/${bench}.wasm
wamrc --target=${aot_target} --format=object -o out/${bench}.o out/${bench}.wasm
wamrc --target=${aot_target} --format=llvmir-opt -o out/${bench}.ir out/${bench}.wasm

wavm disassemble out/${bench}.wasm out/${bench}.wast
wavm compile out/${bench}.wasm out/${bench}.wavm-aot

if false
then
innative-cmd out/${bench}.wasm \
        -f o3 ${cflags_innative1} \
        -o out/lib${bench}.so

gcc -O3 -DINNATIVE_NATIVE -o out/${bench}_innative main/main_${bench}.c -Lout -l${bench}
fi
