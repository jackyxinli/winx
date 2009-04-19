#
# Configuration Default
#
ifeq ($(CFG),)
	CFG = Debug
endif

IncludeDir = -I../../../stdext/include -I../../../boost -I../../../boostmm

#
# Configuration Release
#
ifeq ($(CFG), Release)
	LibDir	    += -LRelease
	Libraries   += -lrt
	Libraries   += -lglib-2.0
	ProductDir   = Release
	ObjectDir    = Release
	Product      = Release/test
	CFlags       = $(Defines) -fPIC -D__32BIT__ -D__INTEL__ -D__LINUX__ -DLINUX  -DNDEBUG -O2 -D_MT -fexceptions -Wall
	CXXFlags     = $(CFlags) -fno-rtti
	CompileC     = @gcc -c $(IncludeDir) $(CFlags) -o $@ $<
	CompileCXX   = @g++ -c $(IncludeDir) $(CXXFlags) -o $@ $<
	Link         = @g++ $(LibDir) -o $(Product)
	CheckLink    = @ldd -u -r $(Product); echo
	LD_LIBRARY_PATH += :$(ProductDir)
endif

#
# Configuration Debug
#
ifeq ($(CFG), Debug)
	LibDir	    += -LDebug
	Libraries   += -lrt
	Libraries   += -lglib-2.0
	ProductDir   = Debug
	ObjectDir    = Debug
	Product      = Debug/test
	CFlags       = $(Defines) -fPIC -D__32BIT__ -D__INTEL__ -D__LINUX__ -DLINUX  -D_DEBUG -g3 -D_MT -fexceptions -Wall
	CXXFlags     = $(CFlags) -fno-rtti
	CompileC     = @gcc -c $(IncludeDir) $(CFlags) -o $@ $<
	CompileCXX   = @g++ -c $(IncludeDir) $(CXXFlags) -o $@ $<
	Link         = @g++ $(LibDir) -o $(Product)
	CheckLink    = @ldd -u -r $(Product); echo
	LD_LIBRARY_PATH += :$(ProductDir)
endif

#
# Making...
#
all : $(ProductDir) $(ObjectDir) StartMake $(Product) EndMake

$(ProductDir) :
	@mkdir -p $(ProductDir)

ifneq ($(ProductDir),$(ObjectDir))
$(ObjectDir) :
	@mkdir -p $(ObjectDir)
endif

clean :
	@rm -f $(ObjectDir)/*.o
	@rm -f $(Product)

StartMake :
	@echo "-------------- Configuration:" $(PROJ) $(CFG) "--------------"
ifeq ($(ForceLink),1)
	@rm -f $(Product)
endif

EndMake :
	@echo ">>> Success <<<"

#
# Source Files, Objects & Product
#
$(ObjectDir)/Performance.o :	\
	Performance.cpp
	@echo "---> Compiling" $< "..."
	$(CompileCXX)

Objects = $(ObjectDir)/Performance.o 

$(Product) : $(Objects)
	@echo "---> Creating" $@ "..."
	@rm -f $@
	$(Link) $(Objects) $(Libraries)
	$(CheckLink)

