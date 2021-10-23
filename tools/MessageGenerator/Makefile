MKDIR=mkdir
CP=cp
CCADMIN=CCadmin

build: .build-post

.build-pre:
	scripts/generateSourceFromXSD

.build-post: .build-impl
	
clean: .clean-post
	$(RM) src/Definitions.cxx
	$(RM) include/Definitions.hxx

.clean-pre:

.clean-post: .clean-impl

clobber: .clobber-post

.clobber-pre:

.clobber-post: .clobber-impl
	
all: .all-post

.all-pre:

.all-post: .all-impl
	
build-tests: .build-tests-post

.build-tests-pre:

.build-tests-post: .build-tests-impl
	
test: .test-post

.test-pre: build-tests

.test-post: .test-impl
	
help: .help-post

.help-pre:
	
.help-post: .help-impl
	
include nbproject/Makefile-impl.mk
include nbproject/Makefile-variables.mk
