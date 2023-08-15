cmake_minimum_required(VERSION 3.25)

set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
if(CMAKE_CROSSCOMPILING)
    find_program(_PROTOBUF_PROTOC protoc)
else()
    set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
endif()

# Find gRPC installation
# Looks for gRPCConfig.cmake file installed by gRPC's cmake installation.
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_GRPC_GRPCPP gRPC::grpc++)
if(CMAKE_CROSSCOMPILING)
    find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
else()
    set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
endif()

# Proto file
get_filename_component(hw_proto "./env_service.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

# Generated sources
set(hw_proto_srcs "${CMAKE_CURRENT_SOURCE_DIR}/env_service.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_SOURCE_DIR}/env_service.pb.h")
set(hw_grpc_srcs "${CMAKE_CURRENT_SOURCE_DIR}/env_service.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_SOURCE_DIR}/env_service.grpc.pb.h")

include_directories($ENV{HOME}/.local/include)

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
        OSRMService.cpp
)
target_link_libraries(service
        store
        ${_PROTOBUF_LIBPROTOBUF} ${_GRPC_GRPCPP} ${_REFLECTION}
        ${LibOSRM_LIBRARIES} ${LibOSRM_DEPENDENT_LIBRARIES}
)
