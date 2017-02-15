#!/bin/bash
fichero="exampleA1.txt";
cpu=1;
planificador=( PRIO PRIOTR );  


make clean
make
#clear


if [[ -d resultados ]]; then
	rm -rf resultados;
fi;

mkdir resultados
cd resultados

echo "Creando archivo CPU..."
for ((num = 1; num <= cpu; num++)); do
	for plan in ${planificador[*]}; do
		../schedsim  -n $num -i ../examples/$fichero -s $plan;
		for ((i = 0; i < num; i++)); do
			mv CPU_$i.log ./$plan-CPU-$i.log;
		done
	done
done

cd ../../gantt-gplot

echo "Generando graficas..."
for ((num = 1; num <= cpu; num++)); do
	for plan in ${planificador[*]}; do
		for ((i = 0; i < num; i++)); do
			./generate_gantt_chart ../schedsim/resultados/$plan-CPU-$i.log;
		done
	done
done
