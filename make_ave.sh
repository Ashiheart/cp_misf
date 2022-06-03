rm -rf task50_pe4_array.log
for i in ./log/raw/50/array/*pe4_array.log; do
    echo -n "$(basename ${i} .log) " >> task50_pe4_array.log
    cat $i | grep -o "[^ ]* $" | awk '{sum+=$1} END {print sum/NR}' >> task50_pe4_array.log
done
