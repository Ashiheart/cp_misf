size="50"
algo="plist"
pe="4"
for s in $size; do
    for a in $algo; do
        dir="../log/raw/${s}/${a}/"
        mkdir -p ${dir}
        for f in ../stg/${s}/*.stg; do
            for p in $pe; do
                log="${dir}$(basename ${f} .stg)_pe${p}_${a}.log"
                echo -n "" > ${log}
                echo "$s $a $(basename ${f} .stg) $p" 
                for i in {1}; do
                    echo -n "$(basename ${f}) " >> "${log}"
                    "./${a}.out" $p < $f 2>> "${log}" > /dev/null
                done
            done
        done
    done
done
