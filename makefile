# Variáveis
CFLAGS = -Wall -Wextra
TARGET = trab1
SOURCES = main.c grafo.c node.c
OBJECTS = $(SOURCES:.c=.o)

# Regra principal: gerar o executável
$(TARGET): $(OBJECTS)
	gcc $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Compilar arquivos-fonte em arquivos objeto
%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

# Limpar arquivos gerados
clean:
	rm -f $(TARGET) $(OBJECTS)