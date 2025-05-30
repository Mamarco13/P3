import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os

# Leer CSV combinado
df = pd.read_csv("resultados_todos.csv")

# Carpeta para tablas y gráficas
os.makedirs("graficas", exist_ok=True)
os.makedirs("tablas", exist_ok=True)

# TABLA 1 — Una por grafo
for grafo in df["Grafo"].unique():
    df_grafo = df[df["Grafo"] == grafo].copy()

    # Ranking por fitness
    df_grafo["Posicion"] = df_grafo.groupby("Seed")["Fitness"].rank(ascending=False, method="min")

    tabla1 = df_grafo.groupby("Algoritmo").agg(
        Fitness_Medio=("Fitness", "mean"),
        Tiempo_Medio=("Tiempo", "mean"),
        Evaluaciones_Promedio=("Evaluaciones", "mean"),
        Posicion_Media=("Posicion", "mean")
    ).reset_index()

    # Reordenar columnas como en el guion
    tabla1 = tabla1[["Algoritmo", "Posicion_Media", "Fitness_Medio", "Tiempo_Medio", "Evaluaciones_Promedio"]]

    # Mostrar y guardar
    print(f"\n📋 Tabla 1 – Resultados para grafo: {grafo}")
    print(tabla1.to_string(index=False, float_format="%.2f"))
    tabla1.to_csv(f"tablas/tabla1_{grafo.replace('.txt','')}.csv", index=False)

# TABLA 2 — Consolidada por algoritmo (promedios globales)
df["Rank"] = df.groupby(["Grafo", "Seed"])["Fitness"].rank(ascending=False, method="min")


tabla2 = df.groupby("Algoritmo").agg(
    Posicion_Promedia=("Rank", "mean"),
    Tiempo_Promedio=("Tiempo", "mean"),
    Evaluaciones_Promedio=("Evaluaciones", "mean")
).reset_index()

print("\n📊 Tabla 2 – Resultados Promedios (todos los grafos):")
print(tabla2.to_string(index=False, float_format="%.2f"))
tabla2.to_csv("tablas/tabla2_final.csv", index=False)

# GRÁFICOS ESTILO "QUESITO" (Barras circulares simuladas)

# Tiempo Medio
plt.figure(figsize=(7, 7))
plt.pie(tabla2["Tiempo_Promedio"], labels=tabla2["Algoritmo"], autopct="%1.1f%%", startangle=90)
plt.title("Distribución de Tiempo Promedio por Algoritmo")
plt.savefig("graficas/pastel_tiempo_promedio.png")
plt.close()

# Evaluaciones Promedio
plt.figure(figsize=(7, 7))
plt.pie(tabla2["Evaluaciones_Promedio"], labels=tabla2["Algoritmo"], autopct="%1.1f%%", startangle=90)
plt.title("Distribución de Evaluaciones Promedio por Algoritmo")
plt.savefig("graficas/pastel_evaluaciones.png")
plt.close()

# Posición Promedio
plt.figure(figsize=(10, 6))
sns.barplot(data=tabla2, x="Algoritmo", y="Posicion_Promedia")
plt.title("Posición Promedio por Algoritmo (menor es mejor)")
plt.xticks(rotation=45)
plt.tight_layout()
plt.savefig("graficas/barras_posicion_promedio.png")
plt.close()

print("\n✅ Tablas y gráficas generadas en carpetas: 'tablas/' y 'graficas/'")
