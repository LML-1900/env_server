cmake_minimum_required(VERSION 3.25)

# Locate protobuf installation
set(protobuf_MODULE_COMPATIBLE TRUE)
list(APPEND CMAKE_PREFIX_PATH "$ENV{HOME}/.local")
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")

# Locate gRPC installation
list(APPEND CMAKE_PREFIX_PATH "$ENV{HOME}/.local") # replace with your actual path
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

# Add the paths to Protobuf, gRPC libraries and OSRM libraries
#link_directories("$ENV{HOME}/.local/lib" "/usr/local/lib")

# Link against the OSRM library
#find_library(OSRM_LIBRARY osrm HINTS /usr/local/lib)

# Proto file
get_filename_component(hw_proto "./env_service.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

# Generated sources
set(hw_proto_srcs "${CMAKE_CURRENT_SOURCE_DIR}/env_service.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_SOURCE_DIR}/env_service.pb.h")
set(hw_grpc_srcs "${CMAKE_CURRENT_SOURCE_DIR}/env_service.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_SOURCE_DIR}/env_service.grpc.pb.h")

add_custom_command(
        OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
        COMMAND protoc
        ARGS --grpc_out "${CMAKE_CURRENT_SOURCE_DIR}"
        --cpp_out "${CMAKE_CURRENT_SOURCE_DIR}"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc=$ENV{HOME}/.local/bin/grpc_cpp_plugin #replace with your actual path
        "${hw_proto}"
        DEPENDS "${hw_proto}")

# Create a library target
add_library(service
        EnvironmentService.cpp  # replace this with actual source files
        ${hw_proto_srcs}
        ${hw_grpc_srcs}
#        OSRMService.cpp
)

# Add the paths to Protobuf, gRPC headers and OSRM headers
#target_include_directories(service PUBLIC "$ENV{HOME}/.local/include" "/usr/local/include")

target_link_libraries(service
        gRPC::grpc++
        gRPC::grpc++_reflection
        protobuf::libprotobuf
        store
#        ${OSRM_LIBRARY}
)
