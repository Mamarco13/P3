#!/bin/bash

EXECUTABLE="./main"
OUTPUT_FILE="resultados_todos.csv"
SEED_FILE="semillas.txt"

# Archivos de entrada (todos los grafos)
GRAFOS=(data/*.txt)

# Semillas fijas
SEEDS=(13 25 45 65 89 101 123 147 159 181 199 213 237 251 269 275 289 307 333 345 369 381 399 421 435 447 459 471 487 499)

# Guardar semillas usadas
printf "%s\n" "${SEEDS[@]}" > "$SEED_FILE"

# Cabecera del CSV
echo "Grafo,Seed,Algoritmo,Fitness,Tiempo,Evaluaciones" > "$OUTPUT_FILE"

# Expresiones regulares válidas para los algoritmos
VALID_ALGOS="^(Random|Greedy|LSsmall|LSall|AGE_SinOrden|ES|BMB|ILS|GRASP-SIBL|GRASP-NOBL)[[:space:]]+([0-9]+)[[:space:]]+([0-9.]+)[[:space:]]+([0-9]+)"

# Iterar sobre cada grafo y semilla
for grafo in "${GRAFOS[@]}"; do
    for seed in "${SEEDS[@]}"; do
        echo "Ejecutando $grafo con semilla $seed..."
        OUTPUT=$($EXECUTABLE "$grafo" "$seed")

        while IFS= read -r line; do
            if [[ $line =~ $VALID_ALGOS ]]; then
                alg=${BASH_REMATCH[1]}
                fit=${BASH_REMATCH[2]}
                time=${BASH_REMATCH[3]}
                evals=${BASH_REMATCH[4]}
                echo "$(basename "$grafo"),$seed,$alg,$fit,$time,$evals" >> "$OUTPUT_FILE"
            fi
        done <<< "$OUTPUT"
    done
done

echo "Finalizado. Resultados guardados en '$OUTPUT_FILE'. Semillas en '$SEED_FILE'."
