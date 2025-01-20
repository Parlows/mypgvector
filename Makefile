EXTENSION = pgsurv
EXTVERSION = 1.0

# Shared library to build from multiple source files (list object fioles in OBJS)
MODULE_big = pgsurv

# Random files to install into prefix/share/extension
DATA = $(wildcard sql/*--*--*.sql)

# Random files to install into prefix/share/extension which need to be built first
DATA_built = sql/$(EXTENSION)--$(EXTVERSION).sql

# List of object files
OBJS = src/utils.o src/vector_utils.o src/vector.o

# Files to (optionally build and) install under prefix/include/server/extension/$MODULE_big
HEADERS = src/utils.h src/vector_utils.h src/vector.h
# Testing
# TESTS = $(wildcard test/sql/*.sql)
# REGRESS = $(patsubst test/sql/%.sql,%,$(TESTS))
# REGRESS_OPTS = --inputdir=test --load-extension=$(EXTENSION)

# To compile for portability, run: make OPTFLAGS=""
OPTFLAGS = -march=native

# # Mac ARM doesn't always support -march=native
# ifeq ($(shell uname -s), Darwin)
# 	ifeq ($(shell uname -p), arm)
# 		# no difference with -march=armv8.5-a
# 		OPTFLAGS =
# 	endif
# endif

# # PowerPC doesn't support -march=native
# ifneq ($(filter ppc64%, $(shell uname -m)), )
# 	OPTFLAGS =
# endif

# For auto-vectorization:
# - GCC (needs -ftree-vectorize OR -O3) - https://gcc.gnu.org/projects/tree-ssa/vectorization.html
# - Clang (could use pragma instead) - https://llvm.org/docs/Vectorizers.html

# Appended to CFLAGS
PG_CFLAGS += $(OPTFLAGS) -ftree-vectorize -fassociative-math -fno-signed-zeros -fno-trapping-math

# Debug GCC auto-vectorization
# PG_CFLAGS += -fopt-info-vec

# Debug Clang auto-vectorization
# PG_CFLAGS += -Rpass=loop-vectorize -Rpass-analysis=loop-vectorize

all: sql/$(EXTENSION)--$(EXTVERSION).sql

# Copy single named sql file to fully named sql file
sql/$(EXTENSION)--$(EXTVERSION).sql: sql/$(EXTENSION).sql
	cp $< $@

# Path to pg_config program for the PostgreSQL installation to build against
PG_CONFIG ?= pg_config

# Include Postgres building infrastructure
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

# for Mac
# ifeq ($(PROVE),)
# 	PROVE = prove
# endif

# for Postgres < 15
PROVE_FLAGS += -I ./test/perl

# Other secondary targets
prove_installcheck:
	rm -rf $(CURDIR)/tmp_check
	cd $(srcdir) && TESTDIR='$(CURDIR)' PATH="$(bindir):$$PATH" PGPORT='6$(DEF_PGPORT)' PG_REGRESS='$(top_builddir)/src/test/regress/pg_regress' $(PROVE) $(PG_PROVE_FLAGS) $(PROVE_FLAGS) $(if $(PROVE_TESTS),$(PROVE_TESTS),test/t/*.pl)

.PHONY: dist

dist:
	mkdir -p dist
	git archive --format zip --prefix=$(EXTENSION)-$(EXTVERSION)/ --output dist/$(EXTENSION)-$(EXTVERSION).zip master

# for Docker
PG_MAJOR ?= 17

.PHONY: docker

docker:
	docker build --pull --no-cache --build-arg PG_MAJOR=$(PG_MAJOR) -t pgvector/pgvector:pg$(PG_MAJOR) -t pgvector/pgvector:$(EXTVERSION)-pg$(PG_MAJOR) .

.PHONY: docker-release

docker-release:
	docker buildx build --push --pull --no-cache --platform linux/amd64,linux/arm64 --build-arg PG_MAJOR=$(PG_MAJOR) -t pgvector/pgvector:pg$(PG_MAJOR) -t pgvector/pgvector:$(EXTVERSION)-pg$(PG_MAJOR) .
