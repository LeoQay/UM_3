TARGET=proga

all: UM3.o Exception.o Memory.o Parser.o Processor.o Translator.o Tools.o Log.o Config.o Convertor.o BinaryStorage.o
	gcc UM3.o Exception.o Memory.o Parser.o Processor.o Translator.o Tools.o Log.o Config.o Convertor.o BinaryStorage.o -o $(TARGET)

UM3.o: UM3.cpp Convertor.h BinaryStorage.h Memory.h Config.h Processor.h Tools.h Exception.h
	gcc UM3.cpp -c

Exception.o: Exception.cpp Exception.h Log.h
	gcc Exception.cpp -c

Memory.o: Memory.cpp Memory.h Config.h Tools.h Exception.h
	gcc Memory.cpp -c

Parser.o: Parser.cpp Parser.h Exception.h Tools.h
	gcc Paresr.cpp -c

Processor.o: Processor.cpp Processor.h Memory.h Config.h Tools.h Parser.h Exception.h Log.h Convertor.h BinaryStorage.h Translator.h
	gcc Processor.cpp -c

Translator.o: Translator.cpp Translator.h Memory.h Config.h Tools.h Exception.h
	gcc Translator.cpp -c

Tools.o: Tools.cpp Tools.h Exception.h
	gcc Tools.cpp -c

Log.o: Log.cpp Log.h Tools.h
	gcc Log.cpp -c

Config.o: Config.cpp Config.h Tools.h Exception.h
	gcc Config.cpp -c

Convertor.o: Convertor.cpp Convertor.h BinaryStorage.h Memory.h Config.h Exception.h Tools.h Translator.h
	gcc Convertor.cpp -c

BinaryStorage.o: BinaryStorage.cpp BinaryStorage.h
	gcc BinaryStorage.cpp -c
