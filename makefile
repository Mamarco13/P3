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
	$(SRC_DIR)/localsearch_solver.cpp \
	$(SRC_DIR)/simulated_annealing.cpp

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

# Headers opcionales
HEADERS = \
	$(INC_DIR)/simulated_annealing.hpp \
	$(INC_DIR)/SNIMPProblem.hpp \
	$(COMMON_DIR)/problem.hpp \
	$(COMMON_DIR)/solution.hpp \
	$(COMMON_DIR)/util.hpp \
	$(COMMON_DIR)/mh.hpp \
	$(COMMON_DIR)/mhtrayectory.hpp \
	$(COMMON_DIR)/random.hpp \
	$(INC_DIR)/graph_loader.hpp \
	$(INC_DIR)/icm.hpp \
	$(INC_DIR)/random_solver.hpp \
	$(INC_DIR)/greedy_solver.hpp \
	$(INC_DIR)/localsearch_solver.hpp

.PHONY: all clean
