#!/bin/bash
for_seq=1
fork_seq=1
for_fork=1
echo "Running reversi with flags "$1 $2 $3 $4 -m -q -a n
for i in {1..10}
do
    output=(`/home/hsz215/cse375/p3/reversi -q -m 3 $1 $2 $3 $4 -a 1 `)
    seq=${output[24]}
    #for something in  "${output[@]}"
    #do
    #    echo $something
    #done

    for_output=(`~/cse375/p3/reversi -q -m 3 $1 $2 $3 $4 -a 2 `)
    par_for=${for_output[27]}

    fork_output=(`~/cse375/p3/reversi -q -m 3 $1 $2 $3 $4 -a 3 `)
    par_fork=${fork_output[27]}
    #echo "test" $i"....."speedup: $val   "    seq:" $seq "sec    " par: $par" sec"
    #avg=("${avg[@]}" "$val")
    
    speedup1=$(echo "scale=6; $seq / $par_for" | bc)
    speedup2=$(echo "scale=6; $seq / $par_fork" | bc)
    speedup3=$(echo "scale=6; $par_for / $par_fork" | bc)
    for_seq=("${for_seq[@]}" "$speedup1")
    fork_seq=("${fork_seq[@]}" "$speedup2")
    for_fork=("${for_fork[@]}" "$speedup3")
    echo execution time::   seq:  "   " $seq for: "    " $par_for fork: "   " $par_fork
    echo speedup "      "::   for/seq: $speedup1   fork/seq: $speedup2  fork/for: $speedup3
    echo ------------------------------------------------------------------------

done
unset for_seq[0]
unset fork_seq[0]
unset for_fork[0]

total1=0
total2=0
total3=0

for var in "${for_seq[@]}"
do
    total1=$(echo "scale=5; $total1 + $var" | bc)
done
for var in "${fork_seq[@]}"
do
    total2=$(echo "scale=5; $total2 + $var" | bc)
done
for var in "${for_fork[@]}"
do
    total3=$(echo "scale=5; $total3 + $var" | bc)
done


average=$(echo  "scale=5; $total1 / 10.0 " | bc)
seqaverage=$(echo  "scale=4; $total2 / 10.0 " | bc)
paraverage=$(echo  "scale=4; $total3 / 10.0 " | bc)
echo average speedup:  for vs seq: $average   "    fork vs seq:" $seqaverage "     fork vs for: "$paraverage ""
