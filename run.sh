for size in {50,100,300,500}; do
    dir="./log/raw/${size}/"
    rm -rf $dir
    mkdir -p $dir
    arraydir="${dir}array/"
    plistdir="${dir}plist/"
    rm -rf "${dir}plist"
    rm -rf "${dir}array"
    mkdir "${dir}plist"
    mkdir "${dir}array"
    for f in ../stg/${size}/*.stg; do
        for p in {4..32..4}; do
            plist="${plistdir}$(basename ${f} .stg)_pe${p}_plist.log" 
            array="${arraydir}$(basename ${f} .stg)_pe${p}_array.log" 
            echo -n "" > ${plist}
            for i in {1..10}; do
                echo "$f $p" 
                echo -n "$(basename ${f}) " >> "${plist}"
                ./main_plist.out $p < $f 2>> "${plist}" > /dev/null
                echo -n "$(basename ${f}) " >> "${array}"
                ./main_task.out $p < $f 2>> "${array}" > /dev/null
            done
        done
    done
done
