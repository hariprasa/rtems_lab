###############################################################################
# Type:
#  - "make"     to compile the example
#  - "make run" to compile and run 
# /home/hari/local/rtems-4.12-installed/4.12/sparc-rtems4.12/leon3/lib/include
###############################################################################

# C source names
CSRCS = initex1.c

###############################################################################
# DO NOT EDIT AFTER THIS LINE

EXEC=test.exe
PGM=${ARCH}/$(EXEC)
COBJS_ = $(CSRCS:.c=.o)
COBJS  = $(COBJS_:%=${ARCH}/%)

MANAGERS = all            # optional managers required
LIBS     = -lrtemsall -lc # libraries

include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
include $(RTEMS_CUSTOM)
include $(PROJECT_ROOT)/make/leaf.cfg
includedir=/home/hari/local/rtems-4.12-installed/4.12/sparc-rtems4.12/leon3/lib/include
OBJS= $(COBJS) $(CXXOBJS) $(ASOBJS)

all: clean ${ARCH} $(PGM)

$(PGM): $(OBJS)
	$(make-exe)

run: all
	tsim-leon3 ${ARCH}/$(EXEC)

help:
	@echo Type:
	@echo  - "make"     to compile the example
	@echo  - "make run" to compile and run
send:
	scp ./o-optimize/test.exe root@prise-space-3:~/taste_examples/
