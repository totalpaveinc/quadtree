
#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <tp/geom/Extent.h>
#include <tp/qt/QuadTree.h>

void handler(int sig) {
    void *array[10];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 10);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}

bool $myfilter(const TP::geom::Extent<double>& extent, const TP::qt::QuadPoint* point) {
    return true;
}

int main(int argc, char** argv) {
    using namespace TP::geom;
    using namespace TP::qt;

    signal(SIGSEGV, handler);

    int* xs = new int[1000];
    for (int i = 0; i < 1000; i++) {
        xs[i] = i + 1;
    }

    Extent<double> extent(0.0, 0.0, 100.0, 100.0);
    QuadTree qt(5, extent);

    qt.insert(25.0, 25.0, &xs[0]);
    qt.insert(10.0, 10.0, &xs[1]);
    qt.insert(15.0, 15.0, &xs[2]);
    qt.insert(1.0, 1.0,   &xs[3]);
    qt.insert(30.0, 30.0, &xs[4]);
    qt.insert(30.0, 10.0, &xs[5]);
    qt.insert(50.0, 10.0, &xs[6]);

    std::vector<const void*> results;
    Extent<double> query(25.0, 25.0, 100.0, 100.0);
    std::function<bool(const Extent<double>&, const QuadPoint*)> filter = $myfilter;
    qt.query(query, results, &filter);

    if (results.size() == 0) {
        std::cout << "No Results" << std::endl;
    }
    else {
        std::cout << std::to_string(results.size()) << " results found." << std::endl;
        for (std::size_t i = 0; i < results.size(); i++) {
            int* x = (int*)results[i];
            std::cout << "\t" << std::to_string(i) << ": " << std::to_string(*x) << std::endl;
        }
    }

    return 0;
}
