#ifndef AES_CIPHER_H
#define AES_CIPHER_H

void aesCtrEncript(char* inputFile, word key[Nk] __attribute__((key)), char* outputFile);
