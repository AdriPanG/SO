#!/bin/bash
var = 0;
planificador=( RR SJF FCFS PRIO );  

make clean
make
clear

cd examples #Entramos en ejemplo
echo "Nombre del fichero:"
read fichero

	while [[ $var -eq 0 ]]; do
		if [[ -f ./$fichero ]]; then
			echo "Existe el fichero.";
			var=1;
		else 
			echo "No existe el fichero, introducelo de nuevo."
			read fichero;
		fi;
	done
cd .. #salimos de examples

#Preguntamos el numero de cpus
var=0;
echo "Numero de cpus: "
read cpu;

	while [[ $var -eq 0 ]]; do
		if [[ cpu -gt 0 && cpu -le 8 ]]; then
			var=1;
		else 
			echo "Introduzca de nuevo el numero de cpus entre 1 y 8." 
			read cpu;
		fi;
	done;

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
