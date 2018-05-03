CXX = distcc arm-linux-gnueabihf-g++-6
SRCDIR = src
BUILDDIR = build
INCDIR = include
TARGET = bin/main

SRCEXT = cpp
INCEXT = hpp
SOURCES = $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
HEADERS = $(shell find $(INCDIR) -type f -name *.$(INCEXT))
LIB = -lboost_system -lboost_thread -lboost_filesystem -lboost_chrono -lssl -lcrypto -lpthread -lboost_atomic -lboost_date_time
LIBDIR = 
CXXFLAGS = -Wall  
INC = -I include/ -I ~/Simple-Web-Server/ 
$(TARGET): $(OBJECTS)
	@echo " Linking..."
	$(CXX) $^ -o $(TARGET) $(LIB) $(LIBDIR) --sysroot=/sysroot/armv7l -Wl,-rpath,/usr/lib/gcc/arm-linux-gnueabihf/6/

-include $(OBJECTS:.o=.d)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT) $(BUILDDIR)/%.d
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<
	$(CXX) $(CXXFLAGS) $(INC) -MM $< > $(BUILDDIR)/$*.d
	@cp -f $(BUILDDIR)/$*.d $(BUILDDIR)/$*.d.tmp
	@sed -e 's/.*://' -e 's/\\$$//' < $(BUILDDIR)/$*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $(BUILDDIR)/$*.d
	@rm -f $(BUILDDIR)/$*.d.tmp

$(BUILDDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

alexa:
	$(CXX) $(CXXFLAGS) $(INC) -c $(INCDIR)/AlexaSkill.hpp $(LIB)

clean:
	@echo " Cleaning..."; 
	$(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean
