#ifndef HEDGEARBITRAGE_INCLUDE_GETDATA_H
#define HEDGEARBITRAGE_INCLUDE_GETDATA_H
#include <Python.h>
#include <iostream>
#include <ioStuff.hpp>
#include <unordered_map>
#include <jsonStuff.hpp>
#include <execution>
#include <algorithm>
#include <mutex>
#include <glob.h>
#include <myOracle.hpp>

namespace getData {
    using mytype=std::map<long long,std::tuple<double,double,double,double,double,double>>;
    void getData1(int beginT,std::string intervel,std::string contract) ;
    std::vector<mytype> getData2();
}
#endif