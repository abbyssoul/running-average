# Running average command line util build file

PROJECT = average
CXXFLAGS = -Wall
CXXFLAGS += `pkg-config --cflags eigen3`
LIBS = `pkg-config --libs eigen3`

SRC_DIR = src
OBJ_DIR = build
BIN_DIR = bin

SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)


.PHONY: clean

all: $(PROJECT)

$(OBJ_DIR):
	@mkdir -p "$@"

$(BIN_DIR):
	@mkdir -p "$@"

$(PROJECT): $(OBJS)
	$(CXX) -o $(PROJECT) $(OBJS) $(LIBS) 
	
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(OBJ_DIR) $(BIN_DIR)
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o "$@" "$<"

.PHONY: clean
clean:
	$(RM) -f $(OBJS) $(PROJECT) $(BIN_DIR)/*

