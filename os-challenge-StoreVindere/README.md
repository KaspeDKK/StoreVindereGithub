cd "os-challenge-StoreVindere\x86_64"

vagrant up

vagrant status

vagrant ssh server

cd /os-challenge-common/

make clean

make

./server <Port>