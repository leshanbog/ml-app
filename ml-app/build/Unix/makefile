include ./Make.Config

SOURCES= \
	$(SRC)/auxiliaryObjects/Obj.cpp \
	$(SRC)/auxiliaryObjects/DataFrame.cpp \
	$(SRC)/auxiliaryObjects/UsefulMath.cpp \
	$(SRC)/algorithms/ConstPrediction.cpp \
	$(SRC)/algorithms/DecisionStump.cpp \
	$(SRC)/algorithms/KNN.cpp \
	$(SRC)/LearningMachineLearning.cpp

EXECUTABLE=proga

all:
	$(CC) $(STD) -pthread $(OPTFLAGS) $(SOURCES) -o $(BUILD_DIR)/$(EXECUTABLE)


.PHONY: clean
clean:
	rm -f $(BUILD_DIR)/*
