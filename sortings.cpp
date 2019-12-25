#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <cmath>
#include <random>

#define int uint32_t
using iterator = std::vector<int>::iterator;

void ssort(std::vector<int>* array) {
    std::stable_sort(array->begin(), array->end());
}

iterator Merge(iterator begin, iterator end) {
    auto mid = begin;
    int dist = std::distance(begin, end);
    std::advance(mid, dist / 2);

    std::vector<int> ar;
    ar.resize(dist);
    auto first_dest = ar.begin();

    int left, right;
    left = right = 0;
    while (begin + left < mid && mid + right < end) {
        if (mid[left] < mid[right]) {
            first_dest[left + right] = mid[left];
            ++left;
        } else {
            first_dest[left + right] = mid[right];
            ++right;
        }
    }
    while (begin + left < mid) {
        first_dest[left + right] = mid[left];
        ++left;
    }
    while (mid + right < end) {
        first_dest[left + right] = mid[right];
        ++right;
    }

    for (int i = 0; i < dist; ++i) {
        begin[i] = first_dest[i];
    }
}

void merge_sort(iterator begin, iterator end) {
    if (end - begin <= 1) {
        return;
    }
    auto mid = begin;
    std::advance(mid, std::distance(begin, end) / 2);

    merge_sort(begin, mid);
    merge_sort(mid, end);
    Merge(begin, end);
}

void std_qsort(std::vector<int>* array) {
    std::qsort(&(*array)[0], array->size(), sizeof(int), [](const void* a, const void* b)
    {
        int arg1 = *static_cast<const int*>(a);
        int arg2 = *static_cast<const int*>(b);

        if(arg1 < arg2) return -1;
        if(arg1 > arg2) return 1;
        return 0;
    });
}

int increment(int32_t  inc[], int32_t  size) {
    int p1, p2, p3, s;

    p1 = p2 = p3 = 1;
    s = -1;
    do {
        if (++s % 2) {
            inc[s] = 8 * p1 - 6 * p2 + 1;
        } else {
            inc[s] = 9 * p1 - 9 * p3 + 1;
            p2 *= 2;
            p3 *= 2;
        }
        p1 *= 2;
    } while(3 * inc[s] < size);

    return s > 0 ? --s : 0;
}

void shellSort2(iterator begin, iterator end) {
    int32_t inc, i, j, seq[40];
    int32_t s;

    // вычисление последовательности приращений
    s = increment(seq, end - begin);
    while (s >= 0) {
        // сортировка вставками с инкрементами inc[]
        inc = seq[s--];

        for (i = inc; i < end - begin; i++) {
            int32_t temp = begin[i];
            for (j = i-inc; (j >= 0) && (*(begin + j) > temp); j -= inc)
                begin[j + inc] = begin[j];
            begin[j + inc] = temp;
        }
    }
}

void my_quick_sort(iterator begin, iterator end, double limit, int depth) {
    if (end <= begin) return;

    if (depth > limit) {
        shellSort2(begin, end);
        return;
    }
    int32_t size = end - begin;

    int32_t v = *end;
    int32_t left = 0;
    int32_t right = size - 1;
    int32_t p = -1;
    int32_t q = size;
    while (left <= right) {
        while (begin[left] < v) {
            ++left;
        } while (begin[right] > v) {
            --right;
        } if (left >= right) {
            break;
        }
        std::swap(begin[left], begin[right]);
        if (*(begin + left) == v) {
            std::swap(begin[++p], begin[left]);
        }
        ++left;
        if (begin[right] == v) {
            std::swap(begin[--q], begin[right]);
        }
        --right;
    }
    std::swap(begin[left], begin[size]);
    right = left - 1;
    ++left;
    for (int32_t k = 0; k <= p; ++k, right--) {
        std::swap(begin[k], begin[right]);
    }
    for (int32_t k = size - 1; k >= q; k--, left++) {
        std::swap(begin[k], begin[left]);
    }
    my_quick_sort(begin, begin + right, limit, depth + 1);
    my_quick_sort(begin + left, end, limit, depth + 1);
}

void radixSort1(iterator begin, int size) {
    int s = 1;
    int trs = pow(2, 8 * s);
    std::vector<int> c(trs);
    std::vector<int> b(size);
    std::vector<int> offsets(size);

    for (int i = 0; i < 4 / s; ++i) {
        int offset_left = 32 - 8 * i * s - 8 * s;
        int offset_right = 32 - 8 * s;

        for (int j = 0; j < trs; ++j)
            c[j] = 0;
        for (int j = 0; j < size; ++j) {
            ++c[(begin[j] << offset_left) >> offset_right];
        }
        int count = 0;

        for (int j = 0; j < trs; ++j)
            c[j] = (count += c[j]) - c[j];

        for (int j = 0; j < size; ++j)
            b[c[(begin[j] << offset_left) >> offset_right]++] = begin[j];

        for (int q = 0; q < size; ++q)
            begin[q] = b[q];
    }
}

