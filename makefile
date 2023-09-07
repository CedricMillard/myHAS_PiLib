CC=g++
CFLAGS = -I.
LIBS = -lmosquitto -lmysqlclient
DEPS = 
ODIR=../build/obj
BDIR=../build/

_OBJ = myHAS_MQTTClient.o myHAS_SQLClient.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

All: $(BDIR)/myHAS_Lib

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BDIR)/myHAS_Lib: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) -shared
