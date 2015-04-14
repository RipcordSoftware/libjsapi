!#/bin/sh
make test

rm -f valgrind-*.out

for f in src/libjsapi/build/Debug/GNU-Linux-x86/tests/TestFiles/*; do
	valgrind --log-file=valgrind-%p.out $f --gtest_repeat=10
done

cat valgrind-*.out
