#ifndef FAULT_H
#define FAULT_H

#include <stdbool.h>

typedef enum
{
    FAULT_OK = 0,
    FAULT_INVALID_ARGUMENT,
} FaultStatus;

struct FaultGenerator
{
    /* fault parameters */
};

FaultStatus fault_init(
    struct FaultGenerator *generator);

float fault_generate(
    struct FaultGenerator *generator);

#endif