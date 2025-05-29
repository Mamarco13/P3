import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Cargar CSV
df = pd.read_csv("resultados.csv")

# Calcular estadísticas por algoritmo
resumen = df.groupby("Algoritmo").agg(
    Media_Fitness=("Fitness", "mean"),
    Desviacion_Fitness=("Fitness", "std"),
    Tiempo_Medio=("Tiempo", "mean"),
    Evaluaciones_Promedio=("Evaluaciones", "mean")
).reset_index()

# Calcular ranking por semilla (1 = mejor)
df["Rank"] = df.groupby("Seed")["Fitness"].rank(ascending=False, method="min")

# Agregar ranking promedio
ranking_promedio = df.groupby("Algoritmo")["Rank"].mean().reset_index().rename(columns={"Rank": "Posicion_Promedio"})

# Combinar con resumen
tabla_final = pd.merge(resumen, ranking_promedio, on="Algoritmo")

# Ordenar por posición promedio
tabla_final = tabla_final.sort_values("Posicion_Promedio")

# Mostrar resultados
print("\n=== Tabla resumen para tu memoria ===")
print(tabla_final.to_string(index=False, float_format="%.2f"))

# Guardar en CSV para la memoria
tabla_final.to_csv("tabla_memoria.csv", index=False)

# Boxplot: comparación de fitness por algoritmo
plt.figure(figsize=(10, 6))
sns.boxplot(data=df, x="Algoritmo", y="Fitness")
plt.xticks(rotation=45)
plt.title("Boxplot de Fitness por Algoritmo")
plt.tight_layout()
plt.savefig("boxplot_fitness.png")
plt.show()

# Gráfico de tiempo medio por algoritmo
plt.figure(figsize=(10, 6))
sns.barplot(data=tabla_final, x="Algoritmo", y="Tiempo_Medio")
plt.xticks(rotation=45)
plt.title("Tiempo Medio por Algoritmo")
plt.tight_layout()
plt.savefig("tiempo_medio.png")
plt.show()
