export bench=$1
export aot_target=$2

export cflags_innative="disable_tail_call check_int_division check_memory_access \
                        check_float_trunc check_indirect_call check_stack_overflow \
                        noinit sandbox strict"

export cflags_innative1="strict noinit library"

wamrc --target=${aot_target} --format=aot -o ${bench}.wamr-aot ${bench}.wasm
wamrc --target=${aot_target} --format=object -o ${bench}.o ${bench}.wasm
wamrc --target=${aot_target} --format=llvmir-opt -o ${bench}.ir ${bench}.wasm

wavm disassemble ${bench}.wasm ${bench}.wast
wavm compile ${bench}.wasm ${bench}.wavm-aot

if false
then
innative-cmd ${bench}.wasm \
        -f o3 ${cflags_innative1} \
        -o lib${bench}.so

gcc -O3 -o ${bench}_innative main_${bench}.c -L. -l${bench}
fi
