for i in {1,4,8,12,16}
do
  time  mpirun -np $i lethe-particles $1  >> "$i"_proc.dat
  # let core cool down
  sleep 20
done
