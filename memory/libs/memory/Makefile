#
# Configuration Default
#
ifeq ($(CFG),)
	CFG = Debug
endif

IncludeDir = -I. -I../.. 

#
# Configuration Release
#
ifeq ($(CFG), Release)
	Libraries   += -lpthread
	ProductDir   = bin/linux
	ObjectDir    = LRelease
	MocFile      = $(ObjectDir)/temp.moc.cpp
	Product      = bin/linux/libboost-memory.so
	CFlags       = $(Defines) -fPIC -D__INTEL__ -D__LINUX__ -DLINUX -DNDEBUG -O2 -D_MT -fexceptions 
	CXXFlags     = $(CFlags) -fno-rtti 
	CXX          = g++ -c $(IncludeDir) $(CXXFlags) -o $@
	CompileC     = @gcc -c $(IncludeDir) $(CFlags) -o $@ $<
	CompileCXX   = @$(CXX) $<
	CompileMoc   = @moc $< -o $(MocFile) && $(CXX) $(MocFile) && rm -f $(MocFile)
	Link         = @g++ $(LibDir) -W1 -shared -ldl -o $(Product) -Wl,--version-script,_export_.def
	CheckLink    = @ldd -r $(Product)
	LD_LIBRARY_PATH += :$(ProductDir)
endif

#
# Configuration ReleaseCS
#
ifeq ($(CFG), ReleaseCS)
	Defines	    += -DBOOST_MEMORY_NO_LOCKFREE
	Libraries   += -lpthread
	ProductDir   = bin/linux
	ObjectDir    = LReleaseCS
	MocFile      = $(ObjectDir)/temp.moc.cpp
	Product      = bin/linux/libboost-memory-cs.so
	CFlags       = $(Defines) -fPIC -D__INTEL__ -D__LINUX__ -DLINUX -DNDEBUG -O2 -D_MT -fexceptions 
	CXXFlags     = $(CFlags) -fno-rtti 
	CXX          = g++ -c $(IncludeDir) $(CXXFlags) -o $@
	CompileC     = @gcc -c $(IncludeDir) $(CFlags) -o $@ $<
	CompileCXX   = @$(CXX) $<
	CompileMoc   = @moc $< -o $(MocFile) && $(CXX) $(MocFile) && rm -f $(MocFile)
	Link         = @g++ $(LibDir) -W1 -shared -ldl -o $(Product) -Wl,--version-script,_export_.def
	CheckLink    = @ldd -r $(Product)
	LD_LIBRARY_PATH += :$(ProductDir)
endif

#
# Configuration Debug
#
ifeq ($(CFG), Debug)
	Libraries   += -lpthread
	ProductDir   = bin/linuxd
	ObjectDir    = LDebug
	MocFile      = $(ObjectDir)/temp.moc.cpp
	Product      = bin/linuxd/libboost-memory.so
	CFlags       = $(Defines) -fPIC -D__INTEL__ -D__LINUX__ -DLINUX -D_DEBUG -g3 -D_MT -fexceptions 
	CXXFlags     = $(CFlags) -fno-rtti 
	CXX          = g++ -c $(IncludeDir) $(CXXFlags) -o $@
	CompileC     = @gcc -c $(IncludeDir) $(CFlags) -o $@ $<
	CompileCXX   = @$(CXX) $<
	CompileMoc   = @moc $< -o $(MocFile) && $(CXX) $(MocFile) && rm -f $(MocFile)
	Link         = @g++ $(LibDir) -W1 -shared -ldl -o $(Product) -Wl,--version-script,_export_.def
	CheckLink    = @ldd -r $(Product)
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
$(ObjectDir)/block_pool.o :	\
	./src/block_pool.cpp
	@echo "---> Compiling" $< "..."
	$(CompileCXX)

$(ObjectDir)/gc_alloc.o :	\
	./src/gc_alloc.cpp
	@echo "---> Compiling" $< "..."
	$(CompileCXX)

$(ObjectDir)/system_pool.o :	\
	./src/system_pool.cpp
	@echo "---> Compiling" $< "..."
	$(CompileCXX)

$(ObjectDir)/StdAfx.o :	\
	./StdAfx.cpp
	@echo "---> Compiling" $< "..."
	$(CompileCXX)


Objects =	\
	$(ObjectDir)/block_pool.o	\
	$(ObjectDir)/gc_alloc.o	\
	$(ObjectDir)/system_pool.o	\
	$(ObjectDir)/StdAfx.o

$(Product) : $(Objects) $(Libs)
	@echo "---> Creating" $@ "..."
	@rm -f $@
	$(Link) $(Objects) $(Libraries)
	$(CheckLink)

