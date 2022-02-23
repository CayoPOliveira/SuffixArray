#! /bin/sh

mkdir -p results 

for i in /home/data/pizza/*; do 

	test=${i##*/}; 
	date >> results/${test}
	echo "####";
	echo ${test}; 
	ls -lah $i

	for j in {0..6}; do 
		echo ALG $j; 
		../main-sa $i -A $j -t >> results/${test};
		echo "" >> results/${test};
		echo ""
	done;
done;