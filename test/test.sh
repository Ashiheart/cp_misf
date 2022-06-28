./test.out 6 < ../stg/sample.stg 2> /dev/null
dot -Tpng graph.gv -o graph.png && display graph.png
