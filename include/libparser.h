#ifndef __LIBPARSER__
#define __LIBPARSER__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "libstruct.h"
#include "libhash.h"
#include "libavl.h"


void leitor(FILE *arquivo, tHash *hash_cidade);

#endif