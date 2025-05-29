#!/bin/bash

GRAPH_FILE="data/CA-GrQc.txt"
EXECUTABLE="./main"
OUTPUT_FILE="resultados.csv"
SEED_FILE="semillas.txt"

# Lista fija de 30 semillas (puedes cambiarlas si quieres)
SEEDS=(13 25 45 65 89 101 123 147 159 181 199 213 237 251 269 275 289 307 333 345 369 381 399 421 435 447 459 471 487 499)

# Guardar las semillas utilizadas para garantizar reproducibilidad
printf "%s\n" "${SEEDS[@]}" > "$SEED_FILE"

# Cabecera del CSV
echo "Seed,Algoritmo,Fitness,Tiempo,Evaluaciones" > "$OUTPUT_FILE"

# Bucle de ejecución
for seed in "${SEEDS[@]}"
do
    echo "Ejecutando semilla $seed..."
    OUTPUT=$($EXECUTABLE "$GRAPH_FILE" "$seed")

    while IFS= read -r line; do
        if [[ $line =~ ^(ES|BMB|ILS|GRASP-SIBL|GRASP-NOBL)[[:space:]]+([0-9]+)[[:space:]]+([0-9.]+)[[:space:]]+([0-9]+) ]]; then
            alg=${BASH_REMATCH[1]}
            fit=${BASH_REMATCH[2]}
            time=${BASH_REMATCH[3]}
            evals=${BASH_REMATCH[4]}
            echo "$seed,$alg,$fit,$time,$evals" >> "$OUTPUT_FILE"
        fi
    done <<< "$OUTPUT"
done

echo "Listo. Resultados guardados en '$OUTPUT_FILE'. Semillas en '$SEED_FILE'."
