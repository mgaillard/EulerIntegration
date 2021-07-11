build/Integration naive > positions_naive.csv
build/Integration symplectic > positions_symplectic.csv
gnuplot -e "filename='positions_naive.csv'" plots/trajectories.pg > images/trajectories_naive.png
gnuplot -e "filename='positions_symplectic.csv'" plots/trajectories.pg > images/trajectories_symplectic.png
gnuplot plots/distances.pg > images/distances.png
rm positions_naive.csv
rm positions_symplectic.csv
