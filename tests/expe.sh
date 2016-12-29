for f in ../instances/*.dat
do
    printf $f
    ./../main $f
done
