# Generating config.h
# (Required by elfcpp)

include(CheckIncludeFiles)
include(TestBigEndian)

check_include_files(byteswap.h HAVE_BYTESWAP_H)
test_big_endian(WORDS_BIGENDIAN)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/config.in
	${CMAKE_CURRENT_SOURCE_DIR}/config.h
)
