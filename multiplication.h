#pragma once
#include "common.h"

void MultiplyMatricesParallel(const TMatrix& a, const TMatrix& b, TMatrix& result);
void MultiplyMatricesParallelWithTransposition(const TMatrix& a, const TMatrix& b, TMatrix& result);
