
SRC_DIR = GateServer LogicServer TestClient Common/BaseServer Common/Client
INC_DIR = ./Common ./Common/rapidjson/include
OBJ_DIR = ./objs
OUT_DIR = output

#PREDEFINE_FLAG = -DDEBUG

TARGET = $(OUT_DIR)/GateServer $(OUT_DIR)/LogicServer $(OUT_DIR)/TestClient

INC_FLAG = $(foreach DIR,$(INC_DIR), $(addprefix -I,$(DIR)))
CPPFLAGS = -std=c++11

SRC = $(foreach DIR,$(SRC_DIR), $(wildcard $(DIR)/*.cpp))
OBJECTS = $(addprefix $(OBJ_DIR)/, $(patsubst %.cpp, %.o, $(SRC)))
COMMONOBJS = $(filter $(addprefix $(OBJ_DIR)/Common/BaseServer, %.o) $(addprefix $(OBJ_DIR)/Common/Client/, %.o), $(OBJECTS))
JSON = $(foreach DIR,$(SRC_DIR), $(wildcard $(DIR)/*.json))

all: Make_Pre $(TARGET) CP_JSON

debug:
	@echo $(SRC)
	@echo $(OBJECTS)
	
Make_Pre:
	$(foreach dir, $(SRC_DIR), $(shell mkdir -p $(OBJ_DIR)/$(dir)))
	$(shell mkdir -p $(OUT_DIR))

CP_JSON: 
	@echo $(JSON)
	$(foreach json, $(JSON), $(shell cp $(json) $(OUT_DIR)/$(notdot json)))

$(TARGET): $(OBJECTS)
	@echo "Linking"
	g++ -o $@ $(filter $(addprefix $(OBJ_DIR)/$(notdir $@)/, %.o), $(OBJECTS)) $(COMMONOBJS) 
		
	
$(OBJ_DIR)/%.o: %.cpp
	g++ $(CPPFLAGS) $(INC_FLAG) -c $< -o $@

.PHONY: clean
clean:
	find ./ -name *.cpp~ | xargs rm -f
	find ./ -name *.h~ | xargs rm -f
	find ./ -name *.json~ | xargs rm -f
	find ./ -name *.un~ | xargs rm -f
	rm -f Makefile~
	rm -rf $(OUT_DIR) $(OBJ_DIR) 
	
