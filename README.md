# opencl-cipher

This project provides the open-cl implementations of the following ciphers, with the support for CTR mode.

## Supported ciphers

- AES128
- AES192
- AES256

- CAMELLIA128
- CAMELLIA192
- CAMELLIA256

- CLEFIA128
- CLEFIA192
- CLEFIA256

- DES
- DES2
- DES3

- HIGHT

- MISTY1

- PRESENT MEMORY
- PRESENT SPEED

- SEED OLD
- SEED

## Project details

The project is divided in 3 main parts.

The implementation itself of the algorithms.
The test suite that verifies the correctness of the implemented algorithms.
The benchmarks suite that can be used to verify the performances of the algorithms by choosing the parallelization parameters and the file size.

By calling the makefile a binary executable is produced that call all the functions of the test suite and tries some default benchmarks.
For more details check : benchmarks/benchmarks_main.c and /tests/test_main.c

## Usage

The functions implemented in this library need a pointer to a file to encrypt a pointer that will point to the output, a valid key, a work group size and the device option.
Example:

`cl_event aes128CtrEncrypt(char* fileName, word* key, uint8_t* output,size_t local_item_size, char* deviceType);`


## Authors

Angelo Gallarello
Francesco Lattari
