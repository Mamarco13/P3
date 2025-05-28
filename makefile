# Nombre del ejecutable
TARGET = main

# Directorios
SRC_DIR = src
INC_DIR = include
COMMON_DIR = common

# Compilador y flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -I$(INC_DIR) -I$(COMMON_DIR)

# Archivos fuente
SRCS = \
	main.cpp \
	$(SRC_DIR)/graph_loader.cpp \
	$(SRC_DIR)/icm.cpp \
	$(SRC_DIR)/random_solver.cpp \
	$(SRC_DIR)/greedy_solver.cpp \
	$(SRC_DIR)/localsearch_solver.cpp

# Objeto resultante
OBJS = $(SRCS:.cpp=.o)

# Reglas
all: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
	@echo "[INFO] Compilando $(TARGET)..."
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

# Limpieza
clean:
	rm -f $(TARGET) *.o $(SRC_DIR)/*.o
	@echo "[INFO] Limpiado."

# Recompila si cambia algún header
$(SRCS): $(HEADERS)

# Headers opcionales (esto no fuerza recompilación automática, pero da trazabilidad)
HEADERS = \
	$(INC_DIR)/GeneticAlgorithm.hpp \
	$(INC_DIR)/AGG_SinOrden.hpp \
	$(INC_DIR)/AGG_ConOrden.hpp \
	$(INC_DIR)/AGE_SinOrden.hpp \
	$(INC_DIR)/AGE_ConOrden.hpp \
	$(INC_DIR)/AM_10_1.hpp \
	$(INC_DIR)/AM_10_01.hpp \
	$(INC_DIR)/AM_10_01mej.hpp \
	$(INC_DIR)/SNIMPProblem.hpp \
	$(COMMON_DIR)/problem.h \
	$(COMMON_DIR)/solution.h \
	$(COMMON_DIR)/util.h \
	$(COMMON_DIR)/mh.h \
	$(COMMON_DIR)/mhtrayectory.h \
	$(COMMON_DIR)/random.hpp \
	$(INC_DIR)/graph_loader.hpp \
	$(INC_DIR)/icm.hpp \
	$(INC_DIR)/random_solver.hpp \
	$(INC_DIR)/greedy_solver.hpp \
	$(INC_DIR)/localsearch_solver.hpp

.PHONY: all clean
