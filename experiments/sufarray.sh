#! /bin/sh

mkdir -p results 

for i in /home/data/pizza/*; do 

	test=${i##*/}; 
	date >> results/${test}
	echo "####";
	echo ${test}; 
	#ls -lah $i

	for j in {1..6}; do 
		echo "ALG $j"; 
		echo "ALG $j" >> results/${test}; 
		./main_sa $i -A $j -t >> results/${test};
		echo "" >> results/${test};
		echo ""
	done;
done;
