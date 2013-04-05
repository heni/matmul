#include "transposition.h"
#include <boost/thread/future.hpp>
#include <algorithm>

class TransposeJob {
    const TMatrix& A;
    TMatrix& Result;
    const size_t StartRow;
    const size_t EndRow;
public:
    TransposeJob(const TMatrix& a, TMatrix& result, const size_t stRow, const size_t endRow) 
	: A(a), Result(result), StartRow(stRow), EndRow(endRow)
    {}
    
    void operator()() {
	for (size_t i = StartRow; i < EndRow; ++i) {
	    Result[i].resize(A.size());
	    for (size_t j = 0; j < A.size(); ++j) 
		Result[i][j] = A[j][i];
	}
    }
};


void TransposeMatrixParallel(const TMatrix& a, TMatrix& result) {
    result.resize(a[0].size());
    const size_t RowsPerJob = (result.size() + NPROC - 1) / NPROC;
    std::vector<boost::unique_future<void>> hndls(NPROC);
    for (size_t i = 1; i < NPROC; ++i) 
	hndls[i] = boost::async(TransposeJob(a, result, i * RowsPerJob, std::min((i+1)*RowsPerJob, result.size())));
    TransposeJob(a, result, 0, RowsPerJob)();
    boost::wait_for_all(hndls.begin() + 1, hndls.end());
}


