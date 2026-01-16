TARGET     = main

# Compiler and linker settings
CXX        = g++
CXXFLAGS   = -std=c++23 -O3 -march=native -pedantic-errors
LDFLAGS    = -flto
LD         = g++ -o

SRCDIR     = src
OBJDIR     = obj
BINDIR     = bin

SOURCES   := $(wildcard $(SRCDIR)/main.cpp) \
			 $(wildcard $(SRCDIR)/heuristic/*.cpp) \
			 $(wildcard $(SRCDIR)/is_final/*.cpp) \
			 $(wildcard $(SRCDIR)/neighbors/*.cpp) \
			 $(wildcard $(SRCDIR)/route_finding/*.cpp) \
			 $(wildcard $(SRCDIR)/scenarios/*.cpp) \
			 $(wildcard $(SRCDIR)/solution/*.cpp) \
			 $(wildcard $(SRCDIR)/status/*.cpp) \
			 $(wildcard $(SRCDIR)/visualization/*.cpp)

OBJECTS   := $(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.cpp=.o)))

RM         = rm -f

# Main target
$(BINDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	@$(LD) $@ $(LDFLAGS) $(OBJECTS)
	@echo "Linking complete!"

# Object file compilation
$(OBJDIR)/%.o: $(SRCDIR)/heuristic/%.cpp
	@mkdir -p $(OBJDIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

$(OBJDIR)/%.o: $(SRCDIR)/is_final/%.cpp
	@mkdir -p $(OBJDIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

$(OBJDIR)/%.o: $(SRCDIR)/neighbors/%.cpp
	@mkdir -p $(OBJDIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

$(OBJDIR)/%.o: $(SRCDIR)/route_finding/%.cpp
	@mkdir -p $(OBJDIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

$(OBJDIR)/%.o: $(SRCDIR)/scenarios/%.cpp
	@mkdir -p $(OBJDIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

$(OBJDIR)/%.o: $(SRCDIR)/solution/%.cpp
	@mkdir -p $(OBJDIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

$(OBJDIR)/%.o: $(SRCDIR)/status/%.cpp
	@mkdir -p $(OBJDIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

$(OBJDIR)/%.o: $(SRCDIR)/visualization/%.cpp
	@mkdir -p $(OBJDIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

$(OBJDIR)/main.o: $(SRCDIR)/main.cpp
	@mkdir -p $(OBJDIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

.PHONY: clean
clean:
	@$(RM) $(OBJECTS)
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@$(RM) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"