void radixSort2(iterator begin, int size) {
    int s = 1;
    int trs = pow(2, 8 * s);
    std::vector<int> c(trs);
    std::vector<int> b(size);
    std::vector<int> offsets(size);

    for (int i = 0; i < 4 / s; ++i) {
//        int offset_left = 32 - 8 * i * s - 8 * s;
//        int offset_right = 32 - 8 * s;

        for (int j = 0; j < trs; ++j)
            c[j] = 0;
        for (int j = 0; j < size; ++j) {
            ++c[(begin[j] << (32 - 8 * i * s - 8 * s)) >> (32 - 8 * s)];
        }
        int count = 0;

        for (int j = 0; j < trs; ++j)
            c[j] = (count += c[j]) - c[j];

        for (int j = 0; j < size; ++j)
            b[c[(begin[j] << (32 - 8 * i * s - 8 * s)) >> (32 - 8 * s)]++] = begin[j];

        for (int q = 0; q < size; ++q)
            begin[q] = b[q];
    }
}

void radixSort3(iterator begin, int size) {
    int s = 1;
    int trs = pow(2, 8 * s);
    std::vector<int> c(trs);
    std::vector<int> b(size);
    std::vector<int> offsets(size);

    for (int i = 0; i < 4 / s; ++i) {

        for (int j = 0; j < trs; ++j)
            c[j] = 0;
        for (int j = 0; j < size; ++j) {
            ++c[offsets[j] = (begin[j] << (32 - 8 * i * s - 8 * s)) >> (32 - 8 * s)];
        }
        int count = 0;

        for (int j = 0; j < trs; ++j)
            c[j] = (count += c[j]) - c[j];

        for (int j = 0; j < size; ++j)
            b[c[offsets[j]]++] = begin[j];

        for (int q = 0; q < size; ++q)
            begin[q] = b[q];
    }
}

uint64_t time(int sort, int size, int maximum) {
    std::random_device rnd_device;
    std::mt19937 mersenne_engine {rnd_device()};
    std::uniform_int_distribution<int> dist {1, maximum};
    auto gen = [&dist, &mersenne_engine](){
        return dist(mersenne_engine);
    };
    std::vector<int> vec0(size);
    generate(vec0.begin(), vec0.end(), gen);

    std::vector<int> vec1(vec0);
    std::vector<int> vec2(vec0);
    std::vector<int> vec3(vec0);
    std::vector<int> vec4(vec0);
    std::vector<int> vec5(vec0);
    std::vector<int> vec6(vec0);


    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point end;
    if (sort == 0) {
        start = std::chrono::high_resolution_clock::now();
        ssort(&vec0);
        end = std::chrono::high_resolution_clock::now();
    } else if (sort == 1) {
        start = std::chrono::high_resolution_clock::now();
        merge_sort(vec1.begin(), vec1.end());
        end = std::chrono::high_resolution_clock::now();
    } else if (sort == 2) {
        start = std::chrono::high_resolution_clock::now();
        std_qsort(&vec2);
        end = std::chrono::high_resolution_clock::now();
    } else if (sort == 3) {
        double limit = std::log2(vec3.size());
        start = std::chrono::high_resolution_clock::now();
        my_quick_sort(vec3.begin(), vec3.end() - 1, limit, 0);
        end = std::chrono::high_resolution_clock::now();
    } else if (sort == 4) {
        start = std::chrono::high_resolution_clock::now();
        radixSort1(vec4.begin(), vec4.size());
        end = std::chrono::high_resolution_clock::now();
    } else if (sort == 5) {
        start = std::chrono::high_resolution_clock::now();
        radixSort2(vec5.begin(), vec5.size());
        end = std::chrono::high_resolution_clock::now();
    } else if (sort == 6) {
        start = std::chrono::high_resolution_clock::now();
        radixSort3(vec5.begin(), vec5.size());
        end = std::chrono::high_resolution_clock::now();
    } else if (sort == 7) {
        start = std::chrono::high_resolution_clock::now();
        shellSort2(vec6.begin(), vec6.end());
        end = std::chrono::high_resolution_clock::now();
    }
    auto int_nsc = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    return int_nsc.count();
}

signed main() {
    uint64_t stdqsort, stdsort, merge, myqsort, radix1, radix2, radix3, shell;
    stdqsort = stdsort = merge = myqsort = radix1 = radix2 = radix3 = shell = 0;

    int s = 34801389;
    int loops = 1;

    for (int i = 0; i < loops; ++i) {
        stdsort += time(0, s, 1000000000);
        merge += time(1, s, 1000000000);
        stdqsort += time(2, s, 1000000000);
        myqsort += time(3, s, 1000000000);
        radix1 += time(4, s, 1000000000);
        radix2 += time(5, s, 1000000000);
        radix3 += time(6, s, 1000000000);
        shell += time(7, s, 1000000000);
        std::cout << i << std::endl;
    }
    std::cout << std::endl;
    std::cout << "std::qsort = " << stdqsort / loops << std::endl;
    std::cout << "std::sort  = " << stdsort / loops << std::endl;
    std::cout << "merge      = " << merge / loops << std::endl;
    std::cout << "my qsort   = " << myqsort / loops << std::endl;
    std::cout << "Shell      = " << shell / loops << std::endl;
    std::cout << "Radix1     = " << radix1 / loops << std::endl;
    std::cout << "Radix2     = " << radix2 / loops << std::endl;
    std::cout << "Radix3     = " << radix3 / loops << std::endl;
    return 0;
}
