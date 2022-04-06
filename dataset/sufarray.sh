#! /bin/sh

mkdir -p results 

for i in /home/data/pizza/*; do 

	test=${i##*/}; 
	date >> results/${test}
	echo "####";
	echo ${test}; 
	ls -lah $i

	for j in {0..7}; do 
		echo ALG $j; 
		./main_sa $i -A $j -t >> results/${test};
		echo "" >> results/${test};
		echo ""
	done;
done;
