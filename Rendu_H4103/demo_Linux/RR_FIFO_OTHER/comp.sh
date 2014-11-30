#/bin/sh

./test $1 90 &
sleep 2;
./test $1 50 &
sleep 2;
./test $1 50 &
sleep 2;
./test $1 30 &
