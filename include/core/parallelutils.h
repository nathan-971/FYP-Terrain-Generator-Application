#ifndef _PARALLEL_UTILS_H_
#define _PARALLEL_UTILS_H_
#include <thread>
#include <vector>

namespace Parallel
{
    template<typename Lambda>
    void ParallelForRange(int width, int depth, Lambda worker)
    {
        int numThreads = std::thread::hardware_concurrency();
        numThreads = std::max(1, numThreads);
        int chunkSize = (width + numThreads - 1) / numThreads;

        std::vector<std::thread> threads;
        for (int t = 0; t < numThreads; t++)
        {
            int startX = t * chunkSize;
            int endX = std::min(startX + chunkSize, width);

            threads.emplace_back([=]() { worker(startX, endX); });
        }

        for (auto& thread : threads)
        {
            thread.join();
        }
    }
}

#endif