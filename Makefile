APPLICATION = sudokuSolver
SRCDIR = source
SOURCES = $(SRCDIR)/main.cpp
SOURCES += $(SRCDIR)/parser.cpp
SOURCES += $(SRCDIR)/downloader.cpp
SOURCES += $(SRCDIR)/solver.cpp
RSRCDIR = additionalFiles
DAT_FILE = \"$(RSRCDIR)/SDKS.dat\"
SKP_FILE = \"$(RSRCDIR)/skippedSdk.sdk\"
SLV_FILE = \"$(RSRCDIR)/Solved.sdk\"
BUILD_WITH_DOWNLOADER=0
all: clean $(APPLICATION)
	./$(APPLICATION)
.PHONY: $(SOURCES)
$(APPLICATION): $(SOURCES)
	g++ -std=c++11 \
	-w \
	-DBUILD_WITH_DOWNLOADER=$(BUILD_WITH_DOWNLOADER) \
	-DDAT_FILE=$(DAT_FILE) \
	-DSKP_FILE=$(SKP_FILE) \
	-DSLV_FILE=$(SLV_FILE) \
	-o $(APPLICATION) \
	$(SOURCES) \
	-lpthread
	
clean:
	-rm -f *.o
	-rm -f $(APPLICATION)