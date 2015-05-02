
PROJECT = average
CFLAGS = -Wall
CXXFLAGS = `pkg-config --cflags eigen3`
LIBS = `pkg-config --libs eigen3`

SRC = $(wildcard src/*.cpp)
OBJS = $(SRC:src/%.cpp=build/%.o)

all: ${PROJECT} 

build/:
	@mkdir -p "$@"

${PROJECT}: $(OBJS)
	g++ -o ${PROJECT} $(OBJS) $(LIBS) 
	
build/%.o: src/%.cpp build/
	g++ -c $(CFLAGS) $(CXXFLAGS) -o "$@" "$<"

clean:
	rm $(OBJS) ${PROJECT}

