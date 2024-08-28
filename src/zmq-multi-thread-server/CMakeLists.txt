set(TARGET zmq-multi-thread-server)

qt_add_executable(${TARGET} pthread zmq)
