#size="50 100 300 500 1000 1500 2000 2500 3000 3500 4000 5000"
#algo="array plist"
#pe="1 2 3 4 8 12 16"
stg="../stg"
log="../log/raw"
size="5000"
algo="array plist"
pe="4"
for s in $size; do
    for a in $algo; do
        dir="../log/raw/${s}/${a}/"
        mkdir -p ${dir}
        for f in ../stg/${s}/*.stg; do
            for p in $pe; do
                log="${dir}$(basename ${f} .stg)_pe${p}_${a}.log"
                echo -n "" > $log
                echo "$s $a $(basename ${f} .stg) $p" 
                for i in {1..10}; do
                    echo -n "$(basename ${f}) " >> $log
                    "simu/${a}.out" $p < $f 2>> $log > /dev/null
                    echo "" >> $log
                done
            done
        done
    done
done
size="5000"
algo="array plist"
pe="1 2 3 4 8 12 16"
for s in $size; do
    for a in $algo; do
        dir="../log/raw/${s}/${a}/"
        mkdir -p ${dir}
        for f in ../stg/${s}/rand0125.stg; do
            for p in $pe; do
                log="${dir}$(basename ${f} .stg)_pe${p}_${a}.log"
                echo -n "" > ${log}
                echo "$s $a $(basename ${f} .stg) $p" 
                for i in {1..10}; do
                    echo -n "$(basename ${f}) " >> "${log}"
                    "simu/${a}.out" $p < $f 2>> "${log}" > /dev/null
                    echo "" >> $log
                done
            done
        done
    done
done
