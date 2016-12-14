export CC=${_CC}
export CXX=${_CXX}

make CC=${_CC} CXX=${_CXX} LDFLAGS=${_LDFLAGS} -j2 all && make CC=${_CC} CXX=${_CXX} LDFLAGS=${_LDFLAGS} test || exit $?

if [ "${_COV}" != "" ]; then
    ./coverage.sh ${_COV}
fi
