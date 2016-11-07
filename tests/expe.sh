for f in ../instances/*.dat
do
    printf $f >> resultats
    ./../main $f >> resultats
done
