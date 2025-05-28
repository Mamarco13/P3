#!/bin/bash
if [ $# -eq 1 ]; then
  SEED=$1
elif [ $# -eq 0 ]; then
  RAW_SEED=$(od -An -N4 -tu4 < /dev/urandom | tr -d ' ')
  SEED=$((RAW_SEED % 1000000))
  echo "[INFO] Semilla aleatoria generada: $SEED"
else
  echo "Uso: $0 [semilla_base]"
  exit 1
fi

GRAFOS=("data/p2p-Gnutella05.txt" "data/p2p-Gnutella08.txt" "data/p2p-Gnutella25.txt" "data/CA-GrQc.txt")

for GRAFO in "${GRAFOS[@]}"; do
  echo "============================================"
  echo ">> Ejecutando sobre grafo: $GRAFO"
  echo ">> Semilla base: $SEED"
  echo "============================================"
  ./main "$GRAFO" "$SEED"
  echo
done
