#include "multiplication.h"
#include "transposition.h"

#include <boost/thread/future.hpp>
#include <cassert>

template<bool ToTransposed=true>
class MultiplyJob {
    const TMatrix& A, B;
    TMatrix& Result;
    const size_t StartRow;
    const size_t EndRow;

    double GetInternalProduct(size_t i, size_t j) const;
public:
    MultiplyJob(const TMatrix& a, const TMatrix& b, TMatrix& result, const size_t stRow, const size_t endRow)
	: A(a), B(b), Result(result), StartRow(stRow), EndRow(endRow)
    {}

    void operator()() {
	for (size_t i = StartRow; i < EndRow; ++i) {
            const size_t columns = ToTransposed ? B.size() : B[0].size();
	    Result[i].resize(columns); 
	    for (size_t j = 0; j < columns; ++j) {
		Result[i][j] = GetInternalProduct(i, j);
	    }
	}
    }
};

template<>
inline double MultiplyJob<true>::GetInternalProduct(size_t i, size_t j) const {
    double retval = 0.0;
    for (size_t k = 0; k < A[i].size(); ++k)
        retval += A[i][k] * B[j][k];
    return retval;
}

template<>
inline double MultiplyJob<false>::GetInternalProduct(size_t i, size_t j) const {
    double retval = 0.0;
    for (size_t k = 0; k < A[i].size(); ++k)
        retval += A[i][k] * B[k][j];
    return retval;
}

void MultiplyMatricesParallel(const TMatrix& a, const TMatrix& b, TMatrix& result) {
    assert(a[0].size() == b.size());
    result.resize(a.size());
    const size_t RowsPerJob = (a.size() + NPROC - 1) / NPROC;
    std::vector<boost::unique_future<void>> hndls(NPROC);
    for (size_t i = 1; i < NPROC; ++i)
        hndls[i] = boost::async(MultiplyJob<false>(a, b, result, i * RowsPerJob, std::min((i + 1)*RowsPerJob, a.size())));
    MultiplyJob<false>(a, b, result, 0, RowsPerJob)();        
    boost::wait_for_all(hndls.begin() + 1, hndls.end());
}


void MultiplyMatricesParallelWithTransposition(const TMatrix& a, const TMatrix& b, TMatrix& result) {
    assert(a[0].size() == b.size());
    TMatrix bt;
    TransposeMatrixParallel(b, bt);

    result.resize(a.size());
    const size_t RowsPerJob = (a.size() + NPROC - 1) / NPROC;
    std::vector<boost::unique_future<void>> hndls(NPROC);
    for (size_t i = 1; i < NPROC; ++i)
	hndls[i] = boost::async(MultiplyJob<true>(a, bt, result, i * RowsPerJob, std::min((i+1)*RowsPerJob, a.size())));
    MultiplyJob<true>(a, bt, result, 0, RowsPerJob)();      
    boost::wait_for_all(hndls.begin() + 1, hndls.end());
}


